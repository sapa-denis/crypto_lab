//
//  SecureKey.h
//  DES_3DES_Algorithms
//
//  Created by Sapa Denys on 28.10.14.
//  Copyright (c) 2014 Sapa Denys. All rights reserved.
//

#ifndef __DES_3DES_Algorithms__SecureKey__
#define __DES_3DES_Algorithms__SecureKey__

#include <stdio.h>

class SecureKey {
public:
	SecureKey(long userKey);
	virtual ~SecureKey();

public:
	long getSecureKeyForNextIteration();
	
	
private:
	short getShiftingBits();
	void removingParityBits();
	void leftShifting(short bits);
	void calculateSekureKey(const long &source);
private:
	long originalKey;
	long lastCalculatedKey;
	
	int countIterations;
	
	int BlockC;
	int BlockD;
};

#endif /* defined(__DES_3DES_Algorithms__SecureKey__) */