#include "FuncHandle.h"
#include "Environment.h"
#include <ctime>
#include <stdlib.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <acl_cpp/redis/redis_string.hpp>
#include <glog/logging.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>



BaseHandler::BaseHandler(const std::string &redisConnString)
    :
      rdsHandler_()
{
    rdsHandler_.set(redisConnString.c_str(), RedisMaxConnect);
}


SetUpHandler::SetUpHandler(const std::string &redisConnString)
    : BaseHandler(redisConnString)
{}


const std::string
SetUpHandler::SetUp(const PSetUp &param)
{
    acl::redis_string cmdStr;
    boost::uuids::random_generator rgen;
    boost::uuids::uuid u(rgen());
    std::string usrId = boost::uuids::to_string(u);
    std::string logInfoKey = LOG_INFO + usrId;
    std::string usrIdKey = USR_ID + param.Email;

    cmdStr.set_cluster(this->handler(), RedisMaxConnect);

    do {
    // step 1 : 设定Email与UsrID关联 Email = UsrID
        if (!cmdStr.setnx(usrIdKey.c_str(), usrId.c_str())) {
            usrId = "user is exists";
            break;
        }
        cmdStr.clear();
    // step 2 : 设定登录信息， UsrID = Password
        if (!cmdStr.setnx(logInfoKey.c_str(), param.Password.c_str())) {
            usrId = "unkown error";
            break;
        }

    } while(0);
    return usrId;
}


UpdatePasswordHandler::UpdatePasswordHandler(const std::string &redisConnString)
    : BaseHandler(redisConnString)
{}

bool
UpdatePasswordHandler::UpdatePassword(const PUpdatePassword &param)
{
    acl::string oldPassword;
    std::string logInfoKey = LOG_INFO + param.UsrID;
    acl::redis_string cmd;

    cmd.set_cluster(this->handler(), RedisMaxConnect);
    do {
	// step 1 : 查找UsrID 对应的 原始密码
        if (!cmd.get(logInfoKey.c_str(), oldPassword)) {
            break;
        }
	// step 2 : 比较
        if  (oldPassword.c_str() != param.OldPassword) {
            break;
        }
	// step 3 : 更新密码
        if (!cmd.set(logInfoKey.c_str(), param.NewPassword.c_str())) {
            break;
        }
        return true;
    } while(false);
    return false;
}


BeginBindPhoneNumberHandler::BeginBindPhoneNumberHandler(const std::string &redisConnString)
    : BaseHandler(redisConnString)
{}


int32_t
BeginBindPhoneNumberHandler::BeginBindPhoneNumber(const PBindPhoneNumber &param)
{
    acl::redis_string cmdStr;
    acl::redis_transaction trans;
    std::string cmd = "SETNX";
    std::vector<acl::string> argList;
    cmdStr.set_cluster(this->handler(), RedisMaxConnect);
    trans.set_cluster(this->handler(), RedisMaxConnect);
    std::string phoneInfoKey = PHONE_INFO + param.UsrID;
    std::string logInfoKey = LOG_INFO + std::to_string(param.PhoneNumber);
    std::string tempInvalidCodeKey = TEMP_INVALID_CODE + param.UsrID;
    acl::string value;
    std::string invalidCode;
    do {
        if (cmdStr.get(logInfoKey.c_str(), value)) {
            break;
        }
        cmdStr.clear();

	// step 1 : 查询 UsrID = PhoneNumber
        if (cmdStr.get(phoneInfoKey.c_str(), value)) {
            break;
        }

        cmdStr.clear();
	// step 2 : 查询 临时验证码 UsrID = invalidCode
        if (cmdStr.get(tempInvalidCodeKey.c_str(), value)) {
            break;
        }

        if (!trans.multi()) {
            break;
        }

        invalidCode = tools::GenInvalidCode();
        argList.push_back(tempInvalidCodeKey.c_str());
        argList.push_back(invalidCode.c_str());
	// step 3 : 设定验证码 UsrID = invalidCode
        if (!trans.run_cmd(cmd.c_str(), argList)) {
            break;
        }
        argList.clear();
        cmd.clear();
        trans.clear();

        cmd = "EXPIRE";
        argList.push_back(tempInvalidCodeKey.c_str());
        argList.push_back("70");
	// step 4 : 设定超时值
        if (!trans.run_cmd(cmd.c_str(), argList)) {
            break;
        }

        if (!trans.exec()) {
            break;
        }
	// step 5 : 发送短信
        SendInvalidCode(invalidCode);
	
        return std::stoi(invalidCode);
    } while(false);
    trans.discard();
    return -1;
}


bool
BeginBindPhoneNumberHandler::SendInvalidCode(const std::string &invalidCode)
{
    return true;
}


EndBindPhoneNumberHandler::EndBindPhoneNumberHandler(const std::string &redisConnString)
    : BaseHandler(redisConnString)
{}


bool
EndBindPhoneNumberHandler::EndBindPhoneNumber(const PBindPhoneNumber &param)
{
    acl::redis_string cmdStr;
    acl::redis_transaction trans;
    std::string cmd = "SETNX";
    std::vector<acl::string> argList;
    acl::string value;
    std::string tempInvalidCodeKey = TEMP_INVALID_CODE + param.UsrID;
    std::string phoneInfoKey = PHONE_INFO + param.UsrID;
    std::string usrIdKey = USR_ID + std::to_string(param.PhoneNumber);
    cmdStr.set_cluster(this->handler(), RedisMaxConnect);
    trans.set_cluster(this->handler(), RedisMaxConnect);

    do {
	// step 1 : 获取验证码
        if (!cmdStr.get(tempInvalidCodeKey.c_str(), value)) {
            break;
        }
	// step 2 : 比较
        if (std::to_string(param.InvalidCode) != value.c_str()) {
            break;
        }

        if (!trans.multi()) {
            break;
        }
	
        argList.push_back(phoneInfoKey.c_str());
        argList.push_back(std::to_string(param.PhoneNumber).c_str());
	// step 3 : 绑定手机号 UsrID = PhoneNumber
        if(!trans.run_cmd(cmd.c_str(), argList)) {
            break;
        }

        argList.clear();
        trans.clear();

        argList.push_back(usrIdKey.c_str());
        argList.push_back(param.UsrID.c_str());
	// step 4 : 添加手机登录方式 PhoneNumber = UsrID
        if (!trans.run_cmd(cmd.c_str(), argList)) {
            break;
        }

        if (!trans.exec()) {
            break;
        }

        return true;
    } while(false);
    trans.discard();
    return false;
}


UpdatePersonalInfoHandler::UpdatePersonalInfoHandler(const std::string &redisConnString)
    : BaseHandler(redisConnString)
{}


bool
UpdatePersonalInfoHandler::UpdatePersonalInfo(const PUpdatePersonalInfo &param)
{
    acl::redis_string cmdStr;
    std::string jsonString;
    std::string perInfoKey = PERSONAL_INFO + param.UsrID;

    cmdStr.set_cluster(this->handler(), RedisMaxConnect);
    do {
        if (!SerializeToJsonString(param, &jsonString)) {
            break;
        }

        if (!cmdStr.set(perInfoKey.c_str(), jsonString.c_str())) {
            break;
        }
        return true;
    } while(false);
    return false;
}


bool
UpdatePersonalInfoHandler::SerializeToJsonString(const PUpdatePersonalInfo &param,
                                                 std::string *jsonString)
{
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType & alloc = doc.GetAllocator();
    rapidjson::Value perInfo(rapidjson::kObjectType);
    perInfo.AddMember("Sex", -1, alloc);
    perInfo.AddMember("Nation", -1, alloc);
    perInfo.AddMember("Education", -1, alloc);
    perInfo.AddMember("Constellation", -1, alloc);
    perInfo.AddMember("ShengXiao", -1, alloc);
    perInfo.AddMember("Blood", -1, alloc);
    perInfo.AddMember("LocalAddress", -1, alloc);
    perInfo.AddMember("Job", -1, alloc);
    perInfo.AddMember("Hometown", -1, alloc);
    perInfo.AddMember("BirthDate", -1, alloc);
    perInfo.AddMember("Name", "", alloc);
    perInfo.AddMember("Nick", "", alloc);
    perInfo.AddMember("College", "", alloc);

    if (param.__isset.Sex) {
        perInfo["Sex"] = static_cast<int>(param.Sex);
    }
    else {
        perInfo["Sex"].SetNull();
    }
    if (param.__isset.Nation) {
        perInfo["Nation"] = static_cast<int>(param.Nation);
    }
    else {
        perInfo["Nation"].SetNull();
    }
    if (param.__isset.Education) {
        perInfo["Education"] = static_cast<int>(param.Education);
    }
    else {
        perInfo["Education"].SetNull();
    }
    if (param.__isset.Constellation) {
        perInfo["Constellation"] = static_cast<int>(param.Constellation);
    }
    else {
        perInfo["Constellation"].SetNull();
    }
    if (param.__isset.ShengXiao) {
        perInfo["ShengXiao"] = static_cast<int>(param.ShengXiao);
    }
    else {
        perInfo["ShengXiao"].SetNull();
    }
    if (param.__isset.Blood) {
        perInfo["Blood"] = static_cast<int>(param.Blood);
    }
    else {
        perInfo["Blood"].SetNull();
    }
    if (param.__isset.LocalAddress) {
        perInfo["LocalAddress"] = param.LocalAddress;
    }
    else {
        perInfo["LocalAddress"].SetNull();
    }
    if (param.__isset.Job) {
        perInfo["Job"] = param.Job;
    }
    else {
        perInfo["Job"].SetNull();
    }
    if (param.__isset.Hometown) {
        perInfo["Hometown"] = param.Hometown;
    }
    else {
        perInfo["Hometown"].SetNull();
    }
    if (param.__isset.BirthDate) {
        perInfo["BirthDate"] = param.BirthDate;
    }
    else {
        perInfo["BirthDate"].SetNull();
    }

    if (param.__isset.Name) {
        perInfo["Name"].SetString(param.Name.c_str(), param.Name.length());
    }
    else {
        perInfo["Name"].SetNull();
    }
    if (param.__isset.Nick) {
        perInfo["Nick"].SetString(param.Nick.c_str(), param.Nick.length());
    }
    else {
        perInfo["Nick"].SetNull();
    }
    if (param.__isset.College) {
        perInfo["College"].SetString(param.College.c_str(), param.College.length());
    }
    else {
        perInfo["College"].SetNull();
    }
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    perInfo.Accept(writer);
    *jsonString = buffer.GetString();
    return true;
}


SetInHandler::SetInHandler(const std::string &redisConnString)
    : BaseHandler(redisConnString)
{}


bool
SetInHandler::SetIn(RSetIn *res, const PSetIn &param)
{
    acl::redis_string cmdStr;
    acl::redis_list cmdList;
    cmdStr.set_cluster(this->handler(), RedisMaxConnect);
    cmdList.set_cluster(this->handler(), RedisMaxConnect);

    std::string usrIdKey = USR_ID +
            (param.__isset.Email ? param.Email : std::to_string(param.PhoneNumber));
    std::string logInfoKey = LOG_INFO;
    std::string usrLogRcdListKey = USR_LOG_RECORD;
    acl::string usrId, pwd;

    do {
        if (!cmdStr.get(usrIdKey.c_str(), usrId)) {
            res->__set_UsrID("usrId not exists");
            break;
        }

        logInfoKey += usrId.c_str();

        if (!cmdStr.get(logInfoKey.c_str(), pwd)) {
            res->__set_UsrID("usrId not exists");
            break;
        }

        if (pwd.c_str() != param.Password) {
            res->__set_UsrID("password error");
            break;
        }

        usrLogRcdListKey += usrId;
        std::string logRcd = BuildLogRecord();
        if (!cmdList.lpush(usrLogRcdListKey.c_str(), logRcd.c_str(), nullptr)) {
            LOG(INFO) << cmdList.result_error()<<std::endl;
        }

        res->__set_UsrID(usrId.c_str());
        return true;
    } while(false);
    return false;
}


const std::string
SetInHandler::BuildLogRecord()
{
    std::time_t tnow = std::time(nullptr);
    rapidjson::Document doc;
    rapidjson::Document::AllocatorType & alloc = doc.GetAllocator();
    rapidjson::Value logRcd(rapidjson::kObjectType);
    logRcd.AddMember("LogType", "SetIn", alloc);
    logRcd.AddMember("LogTime", static_cast<int64_t>(tnow), alloc);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    logRcd.Accept(writer);
    return buffer.GetString();
}



