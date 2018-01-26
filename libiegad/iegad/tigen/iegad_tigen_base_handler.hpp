#ifndef __IEGAD_TIGEN_BASE_HANDLER__
#define __IEGAD_TIGEN_BASE_HANDLER__



#include <memory>
#include <string>



namespace iegad {
namespace tigen {


template<typename __DAI__>
class TigenBaseHandler {
public:
    virtual ~TigenBaseHandler() {}


protected:
    TigenBaseHandler(const std::string &initstr) :
        dai_(initstr)
    {}


    bool
    check(std::string *errstr)
    {
        return dai_.check(errstr);
    }


    template<typename __QUERY__>
    bool
    get(std::shared_ptr<__QUERY__> &query, std::string *errstr)
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
