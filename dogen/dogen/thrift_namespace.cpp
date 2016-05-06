#include "thrift_namespace.h"


bool 
data_structure::ThriftNamespace::AddNamespaceLevel(const std::string & nsName)
{
	for (int i = 0, n = nsVct_.size(); i < n; i++) {
		if (nsVct_[i] == nsName) {
			return false;
		}
	} // for (int i = 0, n = nsVct_.size(); i < n; i++);
	nsVct_.push_back(nsName);
	return true;
}
