#ifndef __TCP_MT_SVC__
#define __TCP_MT_SVC__


// ============ ˵�� ============
//
// @�������� : 2015-11-08
// @������ : iegad
//
// ============================
// @��; : ���̲߳��� �Զ���rpc ����
// @PS : �÷��� ʹ�÷�ʽΪע�� ���񷽷�, �ṩ ��Щ���񷽷�������rpc�ĵ���
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================


#include "tcp_mt_svr.h"
#include <unordered_map>
#include "basic_svc.h"



namespace iegad {
namespace nets {


    class tcp_mt_svc : public tcp_mt_svr {
    // ���̲߳��� rpc����

    public:
	// ============================
	// @��; : ���񷽷�ӳ��ɢ�б� ����
	// ============================
	typedef std::unordered_map<int, basic_svc::basic_svc_ptr> map_t;


	// ============================
	// @��; : ���캯��
	// @port : �˿ں�
	// ============================
	explicit tcp_mt_svc(unsigned short port);


	// ============================
	// @��; : ���캯��
	// @host : ��������IP��ַ
	// @svc : �˿ںŻ������
	// ============================
	tcp_mt_svc(const std::string & host, const std::string & svc);


	// ============================
	// @��; : ע�����
	// @svc : ������ basic_svc �ķ��� ����ָ��
	// @����ֵ : �ɹ����� 0, ���򷵻� -1;
	// ============================
	int regist_svc(basic_svc::basic_svc_ptr svc);


	// ============================
	// @��; : ע�����
	// @clnt : �ͻ���
	// @msgstr : �ͻ��˷��͵� �ַ�����Ϣ
	// @����ֵ : �ɹ����� 0, ���򷵻� -1;
	// ============================
	virtual int _action(ip::tcp::socket & clnt, const std::string & msgstr);


    private:
	// ���񷽷�ӳ��ɢ�б�
	map_t map_;
    }; // class tcp_mt_svc;


} // namespace nets;
} // namespace iegad;



#endif // __TCP_MT_SVC__