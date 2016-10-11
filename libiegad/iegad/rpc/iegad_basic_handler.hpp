#ifndef __IEGAD_BAISC_HANDLER__
#define __IEGAD_BAISC_HANDLER__


#include <boost/smart_ptr.hpp>
#include <map>
#include "iegad_rpc_types.h"
#include "iegad_msg_id.h"


namespace iegad {
namespace rpc {


class basicHandler {
public:
    typedef boost::shared_ptr<basicHandler> ptr_t;
    typedef std::map<int, ptr_t> handler_set_t;


    static bool addHandler(ptr_t p) {
        size_t n = _hset().size();
        _hset().insert(std::make_pair(p->msgId(), p));
        return n + 1U == _hset().size();
    }


    static response func(const request & req) {
        response res;
        do {
            if (!basicHandler::_check(req)) {
                break;
            }

            ptr_t p = basicHandler::_at(req.msg_type);
            if (!p) {
                break;
            }

            res = p->_func(req);
            return res;
        } while(false);
        res.__set_msg_type(MSG_ERR);
        res.__set_msg_data("invalied parameters");
        return res;
    }


    static void action(const request & req) {
        do {
            if (!basicHandler::_check(req)) {
                break;
            }

            ptr_t p = basicHandler::_at(req.msg_type);
            if (!p) {
                break;
            }

            p->_action(req);
        } while(false);
    }


    int32_t msgId() const {
        return msgId_;
    }


protected:
    basicHandler(int msgId) :
        msgId_(msgId)
    {}


    virtual response _func(const request & req) = 0;
    virtual void _action(const request & req) = 0;


private:
    static bool _check(const request & req) {
        return req.__isset.msg_type && req.__isset.msg_flag;
    }


    static ptr_t _at(int msg_id) {
        ptr_t p;
        if (_hset().count(msg_id) == 1) {
            p = _hset()[msg_id];
        }
        return p;
    }


    static handler_set_t & _hset() {
        static handler_set_t hset_;
        return hset_;
    }


    int32_t msgId_;
}; // class basicHandler;


} // namespace rpc;
} // namespace iegad;


#endif // __IEGAD_BAISC_HANDLER__
