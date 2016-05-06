#ifndef __THRIFT_CLASS__
#define __THRIFT_CLASS__


#include <string>
#include <vector>
#include <map>


namespace data_structure {


	struct ThriftClassMember {
	public:
		std::string MemberType;
		std::string MemberName;
		std::string MemberDefaultValue;

		void Clear() {
			MemberDefaultValue.clear();
			MemberName.clear();
			MemberDefaultValue.clear();
		}
	}; // struct ThriftClassMember;


	class ThriftClass {
	public:
		typedef std::map<std::string, ThriftClassMember> ThriftClassMemberMap_t;

		void SetClassName(const std::string & className) {
			className_ = className;
		}

		const std::string & GetClassName() const {
			return className_;
		}

		const ThriftClassMemberMap_t & GetClassMember() const {
			return memMap_;
		}

		void AddClassMember(const std::string & memName, const std::string & memType, const std::string & defaultValue) {
			ThriftClassMember temp;
			temp.MemberDefaultValue = defaultValue;
			temp.MemberName = memName;
			temp.MemberType = memType;
			memMap_[memName] = temp;
		}

		const std::vector<std::string> & GetKeys() const {
			return keys_;
		}

		bool AddKey(const std::string & keyName);

		bool Check(std::string * errstr) const;

	private:
		std::string className_;
		std::vector<std::string> keys_;
		ThriftClassMemberMap_t memMap_;
	}; // class ThriftClass;


} // namespace data_structure;


#endif // __THRIFT_CLASS__