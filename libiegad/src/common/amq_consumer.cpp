#include "amq_consumer.h"


void 
iegad::cms_ex::AMQ_Consumer::_init()
{
    iegad::cms_ex::AMQ_Baser::_init();
    consumer_ = std::shared_ptr<::cms::MessageConsumer>(session_->createConsumer(dest_.get()));
}


void 
iegad::cms_ex::AMQ_Consumer::_cleanup()
{
    if (consumer_ != nullptr) {
	consumer_->close();
    }
}


bool 
iegad::cms_ex::AMQ_Consumer::RecvTextMessage(std::string * txtstr, std::string * errstr /*= nullptr*/)
{
    try {
	if (txtstr == nullptr) {
	    return false;
	}
	std::shared_ptr<::cms::TextMessage> msg((::cms::TextMessage *)consumer_->receive());
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
iegad::cms_ex::AMQ_Consumer::RecvBytesMessage(std::string * buffstr, unsigned int * buffsize, std::string * errstr /*= nullptr*/)
{
    try {
	if (buffstr == nullptr) {
	    return false;
	}
	uint8_t * buff;
	std::shared_ptr<::cms::BytesMessage> msg((::cms::BytesMessage *)consumer_->receive());
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



