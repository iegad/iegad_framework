#ifndef __IEGAD_AES__
#define __IEGAD_AES__



#include <string>



class AES_128 {
public:
	static const std::string GetRandom128Key();
	AES_128(unsigned char* key);
	~AES_128();
	const std::string encrypt(const char* input);
	const std::string decrypt(const char* input);
	void* encrypt(void* input, int length = 0);
	void* decrypt(void* input, int length);

private:
	unsigned char Sbox[256];
	unsigned char InvSbox[256];
	unsigned char w[11][4][4];

	void _key_expansion(unsigned char* key, unsigned char w[][4][4]);
	unsigned char _ffmul(unsigned char a, unsigned char b);

	void _sub_bytes(unsigned char state[][4]);
	void _shift_rows(unsigned char state[][4]);
	void _mix_columns(unsigned char state[][4]);
	void _add_round_key(unsigned char state[][4], unsigned char k[][4]);

	void _inv_sub_bytes(unsigned char state[][4]);
	void _inv_shift_rows(unsigned char state[][4]);
	void _inv_mix_columns(unsigned char state[][4]);
}; // class AES_128;



#endif // __IEGAD_AES__;
