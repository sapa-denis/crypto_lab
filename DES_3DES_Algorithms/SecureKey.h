//
//  SecureKey.h

#ifndef __DES_3DES_Algorithms__SecureKey__
#define __DES_3DES_Algorithms__SecureKey__

#include <stdio.h>

class SecureKey {
public:
	SecureKey(long userKey);

public:
	long getSecureKeyForNextIteration();
	
	
private:
	short getShiftingBits();
	void removingParityBits();
	void leftShifting(char bits);
	void calculateSecureKey(const DWord &source);
private:
	char countIterations;
	
	int BlockC;
	int BlockD;

	DWord originalKey;
	DWord lastCalculatedKey;
};

#endif /* defined(__DES_3DES_Algorithms__SecureKey__) */
