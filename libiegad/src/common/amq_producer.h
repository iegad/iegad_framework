#ifndef __AMQ_PRODUCER__
#define __AMQ_PRODUCER__



// ============ 说明 ============
//
// @创建日期 : 2016-02-21
// @创建人 : iegad
//
// ============================
// @用途 : 1, 对ActiveMQ-CMS 的扩展, 封装一个 "生产者" 方便开发使用.
//		
// @PS : 该文件依赖于 ActiveMQ-CMS 开源库
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



#include <activemq/library/ActiveMQCPP.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <decaf/lang/Runnable.h>
#include <functional>



namespace iegad {
namespace amq_ex {


    class AMQ_Producer {
    //AMQ 生产者
    public:
	// ============================
	// @用途 : 构造函数
	// @PS : 提供默认构造的目的是可以结和 单例模板 来使用.
	// ============================
	AMQ_Producer() {}


	// ============================
	// @用途 : 析构函数
	// ============================
	~AMQ_Producer() { this->_cleanup(); }


	// ============================
	// @用途 : 打开与AMQ对象的连接
	// @errstr : 错误信息 : out参数
	// @返回值 : 成功返回 true, 否则返回 false.
	// ============================
	bool Open(std::string * errstr = nullptr);


	// ============================
	// @用途 : 发送 string 类型消息
	// @serstr : 需要发送的字符串
	// @errstr : 错误信息 : out参数
	// @返回值 : 发送成功返回 true, 否则返回 false.
	// ============================
	bool SendString(const std::string & serstr, std::string * errstr = nullptr);


	// ============================
	// @用途 : 发送 二进制数据 类型消息
	// @buff : 数据缓冲区指针
	// @buffSize : 数据缓冲区大小
	// @errstr : 错误信息 : out参数
	// @返回值 : 发送成功返回 true, 否则返回 false.
	// ============================
	bool SendBytes(char * buff, size_t buffSize, std::string * errstr = nullptr);


	// ============================
	// @用途 : 设置环境
	// @uri : AMQ的URI.	    例 : "tcp://localhost:61616"
	// @persistent_flag : 执行化标志, 0表示执久化, 1 表示非执久化
	// @amq_name : AMQ队列名称
	// @usr : 错误信息 : 用户名
	// @pwd : 错误信息 : 密码
	// @connTimeout : 连接超时  单位 : 毫秒
	// @sendTimeout : 发送超时  单位 : 毫秒
	// @返回值 : void
	// ============================
	void SetEvironment(const std::string & uri, 
	    int persistent_flag, 
	    const std::string & amq_name,
	    const std::string & usr = "", 
	    const std::string & pwd = "", 
	    int32_t connTimeout = -1, 
	    int32_t sendTimeout = -1);


	// ============================
	// @用途 : 获取BrokerUri
	// @返回值 : 当前BrokerUri
	// ============================
	const std::string & GetBrokerURI() const {
	    return uri_;
	}


	// ============================
	// @用途 : 获取 身份验证 : 用户名
	// @返回值 : 当前用户名
	// ============================
	const std::string & GetUser() const {
	    return usr_;
	}


	// ============================
	// @用途 : 获取 身份验证 : 密码
	// @返回值 : 当前密码
	// ============================
	const std::string & GetPassword() const {
	    return pwd_;
	}


	// ============================
	// @用途 : 获取 连接超时
	// @返回值 : 连接超时
	// ============================
	int32_t GetConnectTimeout() const {
	    return connTimeout_;
	}


	// ============================
	// @用途 : 获取 发送超时
	// @返回值 : 发送超时
	// ============================
	int32_t GetSendTimeout() const {
	    return sendTimeout_;
	}

    private:
	// ============================
	// @用途 : 构造BrokerUri串
	// @返回值 : 构造成功的BrokerUri字符串
	// ============================
	const std::string _make_broker_uri();


	// ============================
	// @用途 : 初始化内部字段值
	// @返回值 : void
	// ============================
	void _init();


	// ============================
	// @用途 : 释放所占用的系统资源
	// @返回值 : void
	// ============================
	void _cleanup();


	// 序列化标志
	int persistent_flag_;
	// 连接超时
	int32_t connTimeout_;
	// 发送超时
	int32_t sendTimeout_;

	// 连接对象指针
	std::shared_ptr<::cms::Connection> conn_;
	// 会话指针
	std::shared_ptr<::cms::Session> session_;
	// 目标指针
	std::shared_ptr<::cms::Destination> dest_;
	// 生产者指针
	std::shared_ptr<::cms::MessageProducer> producer_;
	// AMQ连接对象工厂
	std::shared_ptr<::activemq::core::ActiveMQConnectionFactory> connFactory_;

	// 用户名
	std::string usr_;
	// 密码
	std::string pwd_;
	// BrokerUri
	std::string uri_;
	// AMQ队列名称
	std::string amq_name_;
    }; // class AMQ_Producer;


} // namespace amq_ex;
} // namespace iegad;


#endif // __AMQ_PRODUCER__