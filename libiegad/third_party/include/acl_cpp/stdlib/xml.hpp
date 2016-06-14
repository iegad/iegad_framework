#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include <vector>
#include <list>
#include "acl_cpp/stdlib/dbuf_pool.hpp"
#include "acl_cpp/stdlib/string.hpp"
#include "acl_cpp/stdlib/pipe_stream.hpp"

struct ACL_TOKEN;
struct ACL_ITER;

/**
 * �� ACL ���� XML ������ķ�װ������ C++ �û�ʹ�ã������̫ע���������أ�
 * ����ֱ��ʹ�ø��࣬����ڷ����ִ���ҷǳ�ע�����ܣ�����ֱ��ʹ�� ACL ���
 * XML ����������Ϊ����Ҳ�ǵ����� ACL ���е� XML �������̣������ж��ο���
 * ���̣����ܻ���΢Ӱ��һЩ���ܣ�������һ���Ӧ�����Ӱ����΢�������
 */

namespace acl {

class xml;
class xml_node;

class ACL_CPP_API xml_attr : public dbuf_obj
{
public:
	/**
	 * ���������
	 * @return {const char*} ����������Զ���᷵�ؿ�ָ�룬����ֵ
	 *  �п���Ϊ "\0"
	 */
	virtual const char* get_name(void) const = 0;

	/**
	 * �������ֵ
	 * @return {const char*} ����ֵ����Զ���᷵�ؿ�ָ�룬����ֵ
	 *  �п���Ϊ "\0"
	 */
	virtual const char* get_value(void) const = 0;

protected:
	friend class xml_node;

	xml_attr(xml_node* node) : node_(node) {}
	virtual ~xml_attr(void) {}

	xml_node* node_;
};

class ACL_CPP_API xml_node : public dbuf_obj
{
public:
	/**
	 * ȡ�ñ� XML �ڵ�ı�ǩ��
	 * @return {const char*} ���� XML �ڵ��ǩ����������ؿգ���˵��
	 *  �����ڱ�ǩ xxxx���Է���һ����������Ҫ�жϷ���ֵ
	 */
	virtual const char* tag_name(void) const = 0;

	/**
	 * ����� XML �ڵ�� ID �����Բ����ڣ��򷵻ؿ�ָ��
	 * @return {const char*} �� ID ���Դ���ʱ���ض�Ӧ��ֵ�����򷵻ؿ�
	 */
	virtual const char* id(void) const = 0;

	/**
	 * ���ظ� XML �ڵ����������
	 * @return {const char*} ���ؿ�˵��û����������
	 */
	virtual const char* text(void) const = 0;

	/**
	 * ���ظ� XML �ڵ��ĳ������ֵ
	 * @param name {const char*} ������
	 * @return {const char*} ����ֵ��������ؿ���˵�������Բ�����
	 */
	virtual const char* attr_value(const char* name) const = 0;

	/**
	 * ���ظ� XML �ڵ��ĳ������ֵ�ı��д��
	 * @param name {const char*} ������
	 * @return {const char*} ����ֵ��������ؿ���˵�������Բ�����
	 */
	const char* operator[](const char* name) const;

	/**
	 * �����ڵ����������ʱ����Ҫ���ô˺�������õ�һ�����Զ���
	 * @return {const xml_attr*} ���ص�һ�����Զ�����Ϊ�գ����ʾ
	 *  �ýڵ�û������
	 */
	virtual const xml_attr* first_attr(void) const = 0;

	/**
	 * �����ڵ����������ʱ�����ñ����������һ�����Զ���
	 * @return {const xml_attr*} ������һ�����Զ�����Ϊ�գ����ʾ
	 *  �������
	 */
	virtual const xml_attr* next_attr(void) const = 0;

	/**
	 * ��� XML �ڵ�����
	 * @param name {const char*} ������
	 * @param value {const char*} ����ֵ
	 * @return {xml_node&}
	 */
	virtual xml_node& add_attr(const char* name, const char* value) = 0;

	/**
	 * ��� XML �ڵ�����
	 * @param name {const char*} ������
	 * @param n {char} ����ֵ
	 * @return {xml_node&}
	 */
	xml_node& add_attr(const char* name, char n);

	/**
	 * ��� XML �ڵ�����
	 * @param name {const char*} ������
	 * @param n {int} ����ֵ
	 * @return {xml_node&}
	 */
	xml_node& add_attr(const char* name, int n);

	/**
	 * ��� XML �ڵ�����
	 * @param name {const char*} ������
	 * @param n {size_t} ����ֵ
	 * @return {xml_node&}
	 */
	xml_node& add_attr(const char* name, size_t n);

	/**
	 * ��� XML �ڵ�����
	 * @param name {const char*} ������
	 * @param n {acl_int64} ����ֵ
	 * @return {xml_node&}
	 */
#if defined(_WIN32) || defined(_WIN64)
	xml_node& add_attr(const char* name, __int64 n);
#else
	xml_node& add_attr(const char* name, long long int n);
#endif

	/**
	 * ���� xml �ڵ���ı�����
	 * @param str {const char*} �ַ�������
	 * @return {xml_node&}
	 */
	virtual xml_node& set_text(const char* str) = 0;

	/**
	 * ���� xml �ڵ���ı�����
	 * @param number {long long int} 64 λ����
	 * @return {xml_node&}
	 */
#if defined(_WIN32) || defined(_WIN64)
	xml_node& set_text(__int64 number);
#else
	xml_node& set_text(long long int number);
#endif

	/**
	 * ���� xml �ڵ���� xml_node �ӽڵ����
	 * @param child {xml_node*} �ӽڵ����
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {xml_node&} return_child Ϊ true �����ӽڵ�����ã�
	 *  ���򷵻ر� xml �ڵ�����
	 */
	virtual xml_node& add_child(xml_node* child,
			bool return_child = false) = 0;

	/**
	 * ���� xml �ڵ���� xml_node �ӽڵ����
	 * @param child {xml_node&} �ӽڵ����
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {xml_node&} return_child Ϊ true �����ӽڵ�����ã�
	 *  ���򷵻ر� xml �ڵ�����
	 */
	xml_node& add_child(xml_node& child, bool return_child = false);

	/**
	 * ���� xml �ڵ���� xml_node �ӽڵ����
	 * @param tag {const char* tag} �ӽڵ����ı�ǩ��
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @param str {const char*} �ı��ַ���
	 * @return {xml_node&} return_child Ϊ true �����ӽڵ�����ã�
	 *  ���򷵻ر� xml �ڵ�����
	 */
	xml_node& add_child(const char* tag, bool return_child = false,
		const char* str = NULL);

	/**
	 * ���� xml �ڵ���� xml_node �ӽڵ����
	 * @param tag {const char* tag} �ӽڵ����ı�ǩ��
	 * @param number {long long int} 64 λ����
	 * @param return_child {bool} �Ƿ���Ҫ�����������´������ӽڵ������
	 * @return {xml_node&} return_child Ϊ true �����ӽڵ�����ã�
	 *  ���򷵻ر� xml �ڵ�����
	 */
#if defined(_WIN32) || defined(_WIN64)
	xml_node& add_child(const char* tag, __int64 number,
		bool return_child = false);
#else
	xml_node& add_child(const char* tag, long long int number,
		bool return_child = false);
#endif

	/**
	 * ��ñ��ڵ�ĸ����ڵ���������
	 * @return {xml_node&}
	 */
	virtual xml_node& get_parent(void) const = 0;

	/**
	 * ���ñ��ڵ�ĸ����ڵ�
	 * @param node {xml_node*} ���ڵ�
	 * @return {xml_node&}
	 */
	virtual xml_node& set_parent(xml_node* node) = 0;

	/**
	 * �����ڵ㼰���ӽڵ�� xml ���з��룬���ڴ潫�� xml ����ͳһ�ͷ�
	 * @return {int} ���ر��ͷŵĽڵ����
	 */
	virtual int detach(void) = 0;

	/**
	 * ��ñ��ڵ�ĵ�һ���ӽڵ㣬��Ҫ�����ӽڵ�ʱ�������ȵ��ô˺���
	 * @return {xml_node*} ���ؿձ�ʾû���ӽڵ�
	 */
	virtual xml_node* first_child(void) = 0;

	/**
	 * ��ñ��ڵ����һ���ӽڵ�
	 * @return {xml_node*} ���ؿձ�ʾ�������̽���
	 */
	virtual xml_node* next_child(void) = 0;

	/**
	 * ���ظ� XML �ڵ������� XML ���е����
	 * @return {int}
	 */
	virtual int depth(void) const = 0;

	/**
	 * �жϵ�ǰ�ڵ��Ƿ�Ϊ xml �����е� root �ڵ�
	 * @return {bool}
	 */
	virtual bool is_root(void) const = 0;

	/**
	 * ���ظ� xml �ڵ����һ���ӽڵ�ĸ���
	 * @return {int} ��Զ >= 0
	 */
	virtual int children_count(void) const = 0;

	/**
	 * ���ڱ����� xml �ڵ�ʱ���ڲ��ᶯ̬����һЩ��ʱ xml_node ���󣬵���
	 * �˺������������Щ����һ�����ô˺������������������
	 * first_child/next_child ���ص� xml_node �ڵ���󽫲��ٿ��ã������
	 * �����ڴ�Ƿ�����
	 */
	void clear(void);

	/**
	 * ��� xml ���������
	 * @return {xml&}
	 */
	xml& get_xml(void) const;

protected:
	friend class xml;
	friend class dbuf_guard;

	/**
	 * xml �ڵ㹹�캯��
	 * @param xml_ptr {xml*} xml �����󣬷ǿ�
	 */
	xml_node(xml* xml_ptr);

	/**
	 * Ҫ��ö�������Ƕ�̬������
	 */
	virtual ~xml_node(void);

protected:
	xml* xml_;
	std::vector<xml_node*> nodes_tmp_;
	std::vector<xml_attr*> attrs_tmp_;
};

class string;

class ACL_CPP_API xml : public pipe_stream, public dbuf_obj
{
public:
	xml(void);
	virtual ~xml(void);

	/**
	 * ���ڷǱպϵı�ǩ���Ƿ���Ҫ���Ապ��ַ� '/'��ȱʡΪ������
	 * @param on {bool}
	 * @return {xml&}
	 */
	virtual xml& ignore_slash(bool on) = 0;

	/**
	 * ���� xml ����ʱ���Ƿ��Զ����� xml ���룬ȱʡ����
	 * @param on {bool}
	 * @return {xml&}
	 */
	virtual xml& xml_decode(bool on) = 0;

	/**
	 * ���� xml ����ʱ���Ƿ��Զ����� xml ���룬ȱʡ����
	 * @param on {bool}
	 * @return {xml&}
	 */
	virtual xml& xml_encode(bool on) = 0;

	/**
	 * ���� xml ʱ�Ƿ������ж�����ڵ㣨�ڲ�ȱʡΪ����
	 * @param on {bool}
	 * @retrn {xml&}
	 */
	virtual xml& xml_multi_root(bool on) = 0;

	/**
	 * ����ʽ��ʽѭ�����ñ�������� XML ���ݣ�Ҳ����һ�������
	 * ������ XML ���ݣ�������ظ�ʹ�ø� XML ������������� XML
	 * ������Ӧ���ڽ�����һ�� XML ����ǰ���� reset() ��������
	 * ����һ�εĽ������
	 * @param data {const char*} xml ����
	 */
	virtual void update(const char* data) = 0;

	/**
	 * �ж� XML �����Ƿ����
	 * @param root_tag {const char*} ���ڵ��ǩ������ NULL �ַ������øñ�ǩ��
	 *  �� xml �����������ı�ǩ���Ƚ��Ƿ���ͬ
	 * @return {bool}
	 */
	virtual bool complete(const char* root_tag) = 0;

	/**
	 * ���� XML ������״̬���� XML ������������Զ�� XML ����
	 * ���н������ڷ���ʹ�ñ� XML ������ǰ����Ҫ���ñ���������
	 * �ڲ� XML ������״̬�������һ�εĽ������
	 */
	virtual void reset(void) = 0;

	/**
	 * �ӽ����� XML ԭʼ�����н���ȡ�ı�����
	 * @return {const string&} ���ؽ�������������ã�����������
	 *  ���������û�����Ҫ�ͷ�
	 */
	virtual const string& getText(void);

	/**
	 * �� XML ������ȡ��ĳ����ǩ�������нڵ㼯��
	 * @param tag {const char*} ��ǩ��(�����ִ�Сд)
	 * @return {const std::vector<xml_node*>&} ���ؽ�����Ķ������ã�
	 *  �����ѯ���Ϊ�գ���ü���Ϊ�գ�����empty() == true
	 *  ע�����ص������е� xml_node �ڵ����ݿ����޸ģ�������ɾ���ýڵ㣬
	 *  ��Ϊ�ÿ��ڲ����Զ�ɾ���Ļ���
	 */
	virtual const std::vector<xml_node*>&
		getElementsByTagName(const char* tag) const = 0;

	/**
	 * �� xml �����л�ö�Ӧ��ǩ���ĵ�һ�� xml �ڵ����
	 * @param tag {const char*} ��ǩ��(�����ִ�Сд)
	 * @return {xml_node*} ���ؿձ����ñ�ǩ��Ӧ�� xml �ڵ㲻����
	 */
	virtual xml_node* getFirstElementByTag(const char* tag) const = 0;

	/**
	 * �� xml �����л�����е�������༶��ǩ����ͬ�� xml �ڵ�ļ���
	 * @param tags {const char*} �༶��ǩ������ '/' �ָ�������ǩ��������� xml ���ݣ�
	 *  <root> <first> <second> <third name="test1"> text1 </third> </second> </first> ...
	 *  <root> <first> <second> <third name="test2"> text2 </third> </second> </first> ...
	 *  <root> <first> <second> <third name="test3"> text3 </third> </second> </first> ...
	 *  ����ͨ���༶��ǩ����root/first/second/third һ���Բ�����з��������Ľڵ�
	 * @return {const std::vector<xml_node*>&} ���������� xml �ڵ㼯��, 
	 *  �����ѯ���Ϊ�գ���ü���Ϊ�գ�����empty() == true
	 *  ע�����ص������е� xml_node �ڵ����ݿ����޸ģ�������ɾ���ýڵ㣬
	 *  ��Ϊ�ÿ��ڲ����Զ�ɾ���Ļ���
	 */
	virtual const std::vector<xml_node*>&
		getElementsByTags(const char* tags) const = 0;

	/**
	 * �� xml �����л��ָ���༶��ǩ���ĵ�һ�� xml �ڵ�
	 * @param tags {const char*} �༶��ǩ������ '/' �ָ�������ǩ��������� xml ���ݣ�
	 *  <root> <first> <second> <third name="test1"> text1 </third> </second> </first> ...
	 *  <root> <first> <second> <third name="test2"> text2 </third> </second> </first> ...
	 *  <root> <first> <second> <third name="test3"> text3 </third> </second> </first> ...
	 *  ����ͨ���༶��ǩ����root/first/second/third һ���Բ�����з��������Ľڵ�
	 * @return {xml_node*} ���ؿձ�ʾ������
	 */
	virtual xml_node* getFirstElementByTags(const char* tags) const = 0;

	/**
	 * �� xml �����л����������������� name ������ֵ��ͬ�� xml �ڵ㼯��
	 * @param value {const char*} ������Ϊ name ������ֵ
	 * @return {const std::vector<xml_node*>&} ���ؽ�����Ķ������ã�
	 *  �����ѯ���Ϊ�գ���ü���Ϊ�գ�����empty() == true
	 *  ע�����ص������е� xml_node �ڵ����ݿ����޸ģ�������ɾ���ýڵ㣬
	 *  ��Ϊ�ÿ��ڲ����Զ�ɾ���Ļ���
	 */
	virtual const std::vector<xml_node*>&
		getElementsByName(const char* value) const = 0;

	/**
	 * �� xml �����л�����и���������������ֵ�� xml �ڵ�Ԫ�ؼ���
	 * @param name {const char*} ������
	 * @param value {const char*} ����ֵ
	 * @return {const std::vector<xml_node*>&} ���ؽ�����Ķ������ã�
	 *  �����ѯ���Ϊ�գ���ü���Ϊ�գ�����empty() == true
	 */
	virtual const std::vector<xml_node*>& getElementsByAttr(
		const char* name, const char* value) const = 0;

	/**
	 * �� xml �����л��ָ�� id ֵ�� xml �ڵ�Ԫ��
	 * @param id {const char*} id ֵ
	 * @return {const xml_node*} xml �ڵ�Ԫ��, ������ NULL ���ʾû�з���
	 *  ������ xml �ڵ�, ����ֵ����Ҫ�ͷ�
	 */
	virtual xml_node* getElementById(const char* id) const = 0;

	/**
	 * ����һ�� xml_node �ڵ����
	 * @param tag {const char*} ��ǩ��
	 * @param text {const char*} �ı��ַ���
	 * @return {xml_node*} �²����� xml_node ������Ҫ�û��ֹ��ͷţ���Ϊ
	 *  �� xml �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ�����ڲ���ʱ��
	 *  �� reset ���ͷ���Щ xml_node �ڵ����
	 */
	virtual xml_node& create_node(const char* tag,
		const char* text = NULL) = 0;

	/**
	 * ����һ�� xml_node �ڵ����
	 * @param tag {const char*} ��ǩ��
	 * @param number {long long int} 64 λ����
	 * @return {xml_node*} �²����� xml_node ������Ҫ�û��ֹ��ͷţ���Ϊ
	 *  ��xml �����ͷ�ʱ��Щ�ڵ���Զ����ͷţ���Ȼ�û�Ҳ�����ڲ���ʱ����
	 *  reset ���ͷ���Щ xml_node �ڵ����
	 */
#if defined(_WIN32) || defined(_WIN64)
	xml_node& create_node(const char* tag, __int64 number);
#else
	xml_node& create_node(const char* tag, long long int number);
#endif

	/**
	 * ��ø��ڵ���󣬵���Ҫע�⣬�ýڵ�Ϊ��ڵ㣬���治����κ����ݣ�
	 * �������� xml �ڵ�������㸸����
	 * @return {xml_node&}
	 */
	virtual xml_node& get_root(void) = 0;

	/**
	 * ��ʼ������ xml ���󲢻�õ�һ���ڵ�
	 * @return {xml_node*} ���ؿձ�ʾ�� xml ����Ϊ�սڵ�
	 *  ע�����صĽڵ�����û������ֹ��ͷţ���Ϊ�ö���
	 *  �ڲ����Զ��ͷ�
	 */
	virtual xml_node* first_node(void) = 0;

	/**
	 * ������ xml �������һ�� xml �ڵ�
	 * @return {xml_node*} ���ؿձ�ʾ�������
	 *  ע�����صĽڵ�����û������ֹ��ͷţ���Ϊ�ö���
	 *  �ڲ����Զ��ͷ�
	 */
	virtual xml_node* next_node(void) = 0;

	/**
	 * �� xml ������ת���ַ���
	 * @param out {string&} �洢ת������Ļ�����
	 */
	virtual void build_xml(string& out) const { (void) out; };

	/**
	 * �� xml ����ת��Ϊ�ַ���
	 * @param len {size_t*} �� NULL ʱ������ݳ���
	 * @return {const char*} xml �ַ���
	 */
	virtual const char* to_string(size_t* len = NULL) const = 0;

	/**
	 * ��õ�ǰ xml �����Ѿ�������ڴ��С�ܺ�
	 * @return {size_t}
	 */
	virtual size_t space(void) const = 0;

	/**
	 * ����¼ xml �ѷ����ڴ��С�ı����� 0
	 */
	virtual void space_clear(void) = 0;

	/**
	 * ��õ�ǰ xml ������ xml �ڵ������
	 * @return {size_t}
	 */
	virtual size_t node_count(void) const = 0;

	/**
	 * ��õ�ǰ xml ���������� xml �ڵ����Ե�����
	 * @return {size_t}
	 */
	virtual size_t attr_count(void) const = 0;

public:
	// pipe_stream �麯������

	virtual int push_pop(const char* in, size_t len,
		string* out, size_t max = 0);
	virtual int pop_end(string* out, size_t max = 0);
	virtual void clear(void);

protected:
	dbuf_guard dbuf_;
	std::vector<xml_node*> elements_;
	string* buf_;
	//bool dummyRootAdded_;

	ACL_TOKEN* m_pTokenTree;
	//std::list<xml_node*> nodes_tmp_;
};

} // namespace acl
