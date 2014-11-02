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


// Расширение слова в 32 бита до 48 бит, используя матрицу Е


/*
 Проверка слабого ключа
 */
bool checkIsKeyWeak(const long& key)
{
	return (key == 0x0101010101010101)
		|| (key == 0xFEFEFEFEFEFEFEFE)
		|| (key == 0x1F1F1F1F1F1F1F1F)
		|| (key == 0xE0E0E0E0E0E0E0E0);
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




