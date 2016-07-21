#ifndef __IEGAD_TCP_SERVER__
#define __IEGAD_TCP_SERVER__



#include <inttypes.h>
#include "net/iegad_tcp_session.hpp"
#include "net/iegad_ioserive_pool.hpp"



namespace iegad {
namespace net {


template <typename T>
class tcp_server {
public:
    typedef ioservice_pool::io_service io_service;
    typedef boost::asio::ip::tcp::acceptor acceptor;
    typedef std::shared_ptr<tcp_server<T>> ptr_t;



    static ptr_t Create(int port, int n = 1) {
        ptr_t p(new tcp_server(port, n));
        return p;
    }


    void start() {
        iopool_.start();
    }


    void run() {
        iopool_.run();
    }


private:
    tcp_server(int port, int n)
        :
        iopool_(n),
        acptr_(iopool_.get(),
               boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
        _start_accept();
    }


    void _start_accept() {
        tcp_session::ptr_t session(new tcp_session(iopool_.get()));

        acptr_.async_accept(session->sock(),
                            boost::bind(&tcp_server::_handler_accept, this,
                                        boost::asio::placeholders::error,
                                        session));
    }


    void _handler_accept(const boost::system::error_code & ec, tcp_session::ptr_t session) {
        _start_accept();
        if (ec) {
            session->close();
            return;
        }
        session->start(T::Create());
    }

    ioservice_pool iopool_;
    acceptor acptr_;
}; // class tcp_server;



} // namespace net;
} // namespace iegad;



#endif // __IEGAD_TCP_SERVER__
