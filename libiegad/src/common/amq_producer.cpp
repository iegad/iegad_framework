#include "amq_producer.h"


using namespace iegad::amq_ex;


bool
AMQ_Producer::Open(std::string * errstr /*= nullptr*/)
{
    try {
	this->_init();
    }
    catch (std::exception & ex) {
	if (errstr != nullptr) {
	    *errstr = ex.what();
	}
    }
    return false;
}


bool
AMQ_Producer::SendString(const std::string & serstr, std::string * errstr/* = nullptr*/)
{
    try {
	std::shared_ptr<::cms::TextMessage> msg(session_->createTextMessage(serstr));
	producer_->send(msg.get());
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
AMQ_Producer::_init()
{
    connFactory_ = std::shared_ptr<::activemq::core::ActiveMQConnectionFactory>(new ::activemq::core::ActiveMQConnectionFactory(uri_));
    if (!usr_.empty() && !pwd_.empty()) {
	conn_ = std::shared_ptr<::cms::Connection>(connFactory_->createConnection(usr_, pwd_));
    }
    else {
	conn_ = std::shared_ptr<::cms::Connection>(connFactory_->createConnection());
    }    
    conn_->start();
    session_ = std::shared_ptr<::cms::Session>(conn_->createSession());
    dest_ = std::shared_ptr<::cms::Destination>(session_->createQueue(amq_name_));
    producer_ = std::shared_ptr<::cms::MessageProducer>(session_->createProducer(dest_.get()));
    producer_->setDeliveryMode(static_cast<::cms::DeliveryMode::DELIVERY_MODE>(persistent_flag_));
}


void 
AMQ_Producer::_cleanup()
{
    if (producer_ != nullptr) {
	producer_->close();
    }
    if (session_ != nullptr) {
	session_->close();
    }
    if (conn_ != nullptr) {
	conn_->close();
    }
}


bool 
AMQ_Producer::SendBytes(char * buff, size_t buffSize, std::string * errstr /*= nullptr*/)
{
    try {
	std::shared_ptr<::cms::BytesMessage> msg(session_->createBytesMessage((uint8_t *)buffSize, buffSize));
	producer_->send(msg.get());
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
AMQ_Producer::SetEvironment(const std::string & uri, 
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
AMQ_Producer::_make_broker_uri()
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


