#include "AppServiceImp.h"
#include "Environment.h"
#include <acl_cpp/lib_acl.hpp>
#include "FuncHandle.h"



AppServiceHandler::AppServiceHandler(const std::string &redisConnString)
    : redisConnString_(redisConnString)
{}


void
AppServiceHandler::SetUp(std::string &_return, const PSetUp &param)
{
    SetUpHandler h(redisConnString_);
    _return = h.SetUp(param);
}


bool
AppServiceHandler::UpdatePersonalInfo(const PUpdatePersonalInfo &param)
{
    UpdatePersonalInfoHandler h(redisConnString_);
    return h.UpdatePersonalInfo(param);
}


bool
AppServiceHandler::UpdatePassword(const PUpdatePassword &param)
{    
    UpdatePasswordHandler h(redisConnString_);
    return  h.UpdatePassword(param);
}


int32_t
AppServiceHandler::FindPasswordWithEmail(const PFindPasswordWithEmail &param)
{

}


int32_t
AppServiceHandler::FindPasswordWithPhoneNumber(const PFindPasswordWithPhoneNumber &param)
{

}


int32_t
AppServiceHandler::BeginBindPhoneNumber(const PBindPhoneNumber &param)
{    
    BeginBindPhoneNumberHandler h(redisConnString_);
    return  h.BeginBindPhoneNumber(param);
}


bool
AppServiceHandler::EndBindPhoneNumber(const PBindPhoneNumber &param)
{    
    EndBindPhoneNumberHandler h(redisConnString_);
    return  h.EndBindPhoneNumber(param);
}


void
AppServiceHandler::SetIn(RSetIn &_return, const PSetIn &param)
{    
    SetInHandler h(redisConnString_);
    h.SetIn(&_return, param);
}





























