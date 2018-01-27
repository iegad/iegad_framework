#ifndef __IEGAD_TIGEN_BASE_HANDLER__
#define __IEGAD_TIGEN_BASE_HANDLER__



#include <string>



namespace iegad {
namespace tigen {



template<typename __DAI__>
class TigenBaseHandler {
protected:
    typedef typename __DAI__::conn_t conn_t;
    typedef typename __DAI__::dai_t dai_t;

    TigenBaseHandler(conn_t &conn, const std::string &initstr) :
        dai_(conn, initstr)
    {}


    virtual ~TigenBaseHandler()
    {}


    bool
    check(std::string *errstr)
    {
        return dai_.check(errstr);
    }


    bool
    get(dai_t &q, std::string *errstr)
    {
         return dai_.get(q, errstr);
    }


    bool
    reset(std::string *errstr)
    {
        return dai_.reset(errstr);
    }


    std::string
    InvaliedParameters(const std::string &p)
    {
        return "invalied params: " + p;
    }


    std::string
    MissingRequiredParameters(const std::string &p)
    {
        return "miss required params: " + p;
    }


private:
    __DAI__ dai_;
}; // class;



} // namespace tigen;
} // namespace iegad;



#endif // __IEGAD_TIGEN_BASE_HANDLER__
