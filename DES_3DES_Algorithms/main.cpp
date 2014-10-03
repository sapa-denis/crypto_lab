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


int main(int argc, const char * argv[])
{
	string inputString = "Hello, World!";
//	bitset<64> bitMask (inputString);
	
	long long foo = inputString[1];

	cout << bitset<64>(foo) << endl << CHAR_BIT << endl;
	
    return 0;
}


bitset<64> stringToBitset(char *data)
{
	bitset<64> resultBitset;
	
	int numberOfBytes = 64 / CHAR_BIT;
	
	for (int i = 0; i < numberOfBytes; ++i) {
		int offset = i *CHAR_BIT;
		bitset<CHAR_BIT> symbol (data[i]);
		
//		for (; symbol; symbol >>=symbol) {
//			<#statements#>
//		}
		
	}
	
	return 0;
}