#ifndef	ACL_DBUF_POOL_INCLUDE_H
#define	ACL_DBUF_POOL_INCLUDE_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct ACL_DBUF_POOL ACL_DBUF_POOL;

/**
 * �����ڴ�ض���
 * @param block_size {size_t} �ڴ����ÿ�������ڴ��Ĵ�С���ֽڣ�
 * @return {ACL_DBUF_POOL*} ���ط� NULL ����
 */
ACL_API ACL_DBUF_POOL *acl_dbuf_pool_create(size_t block_size);

/**
 * �����ڴ��״̬���Ὣ������ڴ����ݿ��ͷ�
 * @param pool {ACL_DBUF_POOL*} �ڴ�ض���
 * @param off {size_t} Ҫ��������С�ڴ����ƫ��λ��
 * @return {int} ���� 0 ��ʾ�����ɹ����� 0 ��ʾʧ��
 */
ACL_API int  acl_dbuf_pool_reset(ACL_DBUF_POOL *pool, size_t off);

/**
 * �����ڴ��
 * @param pool {ACL_DBUF_POOL*} ����ض���
 */
ACL_API void acl_dbuf_pool_destroy(ACL_DBUF_POOL *pool);

/**
 * ����ָ�����ȵ��ڴ�
 * @param pool {ACL_DBUF_POOL*} ����ض���
 * @param  len {size_t} ��Ҫ������ڴ��С
 * @return {void*} �·�����ڴ��ַ
 */
ACL_API void *acl_dbuf_pool_alloc(ACL_DBUF_POOL *pool, size_t len);

/**
 * ����ָ�����ȵ��ڴ沢���ڴ���������
 * @param pool {ACL_DBUF_POOL*} ����ض���
 * @param len {size_t} ��Ҫ������ڴ泤��
 * @return {void*} �·�����ڴ��ַ
 */
ACL_API void *acl_dbuf_pool_calloc(ACL_DBUF_POOL *pool, size_t len);

/**
 * ����������ַ�����̬�����µ��ڴ沢���ַ������и��ƣ������� strdup
 * @param pool {ACL_DBUF_POOL*} ����ض���
 * @param s {const char*} Դ�ַ���
 * @return {char*} �¸��Ƶ��ַ�����ַ
 */
ACL_API char *acl_dbuf_pool_strdup(ACL_DBUF_POOL *pool, const char *s);

/**
 * ����������ַ�����̬�����µ��ڴ沢���ַ������и��ƣ������� strdup
 * @param pool {ACL_DBUF_POOL*} ����ض���
 * @param s {const char*} Դ�ַ���
 * @param len {size_t} �޶�������ַ�������
 * @return {char*} �¸��Ƶ��ַ�����ַ
 */
ACL_API char *acl_dbuf_pool_strndup(ACL_DBUF_POOL *pool,
	const char *s, size_t len);

/**
 * ����������ڴ����ݶ�̬�����ڴ沢�����ݽ��и���
 * @param pool {ACL_DBUF_POOL*} ����ض���
 * @param addr {const void*} Դ�����ڴ��ַ
 * @param len {size_t} Դ���ݳ���
 * @return {void*} �¸��Ƶ����ݵ�ַ
 */
ACL_API void *acl_dbuf_pool_memdup(ACL_DBUF_POOL *pool,
		const void *addr, size_t len);

/**
 * �黹���ڴ�ط�����ڴ�
 * @param pool {ACL_DBUF_POOL*} ����ض���
 * @param addr {const void*} ���ڴ�ط�����ڴ��ַ
 * @return {int} ������ڴ��ַ���ڴ�ط�����ͷŶ�Σ��򷵻� -1�������ɹ���
 *  ���� 0
 */
ACL_API int acl_dbuf_pool_free(ACL_DBUF_POOL *pool, const void *addr);

/**
 * �������ڴ�ط����ĳ�ε�ַ�����⵱���� reset ʱ����ǰ�ͷŵ�
 * @param pool {ACL_DBUF_POOL*} ����ض���
 * @param addr {const void*} ���ڴ�ط�����ڴ��ַ
 * @return {int} �����ɹ��򷵻� 0��������ڴ��ַ���ڴ�ط��䣬�򷵻� -1
 */
ACL_API int acl_dbuf_pool_keep(ACL_DBUF_POOL *pool, const void *addr);

/**
 * ȡ���������ڴ�ط����ĳ�ε�ַ���Ա��ڵ��� dbuf_reset ʱ���ͷŵ�
 * @param pool {ACL_DBUF_POOL*} ����ض���
 * @param addr {const void*} ���ڴ�ط�����ڴ��ַ
 * @return {int} �����ɹ��򷵻� 0��������ڴ��ַ���ڴ�ط��䣬�򷵻� -1
 */
ACL_API int acl_dbuf_pool_unkeep(ACL_DBUF_POOL *pool, const void *addr);

/**
 * �ڲ������ú���
 */
ACL_API void acl_dbuf_pool_test(size_t max);

#ifdef	__cplusplus
}
#endif

#endif
