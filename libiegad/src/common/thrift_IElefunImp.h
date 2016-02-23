#ifndef __THRIFT_IELEFUNIMP__
#define __THRIFT_IELEFUNIMP__


#include "gen-cpp/Astroboy_types.h"


namespace iegad {
namespace thrift_ex {


    class IElefunImp {
    public:
	explicit IElefunImp(int32_t fun_no = -1)
	    : fun_no_(fun_no) {}

	virtual ~IElefunImp() = 0 {}

	int32_t No() {
	    return fun_no_;
	}

	void Action(res_t * _return, const req_t & obj);

    protected:
	virtual int32_t _action(const std::string & msgstr, int32_t msgsize, std::string * resstr, int32_t * ressize, int32_t * errcode, std::string * errstr) = 0;

    private:

	bool _md5_check(const std::string & msgstr, int32_t msgsize, const std::string & md5str);
	bool _sha1_check(const std::string & md5str, const std::vector<int32_t> & sha1vct);

	int32_t fun_no_;

	IElefunImp(IElefunImp &);
	IElefunImp& operator=(IElefunImp &);
    }; // class IElefunImp;


} // namespace thrift_ex;
} // namespace iegad;



#endif // __THRIFT_IELEFUNIMP__