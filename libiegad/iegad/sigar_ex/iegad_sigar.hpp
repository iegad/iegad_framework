#ifndef __IEGAD_SIGAR__
#define __IEGAD_SIGAR__


#include <sigar.h>
#include <boost/assert.hpp>
#include <cinttypes>
#include <boost/smart_ptr.hpp>
#include <boost/noncopyable.hpp>
#ifndef WIN32
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif

#include "string/iegad_string.hpp"

namespace iegad {
namespace sigar_ex {



//sigar_cpu_t c;
class cpu : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<cpu> ptr_t;


    static ptr_t Create() {
        return ptr_t(new cpu);
    }

    uint64_t getIdle() const {
        return c_.idle;
    }

    uint64_t getIrq() const {
        return c_.irq;
    }

    uint64_t getNice() const {
        return c_.nice;
    }

    uint64_t getSoftIrq() const {
        return c_.soft_irq;
    }

    uint64_t getStolen() const {
        return c_.stolen;
    }

    uint64_t getSys() const {
        return c_.sys;
    }

    uint64_t getTotal() const {
        return c_.total;
    }

    uint64_t getUser() const {
        return c_.user;
    }

    uint64_t getWait() const {
        return c_.wait;
    }

private:
    cpu() {
        memset(&c_, 0, sizeof(c_));
    }

    sigar_cpu_t c_;
}; // class cpu;


class cpuInfo : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<cpuInfo> ptr_t;


    static ptr_t Create() {
        return ptr_t(new cpuInfo);
    }

    uint64_t getCacheSize() const {
        return c_.cache_size;
    }

    uint32_t getCoresPerSocket() const {
        return c_.cores_per_socket;
    }

    uint32_t getMhz() const {
        return c_.mhz;
    }

    const std::string getModel() const {
        return c_.model;
    }

    uint32_t getTotalCores() const {
        return c_.total_cores;
    }

    uint32_t getTotalSockets() const {
        return c_.total_sockets;
    }

    const std::string getVendor() const {
        return c_.vendor;
    }


private:
    cpuInfo() {
        memset(&c_, 0, sizeof(c_));
    }

    sigar_cpu_info_t c_;
}; // class cpuInfo;



class dirStat : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<dirStat> ptr_t;


    static ptr_t Create() {
        return ptr_t(new dirStat);
    }


    double getQueue() const {
        return d_.queue;
    }

    uint64_t getReadBytes() const {
        return d_.read_bytes;
    }

    uint64_t getReads() const {
        return d_.reads;
    }

    double getServiceTime() const {
        return d_.service_time;
    }

    uint64_t getWriteBytes() const {
        return d_.write_bytes;
    }

    uint64_t getWrites() const {
        return d_.writes;
    }

    uint64_t getReadTime() const {
        return d_.rtime;
    }

    uint64_t getWriteTime() const {
        return d_.wtime;
    }

    uint64_t getTime() const {
        return d_.time;
    }

    uint64_t getSnapTime() const {
        return d_.snaptime;
    }

    uint64_t getQueueTime() const {
        return d_.qtime;
    }

private:
    dirStat() {
        memset(&d_, 0, sizeof(d_));
    }


    sigar_disk_usage_t d_;
}; // class dirStat;



class fileSystem : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<fileSystem> ptr_t;


    static ptr_t Create() {

    }


    const std::string getDirName() const {
        return f_.dir_name;
    }

    const std::string getDevName() const {
        return f_.dev_name;
    }

    const std::string getTypeName() const {
        return f_.type_name;
    }

    const std::string getSysTypeName() const {
        return f_.sys_type_name;
    }

    const std::string getOptions() const {
        return f_.options;
    }

    sigar_file_system_type_e getType() const {
        return f_.type;
    }

    uint64_t getFlags() const {
        return f_.flags;
    }


private:
    fileSystem() {
        memset(&f_, 0, sizeof(f_));
    }

    sigar_file_system_t f_;
}; // class fileInfo;



class mem : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<mem> ptr_t;


    static ptr_t Create() {
        return ptr_t(new mem);
    }

    uint64_t getRam() const {
        return m_.ram;
    }

    uint64_t getTotal() const {
        return m_.total;
    }

    uint64_t getUsed() const {
        return m_.used;
    }

    uint64_t getFree() const {
        return m_.free;
    }

    uint64_t getActualUsed() const {
        return m_.actual_used;
    }

    uint64_t getActualFree() const {
        return m_.actual_free;
    }

    double getUsedPercent() const {
        return m_.used_percent;
    }

    double getFreePercent() const {
        return m_.free_percent;
    }

private:
    mem() {
        memset(&m_, 0, sizeof(m_));
    }

    sigar_mem_t m_;
}; // class mem;


class netConnection : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<netConnection> ptr_t;

    static ptr_t Create() {
        return ptr_t(new netConnection);
    }

    uint32_t getLocalPort() const {
        return n_.local_port;
    }

    sigar_net_address_t getLocalAddress() const {
        return n_.local_address;
    }

    uint32_t getRemotePort() const {
        return n_.remote_port;
    }

    sigar_net_address_t getRemoteAddress() const {
        return n_.remote_address;
    }

    uint32_t getUid() const {
        return n_.uid;
    }

    uint32_t getInode() const {
        return n_.inode;
    }

    int getType() const {
        return n_.type;
    }

    int getState() const {
        return n_.state;
    }

    uint32_t getSendQueue() const {
        return n_.send_queue;
    }

    uint32_t getReceiveQueue() const {
        return n_.receive_queue;
    }

private:
    netConnection() {
        memset(&n_, 0, sizeof(n_));
    }


    sigar_net_connection_t n_;
}; // class nbetConnection;


class netInfo : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<netInfo> ptr_t;


    static ptr_t Create() {
        return ptr_t(new netInfo);
    }

    const std::string getDefaultGateWay() const {
        return n_.default_gateway;
    }

    const std::string getDefaultGateWayInterface() const {
        return n_.default_gateway_interface;
    }

    const std::string getHostName() const {
        return n_.host_name;
    }

    const std::string getDomainName() const {
        return n_.domain_name;
    }

    const std::string getPrimaryDns() const {
        return n_.primary_dns;
    }

    const std::string getSecondaryDns() const {
        return n_.secondary_dns;
    }


private:
    netInfo() {
        memset(&n_, 0, sizeof(n_));
    }

    sigar_net_info_t n_;
}; // class netInfo;


class netInterfaceConfig : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<netInterfaceConfig> ptr_t;

    static ptr_t Create() {
        return ptr_t(new netInterfaceConfig);
    }


    const std::string getName() const {
        return n_.name;
    }

    const std::string getType() const {
        return n_.type;
    }

    const std::string getDescription() const {
        return n_.description;
    }

    const std::string getHwaddr() const {
        std::string temp = iegad::string_ex::bin_tostr((char *)n_.hwaddr.addr.mac, 8);
        std::string res;
        for (int i = 0, n = temp.size(); i < n; i++) {
            res.push_back(temp[i]);
            if ((i + 1) % 2 == 0 && (i + 1) != n) {
                res.push_back(':');
            }
        }
        return res;
    }

    const std::string getAddress() const {
        in_addr temp;
        memcpy(&temp, &n_.address.addr.in, 4);
        return inet_ntoa(temp);
    }

    const std::string getAddress6() const {
        char buffer[100] = {0};
        inet_ntop(AF_INET6, n_.address6.addr.in6, buffer, 100);
        return buffer;
    }

    const std::string getDestination() {

    }


    const std::string getBroadCast() {
        in_addr temp;
        memcpy(&temp, &n_.broadcast.addr.in, 4);
        return inet_ntoa(temp);
    }


    const std::string getNetMask() {
        in_addr temp;
        memcpy(&temp, &n_.netmask.addr.in, 4);
        return inet_ntoa(temp);
    }


    uint32_t getMtu() const {
        return n_.mtu;
    }

    uint32_t getMetric() const {
        return n_.metric;
    }

    uint64_t getFlags() const {
        return n_.flags;
    }

private:
    netInterfaceConfig() {
        memset(&n_, 0, sizeof(n_));
    }


    sigar_net_interface_config_t n_;
}; // class netInterfaceconfig;


class netInterfaceStat : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<netInterfaceStat> ptr_t;


    static ptr_t Create() {
        return ptr_t(new netInterfaceStat);
    }


private:
    netInterfaceStat() {
        memset(&n_, 0, sizeof(n_));
    }

    sigar_net_interface_stat_t n_;
}; // class netInterfaceStat;



class sigar {
public:
    explicit sigar() :
        s_(NULL) {
        BOOST_ASSERT(!sigar_open(&s_));
    }


    ~sigar() {
        sigar_close(s_);
    }


    const std::vector<cpu::ptr_t> getCpuList() {
        std::vector<cpu::ptr_t> res;
        sigar_cpu_list_t tempList;
        memset(&tempList, 0, sizeof(tempList));
        do {
            if (sigar_cpu_list_get(s_, &tempList)) {
                break;
            }

            for (size_t i = 0; i < tempList.number; i++) {
                cpu::ptr_t p = cpu::Create();
                p->c_ = tempList.data[i];
                res.push_back(p);
            }
        } while(false);

        if (sigar_cpu_list_destroy(s_, &tempList)) {
            res.clear();
        }

        return res;
    }


    const std::vector<cpuInfo::ptr_t> getCpuInfoList() {
        std::vector<cpuInfo::ptr_t> res;
        sigar_cpu_info_list_t tempList;
        memset(&tempList, 0, sizeof(tempList));

        do {
            if (sigar_cpu_info_list_get(s_, &tempList)) {
                break;
            }

            for (size_t i = 0; i < tempList.number; i++) {
                cpuInfo::ptr_t p = cpuInfo::Create();
                p->c_ = tempList.data[i];
                res.push_back(p);
            }
        } while(false);

        if (sigar_cpu_info_list_destroy(s_, &tempList)) {
            res.clear();
        }
        return res;
    }


    mem::ptr_t getMem() {
        mem::ptr_t res;
        sigar_mem_t temp;
        do {
            if (sigar_mem_get(s_, &temp)) {
                break;
            }
            res = mem::Create();
            res->m_ = temp;
        } while(false);
        return res;
    }


    const std::vector<netInterfaceConfig::ptr_t> getNetInterfaceConfigList() {
        std::vector<netInterfaceConfig::ptr_t> res;
        sigar_net_interface_list_t ifList;
        bool err = false;
        do {
            if (sigar_net_interface_list_get(s_, &ifList)) {
                break;
            }

            for (size_t i = 0; i < ifList.number; i++) {
                sigar_net_interface_config_t temp;
                char * ifname = ifList.data[i];
                if (sigar_net_interface_config_get(s_, ifname, &temp)) {
                    err = true;
                    break;
                }
                netInterfaceConfig::ptr_t p = netInterfaceConfig::Create();
                p->n_ = temp;
                res.push_back(p);
            }
        } while(false);

        if (sigar_net_interface_list_destroy(s_, &ifList)) {
            res.clear();
        }

        if (err) {
            res.clear();
        }

        return res;
    }


private:
    sigar_t * s_;    
}; // class sigar;



} // namespace sigar_ex;
} // namespace iegad;


#endif // __IEGAD_SIGAR__
