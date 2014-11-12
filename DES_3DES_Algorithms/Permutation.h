//
//  Permutation.h

#ifndef __DES_3DES_Algorithms__Permutation__
#define __DES_3DES_Algorithms__Permutation__

#include <stdio.h>

typedef unsigned long long DWord;

class Permutation {

public:
	Permutation (DWord sourceDigit);

public:
	void startPermutation();

	inline DWord getResult() {return result;}

private:
	void swapBits(char bitIndex);

private:
	DWord sourceDigit;
	DWord result;
};

#endif /* defined(__DES_3DES_Algorithms__Permutation__) */
