//
//  Permutation.h
//  DES_3DES_Algorithms
//
//  Created by Sapa Denys on 02.11.14.
//  Copyright (c) 2014 Sapa Denys. All rights reserved.
//

#ifndef __DES_3DES_Algorithms__Permutation__
#define __DES_3DES_Algorithms__Permutation__

#include <stdio.h>

class Permutation {


public:
	static void startPermutation(long long &sourceDigit);

private:
	static void swapBits(long long &source, const short &firstBitIndex, const short &secondBitIndex);
};

#endif /* defined(__DES_3DES_Algorithms__Permutation__) */
