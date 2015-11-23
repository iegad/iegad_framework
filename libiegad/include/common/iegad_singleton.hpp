#ifndef __IEGAD_SINGLETON__
#define __IEGAD_SINGLETON__


// ============ 说明 ============
//
// @创建日期 : 2015-11-07
// @创建人 : iegad
//
// ============================
// @用途 :  在多线程环境下, 提供一个线程安全的单例对象指针
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================


#include <boost/noncopyable.hpp>
#include <boost/thread/once.hpp>


namespace iegad {
namespace tools {


    template <typename T>
    class singleton : public boost::noncopyable {
    // 线程安全的单例模板
    public:
	// ============================
	// @用途 : 提供一个线程安全的单例对象指针
	// @返回值 : 返回 模板类型 的线程安全的单例 指针
	// ============================
	static T * instance();

    private:
	// ============================
	// @用途 : 提供一个线程安全的单例对象指针
	// @返回值 : 返回 模板类型 的线程安全的单例 指针
	// @PS : 内部调用
	// ============================
	static T * _init();

	static boost::once_flag flag_;
    }; // template <T> class singleton;



// =========================== 以下是实现部分 ============================


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