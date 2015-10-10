#ifndef __BASIC_SERVICE__
#define __BASIC_SERVICE__



// ============ 说明 ============
//
// @创建日期 : 2015-10-01
// @创建人 : iegad
//
// ============================
// @用途 : 该服务基类, 是所有服务类对象的基类, 
//		1. 统一调用接口
//		2. 提供一些公共的函数
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================
//  -- 2015-10-10	--iegad			1. 为_response 加入加密功能



#include <string>
#include <memory>
#include <unordered_map>
#include "msg/iegad_io_msg.h"


namespace iegad {
namespace net {

    
    class basic_svc {
    // 服务对象基础类, 抽象类;
    public:
	// ============================
	// @用途 : 类本身的智能指针 类型声明
	// ============================
	typedef std::shared_ptr<basic_svc> basic_svc_ptr;


	// ============================
	// @用途 : "服务ID 与服务对象 映射表" 类型声明
	// ============================
	typedef std::unordered_map<int, basic_svc_ptr> svc_map_t;
	

	// ============================
	// @用途 : 从 映射表 svc_map 中找到 对应 svc_id 的服务对象指针
	// @svc_id : 服务ID
	// @svc_map : 映射表
	// @返回值 : 成功返回 svc对象智能指针, 否则, 返回一个nullptr的智能指针
	// ============================
	static basic_svc_ptr get_svc(int svc_id, svc_map_t & svc_map);


	// ============================
	// @用途 : 把 服务对象 svc_obj 加到 映射表 svc_map 中;
	// @svc_obj : 服务对象的智能指针
	// @svc_map : 映射表
	// @返回值 : 成功返回 0, 否则返回 -1; 
	// @PS : 错误的情况只会是 两个服务对象 具有相同的服务ID;
	// ============================
	static int regist_svc(const basic_svc_ptr & svc_obj, svc_map_t & svc_map);


	// ============================
	// @用途 : 执行 tcp客户端 clnt 所发送的请求服务;
	// @clnt : 请求服务的tcp客户端;
	// @msg_flag : 消息标志 msg_baisc::msg_flag
	// @msg_bdstr : 子消息的构造字符串
	// @返回值 : 调用成功返回 0, 否则返回 -1; 
	// @PS : 服务对象的调用接口, 该函数为纯虚函数, 由实际的服务对象(派生类)实现;
	// ============================
	virtual int action(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr) = 0;


	// ============================
	// @用途 : 返回该对象的服务ID
	// @返回值 : 返回服务ID
	// ============================
	int get_id();

    protected:
	// ============================
	// @用途 : 构造函数
	// @svc_id : 服务ID
	// ============================
	explicit basic_svc(int svc_id);


	// ============================
	// @用途 : 析构函数
	// ============================
	virtual ~basic_svc() {}


	// ============================
	// @用途 : 通过 子消息构造字符串 msgbdstr 来构建 子消息 msg;
	// @msgbdstr : 子消息的构造字符串
	// @msg : 子消息对象
	// @返回值, 成功构建返回0, 否则, 返回-1;
	// @PS : 模板参数 __MSG_T 一定要是由google protocol buffer
	//		所生成的msg对象类型.
	// ============================
	template <class __MSG_T> 
	int _build_svc(const std::string & msgbdstr, __MSG_T & msg);


	// ============================
	// @用途 : 向tcp客户端 clnt 发送 消息 msg;
	// @clnt : tcp 客户端
	// @flag : 消息标志, basic_msg::msg_flag
	// @msg : 子消息类型;
	// @返回值, 发送成功返回0, 否则, 返回-1;
	// @PS : 模板参数 __MSG_T 一定要是由google protocol buffer
	//		所生成的msg对象类型.
	// ============================
	template <class __MSG_T>
	int _send_msg(boost::asio::ip::tcp::socket & clnt, int flag, const __MSG_T & msg);


	// ============================
	// @用途 : 向tcp客户端 clnt 作出应答;;
	// @clnt : tcp 客户端
	// @rzt : 应答的结果, 规则由自己定义
	// @rzt_size : 应答数据的长度
	// @返回值, 发送成功发送的字节数
	// @PS : 为了提高通信从而添加该函数, 这样便可以
	//		每次应答时, 都构建一个basic_msg对象;
	// ============================
	int _response(boost::asio::ip::tcp::socket & clnt, const char * rzt, size_t rzt_size);

    private:
	// 服务ID
	int svc_id_;
    }; // class basic_svc



// ==================  以下为实现部分 ==================


    template <class __MSG_T>
    int iegad::net::basic_svc::_build_svc(const std::string & msgbdstr, __MSG_T & msg)
    {
	msg.Clear();
	return msg.ParseFromString(msgbdstr) ? 0 : -1;
    }


    template <class __MSG_T>
    int iegad::net::basic_svc::_send_msg(boost::asio::ip::tcp::socket & clnt, int flag, const __MSG_T & msg)
    {
	std::string msg_str;

	if (!msg.SerializeToString(&msg_str)) {
	    return -1;
	}

	basic_msg msgbsc;
	msgbsc.set_msg_type(this->svc_id_);
	msgbsc.set_msg_flag(flag);
	msgbsc.set_msg_bdstr(msg_str);
	return iegad::net::send_basic_msg(clnt, msgbsc);
    }


} // namespace net;
} // namespace iegad;


#endif // __BASIC_SERVICE__