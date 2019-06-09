#ifndef __IEGAD_SECURITY__
#define __IEGAD_SECURITY__



// ============ 说明 ============
//
// @创建日期 : 2017-05-31
// @创建人 : iegad
//
// ============================
// @用途 : 安全方面函数封装，
//        目前支持AES128, MD5, SHA1, UUID, base64
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
// -- 2017-06-04        -- iegad           -- 1, 添加注释
//                                         -- 2, 为base64添加入参校验
// -- 2017-06-16        -- iegad           -- 重写sha1算法，改用boost sha1算法



#include "iegad/security/iegad_aes.hpp"
#include "iegad/security/iegad_md5.hpp"
#include "iegad/string/iegad_string.hpp"
#include "iegad/security/iegad_base64.hpp"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/detail/sha1.hpp>


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
// @用途 : 将字符串 src 进行sha1摘要算法
// @src : 需要加密的字符串
// @返回值 : sha1加密之后的字符串
// ============================
static std::string
sha1(const std::string & src, uint32_t *bin = NULL)
{
    boost::uuids::detail::sha1 sha;
    uint32_t dst[5];
    sha.process_bytes(src.c_str(), src.size());
    sha.get_digest(dst);
    for (int i = 0; i < 5; i++) {
        dst[i] = htonl(dst[i]);
    }
    if (bin) {
        memcpy(bin, dst, sizeof(dst));
    }
    return iegad::string::to_lwr(iegad::string::bin_tostr((unsigned char *)dst, sizeof(dst)));
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


// ============================
// @用途 : 将字符串 src 进行AES128加密
// @src : 需要加密的字符串
// @k : 加密的key
// @返回值 : aes128加密后的字符串
// ============================
static const std::string
aes_encrypt(const std::string & src, const std::string & k)
{
    if (src.empty() || k.empty()) {
        return iegad::string::ERR_STR();
    }
    return iegad::AES::encrypt(src, k);
}


// ============================
// @用途 : 将字符串 ensrc 进行AES128解密
// @enstr : 需要解密的字符串
// @k : 加密的key
// @返回值 : aes128解密后的字符串
// ============================
static const std::string
aes_decrypt(const std::string & enstr, const std::string & k)
{
    if (enstr.empty() || k.empty()) {
        return iegad::string::ERR_STR();
    }
    return iegad::AES::decrypt(enstr, k);
}


// ============================
// @用途 : 将字符串 src 进行base64编码
// @src : 需要编码的字符串
// @返回值 : base64编码后的字符串
// ============================
static std::string
base64_encode(const std::string & src)
{
    if (src.empty()) {
        return iegad::string::EMPTY_STR();
    }
    return iegad::base64::encode((const unsigned char *)src.c_str(), src.size());
}


// ============================
// @用途 : 将字符串 src 进行base64解码
// @base64_str : 需要解码的字符串
// @返回值 : base64解码后的字符串
// ============================
static std::string
base64_decode(const std::string & base64_str)
{
    if (base64_str.empty()) {
        return iegad::string::EMPTY_STR();
    }
    return iegad::base64::decode(base64_str);
}


}; // class AES;
} // namespace iegad;


#endif // __IEGAD_SECURITY__
