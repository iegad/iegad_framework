#ifndef __ANALYZER__
#define __ANALYZER__


#include <string>
#include "thrift_file.h"


namespace dogen {
	

	class Analyzer {
	public:
		explicit Analyzer(const std::string & inFilename)
		: inFilename_(inFilename) {}

		bool Analyze(data_structure::ThriftFile * thriftFile, std::string * errstr);

	private:
		int _addNameSpace(const std::string & line, data_structure::ThriftFile * thriftFile);		
		int _addClass(const std::string & line, data_structure::ThriftFile * thriftFile, std::string * className);
		int _addMembers(const std::string & line, const std::string & className, data_structure::ThriftFile * thriftFile);
		bool _getTypeString(const std::string & srcTypeStr, std::string * typeStr);
		std::string inFilename_;
	}; // class Analyzer;


} // namespace dogen;


#endif // __ANALYZER__