//
//  Permutation.cpp

#include "stdafx.h"
#include "Permutation.h"

/*void Permutation::swapBits(long long &source, const short &firstBitIndex, const short &secondBitIndex )
{
	long long mask = (1 << secondBitIndex) | (1 << firstBitIndex); // маска позиций изменяемых битов
	
	char firstBit	= (source >> firstBitIndex) & 1;	// выделяем значение первого бита
	char secondBit	= (source >> secondBitIndex) & 1;	// выделяем значение второго бита
	
	source = (source & ~mask)		// значения битов в заданных индексах стают = 0, остальные - сохраняют свои зн-ния
	| (firstBit << secondBitIndex)	// помещаем значение первого бита на место второго
	| (secondBit << firstBitIndex); // помещаем значение второго бита на место первого
	// объеденяем полученные результаты.
}*/

Permutation::Permutation(DWord aSourceDigit)
	:sourceDigit(aSourceDigit)
	,result(0)
{
}


void Permutation::swapBits(char bitIndex)
{
	result = (result << 1) | ((sourceDigit >> bitIndex) & 1);
}

//Начальная перестановка, матрица IP.
void Permutation::startPermutation()
{
	swapBits(62);
	swapBits(54);
	swapBits(46);
	swapBits(38);
	swapBits(30);
	swapBits(22);
	swapBits(14);
	swapBits(6);

	swapBits(60);
	swapBits(52);
	swapBits(44);
	swapBits(36);
	swapBits(28);
	swapBits(20);
	swapBits(12);
	swapBits(4);

	swapBits(58);
	swapBits(50);
	swapBits(42);
	swapBits(34);
	swapBits(26);
	swapBits(18);
	swapBits(10);
	swapBits(2);

	swapBits(56);
	swapBits(48);
	swapBits(40);
	swapBits(32);
	swapBits(24);
	swapBits(16);
	swapBits(8);
	swapBits(0);

	swapBits(63);
	swapBits(55);
	swapBits(47);
	swapBits(39);
	swapBits(31);
	swapBits(23);
	swapBits(15);
	swapBits(7);

	swapBits(61);
	swapBits(53);
	swapBits(45);
	swapBits(37);
	swapBits(29);
	swapBits(21);
	swapBits(13);
	swapBits(5);

	swapBits(59);
	swapBits(51);
	swapBits(43);
	swapBits(35);
	swapBits(27);
	swapBits(19);
	swapBits(11);
	swapBits(3);

	swapBits(57);
	swapBits(49);
	swapBits(41);
	swapBits(33);
	swapBits(25);
	swapBits(17);
	swapBits(9);
	swapBits(1);
	
}
