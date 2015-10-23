#ifndef __JOB_WORKER__
#define __JOB_WORKER__



// ============ ˵�� ============
//
// @�������� : 2015-10-01
// @������ : iegad
//
// ============================
// @��; :  �����ṩ "������-������ ģ��" �Ĺ�����
//		 ���������߹�������.
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
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
    // �����߶���
    public:
	// ============================
	// @��; : �����߶��� "�ⲿ�����߳̾��" ��������
	// ============================
	typedef std::function<int(T&)> wkr_handler_t;


	// ============================
	// @��; : ���캯��
	// @que : һ��ָ���ⲿ �������л����� ������
	// @wkr_func : �ⲿ�����߳� �ĺ�������
	// ============================
	explicit worker_t(job_que_t<T> & que, wkr_handler_t wkr_func)
	    : que_(que), wkr_handler_(wkr_func) {}


	// ============================
	// @��; : ������������
	// @n : �ڲ������߳���
	// @chs : ָ�����ַ�, ����ָ������ַ�
	// @����ֵ : void
	// ============================
	void run(int n = 4);


	// ============================
	// @��; : ֹͣ��������
	// @����ֵ : void
	// @PS : ֻ���� �������� ��û����ҵ, �����й�
	//	      ���߳� ����������, �Ż�������ֹͣ����
	// ============================
	void stop();

    private:
	// ============================
	// @��; : �����߶��� "�ڲ������߳�"
	// @����ֵ : ��Զ���� 0; ֮������������ 0 ��Ϊ��
	//		    �Ժ����չ;
	// ============================
	int _thread_proc();

	// �ⲿ�����߳�
	wkr_handler_t wkr_handler_; 
	// �ڲ������̳߳�
	std::vector<std::thread> thread_pool_;
	// ������л�����������
	job_que_t<T> & que_;

	// ������
	worker_t(const worker_t &);
	worker_t & operator=(const worker_t &);
    }; // class worker_t<T>;




// ==================  ����Ϊʵ�ֲ��� ==================
    


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