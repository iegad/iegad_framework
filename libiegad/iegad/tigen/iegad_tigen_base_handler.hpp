#ifndef __IEGAD_TIGEN_BASE_HANDLER__
#define __IEGAD_TIGEN_BASE_HANDLER__



#include <string>



namespace iegad {
namespace tigen {



template<typename __DAI__>
class TigenBaseHandler {
protected:
    TigenBaseHandler(const std::string &initstr) :
        dai_(initstr)
    {}


    virtual ~TigenBaseHandler()
    {}


    bool
    check(std::string *errstr)
    {
        return dai_.check(errstr);
    }


    template<typename __QUERY__>
    bool
    get(__QUERY__ &query, std::string *errstr)
    {
         return dai_.get(query, errstr);
    }


    bool
    reset(std::string *errstr)
    {
        return dai_.reset(errstr);
    }


private:
    __DAI__ dai_;
}; // class;



} // namespace tigen;
} // namespace iegad;



#endif // __IEGAD_TIGEN_BASE_HANDLER__
