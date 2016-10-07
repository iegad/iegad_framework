#ifndef __IEGAD_SIGAR__
#define __IEGAD_SIGAR__


#include <sigar.h>
#include <boost/assert.hpp>



namespace iegad {
namespace sigar_ex {


class sigar {
public:
    explicit sigar() :
        s_(NULL) {
        BOOST_ASSERT(sigar_open(&s_));
    }


    ~sigar() {
        sigar_close(s_);
    }


private:
    sigar_t * s_;
}; // class sigar;



} // namespace sigar_ex;
} // namespace iegad;


#endif // __IEGAD_SIGAR__
