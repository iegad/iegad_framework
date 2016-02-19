#ifndef ACL_XML3_INCLUDE_H
#define ACL_XML3_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdlib/acl_array.h"
#include "stdlib/acl_ring.h"
#include "stdlib/acl_vstream.h"
#include "stdlib/acl_htable.h"
#include "stdlib/acl_vstring.h"
#include "stdlib/acl_iterator.h"
#include "stdlib/acl_dbuf_pool.h"

typedef struct ACL_XML3	ACL_XML3;
typedef struct ACL_XML3_NODE	ACL_XML3_NODE;
typedef struct ACL_XML3_ATTR	ACL_XML3_ATTR;

struct ACL_XML3_ATTR {
	ACL_XML3_NODE *node;            /**< �����ڵ� */
	char *name;                     /**< ������ */
	char *value;                    /**< ����ֵ */

	size_t name_size;               /**< ���������� */
	size_t value_size;              /**< ����ֵ���� */

	/* private */
	int   quote;                    /**< �� 0 ��ʾ ' �� " */
	int   backslash;                /**< ת���ַ� \ */
	int   slash;                    /**< �Ƿ��� '/' ��־λ�趨 */
};

struct ACL_XML3_NODE {
	char *ltag;                     /**< ���ǩ�� */
	char *rtag;                     /**< �ұ�ǩ�� */
	size_t ltag_size;               /**< ���ǩ������ */
	size_t rtag_size;               /**< �ұ�ǩ������ */
	const char *id;                 /**< ID��ʶ��, ֻ���� xml->id_table
					  ���ڵĽڵ�� id �ŷǿ� */
	char *text;                     /**< �ı���ʾ���� */
	size_t text_size;               /**< �ļ����ݳ��� */

	ACL_ARRAY *attr_list;           /**< ����(ACL_XML3_ATTR)�б� */
	ACL_XML3_NODE *parent;          /**< ���ڵ� */
	ACL_RING children;              /**< �ӽڵ㼯�� */
	int  depth;                     /**< ��ǰ�ڵ����� */

	/* private */
	ACL_XML3 *xml;                  /**< xml ���� */
	ACL_RING node;                  /**< ��ǰ�ڵ� */
	ACL_XML3_ATTR *curr_attr;       /**< ��ǰ���ڽ��������� */
	int   quote;                    /**< �� 0 ��ʾ ' �� " */
	int   last_ch;                  /**< ����¼���ڵ��ǰһ���ֽ�ֵ */
	int   nlt;                      /**< '<' ���� */
	char  meta[3];                  /**< Ԫ������ʱ������ */

	unsigned int flag;
#define	ACL_XML3_F_META_QM	(1 << 0)    /**< '?' flag */
#define	ACL_XML3_F_META_CM	(1 << 1)    /**< '!--' flag */
#define	ACL_XML3_F_META_EM	(1 << 2)    /**< only '!' flag */
#define ACL_XML3_F_SELF_CL	(1 << 3)    /**< self closed flag */
#define	ACL_XML3_F_LEAF		(1 << 4)    /**< leaf node has no child node */

/**< �Ƿ���Ԫ���� */
#define	ACL_XML3_F_META		\
	(ACL_XML3_F_META_QM | ACL_XML3_F_META_CM | ACL_XML3_F_META_EM)

#define	ACL_XML3_IS_COMMENT(x)	(((x)->flag & ACL_XML3_F_META_CM))

	int   status;                   /**< ״̬����ǰ����״̬ */
#define ACL_XML3_S_NXT	0               /**< ��һ���ڵ� */
#define ACL_XML3_S_LLT	1               /**< ��� '<' */
#define ACL_XML3_S_LGT	2               /**< �ұ� '>' */
#define	ACL_XML3_S_LCH	3               /**< ��� '<' ���һ���ֽ� */
#define ACL_XML3_S_LEM	4               /**< ��� '<' ��� '!' */
#define ACL_XML3_S_LTAG	5               /**< ��ߵı�ǩ�� */
#define ACL_XML3_S_RLT	6               /**< �ұߵ� '<' */
#define ACL_XML3_S_RGT	7               /**< �ұߵ� '>' */
#define ACL_XML3_S_RTAG	8               /**< �ұߵı�ǩ�� */
#define ACL_XML3_S_ATTR	9               /**< ��ǩ������ */
#define ACL_XML3_S_AVAL	10              /**< ��ǩ����ֵ */
#define ACL_XML3_S_TXT	11              /**< �ڵ��ı� */
#define ACL_XML3_S_MTAG	12              /**< Ԫ���ݱ�ǩ */
#define ACL_XML3_S_MTXT	13              /**< Ԫ�����ı� */
#define ACL_XML3_S_MCMT	14              /**< Ԫ����ע�� */
#define ACL_XML3_S_MEND	15              /**< Ԫ���ݽ��� */

	/* for acl_iterator, ͨ�� acl_foreach �г��ýڵ��һ���ӽڵ� */

	/* ȡ������ͷ���� */
	ACL_XML3_NODE *(*iter_head)(ACL_ITER*, ACL_XML3_NODE*);
	/* ȡ��������һ������ */
	ACL_XML3_NODE *(*iter_next)(ACL_ITER*, ACL_XML3_NODE*);
	/* ȡ������β���� */
	ACL_XML3_NODE *(*iter_tail)(ACL_ITER*, ACL_XML3_NODE*);
	/* ȡ��������һ������ */
	ACL_XML3_NODE *(*iter_prev)(ACL_ITER*, ACL_XML3_NODE*);
};

struct ACL_XML3 {
	/* public */
					
	int   depth;                    /**< ������ */
	int   node_cnt;                 /**< �ڵ�����, ���� root �ڵ� */
	int   root_cnt;                 /**< ���ڵ���� */
	ACL_XML3_NODE *root;            /**< XML ���ڵ� */

	/* private */
	char  addr[1];
	ACL_HTABLE *id_table;           /**< id ��ʶ����ϣ�� */
	ACL_XML3_NODE *curr_node;       /**< ��ǰ���ڴ���� XML �ڵ� */
	ACL_DBUF_POOL *dbuf;            /**< �ڴ�ض��� */
	ACL_DBUF_POOL *dbuf_inner;      /**< �ڲ��ֲ����ڴ�ض��� */
	size_t dbuf_keep;               /**< �ڴ���б����ĳ��� */

	unsigned flag;                  /**< ��־λ: ACL_XML3_FLAG_xxx */ 

	/**< �Ƿ�����һ�� xml �ĵ����ж�����ڵ㣬�ڲ�ȱʡΪ���� */
#define	ACL_XML3_FLAG_MULTI_ROOT	(1 << 0)

	/**< �Ƿ���ݵ��ڵ���û�� '/' ��� */
#define	ACL_XML3_FLAG_IGNORE_SLASH	(1 << 1)

	/* for acl_iterator, ͨ�� acl_foreach �����г������ӽڵ� */

	/* ȡ������ͷ���� */
	ACL_XML3_NODE *(*iter_head)(ACL_ITER*, ACL_XML3*);
	/* ȡ��������һ������ */
	ACL_XML3_NODE *(*iter_next)(ACL_ITER*, ACL_XML3*);
	/* ȡ������β���� */
	ACL_XML3_NODE *(*iter_tail)(ACL_ITER*, ACL_XML3*);
	/* ȡ��������һ������ */
	ACL_XML3_NODE *(*iter_prev)(ACL_ITER*, ACL_XML3*);
};

/****************************************************************************/
/*                  ���������ӿڣ��û����Է���ʹ�øýӿڼ�                  */
/****************************************************************************/

/*----------------------------- in acl_xml3.c ------------------------------*/

/**
 * �ж� xml �����Ƿ�պϵ�, ���Ƿ��������������Ƿ�����, ����� xml �������
 * xml �ڵ�Ԫ��Ϊ��, ��Ҳ��Ϊ���Ǳպϵ�
 * @param xml {ACL_XML3*} xml ����
 * @return {int} 0: ��; 1: ��
 */
ACL_API int acl_xml3_is_closure(ACL_XML3 *xml);

/**
 * ����ָ����ǩ���ж� xml �����Ѿ����, ���ñ�ǩ�� xml ������ root һ���ӽڵ�
 * �е����һ�� xml �ڵ�ı�ǩ��ͬʱ, ����Ϊ xml �������, Ϊ��֤�жϵ���ȷ��,
 * ����ԴӦ��֤�����ĸ��ڵ�ֻ��һ��, �� xml->root ��һ���ӽڵ�ֻ��һ��, ����
 * ���������
 * @param xml {ACL_XML3*} xml ����
 * @param tag {const char*} �û�������ǩ��, �ڲ���ƥ��ʱ�����ִ�Сд
 * @return {int} 0: ��; 1: ��
 */
ACL_API int acl_xml3_is_complete(ACL_XML3 *xml, const char *tag);

/**
 * ����һ�� xml ����
 * @return {ACL_XML3*} �´����� xml ����
 */
ACL_API ACL_XML3 *acl_xml3_alloc(void);

/**
 * ����һ�� xml ���󣬸� xml �������е��ڲ��ڴ���䶼�ڸ��ڴ���Ͻ��з���
 * @param dbuf {ACL_DBUF_POOL*} �ڴ�ض��󣬵�����Է� NULL ʱ���� xml ����
 *  �������ڵ��ڴ���������Ͻ��з��䣬�����ڲ��Զ����������� xml ���ڴ��
 * @return {ACL_XML3*} �´����� xml ����
 */
ACL_API ACL_XML3 *acl_xml3_dbuf_alloc(ACL_DBUF_POOL *dbuf);

/**
 * ��ĳһ�� ACL_XML3_NODE �ڵ���Ϊһ�� XML ����ĸ��ڵ㣬�Ӷ����Է���ر�������
 * �ڵ�����ӽڵ�(�ڱ��������е����нڵ㲻�����ڵ�����)���ñ�����ʽ�б��ڵ���
 * ����ĳһ�� ACL_XML3_NODE �ڵ�ʱ���ܱ�����һ���ӽڵ������
 * @param xml {ACL_XML3*} xml ����
 * @param node {ACL_XML3_NODE*} AXL_XML_NODE �ڵ�
 */
ACL_API void acl_xml3_foreach_init(ACL_XML3 *xml, ACL_XML3_NODE *node);

/**
 * ����һ�� xml �ĵ����Ƿ������ж���� xml �ڵ㣬�ڲ�ȱʡ֧�ֶ�����ڵ�
 * @param xml {ACL_XML3*} xml ����
 * @param on {int} �� 0 ������Ϊ 0 ��ʾ����������ֹ�ж���� xml �ڵ�ʱ��
 *  ���ڽ���ʱ��������һ�����ڵ����ʱ�㷵��ʣ�������
 */
ACL_API void acl_xml3_multi_root(ACL_XML3 *xml, int on);

/**
 * ���� XML ���ڵ�����, �Ƿ��������û�� /, ��:
 * <test id=111>, <test id=111 />, ����������û�� / ��������д��
 * ���ǺϷ��ģ�����ֻ�еڶ���д���ǺϷ��ģ�����������ּ����ԣ���
 * �����һ����������ʧ
 * @param xml {ACL_XML3*} xml ����
 * @param ignore {int} ����� 0 ��ʾ���ڵ������ /
 */
ACL_API void acl_xml3_slash(ACL_XML3 *xml, int ignore);

/**
 * �ͷ�һ�� xml ����, ͬʱ�ͷŸö��������ɵ����� xml �ڵ�
 * @param xml {ACL_XML3*} xml ����
 * @return {int} �����ͷŵ� xml �ڵ����
 */
ACL_API int acl_xml3_free(ACL_XML3 *xml);

/**
 * ���� XML ����������
 * @param xml {ACL_XML3*} xml ����
 */
ACL_API void acl_xml3_reset(ACL_XML3 *xml);

/*------------------------- in acl_xml3_parse.c ----------------------------*/

/**
 * ���� xml ����, ���������Զ����� xml �ڵ���
 * @param xml {ACL_XML3*} xml ����
 * @param data {char*} �� '\0' ��β�������ַ���, ������������ xml ����;
 *  Ҳ�����ǲ������� xml ����, ����ѭ�����ô˺���, �����������ݳ���������
 * @return {char*} ��ͨ�� acl_xml3_multi_root ����һ�� xml �ĵ�������
 *  ����� xml �ڵ�ʱ���ú������صĵ�ַ���ֽ�Ϊ '\0'; ���򷵻�ʣ������ݵ�ַ
 *  �����ǿ��ַ���
 *  ע��Ҳ����ͨ�� acl_xml3_is_complete �ж��Ƿ�������
 */
ACL_API char *acl_xml3_update(ACL_XML3 *xml, char *data);
#define	acl_xml3_parse	acl_xml3_update

/*------------------------- in acl_xml3_util.c -----------------------------*/

/**
 * ��ʼ�������� input, br, hr �ȵ��Ապϱ�ǩ, �γ��Ապϱ�ǩ��, �Ա���
 * acl_xml3_tag_selfclosed ��ѯ����, ���������ǩ�Ƿ��Ǳ������Ապϱ�ǩ,
 * �ú���ֻ�ܱ���ʼ��һ��, Ҳ���Բ���ʼ��
 */
ACL_API void acl_xml3_tag_init(void);

/**
 * �����û��Լ����һЩ���Ապϵı�ǩ
 * @param tag {const char*} ��ǩ����ע���ǩ���Ȳ��ô��� 254 ���ֽ�
 */
ACL_API void acl_xml3_tag_add(const char *tag);

/**
 * ������ acl_xml3_tag_init ��ʼ���������Ապϱ�ǩ����, ���Ե��ô˺����ж�����
 * ��ǩ�Ƿ������Ապϱ�ǩ, ���δ���� acl_xml3_tag_init, ��ú�����Զ���� 0
 * @parma tag {const char*} ��ǩ����
 * @return {int} 0: ��ʾ��, 1: ��ʾ��
 */
ACL_API int  acl_xml3_tag_selfclosed(const char *tag);

/**
 * �жϱ�ǩ���� xml �ڵ��Ƿ���Ҷ�ڵ�, Ҷ�ڵ�û���ӽڵ�
 * @param tag {const char*} ��ǩ��
 * @return {int} 0: ����Ҷ�ڵ�; 1: ��Ҷ�ڵ�
 */
ACL_API int  acl_xml3_tag_leaf(const char *tag);

/**
 * �ͷ��� acl_xml3_getElementsByTagName, acl_xml3_getElementsByName,
 * acl_xml3_getElementsByAttr �Ⱥ������صĶ�̬�������, ��Ϊ�ö�̬�����е�
 * Ԫ�ض��� ACL_XML3 ������Ԫ�ص�����, �����ͷŵ��ö�̬�����, ֻҪ ACL_XML3
 * �����ͷ�, ��ԭ�����ڸ������е�Ԫ����Ȼ����ʹ��.
 * �������ͷ������ xml �ڵ�Ԫ��
 * @param a {ACL_ARRAY*} ��̬�������
 */
ACL_API void acl_xml3_free_array(ACL_ARRAY *a);

/**
 * �� xml �����л����������ǩ����ͬ�� xml ��һ���ڵ�
 * @param xml {ACL_XML3*} xml ����
 * @param tag {const char*} ��ǩ����
 * @return {ACL_XML3_NODE*} ���������� xml �ڵ�, ������ NULL ��
 *  ��ʾû�з��������� xml �ڵ�
 */
ACL_API ACL_XML3_NODE *acl_xml3_getFirstElementByTagName(
	ACL_XML3 *xml, const char *tag);

/**
 * �� xml �����л�����е���������ǩ����ͬ�� xml �ڵ�ļ���
 * @param xml {ACL_XML3*} xml ����
 * @param tag {const char*} ��ǩ����
 * @return {ACL_ARRAY*} ���������� xml �ڵ㼯��, ���� ��̬������, ������ NULL ��
 *  ��ʾû�з��������� xml �ڵ�, �ǿ�ֵ��Ҫ���� acl_xml3_free_array �ͷ�
 */
ACL_API ACL_ARRAY *acl_xml3_getElementsByTagName(
	ACL_XML3 *xml, const char *tag);

/**
 * �� xml �����л�����е�������༶��ǩ����ͬ�� xml �ڵ�ļ���
 * @param xml {ACL_XML3*} xml ����
 * @param tags {const char*} �༶��ǩ������ '/' �ָ�������ǩ��������� xml ���ݣ�
 *  <root> <first> <second> <third name="test1"> text1 </third> </second> </first>
 *  <root> <first> <second> <third name="test2"> text2 </third> </second> </first>
 *  <root> <first> <second> <third name="test3"> text3 </third> </second> </first>
 *  ����ͨ���༶��ǩ����root/first/second/third һ���Բ�����з��������Ľڵ�
 * @return {ACL_ARRAY*} ���������� xml �ڵ㼯��, ���� ��̬������, ������ NULL ��
 *  ��ʾû�з��������� xml �ڵ�, �ǿ�ֵ��Ҫ���� acl_xml3_free_array �ͷ�
 */
ACL_API ACL_ARRAY *acl_xml3_getElementsByTags(ACL_XML3 *xml, const char *tags);

/**
 * �� xml �����л�����е������������ name ������ֵ��ͬ�� xml �ڵ�Ԫ�ؼ���
 * @param xml {ACL_XML3*} xml ����
 * @param value {const char*} ������Ϊ name ������ֵ
 * @return {ACL_ARRAY*} ���������� xml �ڵ㼯��, ���� ��̬������, ������ NULL ��
 *  ��ʾû�з��������� xml �ڵ�, �ǿ�ֵ��Ҫ���� acl_xml3_free_array �ͷ�
 */
ACL_API ACL_ARRAY *acl_xml3_getElementsByName(ACL_XML3 *xml, const char *value);

/**
 * �� xml �����л�����и���������������ֵ�� xml �ڵ�Ԫ�ؼ���
 * @param xml {ACL_XML3*} xml ����
 * @param name {const char*} ������
 * @param value {const char*} ����ֵ
 * @return {ACL_ARRAY*} ���������� xml �ڵ㼯��, ���� ��̬������, ������ NULL ��
 *  ��ʾû�з��������� xml �ڵ�, �ǿ�ֵ��Ҫ���� acl_xml3_free_array �ͷ�
 */
ACL_API ACL_ARRAY *acl_xml3_getElementsByAttr(ACL_XML3 *xml,
	const char *name, const char *value);

/**
 * �� xml �����л��ָ�� id ֵ�� xml �ڵ�Ԫ�ص�ĳ�����Զ���
 * @param xml {ACL_XML3*} xml ����
 * @param id {const char*} id ֵ
 * @return {ACL_XML3_ATTR*} ĳ xml �ڵ��ĳ�����Զ���, ������ NULL ���ʾ
 *  û�з�������������, ����ֵ����Ҫ�ͷ�
 */
ACL_API ACL_XML3_ATTR *acl_xml3_getAttrById(ACL_XML3 *xml, const char *id);

/**
 * �� xml �����л��ָ�� id ֵ�� xml �ڵ�Ԫ�ص�ĳ������ֵ
 * @param xml {ACL_XML3*} xml ����
 * @param id {const char*} id ֵ
 * @return {const char*} ĳ xml �ڵ��ĳ������ֵ, ������ NULL ���ʾû�з���
 *  ����������
 */
ACL_API const char *acl_xml3_getAttrValueById(ACL_XML3 *xml, const char *id);

/**
 * �� xml �����л��ָ�� id ֵ�� xml �ڵ�Ԫ��
 * @param xml {ACL_XML3*} xml ����
 * @param id {const char*} id ֵ
 * @return {ACL_XML3_NODE*} xml �ڵ�Ԫ��, ������ NULL ���ʾû�з���
 *  ������ xml �ڵ�, ����ֵ����Ҫ�ͷ�
 */
ACL_API ACL_XML3_NODE *acl_xml3_getElementById(ACL_XML3 *xml, const char *id);

/**
 * �� xml ��������ȡ���� ? ! �ȿ�ͷ�Ľڵ�
 * @param xml {ACL_XML3*} xml ����
 * @param tag {const char*} ��ǩ��
 * @return {ACL_XML3_NODE*} xml �ڵ�Ԫ��, ������ NULL ���ʾû�з���
 *  ������ xml �ڵ�, ����ֵ����Ҫ�ͷ�
 */
ACL_API ACL_XML3_NODE *acl_xml3_getElementMeta(ACL_XML3 *xml, const char *tag);

/**
 * ��� xml ���ַ��������ʽ
 * @param xml {ACL_XML3*} xml ����
 * @return {const char*} �����ַ��������ʽ������ NULL ʱ��ʾû�и�����
 */
ACL_API const char *acl_xml3_getEncoding(ACL_XML3 *xml);

/**
 * ��� xml ���ݵ����ͣ��磺text/xsl
 * @param xml {ACL_XML3*} xml ����
 * @return {const char*} ���� NULL ��ʾû�и�����
 */
ACL_API const char *acl_xml3_getType(ACL_XML3 *xml);

/**
 * �� xml �ڵ��л��ָ�������������Զ���
 * @param node {ACL_XML3_NODE*} xml �ڵ�
 * @param name {const char*} ��������
 * @return {ACL_XML3_ATTR*} ���Զ���, Ϊ�ձ�ʾ������, ����ֵ����Ҫ�ͷ�
 */
ACL_API ACL_XML3_ATTR *acl_xml3_getElementAttr(ACL_XML3_NODE *node,
		const char *name);

/**
 * �� xml �ڵ��л��ָ��������������ֵ
 * @param node {ACL_XML3_NODE*} xml �ڵ�
 * @param name {const char*} ��������
 * @return {const char*} ����ֵ, Ϊ�ձ�ʾ������
 */
ACL_API const char *acl_xml3_getElementAttrVal(
			ACL_XML3_NODE *node, const char *name);

/**
 * �� xml �ڵ�ɾ��ĳ�����Զ���, ���������Ϊ id ����, ��ͬʱ��� xml->id_table ��ɾ��
 * @param node {ACL_XML3_NODE*} xml �ڵ�
 * @param name {const char*} ��������
 * @return {int} 0 ��ʾɾ���ɹ�, -1: ��ʾɾ��ʧ��(�п����Ǹ����Բ�����)
 */
ACL_API int acl_xml3_removeElementAttr(ACL_XML3_NODE *node, const char *name);

#if 0
/**
 * �� xml �ڵ��������, ������������Ѵ���, �����µ�����ֵ�滻������ֵ, ����
 * ����������µ����Զ���
 * @param node {ACL_XML3_NODE*} xml �ڵ�
 * @param name {const char*} ��������
 * @param value {const char*} ����ֵ
 * @return {ACL_XML3_ATTR*} ���ظ����Զ���(�п�����ԭ����, Ҳ�п������µ�), 
 *  ����ֵ�����ͷ�
 */
ACL_API ACL_XML3_ATTR *acl_xml3_addElementAttr(ACL_XML3_NODE *node,
		const char *name, const char *value);

/**
 * ����ǩ�����ڵ��ı���Ϊ�������� xml �ڵ㣬�ú�����Ҫ���ڹ��� xml ����ʱ
 * @param xml {ACL_XML3*} xml ���󣬸ö���Ӧ������ acl_xml3_alloc ������
 * @param tagname {const char*} ��ǩ��������ǿ����ַ������ȴ��� 0
 * @param text {const char*} �ڵ���ı����ݣ�����Ϊ��
 * @return {ACL_XML3_NODE*} �´����� xml �ڵ㣬�÷�����Զ���طǿգ��������
 *  �����Ƿ���ᵼ���ڲ��Զ���������
 */
ACL_API ACL_XML3_NODE *acl_xml3_create_node(ACL_XML3 *xml,
	const char *tagname, const char *text);

/**
 * ��һ�� xml �ڵ�������ԣ��ú�����Ҫ���ڹ��� xml ����ʱ
 * @param node {ACL_XML3_NODE*} �� acl_xml3_create_node �����Ľڵ�
 * @param name {const char*} ������������Ϊ�ǿ��ַ������ַ������ȴ��� 0
 * @param value {const char*} ����ֵ������Ϊ��
 * @return {ACL_XML3_ATTR*} xml �ڵ�����Զ��󣬵���������Ƿ�ʱ�ú���
 *  �ڲ��Զ���������
 */
ACL_API ACL_XML3_ATTR *acl_xml3_node_add_attr(ACL_XML3_NODE *node,
	const char *name, const char *value);

/**
 * ��һ�� xml �ڵ����һ�����ԣ��ú�����Ҫ���ڹ��� xml ����ʱ
 * @param node {ACL_XML3_NODE*} �� acl_xml3_create_node �����Ľڵ�
 * @param ... һ�����ԣ����� NULL ʱ��ʾ�������磺
 *  {name1}, {value1}, {name2}, {value2}, ... NULL
 */
ACL_API void acl_xml3_node_add_attrs(ACL_XML3_NODE *node, ...);

/**
 * ��һ�� xml �ڵ�����ı����ݣ��ú�����Ҫ���ڹ��� xml ����ʱ
 * @param node {ACL_XML3_NODE*} �� acl_xml3_create_node �����Ľڵ�
 * @param text {const char*} �ı�����
 */
ACL_API void acl_xml3_node_set_text(ACL_XML3_NODE *node, const char *text);

/**
 * �� xml ����ת���ַ�������
 * @param xml {ACL_XML3*} xml ����
 * @param buf {ACL_VSTRING*} �洢������Ļ����������ò���Ϊ��ʱ�����ڲ���
 *  �Զ�����һ�λ�������Ӧ���������Ҫ�ͷŵ����ǿպ����ڲ���ֱ�ӽ�����洢����
 * @return {ACL_VSTRING*} xml ����ת�����ַ�����Ĵ洢���������÷���ֵ��Զ�ǿգ�
 *  ʹ���߿���ͨ�� ACL_VSTRING_LEN(x) �����ж������Ƿ�Ϊ�գ����ص� ACL_VSTRING
 *  ָ�����Ϊ�ú����ڲ������ģ����û��������� acl_vstring_free �����ͷ�
 */
ACL_API ACL_VSTRING* acl_xml3_build(ACL_XML3* xml, ACL_VSTRING *buf);

/**
 * �� xml ����ת����ָ�����У�ע����ת����Ϣ��Ϊ�����õ�����
 * @param xml {ACL_XML3*} xml ����
 * @param fp {ACL_VSTREAM*} ������
 */
ACL_API void acl_xml3_dump(ACL_XML3 *xml, ACL_VSTREAM *fp);

/**
 * �� xml ����ת����ָ���������У�ע����ת����Ϣ��Ϊ�����õ�����
 * @param xml {ACL_XML3*} xml ����
 * @param buf {ACL_VSTRING*} ������, ��Ҫ�û��Լ�����ռ�
 */
ACL_API void acl_xml3_dump2(ACL_XML3 *xml, ACL_VSTRING *buf);

#endif

/***************************************************************************/
/*          ����Ϊ��Ϊ�ͼ��Ľӿ�, �û����Ը�����Ҫ�������½ӿ�             */
/***************************************************************************/

/*----------------------------- in acl_xml3.c ------------------------------*/

/**
 * ���� xml �ڵ������
 * @param node {ACL_XML3_NODE*} xml �ڵ�
 * @return {ACL_XML3_ATTR*} �´����Ľڵ�����
 */
ACL_API ACL_XML3_ATTR *acl_xml3_attr_alloc(ACL_XML3_NODE *node);

/**
 * ����һ�� xml �ڵ�
 * @param xml {ACL_XML3*} xml ����
 * @return {ACL_XML3_NODE*} xml �ڵ����
 */
ACL_API ACL_XML3_NODE *acl_xml3_node_alloc(ACL_XML3 *xml);

/**
 * ��ĳ�� xml �ڵ㼰���ӽڵ�� xml ������ɾ��, ���ͷŸýڵ㼰���ӽڵ���ռ�ռ�
 * �������ͷŸ� xml �ڵ���ռ�ڴ�
 * @param node {ACL_XML3_NODE*} xml �ڵ�
 * @return {int} ����ɾ���Ľڵ����
 */
ACL_API int acl_xml3_node_delete(ACL_XML3_NODE *node);

/**
 * ��ĳ�� xml �ڵ�����ֵܽڵ�(���ֵܽڵ�����Ƕ����� xml �ڵ�)
 * @param node1 {ACL_XML3_NODE*} �򱾽ڵ���� xml �ڵ�
 * @param node2 {ACL_XML3_NODE*} ����ӵ��ֵ� xml �ڵ�
 */
ACL_API void acl_xml3_node_append(ACL_XML3_NODE *node1, ACL_XML3_NODE *node2);

/**
 * ��ĳ�� xml �ڵ���Ϊ�ӽڵ����ĳ�� xml �ڵ���
 * @param parent {ACL_XML3_NODE*} ���ڵ�
 * @param child {ACL_XML3_NODE*} �ӽڵ�
 */
ACL_API void acl_xml3_node_add_child(ACL_XML3_NODE *parent, ACL_XML3_NODE *child);

/**
 * ���ĳ�� xml �ڵ�ĸ��ڵ�
 * @param node {ACL_XML3_NODE*} xml �ڵ�
 * @return {ACL_XML3_NODE*} ���ڵ�, ���Ϊ NULL ���ʾ�丸�ڵ㲻����
 */
ACL_API ACL_XML3_NODE *acl_xml3_node_parent(ACL_XML3_NODE *node);

/**
 * ���ĳ�� xml �ڵ�ĺ�һ���ֵܽڵ�
 * @param node {ACL_XML3_NODE*} xml �ڵ�
 * @return {ACL_XML3_NODE*} ���� xml �ڵ�ĺ�һ���ֵܽڵ�, ��ΪNULL���ʾ������
 */
ACL_API ACL_XML3_NODE *acl_xml3_node_next(ACL_XML3_NODE *node);

/**
 * ���ĳ�� xml �ڵ��ǰһ���ֵܽڵ�
 * @param node {ACL_XML3_NODE*} xml �ڵ�
 * @return {ACL_XML3_NODE*} ���� xml �ڵ��ǰһ���ֵܽڵ�, ��ΪNULL���ʾ������
 */
ACL_API ACL_XML3_NODE *acl_xml3_node_prev(ACL_XML3_NODE *node);

#ifdef __cplusplus
}
#endif
#endif
