#ifndef __IEGAD_IOSERVICE_POOL__
#define __IEGAD_IOSERVICE_POOL__



#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/thread.hpp>
#include <boost/functional/factory.hpp>



namespace  iegad {
namespace net {


class ioservice_pool : boost::noncopyable {
public:
    typedef boost::asio::io_service io_service;
    typedef boost::ptr_vector<io_service> ios_vct_t;
    typedef boost::ptr_vector<io_service::work> work_vct_t;


    explicit ioservice_pool(int n = 4)
        :
          next_ios_(0) {
        _init(n);
    }


    io_service & get() {
        if (next_ios_ >= iosVct_.size()) {
            next_ios_ = 0;
        }
        return iosVct_[next_ios_++];
    }


    void start() {
        if (threads_.size() > 0) {
            return;
        }
        for (int i = 0, n = iosVct_.size(); i < n; i++) {
            threads_.create_thread(boost::bind(&io_service::run, boost::ref(iosVct_[i])));
        }
    }

    void run() {
        this->start();
        threads_.join_all();
    }


    void stop() {
        wkVct_.clear();
        std::for_each(iosVct_.begin(), iosVct_.end(),
                      boost::bind(&io_service::stop, boost::placeholders::_1));
    }


private:
    void _init(int n) {
        for (int i = 0; i < n; i++) {
            iosVct_.push_back(boost::factory<io_service *>()());
            wkVct_.push_back(boost::factory<io_service::work *>()(iosVct_.back()));
        }
    }


    ios_vct_t iosVct_;
    work_vct_t wkVct_;

    boost::thread_group threads_;
    size_t next_ios_;
}; // class ioservice_pool;


} // namespace net;
} // namespace iegad;

#endif // __IEGAD_IOSERVICE_POOL__
