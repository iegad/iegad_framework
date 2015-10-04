#ifndef __TCP_CLIENT__
#define __TCP_CLIENT__


#include <boost/asio.hpp>


namespace iegad {
    namespace net {


	class tcp_clnt {
	    // 客户端连接	
	public:
	    explicit tcp_clnt(const std::string & host, std::string svc);
	    ~tcp_clnt();

	    int init();
	    boost::asio::ip::tcp::socket & get();

	private:
	    int _connect(boost::system::error_code & ex);
	    boost::asio::io_service ios_;
	    boost::asio::ip::tcp::socket sock_;
	    std::string host_;
	    std::string svc_;
	    boost::asio::ip::tcp::resolver::iterator iter_;
	};


    } // namespace net;
} // namespace iegad;


#endif // __TCP_CLIENT__