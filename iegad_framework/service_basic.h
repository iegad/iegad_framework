#ifndef __SERVICE_BASIC__
#define __SERVICE_BASIC__


#include <string>
#include <memory>
#include <boost/asio.hpp>


namespace iegad {
namespace net {

    class svc_basic {
    public:
	svc_basic(int svc_id)
	    : svc_id_(svc_id)
	{}

	virtual ~svc_basic() {}

	virtual int action(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr) = 0;

	int get_id() { return svc_id_; }

    private:
	int svc_id_;
    };

    typedef std::shared_ptr<svc_basic> svc_basic_ptr;

} // namespace net;
} // namespace iegad;



#endif // __SERVICE_BASIC__