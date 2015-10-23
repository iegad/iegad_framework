#ifndef __JOB_QUEUE__
#define __JOB_QUEUE__


// ============ 说明 ============
//
// @创建日期 : 2015-10-01
// @创建人 : iegad
//
// ============================
// @用途 :  用来提供 "生产者-消费者 模型" 的工作缓冲队列.
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================


#include <mutex>
#include <deque>
#include <condition_variable>


namespace iegad {
namespace common {


    template<typename T>
    class job_que_t {
    // 工作队列, 基于C++11
    public:
	// ============================
	// @用途 : 构造函数
	// ============================
	explicit job_que_t()
	    : stop_flag_(false) 
	{}


	// ============================
	// @用途 : 停止工作队列, 这样 push 将不在有效, 但
	//		只有当工作队列中的所有任务都完成后, 
	//		队列才真正的停止工作.
	// @返回值 : void
	// ============================
	void stop();


	// ============================
	// @用途 : 将 任务 val 加入队尾
	// @val : 加入队列的 任务, 这里需要注意, 使用时, 这里
	//	     将使用对象传递的方式, 如果是非基础类型, 应
	//	     使用智能指针
	// @返回值 : void
	// ============================
	void push(T val);


	// ============================
	// @用途 : 将 任务 val 从队首取出
	// @val : 用来存放 队首任务值 的 对象
	// @返回值 : 成功取值, 返回 0, 否则返回 -1;
	// ============================
	int pop(T & val);


	// ============================
	// @用途 : 判断队列是否为空
	// @返回值 : 空返回 true, 否则返回 false
	// ============================
	bool empty();


	// ============================
	// @用途 : 返回队列内的任务元素个数
	// @返回值 : 队列现有元素的个数
	// ============================
	size_t size();
	
    private:
	// 停止标志
	bool stop_flag_;
	// 互斥量, 使在多线程操作中, 该队列的成员函数将会是线程安全的.
	std::mutex mtx_;
	// 条件变量, pop调用的等态条件
	std::condition_variable cv_;
	// 内置 std::deque 对象
	std::deque<T> que_;

	// 禁用
	job_que_t(const job_que_t &);
	job_que_t & operator=(const job_que_t &);
    }; // class job_que_t<T>



// ==================  以下为实现部分 ==================



    template<typename T>
    void iegad::common::job_que_t<T>::stop()
    {
	stop_flag_ = true;
	cv_.notify_all();
    }


    template<typename T>
    void iegad::common::job_que_t<T>::push(T val)
    {
	std::unique_lock<std::mutex> locker(mtx_);
	if (stop_flag_) {
	    return;
	}
	que_.push_back(val);
	cv_.notify_one();
    }


    template<typename T>
    int iegad::common::job_que_t<T>::pop(T & val)
    {
	std::unique_lock<std::mutex> locker(mtx_);
	while (!stop_flag_ && que_.empty()) {
	    cv_.wait(locker);
	}

	if (stop_flag_ && que_.empty()) {
	    // job queue stop while singal stopped and all job done;
	    return -1;
	}

	val = que_.front();
	que_.pop_front();
	return 0;
    }


    template<typename T>
    bool iegad::common::job_que_t<T>::empty()
    {
	std::unique_lock<std::mutex> locker(mtx_);
	return que_.empty();
    }


    template<typename T>
    size_t iegad::common::job_que_t<T>::size()
    {
	std::unique_lock<std::mutex> locker(mtx_);
	return que_.size();
    }


} // namespace common;
} // namespace iegad;


#endif // __JOB_QUEUE__