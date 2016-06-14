#ifndef __JOB_WORKER__
#define __JOB_WORKER__



// ============ 说明 ============
//
// @创建日期 : 2015-10-01
// @创建人 : iegad
//
// ============================
// @用途 :  用来提供 "生产者-消费者 模型" 的工作者
//		 或者消费者工作对象.
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



#include "job_queue.hpp"
#include <thread>
#include <condition_variable>
#include <vector>
#include <functional>


namespace iegad {
namespace tools {


template<typename T>
class worker_t {
// 工作者对象
public:
    // ============================
    // @用途 : 工作者对象 "外部工作线程句柄" 类型声明
    // ============================
    typedef std::function<int(T&)> wkr_handler_t;


    // ============================
    // @用途 : 构造函数
    // @que : 一个指向外部 工作队列缓冲区 的引用
    // @wkr_func : 外部工作线程 的函数对象
    // ============================
    explicit worker_t(job_que_t<T> & que, wkr_handler_t wkr_func)
        : wkr_handler_(wkr_func),que_(que) {}


    // ============================
    // @用途 : 启动工作对象
    // @n : 内部工作线程数
    // @chs : 指定的字符, 可以指定多个字符
    // @返回值 : void
    // ============================
    void run(int n = 4);


    // ============================
    // @用途 : 停止工作对象
    // @返回值 : void
    // @PS : 只有在 工作队列 中没有作业, 和所有工
    //	      作线程 工作结束后, 才会真正的停止工作
    // ============================
    void stop();

private:
    // ============================
    // @用途 : 工作者对象 "内部工作线程"
    // @返回值 : 永远返回 0; 之所以让它返回 0 是为了
    //		    以后的扩展;
    // ============================
    int _thread_proc();


    // 外部工作线程
    wkr_handler_t wkr_handler_;
    // 内部工作线程池
    std::vector<std::thread> thread_pool_;
    // 任务队列缓冲区的引用
    job_que_t<T> & que_;

    // 禁用项
    worker_t(const worker_t &);
    worker_t & operator=(const worker_t &);
}; // class worker_t<T>;




=================  以下为实现部分 ==================



template<typename T>
void iegad::tools::worker_t<T>::run(int n /*= 4*/)
{
    for (int i = 0; i < n; i++) {
        thread_pool_.push_back(
        std::thread(
        std::bind(&worker_t::_thread_proc, this)));
    }
}


template<typename T>
void iegad::tools::worker_t<T>::stop()
{
    // stopped while all the threads take the jobs;
    que_.stop();
    for (int i = 0, n = thread_pool_.size(); i < n; i++) {
        thread_pool_[i].join();
    }
}


template<typename T>
int iegad::tools::worker_t<T>::_thread_proc()
{
    for (;;) {
        T val;
        if (que_.pop(val) == -1 || wkr_handler_ == nullptr) {
            break;
        }
        wkr_handler_(val);
    }
    return 0;
}


} // namespace tools;
} // namespace iegad;


#endif // __JOB_WORKER__
