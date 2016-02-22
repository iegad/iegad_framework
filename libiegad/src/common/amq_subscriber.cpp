#include "amq_subscriber.h"



void 
iegad::cms_ex::AMQ_Subscriber::_init()
{
    AMQ_Baser::_init();    
    dest_ = std::shared_ptr<::cms::Destination>(session_->createTopic(amq_name_));
    subscriber_ = std::shared_ptr<::cms::MessageConsumer>(session_->createConsumer(dest_.get()));
}


void 
iegad::cms_ex::AMQ_Subscriber::_cleanup()
{
    if (subscriber_ != nullptr) {
	subscriber_->close();
    }
    AMQ_Baser::_cleanup();
}


bool 
iegad::cms_ex::AMQ_Subscriber::RecvTextMessage(std::string * txtstr, std::string * errstr /*= nullptr*/)
{
    try {
	if (txtstr == nullptr) {
	    return false;
	}
	std::shared_ptr<::cms::TextMessage> msg((::cms::TextMessage *)subscriber_->receive());
	*txtstr = msg->getText();
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
iegad::cms_ex::AMQ_Subscriber::RecvBytesMessage(std::string * buffstr, unsigned int * buffsize, std::string * errstr /*= nullptr*/)
{
    try {
	if (buffstr == nullptr) {
	    return false;
	}
	uint8_t * buff;
	std::shared_ptr<::cms::BytesMessage> msg((::cms::BytesMessage *)subscriber_->receive());
	buff = msg->getBodyBytes();
	*buffsize = msg->getBodyLength();
	*buffstr = std::string((char *)buff, *buffsize);
	return true;
    }
    catch (std::exception & ex) {
	if (errstr != nullptr) {
	    *errstr = ex.what();
	}
    }
    return false;
}

