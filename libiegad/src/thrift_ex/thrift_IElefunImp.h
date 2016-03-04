#ifndef __THRIFT_IELEFUNIMP__
#define __THRIFT_IELEFUNIMP__



// ============ 说明 ============
//
// @创建日期 : 2016-02-28
// @创建人 : iegad
//
// ============================
// @用途 : IElefunImp 是一个抽象类, 
//		IElefunImp 为所有RPC接口实现类的基类
//
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



#include "gen-cpp/Astroboy_types.h"



namespace iegad {
namespace thrift_ex {


    class IElefunImp {
    // 接口实现对象基类, 抽象类
    public:
	// ============================
	// @用途 : 构造函数.
	// @fun_no : 功能号, 
	// @PS : 默认值为-1, 为失败的的功能号, 
	// ============================
	explicit IElefunImp(int32_t fun_no = -1)
	    : fun_no_(fun_no) {}


	// ============================
	// @用途 : 析构函数.
	// ============================
	virtual ~IElefunImp() = 0 {}


	// ============================
	// @用途 : 返回功能号.
	// @返回值 : 返回该实现类的功能号, -1为失效的功能号
	// ============================
	int32_t No() {
	    return fun_no_;
	}


	// ============================
	// @用途 : 执行请求, 并返回应答消息
	// @_return : 应答消息, out参数
	// @obj : 请求消息对象
	// @返回值 : void
	// @PS : 该函数为统一调用接口
	// ============================
	void Action(res_t * _return, const req_t & obj);


    protected:
	// ============================
	// @用途 : 请求处理函数
	// @msgstr : 请求消息数据
	// @msgsize : 请求消息数据串长度
	// @resstr : 应答数据
	// @ressize : 应答数据长度
	// @errcode : 错误信息code
	// @errstr : 错误信息字符串
	// @返回值 : 应答类型
	// @PS : 该函数为纯虚函数, 由派生类实现.
	// ============================
	virtual int32_t _action(const std::string & msgstr, int32_t msgsize, std::string * resstr, int32_t * ressize, int32_t * errcode, std::string * errstr) = 0;


    private:
	// ============================
	// @用途 : 用来校验 请求数据 是否与发送来的md5str匹配, 一级校验
	// @msgstr : 需要校验的数据
	// @msgsize : 需要校验的数据长度
	// @md5str : 用来比对的 MD5字符串
	// @返回值 : 如果校验成功, 返回 true, 否则返回 false
	// ============================
	bool _md5_check(const std::string & msgstr, int32_t msgsize, const std::string & md5str);


	// ============================
	// @用途 : 用来校验 md5str 是否与发送来的sha1vct匹配, 二级校验
	// @md5str : 需要校验的MD5字符串
	// @sha1vct : 用来比对的 SHA1数据
	// @返回值 : 如果校验成功, 返回 true, 否则返回 false
	// ============================
	bool _sha1_check(const std::string & md5str, const std::vector<int32_t> & sha1vct);


	//功能号
	int32_t fun_no_;


	// ============================
	// @用途 : 防止赋值操作
	// ============================
	IElefunImp(IElefunImp &);
	IElefunImp& operator=(IElefunImp &);
    }; // class IElefunImp;


} // namespace thrift_ex;
} // namespace iegad;



#endif // __THRIFT_IELEFUNIMP__