#ifndef __IEGAD_MAP__
#define __IEGAD_MAP__


#include <unordered_map>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <assert.h>


namespace iegad {
namespace tools {

    template <typename __TKey, typename __TValue>
    class safe_map {
    public:
	typedef std::unordered_map<__TKey, __TValue> map_t;
	typedef boost::shared_mutex rw_mtx_t;
	typedef boost::shared_lock<rw_mtx_t> read_lock_t;
	typedef boost::unique_lock<rw_mtx_t> write_lock_t;
	typedef map_t::key_type key_type;
	typedef map_t::mapped_type mapped_type;
	typedef map_t::value_type value_type;
	typedef map_t::pointer pointer;
	typedef map_t::const_pointer const_pointer;
	typedef map_t::size_type size_type;
	typedef map_t::difference_type difference_type;
	typedef map_t::reference reference;
	typedef map_t::const_reference const_reference;
	typedef map_t::iterator iterator;
	typedef map_t::const_iterator const_iterator;


	bool empty() const {
	    read_lock_t lock(mtx_);
	    return map_.empty();
	}

	size_type size() const {
	    read_lock_t lock(mtx_);
	    return map_.size();
	}

	size_type max_size() const {
	    read_lock_t lock(mtx_);
	    return map_.max_size();
	}

	iterator begin() {
	    read_lock_t lock(mtx_);
	    return map_.begin();
	}

	const_iterator begin() const {
	    read_lock_t lock(mtx_);
	    return map_.begin();
	}

	iterator end() {
	    read_lock_t lock(mtx_);
	    return map_.end();
	}

	const_iterator end() const {
	    read_lock_t lock(mtx_);
	    return map_.end();
	}

	bool insert(const key_type & k, const mapped_type & v) {
	    write_lock_t lock(mtx_);
	    return map_.insert(value_type(k, v)).second;
	}

	bool find(const key_type & k) {
	    read_lock_t lock(mtx_);
	    return map_.find(k) != map_.end();
	}

	size_type erase(const key_type & k) {
	    write_lock_t lock(mtx_);
	    return map_.erase(k);
	}

	void clear() {
	    write_lock_t lock(mtx_);
	    map_.clear();
	}

	const mapped_type & operator[](const key_type & k) const {
	    read_lock_t lock(mtx_);
	    assert(map_.find(k) != map_.end());
	    return map_[k];
	}

	void set(const key_type & k, const mapped_type & v) {
	    write_lock_t lock(mtx_);
	    map_[k] = v;
	}

	template<typename Func>
	void for_each(Func func) {
	    read_lock_t lock(mtx_);
	    std::for_each(map_.begin(), map_.end(), func);
	}

    private:
	map_t map_;
	mutable rw_mtx_t mtx_;
    }; // template <K, V> class safe_map;


} // namespace tools;
} // namespace iegad;



#endif // __IEGAD_MAP__