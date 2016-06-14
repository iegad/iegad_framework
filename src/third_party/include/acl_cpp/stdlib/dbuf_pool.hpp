#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include "acl_cpp/stdlib/noncopyable.hpp"
#include <vector>

struct ACL_DBUF_POOL;

namespace acl
{

/**
 * �Ự����ڴ��������࣬������ṩ�ڴ���亯�������������������ʱ���ڴ���
 * �ᱻһ���Ե��ͷţ������ʺ�����ҪƵ������һЩ��С���ȵ�С�ڴ��Ӧ�ã�
 * ����ʵ�����Ƿ�װ�� lib_acl �е� ACL_DBUF_POOL �ṹ������
 */

class ACL_CPP_API dbuf_pool
{
public:
	/**
	 * ���������붯̬����
	 */
	dbuf_pool();

	/**
	 * ����������Ҫ��̬��������������������������ʹ������Ҫ���� destroy
	 * ���������ٶ�̬����
	 */
	void destroy();

	/**
	 * ���� new/delete ��������ʹ dbuf_pool ������Ҳ�������ڴ���ϣ�
	 * �Ӷ������� malloc/free �Ĵ���
	 * @param size {size_t} �ɱ��봫��� dbuf_pool ����ĳ��ȴ�С
	 * @param nblock {size_t} �ڲ����õ��ڴ�飨4096���ı���
	 */
	void *operator new(size_t size, size_t nblock = 2);
#if defined(_WIN32) || defined(_WIN64)
	void operator delete(void* ptr, size_t);
#else
	void operator delete(void* ptr);
#endif

	/**
	 * �����ڴ�ص�״̬�Ա����ظ�ʹ�ø��ڴ�ض���
	 * @param reserve {size_t} ����ֵ > 0������Ҫָ�����Ᵽ�����ڴ��С��
	 *  �ô�С����С�ڵ����Ѿ��ڸ��ڴ�ض������Ĵ�С
	 * @return {bool} �����������Ƿ����򷵻� false
	 */
	bool dbuf_reset(size_t reserve = 0);

	/**
	 * ����ָ�����ȵ��ڴ�
	 * @param len {size_t} ��Ҫ������ڴ泤�ȣ����ڴ�Ƚ�Сʱ(С�ڹ��캯��
	 *  �е� block_size)ʱ����������ڴ����� dbuf_pool ��������ڴ����ϣ�
	 *  ���ڴ�ϴ�ʱ��ֱ��ʹ�� malloc ���з���
	 * @return {void*} �·�����ڴ��ַ
	 */
	void* dbuf_alloc(size_t len);

	/**
	 * ����ָ�����ȵ��ڴ沢���ڴ���������
	 * @param len {size_t} ��Ҫ������ڴ泤��
	 * @return {void*} �·�����ڴ��ַ
	 */
	void* dbuf_calloc(size_t len);

	/**
	 * ����������ַ�����̬�����µ��ڴ沢���ַ������и��ƣ������� strdup
	 * @param s {const char*} Դ�ַ���
	 * @return {char*} �¸��Ƶ��ַ�����ַ
	 */
	char* dbuf_strdup(const char* s);

	/**
	 * ����������ַ�����̬�����µ��ڴ沢���ַ������и��ƣ������� strdup
	 * @param s {const char*} Դ�ַ���
	 * @param len {size_t} �����������ַ�������󳤶�
	 * @return {char*} �¸��Ƶ��ַ�����ַ
	 */
	char* dbuf_strndup(const char* s, size_t len);

	/**
	 * ����������ڴ����ݶ�̬�����ڴ沢�����ݽ��и���
	 * @param addr {const void*} Դ�����ڴ��ַ
	 * @param len {size_t} Դ���ݳ���
	 * @return {void*} �¸��Ƶ����ݵ�ַ
	 */
	void* dbuf_memdup(const void* addr, size_t len);

	/**
	 * �黹���ڴ�ط�����ڴ�
	 * @param addr {const void*} ���ڴ�ط�����ڴ��ַ
	 * @return {bool} ������ڴ��ַ���ڴ�ط�����ͷŶ�Σ��򷵻� false
	 */
	bool dbuf_free(const void* addr);

	/**
	 * �������ڴ�ط����ĳ�ε�ַ�����⵱���� dbuf_reset ʱ����ǰ�ͷŵ�
	 * @param addr {const void*} ���ڴ�ط�����ڴ��ַ
	 * @return {bool} ������ڴ��ַ���ڴ�ط��䣬�򷵻� false
	 */
	bool dbuf_keep(const void* addr);

	/**
	 * ȡ���������ڴ�ط����ĳ�ε�ַ���Ա��ڵ��� dbuf_reset ʱ���ͷŵ�
	 * @param addr {const void*} ���ڴ�ط�����ڴ��ַ
	 * @return {bool} ������ڴ��ַ���ڴ�ط��䣬�򷵻� false
	 */
	bool dbuf_unkeep(const void* addr);

	/**
	 * ����ڲ� ACL_DBUF_POOL �����Ա��ڲ��� C �ӿڵ��ڴ�ض���
	 * @return {ACL_DBUF_POOL*}
	 */
	ACL_DBUF_POOL *get_dbuf()
	{
		return pool_;
	}

private:
	ACL_DBUF_POOL* pool_;
	size_t mysize_;

public:
	~dbuf_pool();
};

/**
 * sample:
 *  void test()
 *  {
 *      acl::dbuf_pool* dbuf = new acl::dbuf_pool;
 *      for (int i = 0; i < 1000; i++)
 *      {
 *          char* ptr = dbuf->dbuf_strdup("hello world!");
 *          printf("%s\r\n", p);
 *      }
 *      dbuf->destroy();
 *
 *      // ���� dbuf ����ʱ��ָ�����ڲ������ڴ���λ��
 *      dbuf = new(8) acl::dbuf_pool;
 *      for (int i = 0; i < 1000; i++)
 *      {
 *          ptr = dbuf->dbuf_strdup("hello world!");
 *          printf("%s\r\n", p);
 *      }
 *
 *	// ���� dbuf ����
 *      dbuf->destroy();
 *  }
 *
 */
//////////////////////////////////////////////////////////////////////////////

class dbuf_guard;

/**
 * �ڻỰ�ڴ�ض����Ϸ���Ķ��������
 */
class ACL_CPP_API dbuf_obj
{
public:
	/**
	 * ���캯��
	 * @param guard {dbuf_guard*} �ò����ǿ�ʱ��������������ᱻ
	 *  dbuf_guard ������Զ�����ͳһ���٣�����ò���Ϊ�գ���Ӧ��Ӧ
	 *  ���� dbuf_guard::push_back �����������������ͳһ����
	 */
	dbuf_obj(dbuf_guard* guard = NULL);

	virtual ~dbuf_obj() {}

	/**
	 * ��øö����� dbuf_guard �е������е��±�λ��
	 * @return {int} ���ظö����� dbuf_guard �е������е��±�λ�ã�����
	 *  ����û�б� dbuf_guard ����ʱ���򷵻� -1����ʱ�п��ܻ�����ڴ�й¶
	 */
	int pos() const
	{
		return pos_;
	}

	/**
	 * ���ع��캯���� dbuf_guard ����
	 * @return {dbuf_guard*}
	 */
	dbuf_guard* get_guard() const
	{
		return guard_;
	}

private:
	friend class dbuf_guard;

	// ��¼������������ dbuf_guard ����
	dbuf_guard* guard_;

	// �ñ������� dbuf_guard ����ʹ�ã������Ӱ�ȫ��
	int nrefer_;

	// �ö����� dbuf_guard �����м�¼�������λ��
	int pos_;
};

/**
 * �Ự�ڴ�ع��������ɸ��������� dbuf_pool ���������Ϸ���Ķ��󣬵�����
 * ��������ʱ��dbuf_pool ��������������ͷš�
 */
class ACL_CPP_API dbuf_guard : public noncopyable
{
public:
	/**
	 * ���캯��
	 * @param dbuf {dbuf_pool*} �����ڴ�ض���ǿ�ʱ��dbuf ���� dbuf_guard
	 *  �ӹܣ����Ϊ�գ��򱾹��캯���ڲ������Զ�����һ�� dbuf_pool ����
	 * @param capacity {size_t} �ڲ������� objs_ ����ĳ�ʼ����
	 */
	dbuf_guard(dbuf_pool* dbuf, size_t capacity = 100);

	/**
	 * ���캯��
	 * @param nblock {size_t} ��������ڲ����� dbuf_pool ����ʱ��������
	 *  ָ�����ڴ��(4096)�ı���
	 * @param capacity {size_t} �ڲ������� objs_ ����ĳ�ʼ����
	 */
	dbuf_guard(size_t nblock = 2, size_t capacity = 100);

	/**
	 * ���������������������ڲ������Զ������ɹ��캯������� dbuf_pool ����
	 */
	~dbuf_guard();

	/**
	 * ���� dbuf_pool::dbuf_reset
	 * @param reserve {size_t}
	 * @return {bool}
	 */
	bool dbuf_reset(size_t reserve = 0);

	/**
	 * ���� dbuf_pool::dbuf_alloc
	 * @param len {size_t}
	 * @return {void*}
	 */
	void* dbuf_alloc(size_t len)
	{
		return dbuf_->dbuf_alloc(len);
	}

	/**
	 * ���� dbuf_pool::dbuf_calloc
	 * @param len {size_t}
	 * @return {void*}
	 */
	void* dbuf_calloc(size_t len)
	{
		return dbuf_->dbuf_calloc(len);
	}

	/**
	 * ���� dbuf_pool::dbuf_strdup
	 * @param s {const char*}
	 * @return {char*}
	 */
	char* dbuf_strdup(const char* s)
	{
		return dbuf_->dbuf_strdup(s);
	}

	/**
	 * ���� dbuf_pool::dbuf_strndup
	 * @param s {const char*}
	 * @param len {size_t}
	 * @return {char*}
	 */
	char* dbuf_strndup(const char* s, size_t len)
	{
		return dbuf_->dbuf_strndup(s, len);
	}

	/**
	 * ���� dbuf_pool::dbuf_memdup
	 * @param addr {const void*}
	 * @param len {size_t}
	 * @return {void*}
	 */
	void* dbuf_memdup(const void* addr, size_t len)
	{
		return dbuf_->dbuf_memdup(addr, len);
	}

	/**
	 * ���� dbuf_pool::dbuf_free
	 * @param addr {const void*}
	 * @return {bool}
	 */
	bool dbuf_free(const void* addr)
	{
		return dbuf_->dbuf_free(addr);
	}

	/**
	 * ���� dbuf_pool::dbuf_keep
	 * @param addr {const void*}
	 * @return {bool}
	 */
	bool dbuf_keep(const void* addr)
	{
		return dbuf_->dbuf_keep(addr);
	}

	/**
	 * ���� dbuf_pool::dbuf_unkeep
	 * @param addr {const void*}
	 * @return {bool}
	 */
	bool dbuf_unkeep(const void* addr)
	{
		return dbuf_->dbuf_unkeep(addr);
	}

	/**
	 * ��� dbuf_pool ����
	 * @return {acl::dbuf_pool&}
	 */
	acl::dbuf_pool& get_dbuf() const
	{
		return *dbuf_;
	}

	/**
	 * �����ֶ����ñ����������� dbuf_pool �Ϸ���� dbuf_obj ������󽻸�
	 * dbuf_guard ����ͳһ�������ٹ����Ͻ���ͬһ�� dbuf_obj �������ͬ
	 * ʱ������� dbuf_guard ������й������򽫻����������ظ��ͷ�
	 * @param obj {dbuf_obj*}
	 * @return {int} ���� obj ����Ӻ����� dbuf_obj ���������е��±�λ�ã�
	 *  dbuf_guard �ڲ��� dbuf_obj ����Ĺ�����з�����ӻ��ƣ����Ե����
	 *  ��ͬһ�� dbuf_obj ��������ͬһ�� dbuf_guard ����ʱ���ڲ�ֻ���һ��
	 */
	int push_back(dbuf_obj* obj);

	/**
	 * ��õ�ǰ�ڴ���й���Ķ�������
	 * @return {size_t}
	 */
	size_t size() const
	{
		return size_;
	}

	/**
	 * ��õ�ǰ�ڴ���й���Ķ��󼯺ϣ��ú��������� size() �������ʹ�ã�
	 * ����ָ���ַԽ��
	 * @return {dbuf_obj**} ���� dbuf_obj ���󼯺ϣ���Զ���ط� NULL ֵ��
	 *  ������Ĵ�С�� size() ��������
	 */
	dbuf_obj** get_objs() const
	{
		return objs_;
	}

	/**
	 * ����ָ���±�Ķ���
	 * @param pos {size_t} ָ��������±�λ�ã���ӦԽ��
	 * @return {dbuf_obj*} ���±�λ��Խ��ʱ���� NULL
	 */
	dbuf_obj* operator[](size_t pos) const;

	/**
	 * ����ָ���±�Ķ���
	 * @param pos {size_t} ָ��������±�λ�ã���ӦԽ��
	 * @return {dbuf_obj*} ���±�λ��Խ��ʱ���� NULL
	 */
	dbuf_obj* get(size_t pos) const;

	/**
	 * �����ڽ� objs_ �������ÿ��������ռ�ʱ���������ڲ�ȱʡֵΪ 100
	 * @param incr {size_t}
	 */
	void set_increment(size_t incr);

public:
	template <typename T>
	T* create()
	{
		T* t = new (dbuf_alloc(sizeof(T))) T();
		(void) push_back(t);
		return t;
	}

	template <typename T, typename P1>
	T* create(P1 p)
	{
		T* t = new (dbuf_alloc(sizeof(T))) T(p);
		(void) push_back(t);
		return t;
	}

	template <typename T, typename P1, typename P2>
	T* create(P1 p1, P2 p2)
	{
		T* t = new (dbuf_alloc(sizeof(T))) T(p1, p2);
		(void) push_back(t);
		return t;
	}

	template <typename T, typename P1, typename P2, typename P3>
	T* create(P1 p1, P2 p2, P3 p3)
	{
		T* t = new (dbuf_alloc(sizeof(T))) T(p1, p2, p3);
		(void) push_back(t);
		return t;
	}

	template <typename T, typename P1, typename P2, typename P3,
		 typename P4>
	T* create(P1 p1, P2 p2, P3 p3, P4 p4)
	{
		T* t = new (dbuf_alloc(sizeof(T))) T(p1, p2, p3, p4);
		(void) push_back(t);
		return t;
	}

	template <typename T, typename P1, typename P2, typename P3,
		 typename P4, typename P5>
	T* create(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		T* t = new (dbuf_alloc(sizeof(T))) T(p1, p2, p3, p4, p5);
		(void) push_back(t);
		return t;
	}

	template <typename T, typename P1, typename P2, typename P3,
		 typename P4, typename P5, typename P6>
	T* create(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
	{
		T* t = new (dbuf_alloc(sizeof(T))) T(p1, p2, p3, p4, p5, p6);
		(void) push_back(t);
		return t;
	}

	template <typename T, typename P1, typename P2, typename P3,
		 typename P4, typename P5, typename P6, typename P7>
	T* create(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
	{
		T* t = new (dbuf_alloc(sizeof(T)))
			T(p1, p2, p3, p4, p5, p6, p7);
		(void) push_back(t);
		return t;
	}

	template <typename T, typename P1, typename P2, typename P3,
		 typename P4, typename P5, typename P6, typename P7,
		 typename P8>
	T* create(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
	{
		T* t = new (dbuf_alloc(sizeof(T)))
			T(p1, p2, p3, p4, p5, p6, p7, p8);
		(void) push_back(t);
		return t;
	}

	template <typename T, typename P1, typename P2, typename P3,
		 typename P4, typename P5, typename P6, typename P7,
		 typename P8,typename P9>
	T* create(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7,
		P8 p8, P9 p9)
	{
		T* t = new (dbuf_alloc(sizeof(T)))
			T(p1, p2, p3, p4, p5, p6, p7, p8, p9);
		(void) push_back(t);
		return t;
	}

	template <typename T, typename P1, typename P2, typename P3,
		 typename P4, typename P5, typename P6, typename P7,
		 typename P8, typename P9, typename P10>
	T* create(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7,
		P8 p8, P9 p9, P10 p10)
	{
		T* t = new (dbuf_alloc(sizeof(T)))
			T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		(void) push_back(t);
		return t;
	}

private:
	size_t nblock_;			// �ڲ��Խ� dbuf_pool �ڴ��ĵ�λ����
	dbuf_pool* dbuf_;		// �ڴ�ض���

	// �˴�֮����ʹ����ʵ�ֵ� dbuf_obj ������󣬶�û��ʹ�� std::vector��
	// һ����ʹ�������Ҳ�� dbuf_pool �ڴ���ϴ�������һ������Ա���
	// std::vector �ڲ�������ʱ���ڴ治�ɿ���

	dbuf_obj** objs_;		// �洢 dbuf_obj ������������
	size_t size_;			// �洢�� objs_ �еĶ������
	size_t capacity_;		// objs_ ����Ĵ�С
	size_t incr_;			// objs_ ��������ʱ����������

	// ���� objs_ �������Ŀռ�
	void extend_objs();
};

/**
 * sample1:
 * // �̳� acl::dbuf_obj ������
 * class myobj1 : public acl::dbuf_obj
 * {
 * public:
 * 	// �� guard ���󴫵ݸ�������󣬻��ཫ��������� guard �Ķ��󼯺���
 * 	myobj1(acl::dbuf_guard* guard) : dbuf_obj(guard) {}
 *
 * 	void doit()
 * 	{
 * 		printf("hello world!\r\n");
 * 	}
 *
 * private:
 * 	~myobj1() {}
 * };
 *
 * void test()
 * {
 * 	acl::dbuf_guard dbuf;
 *
 *	// �� dbuf_guard �����ϴ�����̬ 100 �� myobj ����
 * 	for (int i = 0; i < 100; i++)
 * 	{
 * 		// �� guard �����ϴ�����̬ myobj �����ҽ� guard ��Ϊ�������
 * 		myobj* obj = new (dbuf.dbuf_alloc(sizeof(myobj))) myobj(&dbuf);
 * 		obj->doit();
 * 	}
 *
 *	// �� dbuf ����ʱ���������洴���Ķ�̬�����Զ�����
 * }
 *
 * // sample2
 * class myobj2 : public acl::dbuf_obj
 * {
 * public:
 * 	myobj2() {}
 *
 * 	void doit()
 * 	{
 * 		printf("hello world\r\n");
 * 	}
 *
 * private:
 * 	~myobj2() {}
 * };
 *
 * void test2()
 * {
 * 	acl::dbuf_guard dbuf;
 *
 * 	for (int i = 0; i < 100; i++)
 * 	{
 * 		myobj2* obj = dbuf.create<myobj2>();
 * 		obj->doit();
 * 	}
 * }
 *
 * // sample3
 * class myobj2 : public acl::dbuf_obj
 * {
 * public:
 * 	myobj2(int i) : i_(i) {}
 *
 * 	void doit()
 * 	{
 * 		printf("hello world, i: %d\r\n", i_);
 * 	}
 *
 * private:
 * 	~myobj2() {}
 *
 * private:
 *	int i_;
 * };
 *
 * void test2()
 * {
 * 	acl::dbuf_guard dbuf;
 *
 * 	for (int i = 0; i < 100; i++)
 * 	{
 * 		myobj2* obj = dbuf.create<myobj2>(i);
 * 		obj->doit();
 * 	}
 * }
 */
} // namespace acl
