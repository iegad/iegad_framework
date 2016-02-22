#include "amq_publisher.h"


void 
iegad::cms_ex::AMQ_Publisher::_init()
{
    AMQ_Baser::_init();
    dest_ = std::shared_ptr<::cms::Topic>(session_->createTopic(amq_name_));
    publisher_ = std::shared_ptr<::cms::MessageProducer>(session_->createProducer(dest_.get()));
    publisher_->setDeliveryMode(::cms::DeliveryMode::NON_PERSISTENT);
}


void 
iegad::cms_ex::AMQ_Publisher::_cleanup()
{
    if (publisher_ != nullptr) {
	publisher_->close();
    }
    AMQ_Baser::_cleanup();
}


bool 
iegad::cms_ex::AMQ_Publisher::SendTextMessage(const std::string & txtstr, std::string * errstr /*= nullptr*/)
{
    try {
	std::shared_ptr<::cms::TextMessage> msg(session_->createTextMessage(txtstr));
	publisher_->send(msg.get());
	return true;
    }
    catch (std::exception & ex) {
	if (errstr != nullptr) {
	    *errstr = ex.what();
	}
    }
    return false;
}


bool 
iegad::cms_ex::AMQ_Publisher::SendBytesMessage(const char * buff, unsigned int buffsize, std::string * errstr /*= nullptr*/)
{
    try {
	std::shared_ptr<::cms::BytesMessage> msg(session_->createBytesMessage((uint8_t *)buff, buffsize));
	publisher_->send(msg.get());
	return true;
    }
    catch (std::exception & ex) {
	if (errstr != nullptr) {
	    *errstr = ex.what();
	}
    }
    return false;
}

