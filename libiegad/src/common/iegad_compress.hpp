#ifndef __IEGAD_COMPRESS__
#define __IEGAD_COMPRESS__



// ============ ˵�� ============
//
// @�������� : 2015-11-13
// @������ : iegad
//
// ============================
// @��; :  �ṩѹ���㷨, ʹ��google snappy ѹ��
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================



#include <snappy/snappy.h>


namespace iegad {
namespace tools {


// ============================
// @��; : ѹ�� ����
// @input : ��Ҫѹ��������
// @input_len : ���ݳ���
// @output : ѹ���������
// @����ֵ : ѹ����, ���ݵĳ���
// ============================
inline size_t
compress(const char * input, size_t input_len, std::string * output) 
{
    return snappy::Compress(input, input_len, output);
}


// ============================
// @��; : ��ѹ ����
// @compressed : ��Ҫ��ѹ������
// @compressed_len : ��ѹ���ݵĳ���
// @uncompressed : ��ѹ�������
// @����ֵ : �ɹ����� true, ���򷵻� false
// @PS : ������� compressed ��Ϊѹ������, ����false
// ============================
inline bool
uncompress(const char * compressed, size_t compressed_len, std::string * uncompressed) 
{
    return snappy::Uncompress(compressed, compressed_len, uncompressed);
}


} // namespace tools;
} // namespace iegad;


#endif // __IEGAD_COMPRESS__