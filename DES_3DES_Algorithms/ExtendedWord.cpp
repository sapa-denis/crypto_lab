//
//  ExtendedWord.cpp
//  DES_3DES_Algorithms
//
//  Created by Sapa Denys on 02.11.14.
//  Copyright (c) 2014 Sapa Denys. All rights reserved.
//

#include "ExtendedWord.h"

//const int E[48] = {0, 31, 30 ...};

ExtendedWord::ExtendedWord()
	: leftWord(0)
	, rightWord(0)
{
}

ExtendedWord::ExtendedWord(const long long &sourceDWord)
{
	rightWord = (sourceDWord & 0x00000000FFFFFFFF);
	leftWord =	(sourceDWord & 0xFFFFFFFF00000000) >> 32;
}

ExtendedWord::~ExtendedWord()
{
}

long ExtendedWord::extension32To48()
{
	long extendedWord = 0;
	
	extendedWord |= ((rightWord >> 0) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 31) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 30) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 29) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 28) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 27) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 28) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 27) & 1);
	extendedWord <<= 1;
	
	extendedWord |= ((rightWord >> 26) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 25) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 24) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 23) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 24) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 23) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 22) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 21) & 1);
	extendedWord <<= 1;
	
	extendedWord |= ((rightWord >> 20) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 19) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 20) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 19) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 18) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 17) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 16) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 15) & 1);
	extendedWord <<= 1;
	
	extendedWord |= ((rightWord >> 16) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 15) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 14) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 13) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 12) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 11) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 12) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 11) & 1);
	extendedWord <<= 1;
	
	extendedWord |= ((rightWord >> 10) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 9) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 8) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 7) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 8) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 7) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 6) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 5) & 1);
	extendedWord <<= 1;
	
	extendedWord |= ((rightWord >> 4) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 3) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 4) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 3) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 2) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 1) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 0) & 1);
	extendedWord <<= 1;
	extendedWord |= ((rightWord >> 31) & 1);
	
	return extendedRightWord = extendedWord;
}