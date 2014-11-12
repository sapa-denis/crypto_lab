//  ExtendedWord.cpp


#include "ExtendedWord.h"
#include "PermutationTables.h"

ExtendedWord::ExtendedWord()
	: leftWord(0)
	, rightWord(0)
{
}

ExtendedWord::ExtendedWord(const DWord &sourceDWord)
{
	rightWord = (sourceDWord & 0x00000000FFFFFFFF);
	leftWord =	(sourceDWord & 0xFFFFFFFF00000000) >> 32;
}

long ExtendedWord::extension32To48()
{
	long extendedWord = 0;
	for (int i = 0; i < 48; ++i) {
		extendedWord <<= 1;
		extendedWord |= (rightWord >> E[i]) & 1;
	}
	return extendedRightWord = extendedWord;
}