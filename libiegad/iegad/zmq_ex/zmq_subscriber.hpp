#ifndef __IEGAD_ZMQ_SUBSCRIBER__
#define __IEGAD_ZMQ_SUBSCRIBER__


#include <zmq.hpp>
#include <functional>
#include <thread>


namespace iegad {
namespace zmq_ex {


class subscriber {
public:
    subscriber(const std::string &host,
               int port,
               const std::string &route,
               std::function<void(const std::string &, const std::string &)> func = nullptr) :
        ctx_(),
        subscriber_(ctx_, ZMQ_SUB),
        subcb_(func),
        stop_(false)
    {
        assert(host.size() > 0);
        assert(port > 0 && port < 65535);

        std::string addr = "tcp://" + host + ":" + std::to_string(port);
        subscriber_.connect(addr);
        subscriber_.setsockopt(ZMQ_SUBSCRIBE, route.c_str(), route.size());

        if (subcb_) {
            std::thread t(std::bind(&subscriber::_proc_handler, this));
            t.detach();
        }
    }


    ~subscriber()
    {
        if ((void *)subscriber_) {
            subscriber_.close();
        }

        if ((void *)ctx_) {
            ctx_.close();
        }
    }


    bool
    sub(std::string *msg, std::string *route = nullptr)
    {
        bool ret = false;

        do {
            if (!msg) {
                break;
            }

            zmq::message_t r;
            if (!subscriber_.recv(&r)) {
                break;
            }

            if (route) {
                *route = std::string(static_cast<char*>(r.data()), r.size());
            }

            zmq::message_t m;
            if (!subscriber_.recv(&m)) {
                break;
            }

            *msg = std::string(static_cast<char*>(m.data()), m.size());

            ret = true;
        } while(0);

        return ret;
    }


    void
    stop()
    {
        stop_ = true;
    }


private:
    void
    _proc_handler()
    {
        std::string msg, route;
        for (;;) {
            if (stop_) {
                break;
            }
            if (this->sub(&msg, &route)) {
                if (subcb_) {
                    subcb_(route, msg);
                }
            }
        }
    }


    zmq::context_t ctx_;
    zmq::socket_t subscriber_;
    std::function<void(const std::string &, const std::string &)> subcb_;
    bool stop_;
}; // class subsriber;


} // namespace zmq_ex;
} // namespace iegad;


#endif // __IEGAD_ZMQ_SUBSCRIBER__
