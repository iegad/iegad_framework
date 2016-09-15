#ifndef __IEGAD_RABBIT_HANDLER__
#define __IEGAD_RABBIT_HANDLER__



#include <inttypes.h>
#include <deque>
#include <vector>
#include <memory>


#include <amqpcpp.h>
#include <boost/asio.hpp>


#include "rabbit_ex/iegad_rabbit_buffer.hpp"


namespace iegad {
namespace rabbit_ex {





#define ASIO_INPUT_BUFFER_SIZE (4 * 1024)


class asioHandler: public AMQP::ConnectionHandler {
public:
    typedef std::deque<std::vector<char>> buffer_que_t;


    explicit asioHandler(boost::asio::io_service & ios)
        :
          ios_(ios),
          sock_(ios_),
          readBuff_(ASIO_INPUT_BUFFER_SIZE, 0),
          amqpBuffer_(new amqpBuffer(ASIO_INPUT_BUFFER_SIZE * 2)),
          conn_(nullptr),
          writeInProgress_(false),
          connected_(false),
          quit_(false)
    {}


    void connect(const std::string& host, uint16_t port) {
        _do_connect(host, port);
    }


    virtual ~asioHandler() {}

    asioHandler(const asioHandler&) = delete;
    asioHandler& operator=(const asioHandler&) = delete;

    bool connected() const {
        return connected_;
    }


    virtual void onData(AMQP::Connection *connection, const char *data, size_t size) {
        conn_ = connection;
        outBuffQue_.push_back(std::vector<char>(data, data + size));
        if (!writeInProgress_ && connected_) {
            _do_write();
        }
    }


    virtual void onConnected(AMQP::Connection *) const
    {}


    virtual void onError(AMQP::Connection *connection, const char *message)
    {}


    virtual void onClosed(AMQP::Connection *) {
        quit_ = true;
        if (!writeInProgress_) {
            sock_.close();
        }
    }


private:
    void _do_connect(const std::string& host, uint16_t port) {
        using boost::asio::ip::tcp;
        tcp::resolver::query query(host, std::to_string(port));
        tcp::resolver::iterator iter = tcp::resolver(ios_).resolve(query);

        boost::asio::async_connect(sock_, iter,
                [this](boost::system::error_code ec, tcp::resolver::iterator)
                {
                    connected_ = true;
                    if (!ec) {
                       _do_read();

                        if(!outBuffQue_.empty()) {
                           _do_write();
                        }
                    }
                    else {
                    }
                });
    }


    void _do_read() {
        sock_.async_receive(boost::asio::buffer(readBuff_),
                [this](boost::system::error_code ec, size_t length)
                {
                    if (!ec) {
                        amqpBuffer_->write(readBuff_.data(), length);
                        _parse_data();
                        _do_read();
                    }
                    else {
                    }
                });
    }


    void _do_write() {
        writeInProgress_ = true;
        boost::asio::async_write(sock_,
                boost::asio::buffer(outBuffQue_.front()),
                [this](boost::system::error_code ec, size_t) {
                    if(!ec) {
                        outBuffQue_.pop_front();
                        if(!outBuffQue_.empty()) {
                            _do_write();
                        }
                        else {
                            writeInProgress_ = false;
                        }

                        if(quit_) {
                            sock_.close();
                        }
                    }
                    else {
                        sock_.close();
                    }
                });
    }


    void _parse_data() {
        if (conn_ == nullptr) {
            return;
        }

        const size_t count = conn_->parse(amqpBuffer_->data(),
                amqpBuffer_->available());

        if (count == amqpBuffer_->available()) {
            amqpBuffer_->drain();
        }
        else if (count > 0) {
            amqpBuffer_->shl(count);
        }
    }


    boost::asio::io_service & ios_;
    boost::asio::ip::tcp::socket sock_;

    std::vector<char> readBuff_;
    amqpBuffer::ptr_t amqpBuffer_;
    AMQP::Connection* conn_;
    buffer_que_t outBuffQue_;
    bool writeInProgress_;
    bool connected_;
    bool quit_;
}; // class asioHandler;


} // namespace rabbit_ex;
} // namespace iegad;



#endif // __IEGAD_RABBIT_HANDLER__
