#ifndef __TCP_MT_SVC__
#define __TCP_MT_SVC__


// ============ 说明 ============
//
// @创建日期 : 2015-11-08
// @创建人 : iegad
//
// ============================
// @用途 : 多线程并发 自定义rpc 服务
// @PS : 该服务 使用方式为注册 服务方法, 提供 这些服务方法的类似rpc的调用
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================


#include "tcp_mt_svr.h"
#include <unordered_map>
#include "basic_svc.h"



namespace iegad {
namespace nets {


    class tcp_mt_svc : public tcp_mt_svr {
    // 多线程并发 rpc服务

    public:
	// ============================
	// @用途 : 服务方法映射散列表 声明
	// ============================
	typedef std::unordered_map<int, basic_svc::basic_svc_ptr> map_t;


	// ============================
	// @用途 : 构造函数
	// @port : 端口号
	// ============================
	explicit tcp_mt_svc(unsigned short port);


	// ============================
	// @用途 : 构造函数
	// @host : 机器名或IP地址
	// @svc : 端口号或服务名
	// ============================
	tcp_mt_svc(const std::string & host, const std::string & svc);


	// ============================
	// @用途 : 注册服务
	// @svc : 派生自 basic_svc 的服务 对象指针
	// @返回值 : 成功返回 0, 否则返回 -1;
	// ============================
	int regist_svc(basic_svc::basic_svc_ptr svc);


	// ============================
	// @用途 : 注册服务
	// @clnt : 客户端
	// @msgstr : 客户端发送的 字符串消息
	// @返回值 : 成功返回 0, 否则返回 -1;
	// ============================
	virtual int _action(ip::tcp::socket & clnt, const std::string & msgstr);


    private:
	// 服务方法映射散列表
	map_t map_;
    }; // class tcp_mt_svc;


} // namespace nets;
} // namespace iegad;



#endif // __TCP_MT_SVC__