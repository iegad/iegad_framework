#ifndef __IEGAD_RMQ_EX__
#define __IEGAD_RMQ_EX__



#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <map>
#include <thread>



namespace iegad {
namespace rmq_ex {



typedef std::map<std::__thread_id, AmqpClient::Channel::ptr_t> ChannelMap;



struct ChannelOpt {
    std::string host;
    int port;
    std::string user;
    std::string pwd;
    std::string vhost;

    ChannelOpt() :
        host("127.0.0.1"),
        port(5672),
        user("guest"),
        pwd("guest"),
        vhost("/")
    {}
};



ChannelOpt &
ChannelOption()
{
    static ChannelOpt mm;
    return mm;
}



AmqpClient::BasicMessage::ptr_t
makeMessage(const std::string &data)
{
    return AmqpClient::BasicMessage::Create(data);
}



AmqpClient::Channel::ptr_t
getChannel()
{
    static ChannelMap mm;
    std::__thread_id tid = std::this_thread::get_id();

    if (mm.find(tid) == mm.end()) {
        ChannelOpt &opt = ChannelOption();

        AmqpClient::Channel::ptr_t p = AmqpClient::Channel::Create(
                    opt.host, opt.port, opt.user, opt.pwd, opt.vhost);

        mm.insert(std::make_pair(tid, p));
    }

    return mm[tid];
}



} // namespace rmq_ex;
} // namespace iegad;



#endif // __IEGAD_RMQ_EX__
