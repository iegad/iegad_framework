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



#include "amq_baser.h"
#include "amq_iworker.hpp"



namespace iegad {
namespace cms_ex {


    class AMQ_Producer : public AMQ_Baser, public AMQ_ISender {
    //AMQ 生产者 : 派生自 AMQ_Baser
    public:
	// ============================
	// @用途 : 构造函数
	// @PS : 提供默认构造的目的是可以结和 单例模板 来使用.
	// ============================
	AMQ_Producer() {}


	// ============================
	// @用途 : 析构函数
	// ============================
	virtual ~AMQ_Producer() { this->_cleanup(); }


	// ============================
	// @用途 : 发送 string 类型消息, 继承自 AMQ_ISender
	// @serstr : 需要发送的字符串
	// @errstr : 错误信息 : out参数
	// @返回值 : 发送成功返回 true, 否则返回 false.
	// ============================
	virtual bool SendTextMessage(const std::string & txtstr, std::string * errstr = nullptr) override;


	// ============================
	// @用途 : 发送 二进制数据 类型消息, 继承自 AMQ_ISender
	// @buff : 数据缓冲区指针
	// @buffSize : 数据缓冲区大小
	// @errstr : 错误信息 : out参数
	// @返回值 : 发送成功返回 true, 否则返回 false.
	// ============================
	virtual bool SendBytesMessage(const char * buff, unsigned int buffsize, std::string * errstr = nullptr) override;


    private:
	// ============================
	// @用途 : 初始化内部字段值
	// @返回值 : void
	// ============================
	virtual void _init() override;


	// ============================
	// @用途 : 释放所占用的系统资源
	// @返回值 : void
	// ============================
	virtual void _cleanup() override;


	// 生产者对象指针
	std::shared_ptr<::cms::MessageProducer> producer_;
    }; // class AMQ_Producer;


} // namespace cms_ex;
} // namespace iegad;


#endif // __AMQ_PRODUCER__