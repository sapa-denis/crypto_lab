//
//  main.cpp
//  DES_3DES_Algorithms
//
//  Created by Sapa Denys on 29.09.14.
//  Copyright (c) 2014 Sapa Denys. All rights reserved.
//

#include <iostream>
#include <string>
#include <bitset>

using namespace std;


bitset<64> stringToBitset(char *data)
{
	bitset<64> resultBitset;
	
	int numberOfBytes = 64 / CHAR_BIT;
	
	for (int i = 0; i < numberOfBytes; ++i) {
		int offset = i *CHAR_BIT;
		bitset<CHAR_BIT> symbol = (long)data[i];
		
		for (int j = 0; j < offset; ++j, ++offset) {
			resultBitset[offset] = symbol[j];
		}
		
	}
	
	return resultBitset;
}



void swapBits(long long &source, const short &first, const short &second )
{
	long long mask = (1 << second) | (1 << first); // маска позиций изменяемых битов
	
	char firstBit	= (source >> first) & 1;	// выделяем значение первого бита
	char secondBit	= (source >> second) & 1;	// выделяем значение второго бита
	
	source = (source & ~mask)		// значения битов в заданных индексах стают = 0, остальные - сохраняют свои зн-ния
			| (firstBit << second)	// помещаем значение первого бита на место второго
			| (secondBit << first); // помещаем значение второго бита на место первого
		// объеденяем полученные результаты.
}

//Начальная перестановка, матрица IP.
void startPermutation(long long &sourceDigit)
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

// Расширение слова в 32 бита до 48 бит, используя матрицу Е
void extension32To48(const int &word, long &extendedWord)
{
	extendedWord = 0;

	extendedWord |= ((word >> 0) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 31) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 30) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 29) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 28) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 27) & 1);
	extendedWord <<= 1;
	
	extendedWord |= ((word >> 28) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 27) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 26) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 25) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 24) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 23) & 1);
	extendedWord <<= 1;
	
	extendedWord |= ((word >> 24) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 23) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 22) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 21) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 20) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 19) & 1);
	extendedWord <<= 1;
	
	extendedWord |= ((word >> 20) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 19) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 18) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 17) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 16) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 15) & 1);
	extendedWord <<= 1;
	
	extendedWord |= ((word >> 16) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 15) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 14) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 13) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 12) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 11) & 1);
	extendedWord <<= 1;
	
	extendedWord |= ((word >> 12) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 11) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 10) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 9) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 8) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 7) & 1);
	extendedWord <<= 1;
	
	extendedWord |= ((word >> 8) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 7) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 6) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 5) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 4) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 3) & 1);
	extendedWord <<= 1;
	
	extendedWord |= ((word >> 4) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 3) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 2) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 1) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 0) & 1);
	extendedWord <<= 1;
	extendedWord |= ((word >> 31) & 1);
}

/*
 Проверка слабого ключа
 */
bool checkIsKeyWeak(const long& key)
{
	return (key == 0x0101010101010101)
		|| (key == 0xFEFEFEFEFEFEFEFE)
		|| (key == 0x1F1F1F1F1F1F1F1F)
		|| (key == 0xE0E0E0E0E0E0E0E0);
//	return !(	(key - 0x0101010101010101)
//			 && (key - 0xFEFEFEFEFEFEFEFE)
//			 && (key - 0x1F1F1F1F1F1F1F1F)
//			 && (key - 0xE0E0E0E0E0E0E0E0)
//			 );
}

int main(int argc, const char * argv[])
{
	string inputString = "Hi, Jul!";
	
	long long decimalStringPresentation = inputString[0];
	
	long key = 0xBADACCE551111111;
 //0xBADACCE551111111;
	
	cout << checkIsKeyWeak(key) << endl;
	
	// превращаем 8 символов в последовательность битов. Для хранения используется long long
	decimalStringPresentation <<= 8;
	decimalStringPresentation |= inputString[1];
	decimalStringPresentation <<= 8;
	decimalStringPresentation |= inputString[2];
	decimalStringPresentation <<= 8;
	decimalStringPresentation |= inputString[3];
	decimalStringPresentation <<= 8;
	decimalStringPresentation |= inputString[4];
	decimalStringPresentation <<= 8;
	decimalStringPresentation |= inputString[5];
	decimalStringPresentation <<= 8;
	decimalStringPresentation |= inputString[6];
	decimalStringPresentation <<= 8;
	decimalStringPresentation |= inputString[7];
	
	cout << bitset<64>(decimalStringPresentation) << "++" << endl;
	
	
	swapBits(decimalStringPresentation, 4, 0);
	cout << bitset<64>(decimalStringPresentation) << "++" << endl;
	
//	startPermutation(decimalStringPresentation);
	
	
	
	//Разделение на левые и правые слова по 32 бита.
	int rigthPart = decimalStringPresentation;
	int leftPart = decimalStringPresentation >>= 32;
	
	/*
	leftPart = (keyWithoutParityBits & 0xFFFFFFFF);
	leftPart = (keyWithoutParityBits & 0xFFFFFFFF00000000) >> 32;
	 */
	
	cout << bitset<32>(leftPart) << endl << bitset<32>(rigthPart) << endl;
	
	
	
	
    return 0;
}




