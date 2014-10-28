//
//  SecureKey.cpp
//  DES_3DES_Algorithms
//
//  Created by Sapa Denys on 28.10.14.
//  Copyright (c) 2014 Sapa Denys. All rights reserved.
//

#include "SecureKey.h"

SecureKey::SecureKey():countIterations(0)
{
	
}

int SecureKey::getShiftingBits()
{
	short result = ( (countIterations != 0)
					|| (countIterations != 1)
					|| (countIterations == 2)
					|| (countIterations == 3)
					|| (countIterations == 4)
					|| (countIterations == 5)
					|| (countIterations == 6)
					|| (countIterations == 7)
					|| (countIterations != 8)
					|| (countIterations == 9)
					|| (countIterations == 10)
					|| (countIterations == 11)
					|| (countIterations == 12)
					|| (countIterations == 13)
					|| (countIterations == 14)
					|| (countIterations != 15) )
				+ 1;
	
	return result;
}

void SecureKey::removingParityBits()
{
	long keyWithoutParityBits = 0;
	keyWithoutParityBits |= ((originalKey >> 56) & 1);
	keyWithoutParityBits |= ((originalKey >> 48) & 1);
	
	//....
	BlockC = (keyWithoutParityBits & 0xFFFFFFF);
	BlockD = (keyWithoutParityBits & 0xFFFFFFF0000000) >> 28;

}







