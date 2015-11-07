#ifndef __IEGAD_MAP__
#define __IEGAD_MAP__


// ============ ˵�� ============
//
// @�������� : 2015-11-07
// @������ : iegad
//
// ============================
// @��; :  һ���̰߳�ȫ�� ɢ�б�
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================


#include <unordered_map>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <assert.h>


namespace iegad {
namespace tools {

    template <typename __TKey, typename __TValue>
    class safe_map {
    public:
	// ============================
	// @��; : ���� ɢ�б� ��������
	// ============================
	typedef std::unordered_map<__TKey, __TValue> map_t;


	// ============================
	// @��; : ��boost::shared_mutex Ϊ������ ��д������ ��������
	// ============================
	typedef boost::shared_mutex rw_mtx_t;


	// ============================
	// @��; : ���� ��������
	// ============================
	typedef boost::shared_lock<rw_mtx_t> read_lock_t;


	// ============================
	// @��; : д�� ��������
	// ============================
	typedef boost::unique_lock<rw_mtx_t> write_lock_t;


	// ============================
	// @��; : Key ��������
	// ============================
	typedef typename map_t::key_type key_type;


	// ============================
	// @��; : �ڶ���ģ����� 
	// ============================
	typedef typename map_t::mapped_type mapped_type;


	// ============================
	// @��; : Value ��������
	// ============================
	typedef typename map_t::value_type value_type;


	// ============================
	// @��; : std::allocator_traits<Allocator>::pointer
	// ============================
	typedef typename map_t::pointer pointer;


	// ============================
	// @��; : std::allocator_traits<Allocator>::const_pointer
	// ============================
	typedef typename map_t::const_pointer const_pointer;


	// ============================
	// @��; : �޷�����������
	// ============================
	typedef typename map_t::size_type size_type;


	// ============================
	// @��; : �з�����������
	// ============================
	typedef typename map_t::difference_type difference_type;


	// ============================
	// @��; : value_type&
	// ============================
	typedef typename  map_t::reference reference;


	// ============================
	// @��; : const value_type&
	// ============================
	typedef typename map_t::const_reference const_reference;


	// ============================
	// @��; : ��������� 
	// ============================
	typedef typename map_t::iterator iterator;


	// ============================
	// @��; : ����������� ��������
	// ============================
	typedef typename map_t::const_iterator const_iterator;


	// ============================
	// @��; : �ж������Ƿ�Ϊ��
	// @����ֵ : ����Ϊ��, ���� true, ����, ���� false;
	// ============================
	bool empty() const {
	    read_lock_t lock(mtx_);
	    return map_.empty();
	}


	// ============================
	// @��; : ��ȡ������Ԫ�ظ���
	// @����ֵ : ������Ԫ�ظ���
	// ============================
	size_type size() const {
	    read_lock_t lock(mtx_);
	    return map_.size();
	}


	// ============================
	// @��; : ��ȡ�����ڿ�����Ԫ�ص����ֵ
	// @����ֵ : �����ڿ�����Ԫ�ص����ֵ
	// ============================
	size_type max_size() const {
	    read_lock_t lock(mtx_);
	    return map_.max_size();
	}


	// ============================
	// @��; : ��ȡ��һ�����������
	// @����ֵ : ��һ�����������
	// ============================
	iterator begin()    {
	    read_lock_t lock(mtx_);
	    return map_.begin();
	}


	// ============================
	// @��; : ��ȡ��һ�������������
	// @����ֵ : ��һ�������������
	// ============================
	const_iterator begin() const {
	    read_lock_t lock(mtx_);
	    return map_.begin();
	}


	// ============================
	// @��; : ��ȡ��β�����һ�����������
	// @����ֵ : ��β�����һ�����������
	// ============================
	iterator end() {
	    read_lock_t lock(mtx_);
	    return map_.end();
	}


	// ============================
	// @��; : ��ȡ��β�����һ�������������
	// @����ֵ : ��β�����һ�������������
	// ============================
	const_iterator end() const {
	    read_lock_t lock(mtx_);
	    return map_.end();
	}


	// ============================
	// @��; : ����Ԫ��
	// @k : ��
	// @v : ֵ
	// @����ֵ : �ɹ� ���� true, ���� ���� false
	// ============================
	bool insert(const key_type & k, const mapped_type & v) {
	    write_lock_t lock(mtx_);
	    return map_.insert(value_type(k, v)).second;
	}


	// ============================
	// @��; : ���� ��Ϊ k ��Ԫ���Ƿ����
	// @k : ��
	// @����ֵ : ���� ���� true, ���� ���� false
	// ============================
	bool find(const key_type & k) {
	    read_lock_t lock(mtx_);
	    return map_.find(k) != map_.end();
	}


	// ============================
	// @��; : ɾ�� ��Ϊ k ��Ԫ��
	// @k : ��
	// @����ֵ : ����ɾ����, �����ڵ�Ԫ�ظ���
	// ============================
	size_type erase(const key_type & k) {
	    write_lock_t lock(mtx_);
	    return map_.erase(k);
	}


	// ============================
	// @��; : �������
	// @����ֵ : void
	// ============================
	void clear() {
	    write_lock_t lock(mtx_);
	    map_.clear();
	}


	// ============================
	// @��; : ���� �� k ����Ӧ�� value
	// @k : ��
	// @����ֵ : k����Ӧ��value.
	// ============================
	const mapped_type & operator[](const key_type & k) {
	    read_lock_t lock(mtx_);
	    assert(map_.find(k) != map_.end());
	    return map_[k];
	}


	// ============================
	// @��; : ���� ��k �� ֵΪv
	// @k : ��
	// @v : ֵ
	// @����ֵ : void
	// ============================
	void set(const key_type & k, const mapped_type & v) {
	    write_lock_t lock(mtx_);
	    map_[k] = v;
	}


	// ============================
	// @��; : ���������ڵ�����Ԫ��
	// @func : ��������
	// @����ֵ : void
	// ============================
	template<typename Func>
	void for_each(Func func) {
	    read_lock_t lock(mtx_);
	    std::for_each(map_.begin(), map_.end(), func);
	}


    private:
	// ����ɢ�б�
	map_t map_;
	// ��д��
	mutable rw_mtx_t mtx_;
    }; // class safe_map <K, V>;


} // namespace tools;
} // namespace iegad;



#endif // __IEGAD_MAP__