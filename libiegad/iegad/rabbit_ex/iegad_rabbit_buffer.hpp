#ifndef __IEGAD_RABBIT_BUFFER__
#define __IEGAD_RABBIT_BUFFER__



#include <inttypes.h>
#include <memory>
#include <memory.h>
#include <vector>


namespace iegad {
namespace rabbit_ex {


class amqpBuffer {
public:
    typedef std::shared_ptr<amqpBuffer> ptr_t;

    amqpBuffer(size_t size) :
            data_(size, 0),
            use_(0)
    {}

    size_t write(const char* data, size_t size) {
        if (use_ == data_.size()) {
            return 0;
        }

        const size_t length = (size + use_);
        size_t write = length < data_.size() ? size : data_.size() - use_;
        memcpy(data_.data() + use_, data, write);
        use_ += write;
        return write;
    }

    void drain() {
        use_ = 0;
    }

    size_t available() const {
        return use_;
    }

    const char* data() const {
        return data_.data();
    }

    void shl(size_t count) {
        assert(count < use_);

        const size_t diff = use_ - count;
        std::memmove(data_.data(), data_.data() + count, diff);
        use_ = use_ - count;
    }


private:
    std::vector<char> data_;
    size_t use_;
}; // class amqpBuffer;


} // namespace rabbit_ex;
} // namespace iegad;



#endif // __IEGAD_RABBIT_BUFFER__
