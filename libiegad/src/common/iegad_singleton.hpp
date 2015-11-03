#ifndef __IEGAD_SINGLETON__
#define __IEGAD_SINGLETON__


#include <boost/noncopyable.hpp>
#include <boost/thread/once.hpp>


namespace iegad {
namespace tools {


    template <class T>
    class singleton : public boost::noncopyable {
    public:
	static T & instance() {
	    boost::call_once(flag_, init);
	    return init();
	}

    private:
	static T & init() {
	    static T obj;
	    return obj;
	}

	static boost::once_flag flag_;
    }; // template <T> class singleton;


    template <class T>
    boost::once_flag iegad::tools::singleton<T>::flag_ = BOOST_ONCE_INIT;


} // namespace tools;
} // namespace iegad;


#endif // __IEGAD_SINGLETON__