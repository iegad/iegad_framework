// ============ 说明 ============
//
// @创建日期 : 2018-02-06
// @创建人 : iegad
//
// ============================
// @用途: 多线程tcp服务端
//
// @PS: 该文件依赖于 libevent 开源库
//
// ============================
//
// @修改记录:
// =======================================
//  日期               修改人                     修改说明
// =======================================



#ifndef __TCP_SERVER__
#define __TCP_SERVER__


#include <event.h>
#include <iegad/tools/job_worker.hpp>



namespace iegad {
namespace net {



class TcpServer {
public:
    struct Option {
        uint32_t maxConnection;
        uint32_t connectionNum;
        uint32_t readTimeOut;
        uint32_t writeTimeOut;

        explicit
        Option() :
            maxConnection(0),
            connectionNum(0),
            readTimeOut(0),
            writeTimeOut(0)
        {}
    };


    explicit
    TcpServer(uint32_t port = 6688, uint32_t nthread = 0) :
        port_(port),
        nthread_(nthread)
    {}


    explicit
    TcpServer(uint32_t port = 6688, uint32_t nthread = 0, const Option o = {}) :
        port_(port),
        nthread_(nthread),
        opt_(o)
    {}


    void
    run()
    {

    }



private:
    uint32_t
    _getCpuNumber()
    {

    }


    uint32_t port_;
    uint32_t nthread_;
    Option opt_;
}; // class TcpServer;



} // namespace net;
} // namespace iegad;


#endif // __TCP_SERVER__
