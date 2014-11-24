//
//  SecureKey.cpp


#include "SecureKey.h"
#include "PermutationTables.h"

const int bitsShifting[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };


SecureKey::SecureKey(long userKey):
	countIterations(0),
	lastCalculatedKey(0),
	originalKey(userKey)
{
	removingParityBits();
}

long SecureKey::getSecureKeyForNextIteration()
{
	++countIterations;
	
	short shiftedBits = getShiftingBits();
	
	leftShifting(shiftedBits);
	
	long concatinaitedBlocks = (BlockD << 28) | BlockC;
	calculateSecureKey(concatinaitedBlocks);
	
	return lastCalculatedKey;
}

short SecureKey::getShiftingBits()
{
	return bitsShifting[countIterations-1];
}

//Применяется ф-ция H для превращения 56-битной последовательности в 48-битный ключ
void SecureKey::calculateSecureKey(const DWord &source)
{
	lastCalculatedKey = 0;
	for (int i = 0; i < 48; ++i) {
		lastCalculatedKey <<= 1;
		lastCalculatedKey |= (source >> H[i]) & 1;
	}
}

//Применяется ф-ция G для удаления битов четности и перестановки оставшихся битов
void SecureKey::removingParityBits()
{
	long keyWithoutParityBits = 0;
	for (int i = 0; i < 48; ++i) {
		keyWithoutParityBits <<= 1;
		keyWithoutParityBits |= (originalKey >> G[i]) & 1;
	}

	BlockC = (keyWithoutParityBits & 0xFFFFFFF);
	BlockD = (keyWithoutParityBits & 0xFFFFFFF0000000) >> 28;

}

//Циклический сдвиг блоков C и D на bits (1 || 2) битов влево
void SecureKey::leftShifting(char bits)
{
	int mask = (1 | bits) << (28 - bits);
	short bit = (BlockC & mask) >> (28 - bits);
	BlockC <<= bits;
	BlockC &= 0x0FFFFFFF;
	BlockC |= bit;
	
	bit = (BlockD & mask) >> (28 - bits);
	BlockD <<= bits;
	BlockD &= 0x0FFFFFFF;
	BlockD |= bit;
}

/*
 Проверка слабого ключа
 */
bool SecureKey::checkIsKeyWeak(const long& key)
{
	return (key == 0x0101010101010101)
		|| (key == 0xFEFEFEFEFEFEFEFE)
		|| (key == 0x1F1F1F1F1F1F1F1F)
		|| (key == 0xE0E0E0E0E0E0E0E0);
}
