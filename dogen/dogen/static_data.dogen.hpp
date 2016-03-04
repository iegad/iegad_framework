#ifndef __DOGEN_CPP_GENERATER__
#define __DOGEN_CPP_GENERATER__

#include <string>
#include <stdint.h>
#include <soci.h>
/* =====================
		该文件由dogen生成
		@作者 : iegad
 ===================== */

namespace static_data {

    class Employee {
    public:
	// 构造函数
	Employee() : 
	    age_(0),
	    idcard_(),
	    job_(),
	    name_(),
	    nation_(),
	    sex_(0)
	{}
	bool Insert(soci::session * sql, std::string * errstr = nullptr) {
		try {
			*sql << "INSERT INTO EMPLOYEE_T "
			"(AGE, IDCARD, JOB, "
			"NAME, NATION, SEX)"
			" VALUES"
			"(:AGE, :IDCARD, :JOB, "
			":NAME, :NATION, :SEX)", 
			soci::use(age_), soci::use(idcard_), soci::use(job_), 
			soci::use(name_), soci::use(nation_), soci::use(sex_);
			return true;
		}
		catch (std::exception & ex) {
			if (errstr != nullptr) {
				*errstr = ex.what();
			}
		}
		return false;
	}
    private:
	int32_t age_;
	std::string idcard_;
	std::string job_;
	std::string name_;
	std::string nation_;
	int16_t sex_;
    }; // class Employee;

} // namespace static_data;

#endif // __DOGEN_CPP_GENERATER__
