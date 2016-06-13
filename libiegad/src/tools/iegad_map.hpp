#ifndef __IEGAD_MAP__
#define __IEGAD_MAP__


// ============ 说明 ============
//
// @创建日期 : 2015-11-07
// @创建人 : iegad
//
// ============================
// @用途 :  一个线程安全的 散列表
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================


#include <unordered_map>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <assert.h>


namespace iegad {
namespace tools {

template <typename __TKey, typename __TValue>
class safe_map {
// 线程安全 散列表
public:
    // ============================
    // @用途 : 内置 散列表 类型声明
    // ============================
    typedef std::unordered_map<__TKey, __TValue> map_t;


    // ============================
    // @用途 : 以boost::shared_mutex 为基础的 读写互斥量 类型声明
    // ============================
    typedef boost::shared_mutex rw_mtx_t;


    // ============================
    // @用途 : 读锁 类型声明
    // ============================
    typedef boost::shared_lock<rw_mtx_t> read_lock_t;


    // ============================
    // @用途 : 写锁 类型声明
    // ============================
    typedef boost::unique_lock<rw_mtx_t> write_lock_t;


    // ============================
    // @用途 : Key 类型声明
    // ============================
    typedef typename map_t::key_type key_type;


    // ============================
    // @用途 : 第二个模板参数
    // ============================
    typedef typename map_t::mapped_type mapped_type;


    // ============================
    // @用途 : Value 类型声明
    // ============================
    typedef typename map_t::value_type value_type;


    // ============================
    // @用途 : std::allocator_traits<Allocator>::pointer
    // ============================
    typedef typename map_t::pointer pointer;


    // ============================
    // @用途 : std::allocator_traits<Allocator>::const_pointer
    // ============================
    typedef typename map_t::const_pointer const_pointer;


    // ============================
    // @用途 : 无符号整数类型
    // ============================
    typedef typename map_t::size_type size_type;


    // ============================
    // @用途 : 有符号整数类型
    // ============================
    typedef typename map_t::difference_type difference_type;


    // ============================
    // @用途 : value_type&
    // ============================
    typedef typename  map_t::reference reference;


    // ============================
    // @用途 : const value_type&
    // ============================
    typedef typename map_t::const_reference const_reference;


    // ============================
    // @用途 : 正向迭代器
    // ============================
    typedef typename map_t::iterator iterator;


    // ============================
    // @用途 : 常正向迭代器 类型声明
    // ============================
    typedef typename map_t::const_iterator const_iterator;


    // ============================
    // @用途 : 判断容器是否为空
    // @返回值 : 容器为空, 返回 true, 否则, 返回 false;
    // ============================
    bool empty() const {
        read_lock_t lock(mtx_);
        return map_.empty();
    }


    // ============================
    // @用途 : 获取容器内元素个数
    // @返回值 : 容器内元素个数
    // ============================
    size_type size() const {
        read_lock_t lock(mtx_);
        return map_.size();
    }


    // ============================
    // @用途 : 获取容器内可容纳元素的最大值
    // @返回值 : 容器内可容纳元素的最大值
    // ============================
    size_type max_size() const {
        read_lock_t lock(mtx_);
        return map_.max_size();
    }


    // ============================
    // @用途 : 获取第一个正向迭代器
    // @返回值 : 第一个正向迭代器
    // ============================
    iterator begin()    {
        read_lock_t lock(mtx_);
        return map_.begin();
    }


    // ============================
    // @用途 : 获取第一个常正向迭代器
    // @返回值 : 第一个常正向迭代器
    // ============================
    const_iterator begin() const {
        read_lock_t lock(mtx_);
        return map_.begin();
    }


    // ============================
    // @用途 : 获取起尾的最后一个正向迭代器
    // @返回值 : 起尾的最后一个正向迭代器
    // ============================
    iterator end() {
        read_lock_t lock(mtx_);
        return map_.end();
    }


    // ============================
    // @用途 : 获取起尾的最后一个常正向迭代器
    // @返回值 : 起尾的最后一个常正向迭代器
    // ============================
    const_iterator end() const {
        read_lock_t lock(mtx_);
        return map_.end();
    }


    // ============================
    // @用途 : 插入元素
    // @k : 键
    // @v : 值
    // @返回值 : 成功 返回 true, 否则 返回 false
    // ============================
    bool insert(const key_type & k, const mapped_type & v) {
        write_lock_t lock(mtx_);
        return map_.insert(value_type(k, v)).second;
    }


    // ============================
    // @用途 : 查找 键为 k 的元素是否存在
    // @k : 键
    // @返回值 : 存在 返回 true, 否则 返回 false
    // ============================
    iterator find(const key_type & k) {
        read_lock_t lock(mtx_);
        return map_.find(k);
    }


    // ============================
    // @用途 : 删除 键为 k 的元素
    // @k : 键
    // @返回值 : 返回删除后, 容器内的元素个数
    // ============================
    size_type erase(const key_type & k) {
        write_lock_t lock(mtx_);
        return map_.erase(k);
    }


    // ============================
    // @用途 : 清空容器
    // @返回值 : void
    // ============================
    void clear() {
        write_lock_t lock(mtx_);
        map_.clear();
    }


    // ============================
    // @用途 : 返回 键 k 所对应的 value
    // @k : 键
    // @返回值 : k所对应的value.
    // ============================
    const mapped_type & operator[](const key_type & k) {
        read_lock_t lock(mtx_);
        assert(map_.find(k) != map_.end());
        return map_[k];
    }


    // ============================
    // @用途 : 设置 键k 的 值为v
    // @k : 键
    // @v : 值
    // @返回值 : void
    // ============================
    void set(const key_type & k, const mapped_type & v) {
        write_lock_t lock(mtx_);
        map_[k] = v;
    }


    // ============================
    // @用途 : 迭代容器内的所有元素
    // @func : 函数对象
    // @返回值 : void
    // ============================
    template<typename Func>
    void for_each(Func func) {
        read_lock_t lock(mtx_);
        std::for_each(map_.begin(), map_.end(), func);
    }


private:
    // 内置散列表
    map_t map_;
    // 读写锁
    mutable rw_mtx_t mtx_;
}; // class safe_map <K, V>;


} // namespace tools;
} // namespace iegad;



#endif // __IEGAD_MAP__
