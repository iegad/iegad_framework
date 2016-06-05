#ifndef __DATA_OBJ_CPP_GENERATER__
#define __DATA_OBJ_CPP_GENERATER__


/* ===============================
	�ô����� dogen V0.0.1 ����

	@��; : 
		���ļ����� thrift ����Ľṹ, ����չ thirft �ṹ

		1,  Ϊ�ṹ�ṩ ���ݿ�� ��, ɾ, ��, �� ��SQL����
		2, Ϊ�ṹ�ṩ JSON, ������ ���л��ͷ����л�

	@PS : ��Ҫ�ֶ���thrift ���ɵ� XXX_XXX_type.h ��ͷ�ļ��ֶ������

	@���� : iegad
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
// @Person ��������
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

	}; // PersonAcs;


} // namespace dbo;
} // namespace oa_system;


#endif // __DATA_OBJ_CPP_GENERATER__
