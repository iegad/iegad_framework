#ifndef __IEGAD_DBTAB__
#define __IEGAD_DBTAB__



// ============ ˵�� ============
//
// @�������� : 2015-10-03
// @������ : iegad
//
// ============================
// @��; :  ���ݿ���ʲ���ʱ, �����洢���Ķ���, ģ��C#�е�DataTable
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================



#include "iegad_dbrow.h"
#include <vector>


namespace iegad {
namespace db {


    class db_tab {
    // ���ݱ�
    public:	
	// ============================
	// @��; : �� �����е����� ��Ϊ���ݱ� ��������
	// ============================
	typedef std::vector<dbrow_ptr> rows_t;


	// ============================
	// @��; : ���캯��
	// ============================
	explicit db_tab() {}


	// ============================
	// @��; : ���캯��
	// @PS : �ڶ����ͷ�ʱ, ��������е���.
	// ============================
	~db_tab() {
	    this->clear();
	}


	// ============================
	// @��; : [] ���������, �������� �����е�����ָ�����
	// @col_num : �е�����
	// @����ֵ : ������������ֵ
	// ============================
	dbrow_ptr operator[](int rownum);

	// ============================
	// @��; : ����һ��
	// @row : ��Ҫ���ӵ��е�����ָ��
	// @����ֵ : void
	// ============================
	void add_row(dbrow_ptr & row);


	// ============================
	// @��; : ��������е���Ϣ
	// @����ֵ : void
	// ============================
	void clear();


	// ============================
	// @��; : �����е�����
	// @����ֵ : �����е�����
	// ============================
	int row_count() const;

    private:
	// ������ (�еļ���)
	rows_t tab_;

	// ����
	db_tab(db_tab &);
	db_tab & operator=(db_tab &);
    }; // class db_tab;


    // db_tab ������ָ������
    typedef std::shared_ptr<db_tab> dbtab_ptr;


} // namespace db;
} // namespace iegad;


#endif // __IEGAD_DBTAB__