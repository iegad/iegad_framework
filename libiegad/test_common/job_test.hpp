#ifndef __JOB_TEST__
#define __JOB_TEST__


#include "common/job_worker.hpp"
#include <iostream>
#include <string>
#include <assert.h>




class job_test {
public:
    static int get_string(const std::string & val) {
	std::cout << "\t" << val << std::endl;
	return 0;
    }

    explicit  job_test(std::string * test_str, int n)
	: test_str_(nullptr), n_(n), que_(), wkr_(que_, job_test::get_string) {
	test_str_ = test_str;
    }

    void run() {
	std::thread t(std::bind(&job_test::_thread_push, this));
	wkr_.run();
	t.join();
	wkr_.stop();
    }

private:
    void _thread_push() {
	assert(test_str_ != nullptr);
	for (int i = 0; i < n_; i++) {
	    que_.push(std::move(test_str_[i]));
	    std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
    }

    std::string * test_str_;
    int n_;
    iegad::tools::job_que_t<std::string> que_;
    iegad::tools::worker_t<std::string> wkr_;
};



#endif // __JOB_TEST__