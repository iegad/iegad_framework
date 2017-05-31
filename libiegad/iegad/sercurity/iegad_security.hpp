#ifndef __IEGAD_SECURITY__
#define __IEGAD_SECURITY__



// ============ 说明 ============
//
// @创建日期 : 2017-05-31
// @创建人 : iegad
//
// ============================
// @用途 : 安全方面函数封装，
//        目前支持AES128, MD5, SHA1, UUID
//
//        UUID需要boost的支持
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================
// -- 2017-05-31        -- iegad           -- 1, 将返回值由const std::string 改为std::string
//                                         -- 2, 添加base64编解码函数



#include "../iegad_config.h"

#include "sercurity/iegad_aes.hpp"
#include "sercurity/iegad_sha1.hpp"
#include "sercurity/iegad_md5.hpp"
#include "string/iegad_string.hpp"
#include "sercurity/iegad_base64.hpp"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>



namespace iegad {
class security {
public:

// ============================
// @用途 : 将字符串 src 进行md5加密
// @src : 需要加密的字符串
// @返回值 : md5加密之后的字符串
// ============================
static std::string
md5(const std::string & src)
{
    if (src.empty()) {
        return iegad::string::ERR_STR();
    }

    iegad::MD5 m(src);
    return m.hexdigest();
}


// ============================
// @用途 : 将字符串 src 进行sha1加密, 并将结果存放到 digest中
// @src : 需要加密的字符串
// @ser : 用来保存摘要的数组容器
// @返回值 : sha1加密之后的二进制数据
// @PS : 返回值是二进制数据而不是字符串
// ============================
static std::string
sha1(const std::string & src)
{
    if (src.empty()) {
        return iegad::string::ERR_STR();
    }

    iegad::SHA1 sha1;
    std::string res;
    return sha1.sha_go(src, &res) ? res : iegad::string::ERR_STR();
}


// ============================
// @用途 : 获取一个 36位 uuid 字符串
// @返回值 : 一个 36 位, 小写, 中间还代有 短横杠的 guid字符串
// ============================
static std::string
uuid()
{
    boost::uuids::random_generator rgen;
    boost::uuids::uuid u = rgen();
    return boost::uuids::to_string(u);
}

#if (IEGAD_OPTION_SSL)
static const std::string
aes_encrypt(const std::string & src, const std::string & k)
{
    if (src.empty() || k.empty()) {
        return iegad::string::ERR_STR();
    }
    return iegad::AES::encrypt(src, k);
}


static const std::string
aes_decrypt(const std::string & enstr, const std::string & k)
{
    if (enstr.empty() || k.empty()) {
        return iegad::string::ERR_STR();
    }
    return iegad::AES::decrypt(enstr, k);
}
#endif // (IEGAD_OPTION_SSL)


static std::string
base64_encode(const std::string & src)
{
    return iegad::base64::encode((const unsigned char *)src.c_str(), src.size());
}


static std::string
base64_decode(const std::string & base64_str)
{
    return iegad::base64::decode(base64_str);
}


}; // class AES;
} // namespace iegad;


#endif // __IEGAD_SECURITY__
