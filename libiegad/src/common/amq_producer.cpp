#include "amq_producer.h"


using namespace iegad::cms_ex;


void 
AMQ_Producer::_init()
{
    AMQ_Baser::_init();
    producer_ = std::shared_ptr<::cms::MessageProducer>(session_->createProducer(dest_.get()));
    producer_->setDeliveryMode(static_cast<::cms::DeliveryMode::DELIVERY_MODE>(persistent_flag_));
}


void 
AMQ_Producer::_cleanup()
{
    if (producer_ != nullptr) {
	producer_->close();
    }
}


bool 
iegad::cms_ex::AMQ_Producer::SendTextMessage(const std::string & txtstr, std::string * errstr /*= nullptr*/)
{
    try {
	std::shared_ptr<::cms::TextMessage> msg(session_->createTextMessage(txtstr));
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


bool
iegad::cms_ex::AMQ_Producer::SendBytesMessage(const char * buff, unsigned int buffsize, std::string * errstr /*= nullptr*/)
{
    try {
	std::shared_ptr<::cms::BytesMessage> msg(session_->createBytesMessage((uint8_t *)buff, buffsize));
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



