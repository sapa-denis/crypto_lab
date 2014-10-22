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

int main(int argc, const char * argv[])
{
	string inputString = "Hi, Jul!";
//	bitset<64> bitMask (inputString);
	
	long long foo = inputString[0];

//	cout << inputString.length() << endl;
	
	
	// превращаем 8 символов в последовательность битов. Для хранения используется long long
	foo <<= 8;
	foo |= inputString[1];
	foo <<= 8;
	foo |= inputString[2];
	foo <<= 8;
	foo |= inputString[3];
	foo <<= 8;
	foo |= inputString[4];
	foo <<= 8;
	foo |= inputString[5];
	foo <<= 8;
	foo |= inputString[6];
	foo <<= 8;
	foo |= inputString[7];
	
	cout << bitset<64>(foo) << "++" << endl;
	int r = foo;
	
	
	
	swapBits(foo, 1, 5);
	
	std::cout << std::bitset<64>(foo) << "~~~" << std::endl;
	
//	cout << bitset<32>(r) << endl;
	int l = foo >>= 32;
	cout << bitset<32>(l) << endl << bitset<32>(r) << endl;
	
	
	
    return 0;
}


