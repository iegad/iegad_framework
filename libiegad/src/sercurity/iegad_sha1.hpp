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
	SHA1() {
	    _sha_init();
	}

	~SHA1() {}

	bool sha_go(const std::string input, std::string * output) {
	    if (input.empty() || output == nullptr)
		return false;
	    char buffer[41];
	    _sha_init();

	    // One times analyse 64Bytes, 512 bits.
	    int nInputLen = input.size();
	    int nDealDataLen = 0;							//	the length of can-deal-data, this times;
	    for (int pos = 0; pos <= nInputLen; pos += 64) {
		if (nInputLen - pos >= 64) {
		    nDealDataLen = 64;						// input-data is enough fill 64bytes,
		    memset(Message_Block, 0, sizeof(Message_Block));
		    memcpy(Message_Block, input.c_str() + pos, nDealDataLen);

		    _add_data_len(nDealDataLen);
		    _process_msg_block();
		    _add_data_len(0);
		}
		else {
		    nDealDataLen = nInputLen - pos;			//	input-data isn't enough fill 64bytes,need fill 0x8000000000 and lenth of real-data.
		    memset(Message_Block, 0, sizeof(Message_Block));
		    memcpy(Message_Block, input.c_str() + pos, nDealDataLen);

		    _add_data_len(nDealDataLen);
		    _pad_msg();
		}
	    }

	    // copy result to output
	    for (int i = 0; i < 5; i++) {
		sprintf(&(buffer[8 * i]), "%08x", H[i]);
	    }
	    *output = buffer;
	    return true;
	}

	
    private:
	unsigned int H[5];					// Message digest buffers
	unsigned int Length_Low;				// Message length in bits
	unsigned int Length_High;			// Message length in bits
	unsigned char Message_Block[64];	// 512-bit message blocks
	int Message_Block_Index;				// Index into message block array

	void _sha_init() {
	    Length_Low = 0;
	    Length_High = 0;
	    Message_Block_Index = 0;

	    H[0] = 0x67452301;
	    H[1] = 0xEFCDAB89;
	    H[2] = 0x98BADCFE;
	    H[3] = 0x10325476;
	    H[4] = 0xC3D2E1F0;
	}


	void _add_data_len(int nDealDataLen) {
	    Message_Block_Index = nDealDataLen;

	    if ((Length_Low += ((unsigned int)nDealDataLen << 3)) < ((unsigned int)nDealDataLen << 3))
	    {
		Length_High++;
	    }
	    Length_High += ((unsigned int)nDealDataLen >> 29);
	}

	void _process_msg_block() {
	    const unsigned K[] = { 				// Constants defined for SHA-1
		0x5A827999,
		0x6ED9EBA1,
		0x8F1BBCDC,
		0xCA62C1D6
	    };
	    int 		t;							// Loop counter
	    unsigned 	temp;						// Temporary word value
	    unsigned	W[80];						// Word sequence
	    unsigned	A, B, C, D, E;				// Word buffers

	    for (t = 0; t < 16; t++)  {
		W[t] = ((unsigned)Message_Block[t * 4]) << 24;
		W[t] |= ((unsigned)Message_Block[t * 4 + 1]) << 16;
		W[t] |= ((unsigned)Message_Block[t * 4 + 2]) << 8;
		W[t] |= ((unsigned)Message_Block[t * 4 + 3]);
	    }

	    for (t = 16; t < 80; t++) {
		W[t] = _circular_shift(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]);
	    }

	    A = H[0];
	    B = H[1];
	    C = H[2];
	    D = H[3];
	    E = H[4];

	    for (t = 0; t < 20; t++) {
		temp = _circular_shift(5, A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = _circular_shift(30, B);
		B = A;
		A = temp;
	    }

	    for (t = 20; t < 40; t++) {
		temp = _circular_shift(5, A) + (B ^ C ^ D) + E + W[t] + K[1];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = _circular_shift(30, B);
		B = A;
		A = temp;
	    }

	    for (t = 40; t < 60; t++) {
		temp = _circular_shift(5, A) +
		    ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = _circular_shift(30, B);
		B = A;
		A = temp;
	    }

	    for (t = 60; t < 80; t++) {
		temp = _circular_shift(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
		temp &= 0xFFFFFFFF;
		E = D;
		D = C;
		C = _circular_shift(30, B);
		B = A;
		A = temp;
	    }

	    H[0] = (H[0] + A) & 0xFFFFFFFF;
	    H[1] = (H[1] + B) & 0xFFFFFFFF;
	    H[2] = (H[2] + C) & 0xFFFFFFFF;
	    H[3] = (H[3] + D) & 0xFFFFFFFF;
	    H[4] = (H[4] + E) & 0xFFFFFFFF;
	}


	void _pad_msg() {
	    if (Message_Block_Index > 55) {
		Message_Block[Message_Block_Index++] = 0x80;
		while (Message_Block_Index < 64) {
		    Message_Block[Message_Block_Index++] = 0;
		}

		_process_msg_block();
		while (Message_Block_Index < 56) {
		    Message_Block[Message_Block_Index++] = 0;
		}
	    }
	    else {
		Message_Block[Message_Block_Index++] = 0x80;
		while (Message_Block_Index < 56) {
		    Message_Block[Message_Block_Index++] = 0;
		}
	    }

	    Message_Block[56] = (Length_High >> 24) & 0xFF;
	    Message_Block[57] = (Length_High >> 16) & 0xFF;
	    Message_Block[58] = (Length_High >> 8) & 0xFF;
	    Message_Block[59] = (Length_High)& 0xFF;
	    Message_Block[60] = (Length_Low >> 24) & 0xFF;
	    Message_Block[61] = (Length_Low >> 16) & 0xFF;
	    Message_Block[62] = (Length_Low >> 8) & 0xFF;
	    Message_Block[63] = (Length_Low)& 0xFF;

	    _process_msg_block();
	}

	unsigned _circular_shift(int bits, unsigned word) {
	    return ((word << bits) & 0xFFFFFFFF) | ((word & 0xFFFFFFFF) >> (32 - bits));
	}
    }; // class SHA1;



} // namespace security;
} // namespace iegad;



#endif // __SHA1_H__;
