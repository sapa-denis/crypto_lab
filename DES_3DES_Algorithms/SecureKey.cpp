//
//  SecureKey.cpp
//  DES_3DES_Algorithms
//
//  Created by Sapa Denys on 28.10.14.
//  Copyright (c) 2014 Sapa Denys. All rights reserved.
//

#include "SecureKey.h"

//const int G[48] = {3, 11, 19, ...};

//const int H[48] = {31, 26, 35, ...};

const int bitsShifting[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };


SecureKey::SecureKey(long userKey)
	:countIterations(0)
	,lastCalculatedKey(0)
	,originalKey(userKey)
{
	removingParityBits();
}

SecureKey::~SecureKey()
{
	
}

long SecureKey::getSecureKeyForNextIteration()
{
	++countIterations;
	
	short shiftedBits = getShiftingBits();
	
	leftShifting(shiftedBits);
	
	long concatinaitedBlocks = (BlockD << 28) | BlockC;
	calculateSekureKey(concatinaitedBlocks);
	
	return lastCalculatedKey;
}

short SecureKey::getShiftingBits()
{
	return bitsShifting[countIterations];
}

//Применяется ф-ция H для превращения 56-битной последовательности в 48-битный ключ
void SecureKey::calculateSekureKey(const long &source)
{
	lastCalculatedKey = 0;

// int H[48] = {...};
	for (int i = 0; i < 48; ++i) {
		lastCalculatedKey <<= 1;
		lastCalculatedKey |= (source >> H[i]) & 1;

	}
	

	
	// 31
	lastCalculatedKey |= (source >> 28) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 35) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 49) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 41) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 45) & 1;
	lastCalculatedKey <<= 1;
	
	lastCalculatedKey |= (source >> 52) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 33) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 55) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 38) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 48) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 43) & 1;
	lastCalculatedKey <<= 1;
	
	lastCalculatedKey |= (source >> 47) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 32) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 44) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 50) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 39) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 29) & 1;
	lastCalculatedKey <<= 1;
	
	lastCalculatedKey |= (source >> 54) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 46) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 36) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 30) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 51) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 40) & 1;
	lastCalculatedKey <<= 1;
	
	lastCalculatedKey |= (source >> 1) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 12) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 19) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 26) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 6) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 15) & 1;
	lastCalculatedKey <<= 1;
	
	lastCalculatedKey |= (source >> 7) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 25) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 3) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 11) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 18) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 22) & 1;
	lastCalculatedKey <<= 1;
	
	lastCalculatedKey |= (source >> 9) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 20) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 5) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 14) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 27) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 2) & 1;
	lastCalculatedKey <<= 1;
	
	lastCalculatedKey |= (source >> 4) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 0) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 23) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 10) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 16) & 1;
	lastCalculatedKey <<= 1;
	lastCalculatedKey |= (source >> 13) & 1;
}

//Применяется ф-ция G для удаления битов четности и перестановки оставшихся битов
void SecureKey::removingParityBits()
{
	long keyWithoutParityBits = 0;

	keyWithoutParityBits |= ((originalKey >> 3) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 11) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 19) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 27) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 4) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 12) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 20) & 1);
	keyWithoutParityBits <<= 1;
	
	keyWithoutParityBits |= ((originalKey >> 28) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 36) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 44) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 52) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 60) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 5) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 13) & 1);
	keyWithoutParityBits <<= 1;
	
	keyWithoutParityBits |= ((originalKey >> 21) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 29) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 37) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 45) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 53) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 61) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 6) & 1);
	keyWithoutParityBits <<= 1;
	
	keyWithoutParityBits |= ((originalKey >> 14) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 22) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 30) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 38) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 46) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 54) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 62) & 1);
	keyWithoutParityBits <<= 1;
	
	keyWithoutParityBits |= ((originalKey >> 35) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 43) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 51) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 59) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 2) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 10) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 18) & 1);
	keyWithoutParityBits <<= 1;
	
	keyWithoutParityBits |= ((originalKey >> 26) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 34) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 42) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 50) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 58) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 1) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 9) & 1);
	keyWithoutParityBits <<= 1;
	
	keyWithoutParityBits |= ((originalKey >> 17) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 25) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 33) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 41) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 49) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 57) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 0) & 1);
	keyWithoutParityBits <<= 1;
	
	keyWithoutParityBits |= ((originalKey >> 8) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 16) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 24) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 32) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 40) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 48) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 56) & 1);
	
	BlockC = (keyWithoutParityBits & 0xFFFFFFF);
	BlockD = (keyWithoutParityBits & 0xFFFFFFF0000000) >> 28;

}

//Циклический сдвиг блоков C и D на bits (1 || 2) битов влево
void SecureKey::leftShifting(short bits)
{
	int mask = (1 | bits) << (28 - bits);
	short bit = (BlockC & mask) >> (28 - bits);
	BlockC <<= bits;
	BlockC &= 0x0FFFFFFF;
	BlockC |= bit;
	
	bit = (BlockD & mask) >> (28 - bits);
	BlockD <<= bits;
	BlockD &= 0x0FFFFFFF;
	BlockD |= bit;
}






