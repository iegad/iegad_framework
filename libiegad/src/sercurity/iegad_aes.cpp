#include "iegad_aes.h"
#include "string.h"
#include <ctime>





AES_128::AES_128(unsigned char* key)
{
	memcpy(Sbox, sBox, 256);
	memcpy(InvSbox, invsBox, 256);
	_key_expansion(key, w);
}

AES_128::~AES_128()
{

}

const std::string
AES_128::encrypt(const char* input)
{
	unsigned char state[4][4];
	std::string res = input;
	char * p = const_cast<char *>(res.c_str());
	int i, r, c;

	for (r = 0; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			state[r][c] = input[c * 4 + r];
		}
	}

	_add_round_key(state, w[0]);

	for (i = 1; i <= 10; i++)
	{
		_sub_bytes(state);
		_shift_rows(state);
		if (i != 10)_mix_columns(state);
		_add_round_key(state, w[i]);
	}

	for (r = 0; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			p[c * 4 + r] = state[r][c];
		}
	}

	return res;
}

const std::string 
AES_128::decrypt(const char* input)
{
	unsigned char state[4][4];
	std::string res = input;
	char * p = const_cast<char *>(res.c_str());
	int i, r, c;

	for (r = 0; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			state[r][c] = input[c * 4 + r];
		}
	}

	_add_round_key(state, w[10]);
	for (i = 9; i >= 0; i--)
	{
		_inv_shift_rows(state);
		_inv_sub_bytes(state);
		_add_round_key(state, w[i]);
		if (i)
		{
			_inv_mix_columns(state);
		}
	}

	for (r = 0; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			p[c * 4 + r] = state[r][c];
		}
	}

	return res;
}


void*
AES_128::encrypt(void* input, int length)
{
	unsigned char* in = (unsigned char*)input;
	int i;
	if (!length)
	{
		while (*(in + length++));
		in = (unsigned char*)input;
	}
	for (i = 0; i < length; i += 16)
	{
		encrypt(in + i);
	}
	return input;
}


void* 
AES_128::decrypt(void* input, int length)
{
	char* in = (char*)input;
	int i;
	for (i = 0; i < length; i += 16)
	{
		decrypt(in + i);
	}
	return input;
}


void 
AES_128::_key_expansion(unsigned char* key, unsigned char w[][4][4])
{
	int i, j, r, c;
	unsigned char rc[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };
	for (r = 0; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			w[0][r][c] = key[r + c * 4];
		}
	}
	for (i = 1; i <= 10; i++)
	{
		for (j = 0; j < 4; j++)
		{
			unsigned char t[4];
			for (r = 0; r < 4; r++)
			{
				t[r] = j ? w[i][r][j - 1] : w[i - 1][r][3];
			}
			if (j == 0)
			{
				unsigned char temp = t[0];
				for (r = 0; r < 3; r++)
				{
					t[r] = Sbox[t[(r + 1) % 4]];
				}
				t[3] = Sbox[temp];
				t[0] ^= rc[i - 1];
			}
			for (r = 0; r < 4; r++)
			{
				w[i][r][j] = w[i - 1][r][j] ^ t[r];
			}
		}
	}
}


unsigned char 
AES_128::_ffmul(unsigned char a, unsigned char b)
{
	unsigned char bw[4];
	unsigned char res = 0;
	int i;
	bw[0] = b;
	for (i = 1; i < 4; i++)
	{
		bw[i] = bw[i - 1] << 1;
		if (bw[i - 1] & 0x80)
		{
			bw[i] ^= 0x1b;
		}
	}
	for (i = 0; i < 4; i++)
	{
		if ((a >> i) & 0x01)
		{
			res ^= bw[i];
		}
	}
	return res;
}


void 
AES_128::_sub_bytes(unsigned char state[][4])
{
	int r, c;
	for (r = 0; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			state[r][c] = Sbox[state[r][c]];
		}
	}
}


void 
AES_128::_shift_rows(unsigned char state[][4])
{
	unsigned char t[4];
	int r, c;
	for (r = 1; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			t[c] = state[r][(c + r) % 4];
		}
		for (c = 0; c < 4; c++)
		{
			state[r][c] = t[c];
		}
	}
}


void 
AES_128::_mix_columns(unsigned char state[][4])
{
	unsigned char t[4];
	int r, c;
	for (c = 0; c < 4; c++)
	{
		for (r = 0; r < 4; r++)
		{
			t[r] = state[r][c];
		}
		for (r = 0; r < 4; r++)
		{
			state[r][c] = _ffmul(0x02, t[r])
				^ _ffmul(0x03, t[(r + 1) % 4])
				^ _ffmul(0x01, t[(r + 2) % 4])
				^ _ffmul(0x01, t[(r + 3) % 4]);
		}
	}
}


void 
AES_128::_add_round_key(unsigned char state[][4], unsigned char k[][4])
{
	int r, c;
	for (c = 0; c < 4; c++)
	{
		for (r = 0; r < 4; r++)
		{
			state[r][c] ^= k[r][c];
		}
	}
}


void 
AES_128::_inv_sub_bytes(unsigned char state[][4])
{
	int r, c;
	for (r = 0; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			state[r][c] = InvSbox[state[r][c]];
		}
	}
}


void 
AES_128::_inv_shift_rows(unsigned char state[][4])
{
	unsigned char t[4];
	int r, c;
	for (r = 1; r < 4; r++)
	{
		for (c = 0; c < 4; c++)
		{
			t[c] = state[r][(c - r + 4) % 4];
		}
		for (c = 0; c < 4; c++)
		{
			state[r][c] = t[c];
		}
	}
}


void 
AES_128::_inv_mix_columns(unsigned char state[][4])
{
	unsigned char t[4];
	int r, c;
	for (c = 0; c < 4; c++)
	{
		for (r = 0; r < 4; r++)
		{
			t[r] = state[r][c];
		}
		for (r = 0; r < 4; r++)
		{
			state[r][c] = _ffmul(0x0e, t[r])
				^ _ffmul(0x0b, t[(r + 1) % 4])
				^ _ffmul(0x0d, t[(r + 2) % 4])
				^ _ffmul(0x09, t[(r + 3) % 4]);
		}
	}
}


const std::string 
AES_128::GetRandom128Key()
{
	std::string res;
	for (int i = 0; i < 16; i++) {
		res.push_back(std::rand() % 26 + 65);
	}	
	return res;
}
