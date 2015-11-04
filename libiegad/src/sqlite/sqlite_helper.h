#ifndef __SQLITE_HELPER__
#define __SQLITE_HELPER__



// ============ ˵�� ============
//
// @�������� : 2015-11-04
// @������ : iegad
//
// ============================
// @��; :  SQLite ���ݿ���ʲ��� ����
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================



#include "sqlite/sqlite3.h"
#include <string>
#include <vector>
#include <mutex>


#include "data/iegad_dbtab.h"


namespace iegad {
namespace sqlite {


    class sqlite_helper {
    // sqlite ��������
    public:
	// ============================
	// @��; : ������ ��������
	// ============================
	typedef std::unique_lock<std::mutex> lock_t;


	// ============================
	// @��; : ���캯��
	// ============================
	explicit sqlite_helper();


	// ============================
	// @��; : ���캯��
	// @PS : �ڶ����ͷ�ʱ, ��ر����ݿ�����.
	// ============================
	~sqlite_helper();


	// ============================
	// @��; : �����ݿ�����
	// @filename : sqlite db �ļ���·��
	// @����ֵ : �ɹ����ӷ���0, ���򷵻� errno;
	// ============================
	int open(const std::string &filename);


	// ============================
	// @��; : ִ��DML����
	// @sqlstr : DML���
	// @����ֵ : �ɹ�, ������Ӱ�������, ���򷵻� errno;
	// ============================
	int exec(const std::string & sqlstr);


	// ============================
	// @��; : ִ��DML�������
	// @sqlque : ��Ҫִ�е�����DML���
	// @����ֵ : �ɹ�, ������Ӱ�������, ���򷵻� errno;
	// ============================
	int exec_trans(const std::vector<std::string> & sqlvct);


	// ============================
	// @��; : ִ�в�ѯ����, ����� ���ݱ�tab
	// @sqlstr : DQL���
	// @tab : �����������ݵ� ���ݱ�����ָ��
	// @����ֵ : �ɹ�����0, ���򷵻� errno;
	// ============================
	int query(const std::string & sqlstr, iegad::data::db_tab & tab);


	// ============================
	// @��; : <����>
	// @PS : ����, ֻ��Ϊ�˺���ʵ db_helper ����ͬ�ĵ��ýӿ�
	// ============================
	int call_proc(const std::string & sqlstr) {
	    return -1;
	}


	// ============================
	// @��; : <����>
	// @PS : ����, ֻ��Ϊ�˺���ʵ db_helper ����ͬ�ĵ��ýӿ�
	// ============================
	int call_proc(const std::string & sqlstr, std::vector<std::string> & out_parm) {
	    return -1;
	}


	// ============================
	// @��; : �ر� sqlite ����
	// @����ֵ : void
	// ============================
	void close();


	// ============================
	// @��; : �������һ�γ���� errstr
	// @����ֵ : errstr
	// ============================
	const std::string & errmsg() const;
	
    private:
	// ״̬��־
	bool open_flag_;
	// �������
	sqlite3 * conn_;
	// ������
	mutable std::mutex mtx_;
	// ��������Ϣ
	std::string errmsg_;


	// ����
	sqlite_helper(const sqlite_helper &);
	sqlite_helper & operator=(const sqlite_helper &);
    }; // class sqlite_helper;


} // namespace sqlite;
} // namespace iegad;



#endif // __SQLITE_HELPER__