//
//  Permutation.cpp
//  DES_3DES_Algorithms
//
//  Created by Sapa Denys on 02.11.14.
//  Copyright (c) 2014 Sapa Denys. All rights reserved.
//

#include "Permutation.h"

void Permutation::swapBits(long long &source, const short &firstBitIndex, const short &secondBitIndex )
{
	long long mask = (1 << secondBitIndex) | (1 << firstBitIndex); // маска позиций изменяемых битов
	
	char firstBit	= (source >> firstBitIndex) & 1;	// выделяем значение первого бита
	char secondBit	= (source >> secondBitIndex) & 1;	// выделяем значение второго бита
	
	source = (source & ~mask)		// значения битов в заданных индексах стают = 0, остальные - сохраняют свои зн-ния
	| (firstBit << secondBitIndex)	// помещаем значение первого бита на место второго
	| (secondBit << firstBitIndex); // помещаем значение второго бита на место первого
	// объеденяем полученные результаты.
}

//Начальная перестановка, матрица IP.
void Permutation::startPermutation(long long &sourceDigit)
{
	swapBits(sourceDigit, 0, 57);
	swapBits(sourceDigit, 1, 49);
	swapBits(sourceDigit, 2, 41);
	swapBits(sourceDigit, 3, 33);
	swapBits(sourceDigit, 4, 25);
	swapBits(sourceDigit, 5, 17);
	swapBits(sourceDigit, 6, 9);
	swapBits(sourceDigit, 7, 1);
	
	swapBits(sourceDigit, 8, 59);
	swapBits(sourceDigit, 9, 51);
	swapBits(sourceDigit, 10, 43);
	swapBits(sourceDigit, 11, 35);
	swapBits(sourceDigit, 12, 27);
	swapBits(sourceDigit, 13, 19);
	swapBits(sourceDigit, 14, 11);
	swapBits(sourceDigit, 15, 3);
	
	swapBits(sourceDigit, 16, 61);
	swapBits(sourceDigit, 17, 53);
	swapBits(sourceDigit, 18, 45);
	swapBits(sourceDigit, 19, 37);
	swapBits(sourceDigit, 20, 29);
	swapBits(sourceDigit, 21, 21);
	swapBits(sourceDigit, 22, 13);
	swapBits(sourceDigit, 23, 5);
	
	swapBits(sourceDigit, 24, 63);
	swapBits(sourceDigit, 25, 55);
	swapBits(sourceDigit, 26, 47);
	swapBits(sourceDigit, 27, 39);
	swapBits(sourceDigit, 28, 31);
	swapBits(sourceDigit, 29, 23);
	swapBits(sourceDigit, 30, 15);
	swapBits(sourceDigit, 31, 7);
	
	swapBits(sourceDigit, 32, 56);
	swapBits(sourceDigit, 33, 48);
	swapBits(sourceDigit, 34, 40);
	swapBits(sourceDigit, 35, 32);
	swapBits(sourceDigit, 36, 24);
	swapBits(sourceDigit, 37, 16);
	swapBits(sourceDigit, 38, 8);
	swapBits(sourceDigit, 39, 0);
	
	swapBits(sourceDigit, 40, 58);
	swapBits(sourceDigit, 41, 50);
	swapBits(sourceDigit, 42, 42);
	swapBits(sourceDigit, 43, 34);
	swapBits(sourceDigit, 44, 26);
	swapBits(sourceDigit, 45, 18);
	swapBits(sourceDigit, 46, 10);
	swapBits(sourceDigit, 47, 2);
	
	swapBits(sourceDigit, 48, 60);
	swapBits(sourceDigit, 49, 52);
	swapBits(sourceDigit, 50, 44);
	swapBits(sourceDigit, 51, 36);
	swapBits(sourceDigit, 52, 28);
	swapBits(sourceDigit, 53, 20);
	swapBits(sourceDigit, 54, 12);
	swapBits(sourceDigit, 55, 4);
	
	swapBits(sourceDigit, 56, 62);
	swapBits(sourceDigit, 57, 54);
	swapBits(sourceDigit, 58, 46);
	swapBits(sourceDigit, 59, 38);
	swapBits(sourceDigit, 60, 30);
	swapBits(sourceDigit, 61, 22);
	swapBits(sourceDigit, 62, 14);
	swapBits(sourceDigit, 63, 6);
}
