#ifndef __DATA_OBJ_CPP_GENERATER__
#define __DATA_OBJ_CPP_GENERATER__


/* ===============================
	该代码由 dogen V0.0.1 生成

	@用途 : 
		该文件依赖 thrift 定义的结构, 并扩展 thirft 结构

		1,  为结构提供 数据库的 增, 删, 改, 查 的SQL操作
		2, 为结构提供 JSON, 二进制 序列化和反序列化

	@PS : 需要手动将thrift 生成的 XXX_XXX_type.h 的头文件手动添进来

	@作者 : iegad
 =============================== */


#include <string>
#include <list>
#include <stdint.h>
#include <soci/soci.h>
#include "thrift_ex/thrift_serializer.hpp"
#include "person_types.h"


namespace oa_system {
namespace dbo {


// =============================
// @Person 操作对象
// =============================
	class PersonAcs {
	public:
		static bool Insert(const Person & obj, soci::session & sql, std::string * errstr = nullptr) {
			try {
				sql << "INSERT INTO PERSON_T "
					"(AGE, NAME, USRID) "
					"VALUES"
					"(:AGE, :NAME, :USRID)",
					soci::use(obj.Age), soci::use(obj.Name), soci::use(obj.UsrID);
				return true;
			}
			catch (std::exception & ex) {
				if (errstr != nullptr) {
					*errstr = ex.what();
				}
			} // catch;
			return false;
		} // static bool Insert;

		static bool Delete(const Person & obj, soci::session & sql, std::string * errstr = nullptr) {
			try {
				sql << "DELETE * FROM PERSON_T WHERE "
					"USRID = :USRID",
					soci::use(obj.UsrID);
				return true;
			}
			catch (std::exception & ex) {
				if (errstr != nullptr) {
					*errstr = ex.what();
				}
			} // catch;
			return false;
		} // static bool Delete;

		static bool Update(const Person & obj, soci::session & sql, std::string * errstr = nullptr) {
			try {
				sql << "UPDATE PERSON_T SET "
					"AGE = :AGE, "
					"NAME = :NAME", 
					"WHERE "
					"USRID = :USRID",
					soci::use(obj.Age), soci::use(obj.Name),
					soci::use(obj.UsrID);
				return true;
			}
			catch (std::exception & ex) {
				if (errstr != nullptr) {
					*errstr = ex.what();
				}
			} // catch;
			return false;
		} // static bool Update;

		static bool SelectAll(std::list<Person> * dataList, soci::session & sql, std::string * errstr = nullptr) {
			try {
				soci::rowset<soci::row> rows = (sql.prepare << "SELECT * FROM PERSON_T");
				for (auto itor = rows.begin(); itor != rows.end(); itor++) {
					Person obj;
					obj.__set_Age(itor->get<int32_t>("AGE", 0));
					obj.__set_Name(itor->get<std::string>("NAME", ""));
					obj.__set_UsrID(itor->get<int32_t>("USRID", 0));
					dataList->push_back(obj);
				} // for;
				return true;
			}
			catch (std::exception & ex) {
				if (errstr != nullptr) {
					*errstr = ex.what();
				}
			} // catch;
			return false;
		} // static bool SelectAll;

		static bool SerializeToJSONString(Person & obj, std::string * serstr, int * size) {
			return iegad::thrift_ex::Serializer<Person>::SerializeToJSONString(obj, serstr, size);
		} // static bool SerializeToJSONString;

		static bool ParseFromJSONString(Person * obj, const std::string & serstr) {
			return iegad::thrift_ex::Serializer<Person>::ParseFromJSONString(obj, serstr);
		} // static bool ParseFromJSONString;

		static bool SerializeToArray(Person & obj, std::string * serstr, int * size) {
			return iegad::thrift_ex::Serializer<Person>::SerializeToArray(obj, serstr, size);
		} // static bool SerializeToArray;

		static bool ParseFromArray(Person * obj, const std::string & serstr) {
			return iegad::thrift_ex::Serializer<Person>::ParseFromArray(obj, serstr);
		} // static bool ParseFromArray;

	}; // PersonAcs;


} // namespace dbo;
} // namespace oa_system;


#endif // __DATA_OBJ_CPP_GENERATER__
