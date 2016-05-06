#include "thrift_file.h"


bool 
data_structure::ThriftFile::Check(std::string * errstr)
{
	const ThriftClassMap_t & cmap = this->GetClassMap();
	for (auto itor = cmap.begin(); itor != cmap.end(); itor++) {
		if (!itor->second.Check(errstr)) {
			return false;
		}
	} // for;
	return true;
}
