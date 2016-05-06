#ifndef __IGENERATER__
#define __IGENERATER__


#include <string>
#include <fstream>

#include "thrift_file.h"


namespace dogen {


	class IGenerater {		
	public:
		explicit IGenerater(const std::string & outFilePath)
			: outFilePath_(outFilePath) {}

		virtual ~IGenerater() {}


		bool Generat(const data_structure::ThriftFile & thriftFile);

	protected:
		const std::string & _getOutFilePath() {
			return outFilePath_;
		}

		virtual bool _head(std::ofstream & fout) = 0;
		virtual bool _statment(std::ofstream & fout);
		virtual bool _include(std::ofstream & fout) = 0;
		virtual bool _tail(std::ofstream & fout) = 0;


		virtual bool _namespaceBegin(const ::data_structure::ThriftNamespace & thriftNamespace, std::ofstream & fout) = 0;
		virtual bool _namespaceEnd(const ::data_structure::ThriftNamespace & thriftNamespace, std::ofstream & fout) = 0;


		virtual bool _classInfo(const data_structure::ThriftClass & thriftClass, std::ofstream & fout) = 0;

	private:
		std::string outFilePath_;
	}; // class IGenerater;


} // namespace dogen;


#endif // __IGENERATER__;