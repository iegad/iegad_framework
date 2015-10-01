#ifndef __JOB_QUEUE__
#define __JOB_QUEUE__


// ============ ˵�� ============
//
// @�������� : 2015-10-01
// @������ : iegad
//
// ============================
// @��; :  �����ṩ "������-������ ģ��" �Ĺ����������.
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================


#include <mutex>
#include <deque>
#include <condition_variable>


namespace iegad {
namespace common {


    template<typename T>
    class job_que_t {
    // ��������, ����C++11
    public:
	// ============================
	// @��; : ���캯��
	// ============================
	explicit job_que_t()
	    : stop_flag_(false) 
	{}


	// ============================
	// @��; : ֹͣ��������, ���� push ��������Ч, ��
	//		ֻ�е����������е�����������ɺ�, 
	//		���в�������ֹͣ����.
	// @����ֵ : void
	// ============================
	void stop();


	// ============================
	// @��; : �� ���� val �����β
	// @val : ������е� ����, ������Ҫע��, ʹ��ʱ, ����
	//	     ��ʹ�ö��󴫵ݵķ�ʽ, ����Ƿǻ�������, Ӧ
	//	     ʹ������ָ��
	// @����ֵ : void
	// ============================
	void push(T val);


	// ============================
	// @��; : �� ���� val �Ӷ���ȡ��
	// @val : ������� ��������ֵ �� ����
	// @����ֵ : �ɹ�ȡֵ, ���� 0, ���򷵻� -1;
	// ============================
	int pop(T & val);


	// ============================
	// @��; : �ж϶����Ƿ�Ϊ��
	// @����ֵ : �շ��� true, ���򷵻� false
	// ============================
	bool empty();


	// ============================
	// @��; : ���ض����ڵ�����Ԫ�ظ���
	// @����ֵ : ��������Ԫ�صĸ���
	// ============================
	size_t size();
	
    private:
	// ֹͣ��־
	bool stop_flag_;
	// ������, ʹ�ڶ��̲߳�����, �ö��еĳ�Ա�����������̰߳�ȫ��.
	std::mutex mtx_;
	// ��������, pop���õĵ�̬����
	std::condition_variable cv_;
	// ���� std::deque ����
	std::deque<T> que_;

	// ����
	job_que_t(const job_que_t &);
	job_que_t & operator=(const job_que_t &);
    }; // class job_que_t<T>



// ==================  ����Ϊʵ�ֲ��� ==================



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