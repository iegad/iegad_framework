#include "amq_baser.h"


bool
iegad::cms_ex::AMQ_Baser::Open(std::string * errstr /*= nullptr*/)
{
    try {
	this->_init();
	return true;
    }
    catch (std::exception & ex) {
	if (errstr != nullptr) {
	    *errstr = ex.what();
	}
    }
    return false;
}


void 
iegad::cms_ex::AMQ_Baser::SetEnvironment(const std::string & uri, 
								int persistent_flag, 
								const std::string & amq_name, 
								const std::string & usr /*= ""*/, 
								const std::string & pwd /*= ""*/, 
								int32_t connTimeout /*= -1*/, 
								int32_t sendTimeout /*= -1*/)
{
    this->usr_ = usr;
    this->persistent_flag_ = persistent_flag;
    this->amq_name_ = amq_name;
    this->pwd_ = pwd;
    this->connTimeout_ = connTimeout;
    this->sendTimeout_ = sendTimeout;
    this->uri_ = uri;
    this->uri_ = _make_broker_uri();
}


const std::string 
iegad::cms_ex::AMQ_Baser::_make_broker_uri()
{
    std::string res = uri_ + "?wireFormat=openwire&wireFormat.tightEncodingEnabled=true";
    if (connTimeout_ != -1) {
	res.append("&soConnectTimeout=");
	res.append(std::to_string(connTimeout_));
    }
    if (sendTimeout_ != -1) {
	res.append("&connection.sendTimeout=");
	res.append(std::to_string(sendTimeout_));
    }
    return res;
}


void 
iegad::cms_ex::AMQ_Baser::_init()
{
    std::shared_ptr<::activemq::core::ActiveMQConnectionFactory> temp(new ::activemq::core::ActiveMQConnectionFactory(this->uri_));
    connFactory_ = temp;
    if (!usr_.empty() && !pwd_.empty()) {
	conn_ = std::shared_ptr<::cms::Connection>(connFactory_->createConnection(usr_, pwd_));
    }
    else {
	conn_ = std::shared_ptr<::cms::Connection>(connFactory_->createConnection());
    }
    conn_->start();
    session_ = std::shared_ptr<::cms::Session>(conn_->createSession());
}


void 
iegad::cms_ex::AMQ_Baser::_cleanup()
{
    if (session_ != nullptr) {
	session_->close();
    }
    if (conn_ != nullptr) {
	conn_->close();
    }
}

