#ifndef __IGENERATER__
#define __IGENERATER__



#include <fstream>
#include "IDL_FileInfo.hpp"


class IGenerater {
public:
    virtual ~IGenerater() = 0 {}

    virtual bool Generate(const FileInfo & FileInfo) = 0;
}; // class IGenerate;


#endif // __IGENERATER__