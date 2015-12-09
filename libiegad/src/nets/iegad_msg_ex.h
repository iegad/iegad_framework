#ifndef __IEGAD_MSG_EX__
#define __IEGAD_MSG_EX__



#include <boost/asio.hpp>
#include <string>



namespace iegad {
namespace msg {


    class msg_ex {
    public:


	static int send_data(boost::asio::ip::tcp::socket & clnt, 
	    const char * data, 
	    size_t data_size, 
	    boost::system::error_code & err_code);


	static const std::string recv_data(boost::asio::ip::tcp::socket & clnt,
	    boost::asio::streambuf & recvbuff,
	    boost::system::error_code & err_code);

    }; // class msg_ex;


} // namespace msg;
} // namespace iegad;


#endif // __IEGAD_MSG_EX__