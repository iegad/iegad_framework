#ifndef __CLASS_INFO__
#define __CLASS_INFO__


#include <string>
#include <map>
#include <vector>


enum MemberTypeE {
    CHAR,
    INT8,
    INT16,
    INT32,
    INT64,
    FLOAT,
    DOUBLE,
    STRING
}; // MemberTypeE;


struct ClassMember{
    std::string MemberName;
    MemberTypeE MemberType;
    std::string DefaultValue;
}; // ClassMember;


typedef std::map<std::string/* ³ÉÔ±Ãû³Æ */, ClassMember> ClassMemberMap;


class ClassInfo {
public:
    ClassInfo() {}

    const std::string & GetName() const {
	return className_;
    }

    void SetName(const std::string & name) {
	className_ = name;
    }

    const ClassMemberMap & ClassMembers() const {
	return memMap_;
    }

    void AddMember(const std::string & memName, MemberTypeE memType, const std::string & defaultValue) {
	ClassMember mem;
	mem.MemberName = memName;
	mem.MemberType = memType;
	mem.DefaultValue = defaultValue;
	memMap_[memName] = mem;
    }

    void SetPrimary(const std::vector<std::string> & primaryVct) {
	primaryVct_ = primaryVct;
    }

    const std::vector<std::string> & GetPrimary() const {
	return primaryVct_;
    }


private:
    std::string className_;
    std::vector<std::string> primaryVct_;
    ClassMemberMap memMap_;
}; // class ClassInfo;


#endif // __CLASS_INFO__