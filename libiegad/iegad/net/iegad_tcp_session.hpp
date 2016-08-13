#ifndef __IEGAD_TCP_SESSION__
#define __IEGAD_TCP_SESSION__



#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/function.hpp>


#include "tools/job_queue.hpp"



namespace iegad {
namespace net {


class tcp_session;


struct msg_head {
    uint8_t msg_flag;
    uint8_t msg_ccode;
    uint16_t msg_size;
    uint16_t msg_type;
    uint16_t msg_seq;
};


struct msg_t : public boost::noncopyable {
    typedef msg_t * ptr_t;
    typedef boost::function<void(msg_t *)> destory_t;

    template <typename Func>
    msg_t(boost::shared_ptr<tcp_session> ss, Func func)
        :
          m_tcpss(ss),
          destory_(func)
    {}

    msg_t() {}


    void destory() {
        if (destory_) {
            destory_(this);
        }
        else {
            boost::checked_delete(this);
        }
    }

    msg_head m_head;
    std::string m_data;
    boost::shared_ptr<tcp_session> m_tcpss;
    destory_t destory_;
};



class tcp_session : public boost::enable_shared_from_this<tcp_session> {
public:
    typedef boost::shared_ptr<tcp_session> ptr_t;
    typedef boost::asio::ip::tcp::socket socket;
    typedef boost::asio::io_service io_service;
    typedef io_service::strand strand;
    typedef iegad::tools::job_que_t<msg_t::ptr_t> que_t;
    typedef boost::object_pool<msg_t> msg_pool_t;


    explicit tcp_session(io_service & ios, que_t & que)
        :
          sock_(ios),
          strand_(ios),
          que_(que)
    {}


    ~tcp_session() {
        this->close();
    }


    void start() {
        msg_t::ptr_t msg = _create_msg();
        _read(msg);
    }


    void close() {
        boost::system::error_code ec;        
        sock_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        sock_.close(ec);
    }


    void write(msg_t::ptr_t msg) {
        boost::asio::async_write(sock_,
                                 boost::asio::buffer(&msg->m_head, sizeof(msg->m_head)),
                                 strand_.wrap(
                                     boost::bind(&tcp_session::_write_head_handler, this,
                                                 boost::asio::placeholders::error,
                                                 boost::asio::placeholders::bytes_transferred,
                                                 msg)));
    }


    socket & sock() {
        return sock_;
    }


    io_service & ios() {
        return sock_.get_io_service();
    }


private:
    void _write_head_handler(const boost::system::error_code & ec, size_t n, msg_t::ptr_t msg) {
        if (ec || n != sizeof(msg->m_head)) {
            this->close();
            return;
        }

        boost::asio::async_write(sock_,
                                 boost::asio::buffer(msg->m_data.c_str(), msg->m_data.size()),
                                 strand_.wrap(
                                     boost::bind(&tcp_session::_write_msg_handler, this,
                                                 boost::asio::placeholders::error,
                                                 boost::asio::placeholders::bytes_transferred,
                                                 msg)));
    }


    void _write_msg_handler(const boost::system::error_code & ec, size_t n, msg_t::ptr_t msg) {
        if (ec || n != msg->m_data.size()) {
            this->close();
            return;
        }
        msg->destory();
    }


    msg_t::ptr_t _create_msg() {
        return msg_pool_.construct(
                    shared_from_this(),
                    boost::bind(&msg_pool_t::destroy, boost::ref(msg_pool_), boost::placeholders::_1));
    }

    void _read(msg_t::ptr_t msg) {
        boost::asio::async_read(sock_,
                                boost::asio::buffer(&msg->m_head, sizeof(msg->m_head)),
                                strand_.wrap(
                                    boost::bind(&tcp_session::_read_head_handler, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred,
                                                msg)));
    }


    void _read_head_handler(const boost::system::error_code & ec, size_t n, msg_t::ptr_t msg) {
        if (ec || n != sizeof(msg->m_head)) {
            this->close();
            return;
        }
        msg->m_data.clear();
        msg->m_data.resize(msg->m_head.msg_size + 1);
        boost::asio::async_read(sock_,
                                boost::asio::buffer(const_cast<char *>(msg->m_data.c_str()),
                                                    msg->m_head.msg_size),
                                strand_.wrap(boost::bind(&tcp_session::_read_msg_handler,
                                                         this,
                                                         boost::asio::placeholders::error,
                                                         boost::asio::placeholders::bytes_transferred,
                                                         msg)));
    }


    void _read_msg_handler(const boost::system::error_code & ec, size_t n, msg_t::ptr_t msg) {
        if (ec || n != msg->m_head.msg_size) {
            this->close();
            return;
        }
        que_.push(msg);
        start();
    }



    socket sock_;
    strand strand_;
    que_t & que_;
    msg_pool_t msg_pool_;
}; // class tcp_session;


} // namespace net;
} // namespace iegad;



#endif // __IEGAD_TCP_SESSION__
