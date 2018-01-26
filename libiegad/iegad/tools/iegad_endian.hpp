#ifndef __IEGAD_ENDIAN__
#define __IEGAD_ENDIAN__


#ifdef ihtons
#error "ihtons is alread exists"
#endif
#define ihtons(x)  ((((unsigned short)(x) & 0xff00) >> 8) | (((unsigned short)(x) & 0x00ff) << 8))


#ifdef ihtonl
#error "ihtonl is alread exists"
#endif
#define ihtonl(x)  ((unsigned int)htons(x) << 16) + (unsigned int)htons((x) >> 16)


#ifdef ihtonll
#error "ihtonll is alread exists"
#endif
#define ihtonll(x) ((((unsigned long long)htonl(x)) << 32) + htonl((x) >> 32))



namespace iegad {


class endian {
public:
    static unsigned short
    to_little(unsigned short n)
    {
        return is_big() ? ihtons(n) : n;
    }


    static unsigned int
    to_little(unsigned int n)
    {
        return is_big() ? ihtonl(n) : n;
    }


    static unsigned long long
    to_little(unsigned long long n)
    {
        return is_big() ? ihtonll(n) : n;
    }


    static unsigned short
    to_big(unsigned short n)
    {
        return is_big() ? n : ihtons(n);
    }


    static unsigned int
    to_big(unsigned int n)
    {
        return is_big() ? n: ihtonl(n);
    }


    static unsigned long long
    to_big(unsigned long long n)
    {
        return is_big() ? n : ihtonll(n);
    }


    static bool 
    is_big()
    {
        union {
            int n;
            unsigned char a[4];
        } t;

        t.n = 0x01020304;
        return t.a[0] == 0x01;
    }


private:
    endian();
}; // class endian;


} // namespace iegad;



#endif // __IEGAD_ENDIAN__
