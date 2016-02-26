#ifndef __THRIFT_SERIALIZER__
#define __THRIFT_SERIALIZER__



// ============ ˵�� ============
//
// @�������� : 2016-02-23
// @������ : iegad
//
// ============================
// @��; :  1, ��thirft �����ɵĽṹ�ṩ���л�����.
// 		    ���л���, �������л��뷴���л�, �ṩ ������ �� JSON�������л���ʽ.
//		
// @PS : ���ļ������� thrift ��Դ��    
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================



#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>



namespace iegad {
namespace thrift_ex {


    template <class T>
    class Serializer {
	// thrift ���л���.
    public:
	// ============================
	// @��; : ��ֹ ʵ��������
	// ============================
	virtual ~Serializer() = 0;


	// ============================
	// @��; : �� T ���л�Ϊ �ֽ� ����
	// @data : ��thrift ���ɵ� struct ����
	// @serstr : ���л�������� : out����
	// @size : ���л�������ݴ�С : out����
	// @����ֵ : �ɹ����� true, ���򷵻� false.
	// ============================
	static bool SerializeToArray(T & data, std::string * serstr, int * size);


	// ============================
	// @��; : �� �ֽ����� �����л�Ϊ T ���ݶ���
	// @data : ��thrift ���ɵ� struct ����ָ�� : out����
	// @serstr : ���ڷ����л��� �ֽ�����
	// @����ֵ : �ɹ����� true, ���򷵻� false.
	// ============================
	static bool ParserFromArray(T * data, const std::string & serstr);


	// ============================
	// @��; : �� T ���л�Ϊ JSON ��
	// @data : ��thrift ���ɵ� struct ����
	// @serstr : ���л���� JSON�� : out����
	// @size : ���л���� JSON������ : out����
	// @����ֵ : �ɹ����� true, ���򷵻� false.
	// ============================
	static bool SerializeToJSONString(T & data, std::string * serstr, int * size);


	// ============================
	// @��; : �� JSON�� �����л�Ϊ T ���ݶ���
	// @data : ��thrift ���ɵ� struct ����ָ�� : out����
	// @serstr : ���ڷ����л��� JSON��
	// @����ֵ : �ɹ����� true, ���򷵻� false.
	// ============================
	static bool ParserFromJSONString(T * data, const std::string & serstr);
    }; // class Serializer<T>;



// ======================== ������ʵ�ֲ��� ========================



    template <class T>
    bool iegad::thrift_ex::Serializer<T>::SerializeToArray(T & data, std::string * serstr, int * size)
    {
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
	catch (std::exception &) {
	    //...
	}
	return false;
    }


    template <class T>
    bool iegad::thrift_ex::Serializer<T>::ParserFromArray(T * data, const std::string & serstr)
    {
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
	catch (std::exception &) {
	    //...
	}
	return false;
    }


    template <class T>
    bool iegad::thrift_ex::Serializer<T>::SerializeToJSONString(T & data, std::string * serstr, int * size)
    {
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
	catch (std::exception &) {
	    //...
	}
	return false;
    }


    template <class T>
    bool iegad::thrift_ex::Serializer<T>::ParserFromJSONString(T * data, const std::string & serstr)
    {
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
	catch (std::exception &) {
	    //...
	}
	return false;
    }


} // namespace thrift_ex;
} // namespace iegad;


#endif // __THRIFT_SERIALIZER__

