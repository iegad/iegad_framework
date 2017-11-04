#ifndef __IEGAD_SIGAR__
#define __IEGAD_SIGAR__


#include <sigar.h>
#include <sigar_format.h>
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



class cpu : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<cpu> ptr_t;


    static ptr_t
    create()
    {
        return ptr_t(new cpu);
    }


    uint64_t
    getIdle() const
    {
        return c_.idle;
    }


    uint64_t
    getIrq() const
    {
        return c_.irq;
    }


    uint64_t
    getNice() const
    {
        return c_.nice;
    }


    uint64_t
    getSoftIrq() const
    {
        return c_.soft_irq;
    }


    uint64_t
    getStolen() const
    {
        return c_.stolen;
    }


    uint64_t
    getSys() const
    {
        return c_.sys;
    }


    uint64_t
    getTotal() const
    {
        return c_.total;
    }


    uint64_t
    getUser() const
    {
        return c_.user;
    }


    uint64_t
    getWait() const
    {
        return c_.wait;
    }


    cpu()
    {
        memset(&c_, 0, sizeof(c_));
    }

    sigar_cpu_t c_;
}; // class cpu;


class cpuInfo : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<cpuInfo> ptr_t;


    static ptr_t
    create()
    {
        return ptr_t(new cpuInfo);
    }


    uint64_t
    getCacheSize() const
    {
        return c_.cache_size;
    }

    uint32_t
    getCoresPerSocket() const
    {
        return c_.cores_per_socket;
    }


    uint32_t
    getMhz() const
    {
        return c_.mhz;
    }


    std::string
    getModel() const
    {
        return c_.model;
    }


    uint32_t
    getTotalCores() const
    {
        return c_.total_cores;
    }


    uint32_t
    getTotalSockets() const
    {
        return c_.total_sockets;
    }


    std::string
    getVendor() const
    {
        return c_.vendor;
    }


private:
    cpuInfo()
    {
        memset(&c_, 0, sizeof(c_));
    }


    sigar_cpu_info_t c_;
}; // class cpuInfo;



class dirStat : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<dirStat> ptr_t;


    static ptr_t
    create()
    {
        return ptr_t(new dirStat);
    }


    double
    getQueue() const
    {
        return d_.queue;
    }


    uint64_t
    getReadBytes() const
    {
        return d_.read_bytes;
    }


    uint64_t
    getReads() const
    {
        return d_.reads;
    }


    double
    getServiceTime() const
    {
        return d_.service_time;
    }


    uint64_t
    getWriteBytes() const
    {
        return d_.write_bytes;
    }


    uint64_t
    getWrites() const
    {
        return d_.writes;
    }


    uint64_t
    getReadTime() const
    {
        return d_.rtime;
    }


    uint64_t
    getWriteTime() const
    {
        return d_.wtime;
    }


    uint64_t
    getTime() const
    {
        return d_.time;
    }


    uint64_t
    getSnapTime() const
    {
        return d_.snaptime;
    }


    uint64_t
    getQueueTime() const
    {
        return d_.qtime;
    }


private:
    dirStat()
    {
        memset(&d_, 0, sizeof(d_));
    }


    sigar_disk_usage_t d_;
}; // class dirStat;



class fileSystem : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<fileSystem> ptr_t;


    static ptr_t
    create()
    {
        return ptr_t(new fileSystem);
    }


    std::string
    getDirName() const
    {
        return f_.dir_name;
    }


    std::string
    getDevName() const
    {
        return f_.dev_name;
    }


    std::string
    getTypeName() const
    {
        return f_.type_name;
    }


    std::string
    getSysTypeName() const
    {
        return f_.sys_type_name;
    }


    std::string
    getOptions() const
    {
        return f_.options;
    }


    sigar_file_system_type_e
    getType() const
    {
        return f_.type;
    }


    uint64_t
    getFlags() const
    {
        return f_.flags;
    }


private:
    fileSystem()
    {
        memset(&f_, 0, sizeof(f_));
    }


    sigar_file_system_t f_;
}; // class fileInfo;



class fileSystemUsage : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<fileSystemUsage> ptr_t;


    static ptr_t
    create()
    {
        return ptr_t(new fileSystemUsage);
    }


    uint64_t
    getDiskReads()
    {
        return m_.disk.reads;
    }

    uint64_t
    getDiskWrites()
    {
        return m_.disk.writes;
    }


    uint64_t
    getDiskWriteBytes()
    {
        return m_.disk.write_bytes;
    }


    uint64_t
    getDiskReadBytes()
    {
        return m_.disk.read_bytes;
    }


    uint64_t
    getDiskRtime()
    {
        return m_.disk.rtime;
    }


    uint64_t
    getDiskWtime()
    {
        return m_.disk.wtime;
    }


    uint64_t
    getDiskQtime()
    {
        return m_.disk.qtime;
    }


    uint64_t
    getDiskTime()
    {
        return m_.disk.time;
    }


    uint64_t
    getDiskSnaptime()
    {
        return m_.disk.snaptime;
    }


    double
    getDiskServiceTime()
    {
        return m_.disk.service_time;
    }


    double
    getDiskQueue()
    {
        return m_.disk.queue;
    }


    double
    getUsePercent()
    {
        return m_.use_percent;
    }


    uint64_t
    getTotal()
    {
        return m_.total;
    }


    uint64_t
    getFree()
    {
        return m_.free;
    }


    uint64_t
    getUsed()
    {
        return m_.used;
    }


    uint64_t
    getAvail()
    {
        return m_.avail;
    }


    uint64_t
    getFiles()
    {
        return m_.files;
    }


    uint64_t
    getFreeFiles()
    {
        return m_.free_files;
    }


private:
    fileSystemUsage()
    {
        memset(&m_, 0, sizeof(m_));
    }


    sigar_file_system_usage_t m_;
}; // class fileSystemUsage;



class diskUsage : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<diskUsage> ptr_t;


    static ptr_t
    create()
    {
        return ptr_t(new diskUsage);
    }


    uint64_t
    getDiskReads()
    {
        return m_.reads;
    }

    uint64_t
    getDiskWrites()
    {
        return m_.writes;
    }


    uint64_t
    getDiskWriteBytes()
    {
        return m_.write_bytes;
    }


    uint64_t
    getDiskReadBytes()
    {
        return m_.read_bytes;
    }


    uint64_t
    getDiskRtime()
    {
        return m_.rtime;
    }


    uint64_t
    getDiskWtime()
    {
        return m_.wtime;
    }


    uint64_t
    getDiskQtime()
    {
        return m_.qtime;
    }


    uint64_t
    getDiskTime()
    {
        return m_.time;
    }


    uint64_t
    getDiskSnaptime()
    {
        return m_.snaptime;
    }


    double
    getDiskServiceTime()
    {
        return m_.service_time;
    }


    double
    getDiskQueue()
    {
        return m_.queue;
    }

private:
    diskUsage()
    {
        memset(&m_, 0, sizeof(m_));
    }


    sigar_disk_usage_t m_;
}; // class diskUsage;



class mem : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<mem> ptr_t;


    static ptr_t
    create()
    {
        return ptr_t(new mem);
    }

    uint64_t
    getRam() const
    {
        return m_.ram;
    }

    uint64_t
    getTotal() const
    {
        return m_.total;
    }

    uint64_t
    getUsed() const
    {
        return m_.used;
    }

    uint64_t
    getFree() const
    {
        return m_.free;
    }

    uint64_t
    getActualUsed() const
    {
        return m_.actual_used;
    }

    uint64_t
    getActualFree() const
    {
        return m_.actual_free;
    }

    double
    getUsedPercent() const
    {
        return m_.used_percent;
    }

    double
    getFreePercent() const
    {
        return m_.free_percent;
    }


private:
    mem()
    {
        memset(&m_, 0, sizeof(m_));
    }

    sigar_mem_t m_;
}; // class mem;


class netConnection : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<netConnection> ptr_t;

    static ptr_t
    create()
    {
        return ptr_t(new netConnection);
    }

    uint32_t
    getLocalPort() const
    {
        return n_.local_port;
    }

    std::string
    getLocalAddress() const
    {
        in_addr temp;
        memcpy(&temp, &n_.local_address.addr.in, 4);
        return inet_ntoa(temp);
    }

    uint32_t
    getRemotePort() const
    {
        return n_.remote_port;
    }

    std::string
    getRemoteAddress() const
    {
        in_addr temp;
        memcpy(&temp, &n_.remote_address.addr.in, 4);
        return inet_ntoa(temp);
    }

    uint32_t
    getUid() const
    {
        return n_.uid;
    }

    uint32_t
    getInode() const
    {
        return n_.inode;
    }

    int
    getType() const
    {
        return n_.type;
    }

    int
    getState() const
    {
        return n_.state;
    }

    uint32_t
    getSendQueue() const
    {
        return n_.send_queue;
    }

    uint32_t
    getReceiveQueue() const
    {
        return n_.receive_queue;
    }

private:
    netConnection()
    {
        memset(&n_, 0, sizeof(n_));
    }


    sigar_net_connection_t n_;
}; // class nbetConnection;


class netInfo : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<netInfo> ptr_t;


    static ptr_t
    create()
    {
        return ptr_t(new netInfo);
    }


    std::string
    getDefaultGateWay() const
    {
        return n_.default_gateway;
    }


    std::string
    getDefaultGateWayInterface() const
    {
        return n_.default_gateway_interface;
    }


    std::string
    getHostName() const
    {
        return n_.host_name;
    }


    std::string
    getDomainName() const
    {
        return n_.domain_name;
    }


    std::string
    getPrimaryDns() const
    {
        return n_.primary_dns;
    }


    std::string
    getSecondaryDns() const
    {
        return n_.secondary_dns;
    }


private:
    netInfo()
    {
        memset(&n_, 0, sizeof(n_));
    }


    sigar_net_info_t n_;
}; // class netInfo;


class netInterfaceConfig : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<netInterfaceConfig> ptr_t;


    static ptr_t
    create()
    {
        return ptr_t(new netInterfaceConfig);
    }


    std::string
    getName() const
    {
        return n_.name;
    }

    const std::string getType() const {
        return n_.type;
    }


    std::string
    getDescription() const
    {
        return n_.description;
    }


    std::string
    getHwaddr() const
    {
        std::string temp = iegad::string::bin_tostr((char *)n_.hwaddr.addr.mac, 8);
        std::string res;
        for (int i = 0, n = temp.size(); i < n; i++) {
            res.push_back(temp[i]);
            if ((i + 1) % 2 == 0 && (i + 1) != n) {
                res.push_back(':');
            }
        }
        return res;
    }


    std::string
    getAddress() const
    {
        in_addr temp;
        memcpy(&temp, &n_.address.addr.in, 4);
        return inet_ntoa(temp);
    }


    std::string
    getAddress6() const
    {
        char buffer[100] = {0};
        inet_ntop(AF_INET6, n_.address6.addr.in6, buffer, 100);
        return buffer;
    }


    std::string
    getDestination() const
    {
        in_addr temp;
        memcpy(&temp, &n_.broadcast.addr.in, 4);
        return inet_ntoa(temp);
    }


    std::string
    getBroadCast() const
    {
        in_addr temp;
        memcpy(&temp, &n_.broadcast.addr.in, 4);
        return inet_ntoa(temp);
    }


    std::string
    getNetMask() const
    {
        in_addr temp;
        memcpy(&temp, &n_.netmask.addr.in, 4);
        return inet_ntoa(temp);
    }


    uint32_t
    getMtu() const
    {
        return n_.mtu;
    }


    uint32_t
    getMetric() const
    {
        return n_.metric;
    }


    uint64_t
    getFlags() const
    {
        return n_.flags;
    }


private:
    netInterfaceConfig()
    {
        memset(&n_, 0, sizeof(n_));
    }


    sigar_net_interface_config_t n_;
}; // class netInterfaceconfig;


class netInterfaceStat : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<netInterfaceStat> ptr_t;


    static ptr_t
    create()
    {
        return ptr_t(new netInterfaceStat);
    }


    uint64_t
    getRxPackets() const
    {
        return n_.rx_packets;
    }


    uint64_t
    getRxBytes() const
    {
        return n_.rx_bytes;
    }


    uint64_t
    getRxErrors() const
    {
        return n_.rx_errors;
    }


    uint64_t
    getRxDropped() const
    {
        return n_.rx_dropped;
    }


    uint64_t
    getRxOverruns() const
    {
        return n_.rx_overruns;
    }


    uint64_t
    getRxFrame() const
    {
        return n_.rx_frame;
    }


    uint64_t
    getTxPackets() const
    {
        return n_.tx_packets;
    }


    uint64_t
    getTxBytes() const
    {
        return n_.tx_bytes;
    }


    uint64_t
    getTxErrors() const
    {
        return n_.tx_errors;
    }


    uint64_t
    getTxDropped() const
    {
        return n_.tx_dropped;
    }


    uint64_t
    getTxOverruns() const
    {
        return n_.tx_overruns;
    }


    uint64_t
    getSpeed() const
    {
        return n_.speed;
    }


private:
    netInterfaceStat()
    {
        memset(&n_, 0, sizeof(n_));
    }


    sigar_net_interface_stat_t n_;
}; // class netInterfaceStat;


class sysInfo {
public:
    friend class sigar;
    typedef boost::shared_ptr<sysInfo> ptr_t;

    static ptr_t
    create()
    {
        return ptr_t(new sysInfo);
    }


    std::string
    getName() const
    {
        return s_.name;
    }


    std::string
    getVersion() const
    {
        return s_.version;
    }


    std::string
    getArch() const
    {
        return s_.arch;
    }


    std::string
    getMachine() const
    {
        return s_.machine;
    }


    std::string
    getDescription() const
    {
        return s_.description;
    }


    std::string
    getPatchLevel() const
    {
        return s_.patch_level;
    }


    std::string
    getVendor() const
    {
        return s_.vendor;
    }


    std::string
    getVendorVersion() const
    {
        return s_.vendor_version;
    }


    std::string
    getVendorName() const
    {
        return s_.vendor_name;
    }


    std::string
    getVendorCodeName() const
    {
        return s_.vendor_code_name;
    }


private:
    sysInfo()
    {
        memset(&s_, 0, sizeof(s_));
    }


    sigar_sys_info_t s_;
}; // class sysInfo;


class swap : boost::noncopyable {
public:
    friend class sigar;
    typedef boost::shared_ptr<swap> ptr_t;


    static ptr_t
    create()
    {
        return ptr_t(new swap);
    }


    uint64_t
    getTotal() const
    {
        return s_.total;
    }


    uint64_t
    getUsed() const
    {
        return s_.used;
    }


    uint64_t
    getFree() const
    {
        return s_.free;
    }

    uint64_t
    getPageIn() const
    {
        return s_.page_in;
    }


    uint64_t
    getPageOut() const
    {
        return s_.page_out;
    }


private:
    swap()
    {
        memset(&s_, 0, sizeof(s_));
    }


    sigar_swap_t s_;
}; // class swap;



class sigar {
public:
    enum netConnFlags {
        NETCONN_CLIENT = 0x01,
        NETCONN_SERVER = 0x02,
        NETCONN_TCP = 0x10,
        NETCONN_UDP = 0x20,
        NETCONN_RAW = 0x40,
        NETCONN_UNIX = 0x80,
        NETCONN_ALL = -1
    }; // enum netConnFlags;


    sigar() :
        s_(NULL)
    {
        BOOST_ASSERT(!sigar_open(&s_));
    }


    ~sigar()
    {
        sigar_close(s_);
    }


    std::vector<cpu::ptr_t>
    getCpuList()
    {
        std::vector<cpu::ptr_t> res;
        sigar_cpu_list_t tempList;
        memset(&tempList, 0, sizeof(tempList));
        do {
            if (sigar_cpu_list_get(s_, &tempList)) {
                break;
            }

            for (size_t i = 0; i < tempList.number; i++) {
                cpu::ptr_t p = cpu::create();
                p->c_ = tempList.data[i];
                res.push_back(p);
            }
        } while(false);

        if (sigar_cpu_list_destroy(s_, &tempList)) {
            res.clear();
        }

        return res;
    }


    std::vector<cpuInfo::ptr_t>
    getCpuInfoList()
    {
        std::vector<cpuInfo::ptr_t> res;
        sigar_cpu_info_list_t tempList;
        memset(&tempList, 0, sizeof(tempList));

        do {
            if (sigar_cpu_info_list_get(s_, &tempList)) {
                break;
            }

            for (size_t i = 0; i < tempList.number; i++) {
                cpuInfo::ptr_t p = cpuInfo::create();
                p->c_ = tempList.data[i];
                res.push_back(p);
            }
        } while(false);

        if (sigar_cpu_info_list_destroy(s_, &tempList)) {
            res.clear();
        }
        return res;
    }


    mem::ptr_t
    getMem()
    {
        mem::ptr_t res = mem::create();
        return sigar_mem_get(s_, &(res->m_)) ? NULL : res;
    }


    std::vector<netInterfaceConfig::ptr_t>
    getNetInterfaceConfigList()
    {
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
                netInterfaceConfig::ptr_t p = netInterfaceConfig::create();
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


    netInfo::ptr_t
    getNetInfo()
    {
        netInfo::ptr_t res = netInfo::create();
        return sigar_net_info_get(s_, &(res->n_)) ? NULL : res;
    }


    std::vector<netInterfaceStat::ptr_t>
    getNetInterfaceStat()
    {
        std::vector<netInterfaceStat::ptr_t> res;
        sigar_net_interface_list_t ifList;
        bool err = false;

        do {
            if (sigar_net_interface_list_get(s_, &ifList)) {
                break;
            }

            for (size_t i = 0; i < ifList.number; i++) {
                sigar_net_interface_stat_t temp;

                char *ifname = ifList.data[i];

                if (sigar_net_interface_stat_get(s_, ifname, &temp)) {
                    err = true;
                    break;
                }

                netInterfaceStat::ptr_t p = netInterfaceStat::create();
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


    std::vector<netConnection::ptr_t>
    getNetConnectionList(int flags)
    {
        std::vector<netConnection::ptr_t> res;
        sigar_net_connection_list_t tempList;

        do {
            if (sigar_net_connection_list_get(s_, &tempList, flags)) {
                break;
            }

            for (size_t i = 0; i < tempList.number; i++) {
                netConnection::ptr_t p = netConnection::create();
                p->n_ = tempList.data[i];
                res.push_back(p);
            }
        } while(false);

        if (sigar_net_connection_list_destroy(s_, &tempList)) {
            res.clear();
        }
        return res;
    }


    std::vector<fileSystem::ptr_t>
    getFileSystem()
    {
        std::vector<fileSystem::ptr_t> res;
        sigar_file_system_list_t tempList;

        do {
            if (sigar_file_system_list_get(s_, &tempList)) {
                break;
            }

            for (size_t i = 0; i < tempList.number; i++) {
                fileSystem::ptr_t p = fileSystem::create();
                p->f_ = tempList.data[i];
                res.push_back(p);
            }

        } while(false);

        if (sigar_file_system_list_destroy(s_, &tempList)) {
            res.clear();
        }

        return res;
    }


    std::vector<fileSystemUsage::ptr_t>
    getFileSystemUsage()
    {
        std::vector<fileSystemUsage::ptr_t> res;
        sigar_file_system_list_t tempList;
        bool err = false;

        do {
            if (sigar_file_system_list_get(s_, &tempList)) {
                break;
            }

            for (size_t i = 0; i < tempList.number; i++) {
                fileSystemUsage::ptr_t item = fileSystemUsage::create();
                if (sigar_file_system_usage_get(s_, tempList.data[i].dir_name, &(item->m_))) {
                    err = true;
                    break;
                }
                res.push_back(item);
            }

        } while(false);

        if (sigar_file_system_list_destroy(s_, &tempList)) {
            res.clear();
        }

        if (err) {
            res.clear();
        }

        return res;
    }


    std::vector<diskUsage::ptr_t>
    getDiskUsage()
    {
        std::vector<diskUsage::ptr_t> res;
        sigar_file_system_list_t tempList;
        bool err = false;

        do {
            if (sigar_file_system_list_get(s_, &tempList)) {
                break;
            }

            for (size_t i = 0; i < tempList.number; i++) {
                diskUsage::ptr_t item = diskUsage::create();
                if (sigar_disk_usage_get(s_, tempList.data[i].dir_name, &(item->m_))) {
                    err = true;
                    break;
                }
                res.push_back(item);
            }

        } while(false);

        if (sigar_file_system_list_destroy(s_, &tempList)) {
            res.clear();
        }

        if (err) {
            res.clear();
        }

        return res;
    }


    sysInfo::ptr_t
    getSysInfo()
    {
        sysInfo::ptr_t p = sysInfo::create();
        return sigar_sys_info_get(s_, &(p->s_)) ? NULL : p;
    }


    swap::ptr_t
    getSwap()
    {
        swap::ptr_t p = swap::create();
        return sigar_swap_get(s_, &(p->s_)) ? NULL : p;
    }


private:
    sigar_t * s_;    
}; // class sigar;



} // namespace sigar_ex;
} // namespace iegad;


#endif // __IEGAD_SIGAR__
