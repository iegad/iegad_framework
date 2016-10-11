#ifndef __IEGAD_AES__
#define __IEGAD_AES__


#include <openssl/aes.h>
#include <memory.h>
#include <string>


namespace iegad {
namespace security {


class AES {
public:
    static const std::string encrypt(const std::string & src, const std::string & key) {
        char * in_data = NULL;
        char * en_data = NULL;
        std::string res;

        unsigned char k[AES_BLOCK_SIZE + 1] = {0};
        unsigned char ivec[AES_BLOCK_SIZE] = {0};
        AES_KEY aes_key;

        int srclen=src.size(), len = 0;

        do {
            memset(&aes_key, 0, sizeof(AES_KEY));
            memcpy(k, key.c_str(), AES_BLOCK_SIZE);
            len = srclen % AES_BLOCK_SIZE == 0 ? srclen : ((srclen / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;

            in_data = (char *)calloc(len + 1, sizeof(char));
            if(in_data == NULL) {
                break;
            }
            memcpy(in_data, src.c_str(), srclen);

            en_data = (char *)calloc(len + 1, sizeof(char));
            if(en_data == NULL)	{
                break;
            }

            if(AES_set_encrypt_key(k, 128, &aes_key) < 0) {
                break;
            }

            AES_cbc_encrypt((unsigned char *)in_data, (unsigned char *)en_data,
                len, &aes_key, ivec, AES_ENCRYPT);
            res = std::string(en_data, len);
        } while(false);

        if (in_data) {
            free(in_data);
        }

        if (en_data) {
            free(en_data);
        }

        return res;
    }


    static const std::string decrypt(const std::string & enstr, const std::string & key) {
        char * de_data = NULL;
        unsigned char k[AES_BLOCK_SIZE+1] = {0};
        unsigned char ivec[AES_BLOCK_SIZE] = {0};

        std::string res;
        AES_KEY aes_key;
        int len = enstr.size();

        do {
            memset(&aes_key, 0, sizeof(AES_KEY));
            memcpy(k, key.c_str(), AES_BLOCK_SIZE);

            de_data = (char *)calloc(len + 1, sizeof(char));
            if(de_data == NULL) {
                break;
            }

            if(AES_set_decrypt_key(k, 128, &aes_key) < 0) {
                break;
            }

            AES_cbc_encrypt((unsigned char *)enstr.c_str(), (unsigned char *)de_data,
                len, &aes_key, ivec, AES_DECRYPT);

            res = std::string(de_data, len - 1);
        } while(false);

        if (de_data) {
            free(de_data);
        }

        return res;
    }
}; // class AES;


} // namespace security;
} // namespace iegad;


#endif // __IEGAD_AES__
