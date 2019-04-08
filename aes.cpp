
//	************************************************************
//	*  aes.cpp : implementation program for the AES algorithm  *
//	************************************************************

//#include "stdafx.h"
#include "aes.h"
#include "string.h"
#include <stdio.h>

//	====================================================================

//	subrutine                             

//	--------------------------------------------------------------------

uint32 SubWord(uint32 T)
{
	// returns an unsigned int as result of applying the sandbox to T
	uint8 temp[4];
	PUT_UINT32(T, temp, 0);
	for (int i = 0; i < 4; i++) temp[i] = SBOX[temp[i]];
	GET_UINT32(T, temp, 0);
	return T;
}

//	--------------------------------------------------------------------

uint32 RotWord(uint32 T)
{
	// returns the rotated unsigned int T
	return (T << 8) | ((T & 0xFFFFFFFF) >> 24);
}

//	--------------------------------------------------------------------

void KeyExpansion(uint8 key[], uint32 W[], int NK)
{
	// fill out with NK keys the array W[] starting with the key key[]
	uint32 temp, int i;
	for (i = 0; i < NK; i++) GET_UINT32(W[i], key, 4 * i);
	for (i = NK; i < NB*(NK + 7); i++)
	{
		temp = W[i - 1];
		if (i%NK) temp = SubWord(RotWord(temp)) ^ RCON[i / NK];
		else if ((NK > 6) && (i % NK == 4)) temp = SubWord(temp);
		W[i] = W[i - NK] ^ temp;
	}
}

//	--------------------------------------------------------------------


//Multiplication by x = { 02 }
//if (deg(aa)) = 7 = > x ^ 8 = x ^ 4 + x ^ 3 + x + 1 = 00011011 = 0x1B; 2 ^ 7 = x ^ 7 = 10000000 = 0x80

uint8 xtime(uint8 aa)
{
	// returns the result of multiplying the byte aa by x
	if (aa & 0x80) return (aa << 1) ^ 0x1B;
	return aa << 1;
}

//	--------------------------------------------------------------------

void init_xtim()
{
	// fill out the array XTIME[256][8]
	for (int i = 0; i < 256; i++) XTIM[1][0] = i;
	for (int k = 0; k < 8; k++)
		for (int i = 0; i < 256; i++) XTIM[i][k] = xtime(XTIM[i][k - 1]);
}

//	--------------------------------------------------------------------

uint8 dot(uint8 aa, uint8 bb)
{
	// returns the result of the dot operation aa * bb 

	int index = 1, i, uint8 cc = 0;
	for (int j = 0; j < 8; j++)
	{
		if (index & bb) cc = cc ^ XTIM[aa][i];
		index *= 2;
	}

	return cc;
}

//	--------------------------------------------------------------------

void init_state(uint8 in[], uint8 stat[])
{
	// initializes stat[] with the values from in[]

	for (int i = 0; i < NB * 4; i++) stat[i] = in[i];
}

//	--------------------------------------------------------------------

void display_b(uint8 stat[])
{
	for (int j = 0; j < NB * 4; j++)
	{
		printf("%3x", stat[j]);
	}
	printf("\n");
}

//	--------------------------------------------------------------------

void AddRoundKey(uint8 stat[], uint32 W[], int round)
{
	// add the key from W[] for the round round in stat[]

	int i, uint8 temp[4];
	for (i = 0; i < NB; i++) {
		PUT_UINT32(W[NB*round + i], temp, 0);
		for (int k = 0; k < 4; k++) stat[i * 4 + k] ^= temp[k];
	}
}

//	--------------------------------------------------------------------

void SubBytes(uint8 stat[])
{
	// aply the sandbox 


}

//	--------------------------------------------------------------------

void ShiftRows(uint8 stat[])
{
	// shift the lines in stat[] according to the AES specification
}

//	--------------------------------------------------------------------

void MixColumns(uint8 stat[])
{
	// mix the columns in stat[] according to the AES specification
}

//	--------------------------------------------------------------------

void InvSubBytes(uint8 stat[])
{
	// apply the inverse sandbox 
}

//	--------------------------------------------------------------------

void InvShiftRows(uint8 stat[])
{
	// shift the lines in stat[] according to the AES specification
}

//	--------------------------------------------------------------------

void InvMixColumns(uint8 stat[])
{
	// mix the columns in stat[] according to the AES specification
}

//	====================================================================

int main(int argc, char* argv[])
{
	int i, NK[3];
	uint8 state[NB * 4];
	uint32 WW[NB * (1 + MAX_NUM_ROUNDS)];

	//	----------------------------------------------------------------
	//	init phase
	//	----------------------------------------------------------------

	static uint8 key4[16] =
	{
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
	};

	static uint8 key6[24] =
	{
		0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52,
		0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5,
		0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b
	};

	static uint8 key8[32] =
	{
		0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
		0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
		0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
		0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
	};

	init_xtim();

	//	----------------------------------------------------------------

	//	APPENDIX A - key schedules for 128, 192, and 256 bit keys

	for (int ii = 2; ii >= 0; --ii)
	{
		NK[ii] = 4 + ii * 2;
		NR = NK[ii] + 6;

		if (ii == 0)
			KeyExpansion(key4, WW, NK[ii]);
		else if (ii == 1)
			KeyExpansion(key6, WW, NK[ii]);
		else
			KeyExpansion(key8, WW, NK[ii]);
	}

	//	----------------------------------------------------------------

	//	APPENDIX B - cipher phase for test_appb - 128 bits key from key4

	NR = NK[0] + 6;
	init_state(test_appb, state);
	display_b(state);

	AddRoundKey(state, WW, 0);	// WW contains the key schedule for key4

	for (i = 1; i < NR; i++)
	{
		SubBytes(state);
		display_b(state);
		ShiftRows(state);
		display_b(state);
		MixColumns(state);
		display_b(state);
		AddRoundKey(state, WW, i);
		display_b(state);
	}

	SubBytes(state);
	display_b(state);
	ShiftRows(state);
	display_b(state);
	AddRoundKey(state, WW, NR);

	display_b(state);

	//	----------------------------------------------------------------

	//	APPENDIX C - cipher, decipher phase for test_appc using all keys

	for (int ii = 0; ii < 3; ii++)
	{
		NR = NK[ii] + 6;
		KeyExpansion(keys_appc, WW, NK[ii]);

		//	cipher phase - like APPENDIX B, from init_state() to the end

		//	decipher phase (inverse cipher only) - similar to APPENDIX B
	}

	return 0;
}

//	********************************************************************

