#ifndef	ACL_SANE_SOCKET_INCLUDE_H
#define	ACL_SANE_SOCKET_INCLUDE_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "stdlib/acl_define.h"

/**
 * ȡ���׽������ӶԷ��������ַ, ��ַ��ʽΪ: IP:PORT
 * @param fd {ACL_SOCKET} �����׽���
 * @param buf {char*} �洢��ַ�Ļ�����������Ϊ��
 * @param bsize {size_t} buf �ռ��С
 * @return {int} 0: ok; -1: error
 */
ACL_API int acl_getpeername(ACL_SOCKET fd, char *buf, size_t bsize);

/**
 * ȡ���׽������ӱ��ص������ַ, ��ַ��ʽΪ: IP:PORT
 * @param fd {ACL_SOCKET} �����׽���
 * @param buf {char*} �洢��ַ�Ļ�����������Ϊ��
 * @param bsize {size_t} buf �ռ��С
 * @return {int} 0: ok; -1: error
 */
ACL_API int acl_getsockname(ACL_SOCKET fd, char *buf, size_t bsize);

/**
 * ȡ���׽��ֵ����ͣ��������׽��ֻ������׽���
 * @param fd {ACL_SOCKET} �����׽���
 * @return {int} -1: ��ʾ���������Ƿ�����׽���; >= 0 ��ʾ�ɹ�����׽���
 *  ���ͣ�����ֵ�� AF_INET �� AF_UNIX(���� UNIX ƽ̨)
 */
ACL_API int acl_getsocktype(ACL_SOCKET fd);

/**
 * ����׽��֣��Ǽ����׽��ֻ��������׽���
 * @param sockefd {ACL_SOCKET} �׽��־��
 * @return {int} ���� -1 ��ʾ�þ�����׽��֣�1 Ϊ�����׽��֣�0 Ϊ�Ǽ����׽���
 */
ACL_API int acl_check_socket(ACL_SOCKET fd);

/**
 * �ж��׽����Ƿ�Ϊ�����׽���
 * @param sockefd {ACL_SOCKET} �׽��־��
 * @return {int} ����ֵ 0 ��ʾ�Ǽ����׽��֣��� 0 ��ʾΪ�����׽���
 */
ACL_API int acl_is_listening_socket(ACL_SOCKET fd);

#ifdef	__cplusplus
}
#endif

#endif

