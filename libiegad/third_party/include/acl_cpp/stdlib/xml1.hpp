#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include <vector>
#include <list>
#include "acl_cpp/stdlib/string.hpp"
#include "acl_cpp/stdlib/pipe_stream.hpp"
#include "acl_cpp/stdlib/xml.hpp"

struct ACL_XML;
struct ACL_XML_NODE;
struct ACL_XML_ATTR;

/**
 * �� ACL ���� XML ������ķ�װ������ C++ �û�ʹ�ã������̫ע���������أ�
 * ����ֱ��ʹ�ø��࣬����ڷ����ִ���ҷǳ�ע�����ܣ�����ֱ��ʹ�� ACL ���
 * XML ����������Ϊ����Ҳ�ǵ����� ACL ���е� XML �������̣������ж��ο���
 * ���̣����ܻ���΢Ӱ��һЩ���ܣ�������һ���Ӧ�����Ӱ����΢�������
 */

namespace acl {

class xml1;
class xml1_node;

class ACL_CPP_API xml1_attr : public xml_attr
{
public:
	/**
	 * @override
	 */
	const char* get_name(void) const;

	/**
	 * @override
	 */
	const char* get_value(void) const;

protected:
	friend class xml1_node;

	xml1_attr(xml_node* node, ACL_XML_ATTR* attr);
	~xml1_attr(void) {}

private:
	ACL_XML_ATTR* attr_;
};

class ACL_CPP_API xml1_node : public xml_node
{
public:
	/**
	 * @override
	 */
	const char* tag_name(void) const;

	/**
	 * @override
	 */
	const char* id(void) const;

	/**
	 * @override
	 */
	const char* text(void) const;

	/**
	 * @override
	 */
	const char* attr_value(const char* name) const;

	/**
	 * @override
	 */
	const xml_attr* first_attr(void) const;

	/**
	 * @override
	 */
	const xml_attr* next_attr(void) const;

	/**
	 * @override
	 */
	xml_node& add_attr(const char* name, const char* value);

	/**
	 * @override
	 */
	xml_node& set_text(const char* str);

	/**
	 * @override
	 */
	xml_node& add_child(xml_node* child, bool return_child = false);

	/**
	 * @override
	 */
	int detach(void);

	/**
	 * @override
	 */
	xml_node& set_parent(xml_node* node);

	/**
	 * @override
	 */
	xml_node& get_parent(void) const;

	/**
	 * @override
	 */
	xml_node* first_child(void);

	/**
	 * @override
	 */
	xml_node* next_child(void);

	/**
	 * @override
	 */
	int depth(void) const;

	/**
	 * @override
	 */
	bool is_root(void) const;

	/**
	 * @override
	 */
	int children_count(void) const;

public:
	/**
	 * ȡ����Ӧ�� ACL ���е� XML �ڵ����
	 * @return {ACL_XML_NODE*} ���ؽڵ����ע���ýڵ��û����ܵ����ͷ�
	 */
	ACL_XML_NODE* get_xml_node(void) const;

protected:
	friend class xml1;
	friend class dbuf_guard;

	/**
	 * xml �ڵ㹹�캯��
	 * @param xml_ptr {xml*} xml �����󣬷ǿ�
	 * @param node {ACL_XML_NODE*} C ���е� xml �ڵ�ָ��
	 */
	xml1_node(xml* xml_ptr, ACL_XML_NODE* node);

	/**
	 * Ҫ��ö�������Ƕ�̬������
	 */
	~xml1_node(void);

private:
	ACL_XML_NODE *node_;
	ACL_ITER* child_iter_;
	ACL_ITER* attr_iter_;

	xml_node* parent_;
	xml1_node* parent_internal_;
};

class string;

class ACL_CPP_API xml1 : public xml
{
public:
	/**
	 * ���캯��
	 * @param data {const char*} xml ��ʽ���ַ�����������������
	 *  xml �ַ�����Ҳ�����ǲ��ֵ� xml �ַ�����Ҳ�����ǿ�ָ�룬
	 *  ������Σ��û���Ȼ�����ò��ֻ������� xml �ַ������� update
	 *  �������ڵ��� update �����н��� xml����ʵ�������캯����
	 *  �� data �����ǿ�ʱ����Ҳ����� update
	 */
	xml1(const char* data = NULL);
	~xml1(void);

	/**
	 * @override
	 */
	xml& ignore_slash(bool on);

	/**
	 * @override
	 */
	xml& xml_decode(bool on);

	/**
	 * @override
	 */
	xml& xml_encode(bool on);

	/**
	 * @override
	 */
	xml& xml_multi_root(bool on);

	/**
	 * @override
	 */
	void update(const char* data);

	/**
	 * @override
	 */
	bool complete(const char* root_tag);

	/**
	 * @override
	 */
	void reset(void);

	/**
	 * @override
	 */
	const string& getText(void);

	/**
	 * @override
	 */
	const std::vector<xml_node*>&
		getElementsByTagName(const char* tag) const;

	/**
	 * @override
	 */
	xml_node* getFirstElementByTag(const char* tag) const;

	/**
	 * @override
	 */
	const std::vector<xml_node*>& getElementsByTags(const char* tags) const;

	/**
	 * @override
	 */
	xml_node* getFirstElementByTags(const char* tags) const;

	/**
	 * @override
	 */
	const std::vector<xml_node*>&
		getElementsByName(const char* value) const;

	/**
	 * @override
	 */
	const std::vector<xml_node*>& getElementsByAttr(
		const char* name, const char* value) const;

	/**
	 * @override
	 */
	xml_node* getElementById(const char* id) const;

	/**
	 * @override
	 */
	xml_node& create_node(const char* tag, const char* text = NULL);

	/**
	 * @override
	 */
	xml_node& get_root(void);

	/**
	 * @override
	 */
	xml_node* first_node(void);

	/**
	 * @override
	 */
	xml_node* next_node(void);

	/**
	 * @override
	 */
	void build_xml(string& out) const;

	/**
	 * @override
	 */
	const char* to_string(size_t* len = NULL) const;

	/**
	 * @override
	 */
	size_t space(void) const;

	/**
	 * @override
	 */
	void space_clear(void);

	/**
	 * @override
	 */
	size_t node_count(void) const;

	/**
	 * @override
	 */
	size_t attr_count(void) const;

public:
	/**
	 * ȡ�� acl ���е� ACL_XML ����
	 * @return {ACL_XML*} ��ֵ������Ϊ�գ�ע���û������޸ĸö����ֵ��
	 *  ���������ͷŸö���
	 */
	ACL_XML* get_xml(void) const
	{
		return xml_;
	}

private:
	ACL_XML *xml_;
	ACL_ITER* iter_;
	xml1_node* root_;
};

} // namespace acl
