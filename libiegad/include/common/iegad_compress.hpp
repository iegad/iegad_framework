#ifndef __IEGAD_COMPRESS__
#define __IEGAD_COMPRESS__



// ============ 说明 ============
//
// @创建日期 : 2015-11-13
// @创建人 : iegad
//
// ============================
// @用途 :  提供压缩算法, 使用google snappy 压缩
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



#include <snappy/snappy.h>


namespace iegad {
namespace tools {


// ============================
// @用途 : 压缩 数据
// @input : 需要压缩的数据
// @input_len : 数据长度
// @output : 压缩后的数据
// @返回值 : 压缩后, 数据的长度
// ============================
inline size_t
compress(const char * input, size_t input_len, std::string * output) 
{
    return snappy::Compress(input, input_len, output);
}


// ============================
// @用途 : 解压 数据
// @compressed : 需要解压的数据
// @compressed_len : 解压数据的长度
// @uncompressed : 解压后的数据
// @返回值 : 成功返回 true, 否则返回 false
// @PS : 如果参数 compressed 不为压缩数据, 返回false
// ============================
inline bool
uncompress(const char * compressed, size_t compressed_len, std::string * uncompressed) 
{
    return snappy::Uncompress(compressed, compressed_len, uncompressed);
}


} // namespace tools;
} // namespace iegad;


#endif // __IEGAD_COMPRESS__