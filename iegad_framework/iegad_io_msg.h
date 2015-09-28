#ifndef __IEGAD_IO_MSG__
#define __IEGAD_IO_MSG__


#ifdef WIN32

#pragma comment(lib, "libprotoc.lib")
#pragma comment(lib, "libprotobuf.lib")

#endif // WIN32


#include <boost/asio.hpp>
#include <string>
#include "msg_basic.pb.h"



namespace iegad {
namespace io {
     

    const std::string recv_end(boost::asio::ip::tcp::socket & clnt);

    int send_n(boost::asio::ip::tcp::socket & clnt, const char * send_buf, int buf_size);



} // namespace net;


namespace net {


    int recv_msg_basic(boost::asio::ip::tcp::socket & clnt, msg_basic & msgbsc);

    int send_msg_basic(boost::asio::ip::tcp::socket & clnt, const msg_basic & msgbsc);

    int send_msg_basic(boost::asio::ip::tcp::socket & clnt, int type, int flag, const std::string & msg_str);


} // namespace net;



} // namespace iegad;



#endif // __IEGAD_IO_MSG__