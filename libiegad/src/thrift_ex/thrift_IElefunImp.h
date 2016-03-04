#ifndef __THRIFT_IELEFUNIMP__
#define __THRIFT_IELEFUNIMP__



// ============ ˵�� ============
//
// @�������� : 2016-02-28
// @������ : iegad
//
// ============================
// @��; : IElefunImp ��һ��������, 
//		IElefunImp Ϊ����RPC�ӿ�ʵ����Ļ���
//
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================



#include "gen-cpp/Astroboy_types.h"



namespace iegad {
namespace thrift_ex {


    class IElefunImp {
    // �ӿ�ʵ�ֶ������, ������
    public:
	// ============================
	// @��; : ���캯��.
	// @fun_no : ���ܺ�, 
	// @PS : Ĭ��ֵΪ-1, Ϊʧ�ܵĵĹ��ܺ�, 
	// ============================
	explicit IElefunImp(int32_t fun_no = -1)
	    : fun_no_(fun_no) {}


	// ============================
	// @��; : ��������.
	// ============================
	virtual ~IElefunImp() = 0 {}


	// ============================
	// @��; : ���ع��ܺ�.
	// @����ֵ : ���ظ�ʵ����Ĺ��ܺ�, -1ΪʧЧ�Ĺ��ܺ�
	// ============================
	int32_t No() {
	    return fun_no_;
	}


	// ============================
	// @��; : ִ������, ������Ӧ����Ϣ
	// @_return : Ӧ����Ϣ, out����
	// @obj : ������Ϣ����
	// @����ֵ : void
	// @PS : �ú���Ϊͳһ���ýӿ�
	// ============================
	void Action(res_t * _return, const req_t & obj);


    protected:
	// ============================
	// @��; : ��������
	// @msgstr : ������Ϣ����
	// @msgsize : ������Ϣ���ݴ�����
	// @resstr : Ӧ������
	// @ressize : Ӧ�����ݳ���
	// @errcode : ������Ϣcode
	// @errstr : ������Ϣ�ַ���
	// @����ֵ : Ӧ������
	// @PS : �ú���Ϊ���麯��, ��������ʵ��.
	// ============================
	virtual int32_t _action(const std::string & msgstr, int32_t msgsize, std::string * resstr, int32_t * ressize, int32_t * errcode, std::string * errstr) = 0;


    private:
	// ============================
	// @��; : ����У�� �������� �Ƿ��뷢������md5strƥ��, һ��У��
	// @msgstr : ��ҪУ�������
	// @msgsize : ��ҪУ������ݳ���
	// @md5str : �����ȶԵ� MD5�ַ���
	// @����ֵ : ���У��ɹ�, ���� true, ���򷵻� false
	// ============================
	bool _md5_check(const std::string & msgstr, int32_t msgsize, const std::string & md5str);


	// ============================
	// @��; : ����У�� md5str �Ƿ��뷢������sha1vctƥ��, ����У��
	// @md5str : ��ҪУ���MD5�ַ���
	// @sha1vct : �����ȶԵ� SHA1����
	// @����ֵ : ���У��ɹ�, ���� true, ���򷵻� false
	// ============================
	bool _sha1_check(const std::string & md5str, const std::vector<int32_t> & sha1vct);


	//���ܺ�
	int32_t fun_no_;


	// ============================
	// @��; : ��ֹ��ֵ����
	// ============================
	IElefunImp(IElefunImp &);
	IElefunImp& operator=(IElefunImp &);
    }; // class IElefunImp;


} // namespace thrift_ex;
} // namespace iegad;



#endif // __THRIFT_IELEFUNIMP__