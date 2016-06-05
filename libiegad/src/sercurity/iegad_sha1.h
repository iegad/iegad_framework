#ifndef __SHA1_H__
#define __SHA1_H__



#include<string.h>
#include <string>
#include<stdio.h>



namespace iegad {
namespace security {



    class SHA1
    {
    public:
	SHA1();
	virtual ~SHA1();
	bool sha_go(const std::string input, std::string * output);
    private:
	unsigned int H[5];					// Message digest buffers
	unsigned int Length_Low;				// Message length in bits
	unsigned int Length_High;			// Message length in bits
	unsigned char Message_Block[64];	// 512-bit message blocks
	int Message_Block_Index;				// Index into message block array
    private:
	void _sha_init();
	void _add_data_len(int nDealDataLen);

	// Process the next 512 bits of the message
	void _process_msg_block();

	// Pads the current message block to 512 bits
	void _pad_msg();

	// Performs a circular left shift operation
	inline unsigned _circular_shift(int bits, unsigned word);
    }; // class SHA1;



} // namespace security;
} // namespace iegad;



#endif // __SHA1_H__;
