#ifndef __IEGAD_ZMQ_PUBLISHER__
#define __IEGAD_ZMQ_PUBLISHER__


#include <zmq.hpp>


namespace iegad {
namespace zmq_ex {


class publisher {
public:
    publisher(const std::string &host, int port) :
        ctx_(),
        publisher_(ctx_, ZMQ_PUB)
    {
        std::string addr = std::string("tcp://") +
                (host.empty() ? "*" : host) + ":" + std::to_string(port);
        publisher_.bind(addr);
    }


    bool
    pub(const std::string &route, const std::string &msg)
    {
        bool ret = false;

        do {
            if (route.empty() || msg.empty()) {
                break;
            }

            zmq::message_t r(route.size());
            memcpy(r.data(), route.data(), route.size());

            if (!publisher_.send (r, ZMQ_SNDMORE)) {
                break;
            }

            zmq::message_t m(msg.size());
            memcpy(m.data(), msg.data(), msg.size());

            if (!publisher_.send(m)) {
                break;
            }

            ret = true;
        } while(0);

        return ret;
    }


    ~publisher()
    {
        if ((void *)publisher_) {
            publisher_.close();
        }

        if ((void *)ctx_) {
            ctx_.close();
        }
    }


private:
    zmq::context_t ctx_;
    zmq::socket_t publisher_;
}; // class publisher;


} // namespace zmq_ex;
} // namespace iegad;


#endif // __IEGAD_ZMQ_PUBLISHER__
