#ifndef	__ACL_XMLCODE_INCLUDE_H__
#define	__ACL_XMLCODE_INCLUDE_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include "stdlib/acl_define.h"
#include "stdlib/acl_vstring.h"

ACL_API int acl_xml_encode(const char *in, ACL_VSTRING *out);
ACL_API int acl_xml_decode(const char *in, ACL_VSTRING *out);

/**
 * xml �ַ�������
 * @param in {const char**} Դ����ַ�ĵ�ַ���������غ�õ�ַ��¼δ����������ݵ�ַ��
 *  �������������㹻����õ�ַ��ָ��Դ��β���ĺ�һ��λ��
 * @param ilen {size_t} Դ���ݵ����ݳ���
 * @param out {char*} ����������������洢ת���Ľ��
 * @param olen {size_t} ����������Ĵ�С
 * @return {size_t} ת���洢������������ڵ����ݳ���(�ó����п��ܴ���Դ���ݳ���)��
 *  ���������������Ϊ��
 *  1) == 0 ʱ������ 0
 *  2) == 1 ʱ������ 0�������һ���ֽڱ��� '\0'
 *  3) > 1 ʱ�����һ���ֽڱ��� '\0'������ֵ > 0(���������� '\0')
 *  ע��
 *  1) �������غ� in �ĵ�ַ�ᷢ���ı䣬ָ����һ��������ĵ�ַ
 *  2) �������ڵ���ǰӦ�ȱ��� in �ĵ�ַ���� in ��δ�������ʣ��ĳ��ȼ��㷽ʽ��
 *     ilen -= in_saved - in;
 *  3) ��Ȼ�� olen > 0 ʱ�ڲ��Զ���� out ��β���� '\0���������ص����ݳ���
 *     ���������� '\0'
 */
ACL_API size_t acl_xml_encode2(const char** in, size_t ilen,
		char* out, size_t olen);

ACL_API const char *acl_xml_decode2(const char *in, char **out, size_t *size);

#ifdef	__cplusplus
}
#endif

#endif
