#ifndef __CPP_GENERATER__
#define __CPP_GENERATER__


#include "IGenerater.h"


namespace dogen {


	class CppGenerater : public IGenerater {
	public:
		explicit CppGenerater(const std::string & outFilePath)
			: IGenerater(outFilePath) {}

	protected:
		virtual bool _head(std::ofstream & fout) override;
		virtual bool _include(std::ofstream & fout) override;
		virtual bool _tail(std::ofstream & fout) override;
		virtual bool _namespaceBegin(const ::data_structure::ThriftNamespace & thriftNamespace, std::ofstream & fout) override;
		virtual bool _namespaceEnd(const ::data_structure::ThriftNamespace & thriftNamespace, std::ofstream & fout) override;
		virtual bool _classInfo(const data_structure::ThriftClass & thriftClass, std::ofstream & fout) override;

	private:
		bool _classHead(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);
		bool _classTail(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);

		bool _classInsertFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);
		bool _classUpdateFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);
		bool _classDeleteFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);
		bool _classSelectAllFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);

		bool _classSerializeToJSONStringFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);
		bool _classParseFromJSONStringFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);

		bool _classSerializeToArrayFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);
		bool _classParseFromArrayFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);

		bool _classAddInsertKeyParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);
		bool _classAddInsertSignedParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);
		bool _classAddInsertValueParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);

		bool _classAddSelectValueParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);

		bool _classAddUpdateSignedParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);
		bool _classAddUpdateValueParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);		

		bool _classAddWhereSignedParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);
		bool _classAddWhereValueParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout);
	}; // class CppGenerater;


} // namespace dogen;


#endif // __CPP_GENERATER__