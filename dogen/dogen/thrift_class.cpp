#include "thrift_class.h"


bool 
data_structure::ThriftClass::AddKey(const std::string & keyName)
{
	for (int i = 0, n = keys_.size(); i < n; i++) {
		if (keyName == keys_[i]) {
			return false;
		}
	}
	keys_.push_back(keyName);
	return true;
}


bool
data_structure::ThriftClass::Check(std::string * errstr) const
{
	do {
		if (this->className_ == "") {
			*errstr = "ClassName is not set";
			break;
		}

		if (this->memMap_.size() == 0) {
			*errstr = "Class member is empty";
			break;
		}

		for (auto itor = memMap_.begin(); itor != memMap_.end(); itor++) {
			if (itor->second.MemberName == "" ||
				itor->second.MemberType == "" ||
				(itor->second.MemberDefaultValue == "" && itor->second.MemberType != "std::string")) {
				*errstr = "Class has invalied member filed";
				return false;
			}
		} // for;

		return true;
	} while (false);
	return false;
}
