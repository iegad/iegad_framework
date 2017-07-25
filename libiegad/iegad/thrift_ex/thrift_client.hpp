#ifndef __THRIFT_CLIENT__
#define __THRIFT_CLIENT__



#include "thrift_ex_macro.h"



namespace iegad { 
namespace thrift_ex { 


template <typename __SVC_CLIENT_T_, typename __PROTOCOL_T_, bool __NON_BLOCK_>
class thriftClient {
// thrift客户端模板类
public:
    typedef ::apache::thrift::transport::TSocket TSocket;
    typedef ::apache::thrift::transport::TTransport TTransport;
    typedef ::apache::thrift::transport::TFramedTransport TFramedTransport;
    typedef ::apache::thrift::transport::TBufferedTransport TBufferedTransport;
    typedef ::apache::thrift::protocol::TProtocol TProtocol;
    typedef __PROTOCOL_T_ protocol_t;


    explicit thriftClient(const std::string & ipstr, int port)
    {
        assert(ipstr.size() > 0 && port > 0 && port <= 65535);
        if (!__NON_BLOCK_) {
            _init_thread_clnt(ipstr, port);
        }
        else {
            _init_nonblocking_clnt(ipstr, port);
        }
        this->open();
    }


    ~thriftClient()
    {
        this->close();
    }


    bool 
    open()
    {
        do {
            if (!sock_) {
                break;
            }
            if (!sock_->isOpen()) {
                sock_->open();
            }
            return true;
        } while (false);
        return false;
    }


    void 
    close()
    {
        if (sock_ && sock_->isOpen()) {
            sock_->close();
        }
    }


    boost::shared_ptr<__SVC_CLIENT_T_>
    imp() 
    {
        return client_;
    }


private:
    void 
    _init_nonblocking_clnt(const std::string & ipstr, int port) 
    {
        sock_ = boost::shared_ptr<TSocket>(new TSocket(ipstr, port));
        trans_ = boost::shared_ptr<TFramedTransport>(new TFramedTransport(sock_));
        protoc_ = boost::shared_ptr<protocol_t>(new protocol_t(trans_));
        client_ = boost::shared_ptr<__SVC_CLIENT_T_>(new __SVC_CLIENT_T_(protoc_));
    }


    void 
    _init_thread_clnt(const std::string & ipstr, int port) 
    {
        sock_ = boost::shared_ptr<TSocket>(new TSocket(ipstr, port));
        trans_ = boost::shared_ptr<TBufferedTransport>(new TBufferedTransport(sock_));
        protoc_ = boost::shared_ptr<protocol_t>(new protocol_t(trans_));
        client_ = boost::shared_ptr<__SVC_CLIENT_T_>(new __SVC_CLIENT_T_(protoc_));
    }


    boost::shared_ptr<TSocket> sock_;
    boost::shared_ptr<protocol_t> protoc_;
    boost::shared_ptr<TTransport> trans_;
    boost::shared_ptr<__SVC_CLIENT_T_> client_;


    thriftClient(const thriftClient &);
    thriftClient & operator=(thriftClient &);
}; // thriftClient<__PROTOCOL_T_, __SVC_CLIENT_T_>;


} // namespace thrift_ex;
} // namesapce iegad;



#define DEFINE_THRIFT_CLIENT(__class_name_, __service_name_, __protocol_, __non_block_) \
typedef iegad::thrift_ex::thriftClient<XXX_Client(__service_name_), XXX_Protocol(__protocol_), __non_block_> __class_name_;




#endif // __THRIFT_CLIENT__
