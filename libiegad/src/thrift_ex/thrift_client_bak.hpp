#ifndef __THRIFT_CLIENT__
#define __THRIFT_CLIENT__



#ifndef THRIFT_SERVICE_NAME
#error 未定义服务名称
#endif



#include <string>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TSocket.h>
#include "thrift/protocol/TBinaryProtocol.h"



#define __XXX_Client__(a)		a##Client
#define _XXX_Client(a)			__XXX_Client__(a)



namespace iegad {
namespace thrift_ex {


	class TProxy {
	public:
		typedef ::apache::thrift::transport::TSocket					TSocket;
		typedef ::apache::thrift::transport::TTransport				TTransport;
		typedef ::apache::thrift::protocol::TBinaryProtocol			TBinaryProtocol;
		typedef ::apache::thrift::protocol::TJSONProtocol			TJSONProtocol;
		typedef ::apache::thrift::protocol::TProtocol					TProtocol;
		typedef ::apache::thrift::transport::TFramedTransport		TFramedTransport;
#if defined(JSON_PROTOCOL)
		typedef ::apache::thrift::protocol::TJSONProtocol protoc_t;
#elif defined(COMPCAT_PROTOCOL)
		typedef ::apache::thrift::protocol::TCompactProtocol protoc_t;
#else 
		typedef ::apache::thrift::protocol::TBinaryProtocol protoc_t;
#endif 


#if  defined(NON_BLOCK_SERVER)
		explicit TProxy(const std::string & host, int port)
			:
			sock_(new TSocket(host, port)),
			trans_(new TFramedTransport(sock_)),
			protoc_(new protoc_t(trans_)),
			client_(new _XXX_Client(THRIFT_SERVICE_NAME)(protoc_)) {
			this->Open();
		}
#else
		explicit TProxy(const std::string & ipstr, int port)
			:
			sock_(new TSocket(ipstr, port)),
			protoc_(new protoc_t(sock_)),
			client_(new _XXX_Client(THRIFT_SERVICE_NAME)(protoc_)) {
			this->Open();
		}
#endif

		~TProxy() { }

		bool Open() {
			do {
				if (sock_ == nullptr) {
					break;
				}
				if (!sock_->isOpen()) {
					sock_->open();
				}
				return true;
			} while (false);
			return false;
		} // bool Open();

	void Close() {
		if (sock_ != nullptr && sock_->isOpen()) {
			sock_->close();
		}
	} // void Close();


	boost::shared_ptr<_XXX_Client(THRIFT_SERVICE_NAME)> imp() {
		return client_;
	} // boost::shared_ptr<_XXX_Client(THRIFT_SERVICE_NAME)> imp();

	private:
		boost::shared_ptr<TSocket> sock_;
		boost::shared_ptr<TFramedTransport> trans_;
		boost::shared_ptr<protoc_t> protoc_;
		boost::shared_ptr<_XXX_Client(THRIFT_SERVICE_NAME)> client_;
	}; // class TProxy;


} // namespace thrift_ex;
} // namespace iegad;



#endif // __THRIFT_CLIENT__