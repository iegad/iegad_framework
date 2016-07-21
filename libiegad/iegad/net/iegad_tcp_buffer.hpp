#ifndef __IEGAD_TCP_BUFFER__
#define __IEGAD_TCP_BUFFER__



#include <boost/asio.hpp>
#include <boost/config/suffix.hpp>
#include <boost/cast.hpp>



namespace iegad {
namespace net {



class tcp_buffer {
public:
    BOOST_STATIC_CONSTANT(size_t, BUFSIZE = 1440);
    typedef boost::asio::streambuf streambuf;
    typedef streambuf::const_buffers_type const_buffers_type;
    typedef streambuf::mutable_buffers_type mutable_buffers_type;

    mutable_buffers_type prepare(size_t n = BUFSIZE) {
        return buff_.prepare(n);
    }


    void commit(size_t n) {
        buff_.commit(n);
    }


    size_t size() const {
        return buff_.size();
    }


    const char * peek() const {
        return boost::asio::buffer_cast<const char *>(buff_.data());
    }


    void append(const void * data, size_t len) {
        buff_.sputn(static_cast<const char *>(data),
                    boost::numeric_cast<std::streamsize>(len));
    }


    const_buffers_type data() const {
        return buff_.data();
    }


    void consume(size_t n) {
        buff_.consume(n);
    }


private:
    streambuf buff_;
}; // class tcp_buffer;



} // namespace net;
} // namespace iegad;


#endif // __IEGAD_TCP_BUFFER__
