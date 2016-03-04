#ifndef __AMQ_IWORKER__
#define __AMQ_IWORKER__



// ============ 说明 ============
//
// @创建日期 : 2016-02-21
// @创建人 : iegad
//
// ============================
// @用途 : 1, 对ActiveMQ-CMS 的扩展, 提供 发送者 和 接收者 接口约束
//		
// @PS : 该文件依赖于 ActiveMQ-CMS 开源库
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



#include <string>



namespace iegad {
namespace cms_ex {


    class AMQ_ISender {
    // 发送者 接口
    public:
	virtual bool SendTextMessage(const std::string & txtstr, std::string * errstr = nullptr) = 0;
	virtual bool SendBytesMessage(const char * buff, unsigned int buffsize, std::string * errstr = nullptr) = 0;
    }; // class AMQ_ISender;


    class AMQ_IRecver {
    // // 接收者 接口
	virtual bool RecvTextMessage(std::string * txtstr, std::string * errstr = nullptr) = 0;
	virtual bool RecvBytesMessage(std::string * buffstr, unsigned int * buffsize, std::string * errstr = nullptr) = 0;
    }; // class AMQ_IRecver;


} // namespace cms_ex;
} // namespace iegad;


#endif // __AMQ_IWORKER__