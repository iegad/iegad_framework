#ifndef __IEGAD_SAFE_MAP__
#define __IEGAD_SAFE_MAP__



#include <map>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <initializer_list>



namespace iegad {


template <class Key,
          class T,
          class Compare = std::less<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
class map {
public:
    typedef boost::shared_mutex rw_mutex_t;
    typedef boost::shared_lock<rw_mutex_t> read_lock_t;
    typedef boost::unique_lock<rw_mutex_t> write_lock_t;

    typedef std::map<Key, T, Compare, Allocator> map_t;
    typedef T mapped_t;
    typedef Key key_t;
    typedef std::pair<const key_t, T> value_t;
    typedef Compare key_compare_t;
    typedef Allocator allocator_t;
    typedef typename Allocator::reference reference_t;
    typedef typename Allocator::const_reference const_reference_t;
    typedef typename Allocator::pointer pointer_t;
    typedef typename Allocator::const_pointer const_pointer_t;
    typedef typename map_t::iterator iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef typename map_t::const_iterator const_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;


    T & at(const Key & key) {
        read_lock_t lk(x_);
        return m_.at(key);
    }

    const T & at(const Key & key) const {
        read_lock_t lk(x_);
        return m_.at(key);
    }

    T& operator[](const Key & key) {
        read_lock_t lk(x_);
        return m_[key];
    }

    T & operator [](Key && key) {
        read_lock_t lk(x_);
        return m_[key];
    }

    iterator begin() {
        read_lock_t lk(x_);
        return m_.begin();
    }

    const_iterator begin() const {
        read_lock_t lk(x_);
        return m_.begin();
    }

    const_iterator cbegin() const {
        read_lock_t lk(x_);
        return m_.cbegin();
    }

    iterator end() {
        read_lock_t lk(x_);
        return m_.end();
    }


    const_iterator end() const {
        read_lock_t lk(x_);
        return m_.end();
    }


    const_iterator cend() const {
        read_lock_t lk(x_);
        return m_.end();
    }

    reverse_iterator rbegin() {
        read_lock_t lk(x_);
        return m_.rbegin();
    }


    const_reverse_iterator rbegin() const {
        read_lock_t lk(x_);
        return m_._rbegin();
    }


    const_reverse_iterator crbegin() const {
        read_lock_t lk(x_);
        return m_.crbegin();
    }

    reverse_iterator rend() {
        read_lock_t lk(x_);
        return m_.rend();
    }

    const_reverse_iterator rend() const {
        read_lock_t lk(x_);
        return m_.rend();
    }

    const_reverse_iterator crend() const {
        read_lock_t lk(x_);
        return m_.crend();
    }

    bool empty() const {
        read_lock_t lk(x_);
        return m_.empty();
    }

    size_t size() const {
        read_lock_t lk(x_);
        return m_.size();
    }


    size_t max_size() const {
        read_lock_t lk(x_);
        return m_.max_size();
    }


    void clear() {
        write_lock_t lk(x_);
        m_.clear();
    }


    std::pair<iterator, bool> insert(const value_t & v) {
        write_lock_t lk(x_);
        return m_.insert(v);
    }

    template<class P>
    std::pair<iterator, bool> insert(P && v) {
        write_lock_t lk(x_);
        return m_.insert(v);
    }


    iterator insert(iterator hint, const value_t & v) {
        write_lock_t lk(x_);
        return m_.insert(hint, v);
    }


    iterator insert(const_iterator hint, const value_t & v) {
        write_lock_t lk(x_);
        return m_.insert(hint, v);
    }

    template <class P>
    iterator insert(const_iterator hint, P && v) {
        write_lock_t lk(x_);
        return m_.insert(hint, v);
    }


    template <class InputIt>
    void insert(InputIt first, InputIt last) {
        write_lock_t lk(x_);
        return m_.insert(first, last);
    }


    void insert(std::initializer_list<value_t> ilist) {
        write_lock_t lk(x_);
        return m_.insert(ilist);
    }


    void erase(iterator pos) {
        write_lock_t lk(x_);
        m_.erase(pos);
    }


    iterator erase(const_iterator pos) {
        write_lock_t lk(x_);
        return m_.erase(pos);
    }


    void erase(iterator first, iterator last) {
        write_lock_t lk(x_);
        return m_.erase(first, last);
    }


    iterator erase(const_iterator first, const_iterator last) {
        write_lock_t lk(x_);
        return m_.erase(first, last);
    }


    size_t erase(const key_t & k) {
        write_lock_t lk(x_);
        return m_.erase(k);
    }


    void swap(map_t & other) {
        write_lock_t lk(x_);
        m_.swap(other);
    }


    size_t count(const key_t & k) const {
        read_lock_t lk(x_);
        return m_.count(k);
    }


    iterator find(const key_t & k) {
        read_lock_t lk(x_);
        return m_.find(k);
    }

    const_iterator find(const key_t &k) const {
        read_lock_t lk(x_);
        return m_.find(k);
    }


    std::pair<iterator, iterator> equal_range(const key_t & k) {
        read_lock_t lk(x_);
        return m_.equal_range(k);
    }


    std::pair<const_iterator, const_iterator> equal_range(const key_t &k) const {
        read_lock_t lk(x_);
        return m_.equal_range(k);
    }


    iterator lower_bound(const key_t & k) {
        read_lock_t lk(x_);
        return m_.lower_bound(k);
    }


    const_iterator lower_bound(const key_t &k) const {
        read_lock_t lk(x_);
        return m_.lower_bound(k);
    }


    iterator upper_bound(const key_t & k) {
        read_lock_t lk(x_);
        return m_.upper_bound(k);
    }

    const_iterator upper_bound(const key_t &k) const {
        read_lock_t lk(x_);
        return m_.upper_bound(k);
    }


    key_compare_t key_comp() const {
        return m_.key_comp();
    }
private:
    mutable rw_mutex_t x_;
    map_t m_;
}; // class map<K, V>;


} // namespace iegad;



#endif // __IEGAD_SAFE_MAP__
