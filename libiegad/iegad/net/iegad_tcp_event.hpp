#ifndef __IEGAD_TCP_EVENT__
#define __IEGAD_TCP_EVENT__


#include "net/iegad_tcp_session.hpp"
#include <map>
#include <mutex>




namespace iegad {
namespace net {


class tcp_event {
public:
    typedef std::unique_lock<std::mutex> lock_t;

    explicit tcp_event()
        :
        base_(nullptr)
    {}


    virtual ~tcp_event() {}


    virtual void onConnected(tcp_session::ptr_t session) {
        std::unique_lock<std::mutex> lk(mtx_);
        sm_.insert(std::make_pair(session->sockfd(), session));
    }


    virtual void onReadEof(int sockfd) {
        std::unique_lock<std::mutex> lk(mtx_);
        auto itor = sm_.find(sockfd);
        if (itor != sm_.end()) {
            sm_.erase(itor);
        }
    }


    virtual void onReadErr(int sockfd) {
        std::unique_lock<std::mutex> lk(mtx_);
        auto itor = sm_.find(sockfd);
        if (itor != sm_.end()) {
            sm_.erase(itor);
        }
    }


    virtual int onReadBuff(tcp_session::ptr_t session) = 0;


    event_base * base() {
        return base_;
    }


    void setBase(event_base *base) {
        base_ = base;
    }


    std::map<int, tcp_session::ptr_t> & sm() {
        return sm_;
    }


    void add_session(tcp_session::ptr_t session) {
        lock_t lk(mtx_);
        sm_.insert(std::make_pair(session->sockfd(), session));
    }


    void rm_session(tcp_session::ptr_t session) {
        lock_t lk(mtx_);
        auto itr = sm_.find(session->sockfd());
        if (itr != sm_.end()) {
            sm_.erase(itr);
        }
    }


    void rm_session(int sockfd) {
        lock_t lk(mtx_);
        auto itr = sm_.find(sockfd);
        if (itr != sm_.end()) {
            sm_.erase(itr);
        }
    }


    tcp_session::ptr_t get_session(int sockfd) {
        lock_t lk(mtx_);
        auto itr = sm_.find(sockfd);
        return itr == sm_.end() ? nullptr : itr->second;
    }


private:
    std::mutex mtx_;
    std::map<int, tcp_session::ptr_t> sm_;
    event_base * base_;
};






 // class tcp_event;


}
}



#endif // __IEGAD_TCP_EVENT__
