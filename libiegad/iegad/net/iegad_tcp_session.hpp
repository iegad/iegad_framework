#ifndef __IEGAD_TCP_SESSION__
#define __IEGAD_TCP_SESSION__



#include <string>
#include <memory.h>
#include <memory>
#include <unistd.h>
#include <event.h>



namespace iegad {
namespace net {


class tcp_session {
public:
    typedef std::shared_ptr<tcp_session> ptr_t;


    int send(const char * buff, size_t buffsize) {
        int nleft = buffsize;
        int n;
        const char * p = buff;
        while (nleft != 0) {
            n = write(sockfd_, p, nleft);
            if (n < 0) {
                return -1;
            }
            p += n;
            nleft -= n;
        }
        return buffsize - nleft;
    }


    explicit tcp_session(int sockfd, event * ev)
        :
        sockfd_(sockfd),
        read_event_(ev)
    {}


    explicit tcp_session(int sockfd, const std::string & recvstr)
        :
        sockfd_(sockfd),
        read_event_(nullptr),
        recvbuff_(recvstr)
    {}


    ~tcp_session() {
        if (sockfd_ != -1) {
            close(sockfd_);
        }
        if (read_event_ != nullptr) {
            event_del(read_event_);
            read_event_ = nullptr;
        }
    }


    int sockfd() const {
        return sockfd_;
    }


    void reset_sockfd() {
        if (sockfd_ != -1) {
            close(sockfd_);
            sockfd_ = -1;
        }
    }


    std::string & recvbuff() {
        return recvbuff_;
    }


    void setMsgbuff(const std::string &recvbuff) {
        recvbuff_ = recvbuff;
    }


    event * read_event() const {
        return read_event_;
    }


private:
    int sockfd_;
    event * read_event_;
    std::string recvbuff_;
}; // class tcp_session;


} // namespace net;
} // namespace iegad;



#endif // __IEGAD_TCP_SESSION__
