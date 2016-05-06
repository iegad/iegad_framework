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
// 2016-04-11				iegad							1. ���� �䳤���� ���л�/�����л�
//																		2. �޸Ķ��������л��뷴���л��������е�ArrayΪBinary
//																		3. ��Ӵ�����־



#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>


#include "tools/iegad_log.hpp"



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
			static bool SerializeToBinary(T & data, std::string * serstr, int * size);


			// ============================
			// @��; : �� �ֽ����� �����л�Ϊ T ���ݶ���
			// @data : ��thrift ���ɵ� struct ����ָ�� : out����
			// @serstr : ���ڷ����л��� �ֽ�����
			// @����ֵ : �ɹ����� true, ���򷵻� false.
			// ============================
			static bool ParserFromBinary(T * data, const std::string & serstr);


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


			// ============================
			// @��; : �� T ���л�Ϊ�䳤����
			// @data : ��thrift ���ɵ� struct ����
			// @serstr : ���л���� �䳤���� : out����
			// @size : ���л���� �䳤���ݳ��� : out����
			// @����ֵ : �ɹ����� true, ���򷵻� false.
			// ============================
			static bool SerializeToVarint(T & data, std::string * serstr, int * size);


			// ============================
			// @��; : �� �䳤���� �����л�Ϊ T ���ݶ���
			// @data : ��thrift ���ɵ� struct ����ָ�� : out����
			// @serstr : ���ڷ����л��� �䳤����
			// @����ֵ : �ɹ����� true, ���򷵻� false.
			// ============================
			static bool ParserFromVarint(T * data, const std::string & serstr);
		}; // class Serializer<T>;



	// ======================== ������ʵ�ֲ��� ========================



	template <class T>
	bool iegad::thrift_ex::Serializer<T>::SerializeToBinary(T & data, std::string * serstr, int * size)
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
		catch (std::exception & ex) {
			iERR<<ex.what()<<std::endl;
		}
		return false;
	}


	template <class T>
	bool iegad::thrift_ex::Serializer<T>::ParserFromBinary(T * data, const std::string & serstr)
	{
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
		catch (std::exception & ex) {
			iERR << ex.what() << std::endl;
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
		catch (std::exception & ex) {
			iERR << ex.what() << std::endl;
		}
		return false;
	}


	template <class T>
	bool iegad::thrift_ex::Serializer<T>::ParserFromJSONString(T * data, const std::string & serstr)
	{
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
		catch (std::exception & ex) {
			iERR << ex.what() << std::endl;
		}
		return false;
	}



	template <class T>
	bool iegad::thrift_ex::Serializer<T>::SerializeToVarint(T & data, std::string * serstr, int * size)
	{
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
		catch (std::exception & ex) {
			iERR << ex.what() << std::endl;
		}
		return false;
	}


	template <class T>
	bool iegad::thrift_ex::Serializer<T>::ParserFromVarint(T * data, const std::string & serstr)
	{
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
		catch (std::exception & ex) {
			iERR << ex.what() << std::endl;
		}
		return false;
	}


} // namespace thrift_ex;
} // namespace iegad;


#endif // __THRIFT_SERIALIZER__

