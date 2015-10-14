#ifndef __UDP_SVR__
#define __UDP_SVR__


#include <boost/asio.hpp>
#include <unordered_map>
#include "iegad_define.h"



namespace iegad {
namespace net {


    class udp_svr {
    public:
	typedef std::pair<std::string, unsigned short> rmt_addr_t;
	typedef std::unordered_map<std::string, boost::asio::ip::udp::endpoint> remotes_t;
	const int timeout_ = 5;

	explicit udp_svr(const std::string & ipstr);
	~udp_svr();

	int send_all(const char * buff, int buff_size);

	int send_one(const std::string & rmt_id, const char * buff, int buff_size);

	int add_client(const std::string & rmt_id, rmt_addr_t & rmt_addr);

	int rm_client(const std::string & rmt_id);

    private:
	int _build_sock(boost::asio::ip::udp::socket & sock);

	int _send(boost::asio::ip::udp::socket & sock, const std::string & rmt_id, const char * buff, int buff_size);

	std::string ip_;
	remotes_t remotes_;
    };


} // namespace net;
} // namespace iegad;


#endif // __UDP_SVR__