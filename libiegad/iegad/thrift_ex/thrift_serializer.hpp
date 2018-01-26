#ifndef __THRIFT_SERIALIZER__
#define __THRIFT_SERIALIZER__



// ============ 说明 ============
//
// @创建日期 : 2016-02-23
// @创建人 : iegad
//
// ============================
// @用途 :  1, 对thirft 所生成的结构提供序列化的类.
//     序列化器, 用于序列化与反序列化, 提供 二进制 与 JSON串的序列化方式.
//
// @PS : 该文件依赖于 thrift 开源库    
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================
// -- 2016-04-11               -- iegad              -- 1. 增加 变长数据 序列化/反序列化
//                                                   -- 2. 修改二进制序列化与反序列化函数名中的Array为Binary
//                                                   -- 3. 添加错误级日志
// -- 2017-06-04               -- iegad              -- 修改命名方式



#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>


#include "iegad/tools/iegad_log.hpp"



namespace iegad {
namespace thrift_ex {


template <class T>
class serializer {
// thrift 序列化器.
public:
    // ============================
    // @用途 : 防止 实例化该类
    // ============================
    virtual ~serializer() = 0;


    // ============================
    // @用途 : 将 T 序列化为 字节 数据
    // @data : 由thrift 生成的 struct 对象
    // @serstr : 序列化后的数据 : out参数
    // @size : 序列化后的数据大小 : out参数
    // @返回值 : 成功返回 true, 否则返回 false.
    // ============================
    static bool serializeToBinary(T & data, std::string * serstr, int * size);


    // ============================
    // @用途 : 将 字节数据 反序列化为 T 数据对象
    // @data : 由thrift 生成的 struct 对象指针 : out参数
    // @serstr : 用于反序列化的 字节数据
    // @返回值 : 成功返回 true, 否则返回 false.
    // ============================
    static bool parseFromBinary(T * data, const std::string & serstr);


    // ============================
    // @用途 : 将 T 序列化为 JSON 串
    // @data : 由thrift 生成的 struct 对象
    // @serstr : 序列化后的 JSON串 : out参数
    // @size : 序列化后的 JSON串长度 : out参数
    // @返回值 : 成功返回 true, 否则返回 false.
    // ============================
    static bool serializeToJSON(T & data, std::string * serstr, int * size);


    // ============================
    // @用途 : 将 JSON串 反序列化为 T 数据对象
    // @data : 由thrift 生成的 struct 对象指针 : out参数
    // @serstr : 用于反序列化的 JSON串
    // @返回值 : 成功返回 true, 否则返回 false.
    // ============================
    static bool parseFromJSON(T * data, const std::string & serstr);


    // ============================
    // @用途 : 将 T 序列化为变长数据
    // @data : 由thrift 生成的 struct 对象
    // @serstr : 序列化后的 变长数据 : out参数
    // @size : 序列化后的 变长数据长度 : out参数
    // @返回值 : 成功返回 true, 否则返回 false.
    // ============================
    static bool serializeToVarint(T & data, std::string * serstr, int * size);


    // ============================
    // @用途 : 将 变长数据 反序列化为 T 数据对象
    // @data : 由thrift 生成的 struct 对象指针 : out参数
    // @serstr : 用于反序列化的 变长数据
    // @返回值 : 成功返回 true, 否则返回 false.
    // ============================
    static bool parseFromVarint(T * data, const std::string & serstr);
}; // class Serializer<T>;



// ======================== 以下是实现部分 ========================



    template <class T>
    bool iegad::thrift_ex::serializer<T>::serializeToBinary(T & data, std::string * serstr, int * size)
    {
        assert(serstr && size);

        using ::apache::thrift::protocol::TBinaryProtocol;
        using ::apache::thrift::transport::TMemoryBuffer;

        try {
            boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
            boost::shared_ptr<TBinaryProtocol> proto(new TBinaryProtocol(buff));
            data.write(proto.get());
            uint8_t * p;
            buff->getBuffer(&p, (uint32_t *)size);
            *serstr = std::string((char *)p, *size);
            return true;
        }
        catch (std::exception &)
        {}
        return false;
    }


    template <class T>
    bool iegad::thrift_ex::serializer<T>::parseFromBinary(T * data, const std::string & serstr)
    {
        assert(data && serstr.size() > 0);

        using ::apache::thrift::protocol::TBinaryProtocol;
        using ::apache::thrift::protocol::TProtocol;
        using ::apache::thrift::transport::TMemoryBuffer;

        try {
            boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
            boost::shared_ptr<TProtocol> proto(new TBinaryProtocol(buff));
            uint8_t * p = (uint8_t *)const_cast<char *>(serstr.c_str());
            buff->resetBuffer(p, serstr.size());
            data->read(proto.get());
            return true;
        }
        catch (std::exception &)
        {}
        return false;
    }


    template <class T>
    bool iegad::thrift_ex::serializer<T>::serializeToJSON(T & data, std::string * serstr, int * size)
    {
        assert(serstr && size);

        using ::apache::thrift::protocol::TJSONProtocol;
        using ::apache::thrift::transport::TMemoryBuffer;
        try {
            boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
            boost::shared_ptr<TJSONProtocol> proto(new TJSONProtocol(buff));
            data.write(proto.get());
            uint8_t * p;
            buff->getBuffer(&p, (uint32_t *)size);
            *serstr = std::string((char *)p, *size);
            return true;
        }
        catch (std::exception &)
        {}
        return false;
    }


    template <class T>
    bool iegad::thrift_ex::serializer<T>::parseFromJSON(T * data, const std::string & serstr)
    {
        assert(data);

        using ::apache::thrift::protocol::TJSONProtocol;
        using ::apache::thrift::protocol::TProtocol;
        using ::apache::thrift::transport::TMemoryBuffer;

        try {
            boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
            boost::shared_ptr<TProtocol> proto(new TJSONProtocol(buff));
            uint8_t * p = (uint8_t *)const_cast<char *>(serstr.c_str());
            buff->resetBuffer(p, serstr.size());
            data->read(proto.get());
            return true;
        }
        catch (std::exception &)
        {}
        return false;
    }


    template <class T>
    bool iegad::thrift_ex::serializer<T>::serializeToVarint(T & data, std::string * serstr, int * size)
    {
        assert(serstr && size);

        using ::apache::thrift::protocol::TCompactProtocol;
        using ::apache::thrift::transport::TMemoryBuffer;

        try {
            boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
            boost::shared_ptr<TCompactProtocol> proto(new TCompactProtocol(buff));
            data.write(proto.get());
            uint8_t * p;
            buff->getBuffer(&p, (uint32_t *)size);
            *serstr = std::string((char *)p, *size);
            return true;
        }
        catch (std::exception &)
        {}
        return false;
    }


    template <class T>
    bool iegad::thrift_ex::serializer<T>::parseFromVarint(T * data, const std::string & serstr)
    {
        assert(data);

        using ::apache::thrift::protocol::TCompactProtocol;
        using ::apache::thrift::protocol::TProtocol;
        using ::apache::thrift::transport::TMemoryBuffer;

        try {
            boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
            boost::shared_ptr<TProtocol> proto(new TCompactProtocol(buff));
            uint8_t * p = (uint8_t *)const_cast<char *>(serstr.c_str());
            buff->resetBuffer(p, serstr.size());
            data->read(proto.get());
            return true;
        }
        catch (std::exception &)
        {}
        return false;
    }


} // namespace thrift_ex;
} // namespace iegad;


#endif // __THRIFT_SERIALIZER__

