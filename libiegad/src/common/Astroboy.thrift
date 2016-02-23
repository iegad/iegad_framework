namespace cpp iegad.thrift_ex
namespace php iegad.thrift_ex
namespace csharp iegad.thrift_ex
namespace java iegad.thrift_ex




// ============ ˵�� ============
//
// @�������� : 2016-02-23
// @������ : iegad
//
// ============================
// @��; : �ṩһ��������Զ�̹��̵��ýӿ�.
//   	   ĿǰΪ v0.1.0��
// 
//		δ��ֹ����������, ҵ������ı��, 
//		����ͳһ�Ľӿڵ���, ��߿���չ��.
//		ͬʱ��߿���Ч��.
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================




struct req_t {
// ������Ϣ
1 : required i32 r_type;				// ��Ϣ����
2 : optional i32 r_size = 0;			// ��Ϣ����
3 : optional string r_ver = "v0.1.0";	// �汾��
4 : optional string r_str;				// ��Ϣ����
5 : optional string r_md5;				// MD5У��
6 : optional list<i32> r_sha1;			// SHA1У��
}


struct res_t {
// Ӧ����Ϣ
1 : required i32 r_type;				// ��Ϣ����
2 : optional i32 r_size = 0;			// ��Ϣ����
3 : optional i32 r_errcode = 0;			// ����code
4 : optional string r_ver = "v0.1.0";	// �汾��
5 : optional string r_str;				// ��Ϣ����
6 : optional string r_errstr;			// ������Ϣ
}



// ============================
// @��; : Service ����ӿ���
// @PS : ���ֽ��� "��ͯľ"
// ============================
service Astroboy {
	// ============================
	// @��; : ���� string ������Ϣ, �̳��� AMQ_ISender
	// @req_msg : �������Ϣ
	// @����ֵ : Ӧ����Ϣ
	// @���ֽ��� ��ˮ��ʿ(��ͯľ�Ĵ�����)
	// ============================
	res_t Elefun(1: req_t req_msg);
}