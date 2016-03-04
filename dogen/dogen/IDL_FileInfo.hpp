#ifndef __FileInfo__
#define __FileInfo__


#include <string>
#include <fstream>
#include "ClassInfo.hpp"
#include "NamespaceInfo.hpp"


typedef std::map<std::string/* ÀàÃû³Æ */, ClassInfo> ClassInfoMap;


class FileInfo {
public:
    explicit FileInfo(const std::string & inFilePath, const std::string & outFilePath)
	: inFilePath_(inFilePath), outFilePath_(outFilePath) {}

    const NamespaceInfo & GetNameSpaceInfo() const {
	return namespc_;
    }

    void SetNameSapceInfo(NamespaceInfo & namespc) {
	this->namespc_.SetName(namespc.GetName());
	this->fileName_ = outFilePath_ + "/" + namespc_.GetName() + ".dogen.hpp";
    }

    const ClassInfoMap & GetClassMap() const {
	return classMap_;
    }

    void AddClass(const ClassInfo & classInfo) {
	classMap_[classInfo.GetName()] = classInfo;
    }

    const std::string & FileName() const {
	return fileName_;
    }

    bool ReadFileInfo() {
	std::ifstream fin;
	try {
	    do {
		fin.open(inFilePath_);


		return true;
	    } while (false);
	}
	catch (std::exception &) {
	    if (fin.is_open()) {
		fin.close();
	    }
	} // catch (std::exception &);
	return false;
    }


private:
    std::string fileName_;
    NamespaceInfo namespc_;
    ClassInfoMap classMap_;
    std::string inFilePath_;
    std::string outFilePath_;
}; // class FileInfo;





#endif // __FileInfo__