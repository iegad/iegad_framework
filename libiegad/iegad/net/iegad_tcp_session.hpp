#ifndef __IEGAD_TCP_SESSION__
#define __IEGAD_TCP_SESSION__



#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>

#include "net/iegad_tcp_buffer.hpp"
#include "net/iegad_tcp_event.hpp"



namespace iegad {
namespace net {


class tcp_session : public boost::enable_shared_from_this<tcp_session> {
public:
    typedef boost::shared_ptr<tcp_session> ptr_t;
    typedef boost::asio::ip::tcp::socket socket;
    typedef boost::asio::io_service io_service;


    explicit tcp_session(io_service & ios)
        :
          sock_(ios)
    {}


    void start(tcp_event::ptr_t ev) {        
        tcpev_ = ev;
        if (tcpev_ && tcpev_->open_handler) {
            tcpev_->open_handler(shared_from_this());
        }
        this->read();
    }


    void close() {
        boost::system::error_code ec;
        if (tcpev_ && tcpev_->close_handler) {
            tcpev_->close_handler(shared_from_this());
        }
        sock_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        sock_.close(ec);
    }

<<<<<<< HEAD

    int send(const std::string & msgstr) {
        int n = -1, nleft = msgstr.size();
        const char * p = msgstr.c_str();
        while(nleft) {
            n = write(sockfd_, p, nleft);
            if (n > 0) {
                nleft -= n;
                p += n;
            }
            else {
                return -1;
            }
            return msgstr.size() - nleft;
        }
    }


    int sockfd() const {
        return sockfd_;
=======
    void write() {
        sock_.async_write_some(wbuff_.data(),
                               boost::bind(&tcp_session::write_handler,
                                           shared_from_this(),
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
    }


    void write(const void * data, size_t len) {
        wbuff_.append(data, len);
        this->write();
    }


    void read() {
        sock_.async_read_some(rbuff_.prepare(),
                              boost::bind(&tcp_session::read_handler, shared_from_this(),
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred));
>>>>>>> ab2d0a0d98186f8a443fe810bb73c6e41cea8479
    }


    void read_handler(const boost::system::error_code & ec, size_t bytes_transferred) {
        if (ec) {            
            this->close();
            return;
        }
        rbuff_.commit(bytes_transferred);

        if (tcpev_ && tcpev_->read_handler) {
            tcpev_->read_handler(shared_from_this(), bytes_transferred);
        }

        rbuff_.consume(bytes_transferred);
        read();
    }


    void write_handler(const boost::system::error_code & ec, size_t bytes_transferred) {
        if (ec) {
            this->close();
            return;
        }
        if (tcpev_ && tcpev_->write_handler) {
            tcpev_->write_handler(shared_from_this(), bytes_transferred);
        }
        wbuff_.consume(bytes_transferred);
    }


    socket & sock() {
        return sock_;
    }


    io_service & ios() {
        return sock_.get_io_service();
    }


    tcp_buffer & rbuff() {
        return rbuff_;
    }

    tcp_buffer & wbuff() {
        return wbuff_;
    }


private:
    socket sock_;
    tcp_event::ptr_t tcpev_;
    tcp_buffer rbuff_;
    tcp_buffer wbuff_;
}; // class tcp_session;


} // namespace net;
} // namespace iegad;



#endif // __IEGAD_TCP_SESSION__
