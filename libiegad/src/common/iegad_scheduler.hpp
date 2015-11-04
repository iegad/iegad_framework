#ifndef __IEGAD_SCHEDULER__
#define __IEGAD_SCHEDULER__


#include <thread>
#include <assert.h>


namespace iegad {
namespace tools {


    template <typename __TParam>
    class scheduler {
    public:
	typedef std::function<void(__TParam &)> func_t;
	scheduler(func_t func, int ms);
	   
	void run();
	void stop();

    private:
	bool stop_flag_;
	int ms_;
	func_t func_;
	std::thread * thrd_;

	scheduler(const scheduler &);
	scheduler & operator=(const scheduler &);
    }; // class scheduler;


// =============================== 以下是实现部分 ===============================


    template <typename __TParam>
    void iegad::tools::scheduler<__TParam>::stop()
    {
	stop_flag_ = true;
    }


    template <typename __TParam>
    void iegad::tools::scheduler<__TParam>::run()
    {
	stop_flag_ = true;
	for (;;) {
	    if (stop_flag_) {
		break;
	    }
	    func_();
	    std::this_thread::sleep_for(std::chrono::milliseconds(ms_));
	}
    }


    template <typename __TParam>
    iegad::tools::scheduler<__TParam>::scheduler(func_t func, int ms)
	: stop_flag_(false), ms_(ms), func_(func)
    {
	assert(ms > 0);
    }


} // namespace tools;
} // namespace iegad;



#endif // __IEGAD_SCHEDULER__