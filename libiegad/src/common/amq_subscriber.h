#ifndef __AMQ_SUBSCRIBER__
#define __AMQ_SUBSCRIBER__



// ============ 说明 ============
//
// @创建日期 : 2016-02-21
// @创建人 : iegad
//
// ============================
// @用途 : 1, 对ActiveMQ-CMS 的扩展, 封装一个 "订阅者" 方便开发使用.
//		
// @PS : 该文件依赖于 ActiveMQ-CMS 开源库
//
// @bugs : 目前 "发布-订阅"模式 不支持久化, 关键问题出在 订阅者.
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



#include "amq_baser.h"
#include "amq_iworker.hpp"



namespace iegad {
namespace cms_ex {


    class AMQ_Subscriber : public AMQ_Baser, public AMQ_IRecver {
	//AMQ 订阅者, 派生自 AMQ_Baser, 并继承AMQ_IRecver 接口
    public:
	// ============================
	// @用途 : 构造函数
	// @PS : 提供默认构造的目的是可以结和 单例模板 来使用.
	// ============================
	AMQ_Subscriber() {}
    

	// ============================
	// @用途 : 析构函数
	// ============================
	virtual ~AMQ_Subscriber() {
	    this->_cleanup();
	}


	void SetClientId(const std::string & clientId) {
	    this->client_id_ = clientId;
	}


	// ============================
	// @用途 : 接收字符串消息, 继承自 AMQ_IRecver
	// @txtstr : 字符串消息 : out参数
	// @errstr : 错误信息 : out参数
	// @返回值 : 成功接收返回 true, 否则返回 false	    
	// ============================
	virtual bool RecvTextMessage(std::string * txtstr, std::string * errstr = nullptr);


	// ============================
	// @用途 : 接收二进制消息, 继承自 AMQ_IRecver
	// @txtstr : 字符串消息 : out参数
	// @errstr : 错误信息 : out参数
	// @返回值 : 成功接收返回 true, 否则返回 false	    
	// ============================
	virtual bool RecvBytesMessage(std::string * buffstr, unsigned int * buffsize, std::string * errstr = nullptr);


    private:
	// ============================
	// @用途 : 初始化内部字段值
	// @返回值 : void
	// @PS : 扩展基类函数
	// ============================
	virtual void _init();


	// ============================
	// @用途 : 释放所占用的系统资源
	// @返回值 : void
	// @PS : 扩展基类函数
	// ============================
	virtual void _cleanup();


	// 消费者对象指针
	std::shared_ptr<::cms::MessageConsumer> subscriber_;

	std::string client_id_;
    }; // AMQ_Subscriber;


} // namespace cms_ex;
} // namespace iegad;


#endif // __AMQ_SUBSCRIBER__