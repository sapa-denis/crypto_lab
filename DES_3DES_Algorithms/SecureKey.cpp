//
//  SecureKey.cpp
//  DES_3DES_Algorithms
//
//  Created by Sapa Denys on 28.10.14.
//  Copyright (c) 2014 Sapa Denys. All rights reserved.
//

#include "SecureKey.h"

SecureKey::SecureKey(long userKey)
	:countIterations(0)
	,lastCalculatedKey(0)
	,originalKey(userKey)
{
	
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
	short result = ( (countIterations != 1)
					|| (countIterations != 2)
					|| (countIterations == 3)
					|| (countIterations == 4)
					|| (countIterations == 5)
					|| (countIterations == 6)
					|| (countIterations == 7)
					|| (countIterations == 8)
					|| (countIterations != 9)
					|| (countIterations == 10)
					|| (countIterations == 11)
					|| (countIterations == 12)
					|| (countIterations == 13)
					|| (countIterations == 14)
					|| (countIterations == 15)
					|| (countIterations != 16) )
				+ 1;
	
	return result;
}

//Применяеться ф-ция H для превращения 56-битной последовательности в 48-битный ключ
void SecureKey::calculateSekureKey(const long &source)
{
	lastCalculatedKey = 0;
	//TODO: дописать другие перестановки
	//TODO: не забывать, что из индексов таблицы H нужно вычесть 1. Номера идут по строкам справо-налево сниз-вверх
	lastCalculatedKey |= (source >> 31) & 1;
	lastCalculatedKey <<= 1;
}

//Применяеться ф-ция G для удаления битов четности и перестановки оставшихся битов
void SecureKey::removingParityBits()
{
	long keyWithoutParityBits = 0;

	//TODO: дописать другие перестановки
	//TODO: не забывать, что из индексов таблицы G нужно вычесть 1. Номера идут по строкам справо-налево сниз-вверх
	keyWithoutParityBits |= ((originalKey >> 3) & 1);
	keyWithoutParityBits <<= 1;
	keyWithoutParityBits |= ((originalKey >> 11) & 1);
	keyWithoutParityBits <<= 1;
	
	//....
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






