#ifndef __AMQ_CONSUMER__
#define __AMQ_CONSUMER__



// ============ 说明 ============
//
// @创建日期 : 2016-02-21
// @创建人 : iegad
//
// ============================
// @用途 : 1, 对ActiveMQ-CMS 的扩展, 封装一个 "消费者" 方便开发使用.
//		
// @PS : 该文件依赖于 ActiveMQ-CMS 开源库
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


	class AMQ_Consumer : public AMQ_Baser, public AMQ_IRecver {
	//AMQ 消费者, 派生自 AMQ_Baser, 并继承AMQ_IRecver 接口
	public:
	    // ============================
	    // @用途 : 构造函数
	    // @PS : 提供默认构造的目的是可以结和 单例模板 来使用.
	    // ============================
	    AMQ_Consumer() {}


	    // ============================
	    // @用途 : 析构函数
	    // ============================
	    virtual ~AMQ_Consumer() { this->_cleanup(); }


	    // ============================
	    // @用途 : 接收字符串消息, 继承自 AMQ_IRecver
	    // @txtstr : 字符串消息 : out参数
	    // @errstr : 错误信息 : out参数
	    // @返回值 : 成功接收返回 true, 否则返回 false	    
	    // ============================
	    virtual bool RecvTextMessage(std::string * txtstr, std::string * errstr = nullptr) override;


	    // ============================
	    // @用途 : 接收二进制消息, 继承自 AMQ_IRecver
	    // @txtstr : 字符串消息 : out参数
	    // @errstr : 错误信息 : out参数
	    // @返回值 : 成功接收返回 true, 否则返回 false	    
	    // ============================
	    virtual bool RecvBytesMessage(std::string * buffstr, unsigned int * buffsize, std::string * errstr = nullptr) override;


	private:
	    // ============================
	    // @用途 : 初始化内部字段值
	    // @返回值 : void
	    // @PS : 扩展基类函数
	    // ============================
	    virtual void _init() override;


	    // ============================
	    // @用途 : 释放所占用的系统资源
	    // @返回值 : void
	    // @PS : 扩展基类函数
	    // ============================
	    virtual void _cleanup() override;


	    // 消费者指针
	    std::shared_ptr<::cms::MessageConsumer> consumer_;	    
	}; // class AMQ_Consumer;


    } // namespace cms_ex;
} // namespace iegad;


#endif // __AMQ_CONSUMER__