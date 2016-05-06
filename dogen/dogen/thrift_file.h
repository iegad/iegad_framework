#ifndef __THRIFT_FILE__
#define __THRIFT_FILE__


#include "thrift_namespace.h"
#include "thrift_class.h"


namespace data_structure {


	class ThriftFile {
	public:
		typedef std::map<std::string, ThriftClass> ThriftClassMap_t;

		ThriftFile(const std::string & inPath, const std::string & outPath) {
			inFilename_ = inPath;
			outFilename_ = outPath;
		}

		const ThriftNamespace & GetNameSpace() const {
			return tns_;
		}

		bool AddNamespaceLevel(const std::string & nsName) {
			return tns_.AddNamespaceLevel(nsName);
		}

		const std::string & GetOutFilename() const {
			return outFilename_;
		}

		void AddClass(const ThriftClass & thriftClass) {
			classMap_[thriftClass.GetClassName()] = thriftClass;
		}

		ThriftClassMap_t & GetClassMap() {
			return classMap_;
		}

		const ThriftClassMap_t & GetClassMap() const {
			return classMap_;
		}

		bool Check(std::string * errstr);

	private:
		std::string inFilename_;
		std::string outFilename_;
		ThriftNamespace tns_;
		ThriftClassMap_t classMap_;
	}; // class ThriftFile;


} // namespace data_structure;


#endif // __THRIFT_FILE__