#ifndef __THRIFT_NAMESPACE__
#define __THRIFT_NAMESPACE__


#include <string>
#include <vector>


namespace data_structure {


	class ThriftNamespace {
	public:
		ThriftNamespace() {}

		const std::vector<std::string> & GetNamespaces() const {
			return nsVct_;
		}

		bool AddNamespaceLevel(const std::string & nsName);

	private:
		std::vector<std::string> nsVct_;
	}; // class ThriftNamespace;


} // namespace data_structure;


#endif // __THRIFT_NAMESPACE__;