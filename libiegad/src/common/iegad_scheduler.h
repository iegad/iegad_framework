#ifndef __IEGAD_SCHEDULER__
#define __IEGAD_SCHEDULER__


#include <thread>


namespace iegad {
namespace tools {


    class scheduler {
    public:
	typedef std::function<void()> func_t;
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


} // namespace tools;
} // namespace iegad;



#endif // __IEGAD_SCHEDULER__