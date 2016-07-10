#ifndef __IEGAD_ENDIAN__
#define __IEGAD_ENDIAN__


#include <netinet/in.h>
#if (__linux__)
#define htonll(x) ((((uint64_t)htonl(x)) << 32) + htonl((x) >> 32))
#endif



namespace iegad {


class endian {
public:
    static uint16_t to_little16(uint16_t n) {
        return is_little() ? htons(n) : n;
    }

    static uint32_t to_little32(uint32_t n) {
        return is_little() ? htonl(n) : n;
    }


    static uint64_t to_little64(uint64_t n) {
        return is_little() ? htonll(n) : n;
    }

    static bool is_little() {
        uint16_t n = 0x0102;
        char * p = (char *)&n;
        return p[0] == 0x02 && p[1] == 0x01;
    }

private:
    endian();
}; // class endian;


} // namespace iegad;



#endif // __IEGAD_ENDIAN__
