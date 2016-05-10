#ifndef __THRIFT_CLIENT__
#define __THRIFT_CLIENT__



#include "thrift_ex_macro.h"



#define THRIFT_THREAD_CLIENT(clientName, serviceName, protocol_type) \
namespace iegad { \
namespace thrift_ex { \
class clientName { \
public: \
	typedef ::apache::thrift::transport::TSocket					TSocket; \
	typedef ::apache::thrift::transport::TTransport				TTransport; \
	typedef ::apache::thrift::protocol::TProtocol					TProtocol; \
	typedef protocol_type													protocol_t; \
	typedef ::apache::thrift::transport::TFramedTransport		TFramedTransport; \
	explicit clientName(const std::string & ipstr, int port) \
		: \
		sock_(new TSocket(ipstr, port)), \
		protoc_(new protocol_t(sock_)), \
		client_(new serviceName(protoc_)) { \
		this->Open(); \
				} \
	~clientName() {} \
	bool Open() { \
		do { \
			if (sock_ == nullptr) { \
				break; \
									} \
			if (!sock_->isOpen()) { \
				sock_->open(); \
									} \
			return true; \
						} while (false); \
		return false; \
				} \
	void Close() { \
		if (sock_ != nullptr && sock_->isOpen()) { \
			sock_->close(); \
						} \
				} \
	boost::shared_ptr<serviceName> imp() { \
		return client_; \
				} \
private: \
	boost::shared_ptr<::apache::thrift::transport::TSocket> sock_; \
	boost::shared_ptr<protocol_t> protoc_; \
	boost::shared_ptr<serviceName> client_; \
	\
	clientName(const clientName &); \
	clientName & operator=(clientName &); \
}; \
} \
}



#define THRIFT_NON_BLOCKING_CLIENT(clientName, serviceName, protocol_type) \
namespace iegad { \
namespace thrift_ex { \
 \
class clientName { \
public: \
	typedef ::apache::thrift::transport::TSocket					TSocket; \
	typedef ::apache::thrift::transport::TTransport				TTransport; \
	typedef ::apache::thrift::protocol::TProtocol					TProtocol; \
	typedef protocol_type													protocol_t; \
	typedef ::apache::thrift::transport::TFramedTransport		TFramedTransport; \
	explicit clientName(const std::string & ipstr, int port) \
		: \
		sock_(new TSocket(ipstr, port)), \
		trans_(new TFramedTransport(sock_)), \
		protoc_(new protocol_t(trans_)), \
		client_(new serviceName(protoc_)) { \
		this->Open(); \
	} \
	\
	~clientName() {} \
	\
	bool Open() { \
		do { \
			if (sock_ == nullptr) { \
				break; \
			} \
			if (!sock_->isOpen()) { \
				sock_->open(); \
			} \
			return true; \
		} while (false); \
		return false; \
	} \
	\
	void Close() { \
		if (sock_ != nullptr && sock_->isOpen()) { \
			sock_->close(); \
		} \
	} \
	\
	boost::shared_ptr<serviceName> imp() { \
		return client_; \
	} \
	\
private: \
	boost::shared_ptr<::apache::thrift::transport::TSocket> sock_; \
	boost::shared_ptr<::apache::thrift::transport::TFramedTransport> trans_; \
	boost::shared_ptr<protocol_t> protoc_; \
	boost::shared_ptr<serviceName> client_; \
	\
	clientName(const clientName &); \
	clientName & operator=(clientName &); \
}; \
} \
}


#define DEFINE_THREAD_CLIENT(clientName, serviceName, __protocol) THRIFT_THREAD_CLIENT(clientName, \
XXX_Client(serviceName), XXX_Protocol(__protocol))


#define DEFINE_NON_BLOCKING_CLIENT(clientName, serviceName, __protocol) THRIFT_NON_BLOCKING_CLIENT(clientName, \
XXX_Client(serviceName), XXX_Protocol(__protocol))



#endif // __THRIFT_CLIENT__