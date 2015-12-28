#ifndef __FUNC_HANDLE__
#define __FUNC_HANDLE__


#include "AppService_types.h"
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <acl_cpp/lib_acl.hpp>


using namespace onepc;
using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;



const std::string LOG_INFO = "LoginInfo.";
const std::string USR_ID = "UsrID.";
const std::string PHONE_INFO = "PhoneNumber.";
const std::string EMAIL_INFO = "Email.";
const std::string TEMP_INVALID_CODE = "TempInvalidCode.";
const std::string PERSONAL_INFO = "PersonalInfo.";
const std::string USR_LOG_RECORD = "UsrLogRecord.";



class BaseHandler {
public:
    explicit BaseHandler(const std::string & redisConnString);
    virtual ~BaseHandler() {}

protected:
    acl::redis_client_cluster * handler() {
        return &rdsHandler_;
    }

private:
    BaseHandler(const BaseHandler &);
    BaseHandler & operator=(const BaseHandler &);
    acl::redis_client_cluster rdsHandler_;
}; // class BaseHandler;



class SetUpHandler : public BaseHandler {
public:
    explicit SetUpHandler(const std::string & redisConnString);
    const std::string SetUp(const PSetUp & param);
}; // class SetUpHandler;


class UpdatePasswordHandler : public BaseHandler {
public:
    explicit UpdatePasswordHandler(const std::string & redisConnString);
    bool UpdatePassword(const PUpdatePassword & param);

private:
    acl::redis_client_cluster rdsHandler_;
}; // class UpdatePasswordHandler;


class BeginBindPhoneNumberHandler : public BaseHandler {
public:
    explicit BeginBindPhoneNumberHandler(const std::string & redisConnString);
    int32_t BeginBindPhoneNumber(const PBindPhoneNumber& param);

private:
    bool SendInvalidCode(const std::string & invalidCode);
    acl::redis_client_cluster rdsHandler_;
}; // class BeginBindPhoneNumberHandler;


class EndBindPhoneNumberHandler : public BaseHandler {
public:
    explicit EndBindPhoneNumberHandler(const std::string & redisConnString);
    bool EndBindPhoneNumber(const PBindPhoneNumber & param);

private:
    acl::redis_client_cluster rdsHandler_;
}; // class EndBindPhoneNumberHandler;


class UpdatePersonalInfoHandler : public BaseHandler {
public:
    explicit UpdatePersonalInfoHandler(const std::string & redisConnString);
    bool UpdatePersonalInfo(const PUpdatePersonalInfo & param);

private:
    bool SerializeToJsonString(const PUpdatePersonalInfo & param, std::string * jsonString);
    acl::redis_client_cluster rdsHandler_;
}; // class UpdatePersonalInfoHandler;


class SetInHandler : public BaseHandler {
public:
    explicit SetInHandler(const std::string & redisConnString);
    bool SetIn(RSetIn * res, const PSetIn & param);

private:
    const std::string BuildLogRecord();
    acl::redis_client_cluster rdsHandler_;
}; // class UpdatePersonalInfoHandler;



#endif // __FUNC_HANDLE__
