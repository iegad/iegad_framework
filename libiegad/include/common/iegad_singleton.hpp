#ifndef __IEGAD_SINGLETON__
#define __IEGAD_SINGLETON__


// ============ ˵�� ============
//
// @�������� : 2015-11-07
// @������ : iegad
//
// ============================
// @��; :  �ڶ��̻߳�����, �ṩһ���̰߳�ȫ�ĵ�������ָ��
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================


#include <boost/noncopyable.hpp>
#include <boost/thread/once.hpp>


namespace iegad {
namespace tools {


    template <typename T>
    class singleton : public boost::noncopyable {
    // �̰߳�ȫ�ĵ���ģ��
    public:
	// ============================
	// @��; : �ṩһ���̰߳�ȫ�ĵ�������ָ��
	// @����ֵ : ���� ģ������ ���̰߳�ȫ�ĵ��� ָ��
	// ============================
	static T * instance();

    private:
	// ============================
	// @��; : �ṩһ���̰߳�ȫ�ĵ�������ָ��
	// @����ֵ : ���� ģ������ ���̰߳�ȫ�ĵ��� ָ��
	// @PS : �ڲ�����
	// ============================
	static T * _init();

	static boost::once_flag flag_;
    }; // template <T> class singleton;



// =========================== ������ʵ�ֲ��� ============================


    template <typename T>
    T * iegad::tools::singleton<T>::instance()
    {
	boost::call_once(flag_, _init);
	return _init();
    }


    template <typename T>
    T * iegad::tools::singleton<T>::_init()
    {
	static T obj;
	return &obj;
    }


    template <typename T>
    boost::once_flag iegad::tools::singleton<T>::flag_ = BOOST_ONCE_INIT;


} // namespace tools;
} // namespace iegad;


#endif // __IEGAD_SINGLETON__