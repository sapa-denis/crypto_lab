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
	long long mask = (1 << second) | (1 << first);
	
	//	std::cout << std::bitset<64>(source) << std::endl;
	std::cout << std::bitset<64>(mask) << std::endl << std::endl;
	
	std::cout << std::bitset<18>( (source & mask) << (abs(second - first)) & mask) << std::endl;
	std::cout << std::bitset<18>((source >> abs(second - first)) & mask) << std::endl;
	
	std::cout << std::bitset<18>( (source & mask) << (abs(second - first)) | ((source >> abs(second - first)) & mask) ) << "-" << std::endl;
	
	std::cout << std::bitset<18>(source & ~(1 << (second) | mask)) << std::endl;
	
	std::cout << std::bitset<64>(1 << second | mask) << std::endl << std::endl;
	
	source = (((source & mask) << abs(second - first)) & mask)
			| ((source >> abs(second - first)) & mask)
			| (source & ~mask);
	
//	std::cout << std::bitset<64>(source) << "+" << std::endl;
	

}

void extension32To48(const int &word, long &extendedWord)
{
	extendedWord = 0;
	
	// Расширение слова в 32 бита до 48 бит, используя матрицу Е
	//TODO: дописать остальные 45 элементов для расширения.
	//TODO: не забывать, что из индексов таблицы Е нужно вычесть 1. Номера идут по строка слева-направо сверху-вниз
	extendedWord |= ((word >> 31) & 1);
	extendedWord |= ((word >> 00) & 1);
	extendedWord |= ((word >> 01) & 1);
}

bool chackWeakKey(const long& key)
{
	return !(	(key - 0x0101010101010101)
			 && (key - 0xFEFEFEFEFEFEFEFE)
			 && (key - 0x1F1F1F1F1F1F1F1F)
			 && (key - 0xE0E0E0E0E0E0E0E0)
			 );
}

int main(int argc, const char * argv[])
{
	string inputString = "Hi, Jul!";
	
	long long decimalStringPresentation = inputString[0];
	
	long key = 0xBADACCE551111111;
	
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

	//Начальная перестановка, матрица IP. Всего 64 вызова
	//TODO: дописать остальные 63 вызова перестановки.
	swapBits(decimalStringPresentation, 0, 58);
	
	
	//Разделение на левые и правые полуслова по 32 бита.
	int rigthPart = decimalStringPresentation;
	int leftPart = decimalStringPresentation >>= 32;
	cout << bitset<32>(leftPart) << endl << bitset<32>(rigthPart) << endl;
	
	
	
	
    return 0;
}




