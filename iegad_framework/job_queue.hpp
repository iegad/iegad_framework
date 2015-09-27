#ifndef __JOB_QUEUE__
#define __JOB_QUEUE__






#include <mutex>
#include <deque>
#include <condition_variable>


namespace iegad {
namespace tools {


    template<typename T>
    class job_que_t {
    public:
	explicit job_que_t()
	    : stop_flag_(false) 
	{}

	void stop() {
	    stop_flag_ = true;
	    cv_.notify_all();
	}

	void push(T val) {
	    std::unique_lock<std::mutex> locker(mtx_);
	    que_.push_back(val);
	    cv_.notify_one();
	}

	int pop(T & val) {
	    std::unique_lock<std::mutex> locker(mtx_);
	    while (!stop_flag_ && que_.empty()) {
		cv_.wait(locker);
	    }

	    if (stop_flag_ && que_.empty()) {
		return -1;
	    }

	    val = que_.front();
	    que_.pop_front();
	    return 0;
	}

	bool empty() { 
	    std::unique_lock<std::mutex> locker(mtx_);
	    return que_.empty();
	}

	size_t size() {
	    std::unique_lock<std::mutex> locker(mtx_);
	    return que_.size();
	}

	std::mutex & get_mtx() {
	    return mtx_; 
	}
	
    private:
	bool stop_flag_;
	std::mutex mtx_;
	std::condition_variable cv_;
	std::deque<T> que_;

	job_que_t(const job_que_t &);
	job_que_t & operator=(const job_que_t &);
    }; // class job_que_t<T>


} // namespace tools;
} // namespace iegad;


#endif // __JOB_QUEUE__