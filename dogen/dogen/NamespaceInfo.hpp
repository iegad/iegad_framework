#ifndef __NAMESPACE_INFO__
#define __NAMESPACE_INFO__


#include <string>


class NamespaceInfo {
public:
    NamespaceInfo() {}

    const std::string & GetName() const {
	return name_;
    }

    void SetName(const std::string & name) {
	this->name_ = name;
    }

private:
    std::string name_;
}; // class NamespaceInfo;


#endif // __NAMESPACE_INFO__