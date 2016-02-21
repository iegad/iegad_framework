#ifndef __AMQ_BASER__
#define __AMQ_BASER__



// ============ 说明 ============
//
// @创建日期 : 2016-02-21
// @创建人 : iegad
//
// ============================
// @用途 : 1, 对ActiveMQ-CMS 的扩展, 
//		2, AMQ_Baser 是所有 AMQ扩展类 的基类
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



namespace iegad {
namespace cms_ex {


    inline void InitActiveMQEnvironment() {
	activemq::library::ActiveMQCPP::initializeLibrary();
    }


    class AMQ_Baser {
    //AMQ 基类 (抽象类)
    public:
        // ============================
        // @用途 : 构造函数
        // @PS : 提供默认构造的目的是可以结和 单例模板 来使用.
        // ============================
	AMQ_Baser() {}
    
    
        // ============================
        // @用途 : 析构函数, 防止 该类被实例化
        // ============================
	virtual ~AMQ_Baser() = 0 { 
	    this->_cleanup(); 
	}
    
    
        // ============================
        // @用途 : 打开与AMQ对象的连接
        // @errstr : 错误信息 : out参数
        // @返回值 : 成功返回 true, 否则返回 false.
        // ============================
        bool Open(std::string * errstr = nullptr);   
    
    
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
        void SetEnvironment(const std::string & uri,
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
    
    protected:
        // ============================
        // @用途 : 构造BrokerUri串
        // @返回值 : 构造成功的BrokerUri字符串
        // ============================
        const std::string _make_broker_uri();
    
    
        // ============================
        // @用途 : 初始化内部字段值
        // @返回值 : void
        // ============================
        virtual void _init();
    
    
        // ============================
        // @用途 : 释放所占用的系统资源
        // @返回值 : void
        // ============================
        virtual void _cleanup();
    
    
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

    private:
	// ============================
	// @用途 : 防止赋值操作
	// ============================
	AMQ_Baser(AMQ_Baser &);
	AMQ_Baser& operator=(AMQ_Baser &);
    }; // class AMQ_Baser;


} // namespace cms_ex;
} // namespace iegad;



#endif __AMQ_BASER__