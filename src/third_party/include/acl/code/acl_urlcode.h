#ifndef ACL_URLCODE_INCLUDE_H
#define ACL_URLCODE_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdlib/acl_define.h"
#include "stdlib/acl_dbuf_pool.h"

/**
 * URL ���뺯��
 * @param str {const char*} Դ�ַ���
 * @param dbuf {ACL_DBUF_POOL*} �ڴ�ض�������ǿգ����ڲ�ʹ�ø��ڴ�ؽ���
 *  �ڴ�Ķ�̬���䣬������ʹ�� acl_mymalloc ���䶯̬�ڴ�
 * @return {char*} �������ַ���������ֵ������Ϊ�գ���Ҫ�� acl_myfree �ͷ�
 */
ACL_API char *acl_url_encode(const char *str, ACL_DBUF_POOL *dbuf);

/**
 * URL ���뺯��
 * @param str {const char*} ��URL�������ַ���
 * @param dbuf {ACL_DBUF_POOL*} �ڴ�ض�������ǿգ����ڲ�ʹ�ø��ڴ�ؽ���
 *  �ڴ�Ķ�̬���䣬������ʹ�� acl_mymalloc ���䶯̬�ڴ�
 * @return {char*} �������ַ���������ֵ������Ϊ�գ���Ҫ�� acl_myfree �ͷ�
 */
ACL_API char *acl_url_decode(const char *str, ACL_DBUF_POOL *dbuf);

#ifdef __cplusplus
}
#endif
#endif
