#ifndef __JOB_WORKER__
#define __JOB_WORKER__


#include "job_queue.hpp"
#include <thread>
#include <condition_variable>
#include <vector>
#include <functional>


namespace iegad {
namespace tools {


    template<typename T>
    class worker_t {
    public:
	typedef std::function<int(T&)> wkr_thread_t;

	explicit worker_t(job_que_t<T> & que, wkr_thread_t wkr_func)
	    : que_(que), wkr_handler_(wkr_func) {}

	void run(int n = 4) {
	    for (int i = 0; i < n; i++) {
		thread_pool_.push_back(
		    std::thread(
		    std::bind(&worker_t::_thread_proc, this)));
	    }
	}

	void stop() {
	    //wkr_handler_ = nullptr;
	    que_.stop();

	    for (int i = 0, n = thread_pool_.size(); i < n; i++) {
		thread_pool_[i].join();
	    }
	}

    private:
	int _thread_proc() {

	    for (;;) {
		T val;
		if (que_.pop(val) == -1 || wkr_handler_ == nullptr) {
		    break;
		}
		wkr_handler_(val);
	    }

	    return 0;
	}

	job_que_t<T> & que_;
	std::vector<std::thread> thread_pool_;
	wkr_thread_t wkr_handler_;

	worker_t(const worker_t &);
	worker_t & operator=(const worker_t &);
    };


} // namespace tools;
} // namespace iegad;


#endif // __JOB_WORKER__