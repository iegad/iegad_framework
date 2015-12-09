#ifndef __UDP_SVR__
#define __UDP_SVR__



// ============ 说明 ============
//
// @创建日期 : 2015-10-17
// @创建人 : iegad
//
// ============================
// @用途 : 消息推送 : 信号推送
// @PS : 目前使用的可靠UDP通信, 可靠算法可能有问题.
//		目前可能无法实对 多个服务信号端推送一个客户端.
// ============================
//
// @修改记录:
// =======================================
//  日期				    修改人                                   修改说明
// =======================================
// -- 2015-11-14		    --iegad				    -- << 暂停该任务 >>


#include <boost/asio.hpp>
#include <unordered_map>



namespace iegad {
namespace net {


    class udp_svr {
    public:
	typedef std::pair<std::string, unsigned short> rmt_addr_t;
	typedef std::unordered_map<std::string, boost::asio::ip::udp::endpoint> remotes_t;

	explicit udp_svr(const std::string & ipstr);
	~udp_svr();

	int notify_all(char buff);

	int notify_one(const std::string & rmt_id, char buff);

	int add_client(const std::string & rmt_id, rmt_addr_t & rmt_addr);

	int rm_client(const std::string & rmt_id);

    private:
	int _build_sock(boost::asio::ip::udp::socket & sock);

	int _send(boost::asio::ip::udp::socket & sock, const std::string & rmt_id, char buff);

	std::string ip_;
	remotes_t remotes_;
    };


} // namespace net;
} // namespace iegad;


#endif // __UDP_SVR__