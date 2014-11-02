//
//  ExtendedWord.h
//  DES_3DES_Algorithms
//
//  Created by Sapa Denys on 02.11.14.
//  Copyright (c) 2014 Sapa Denys. All rights reserved.
//
#ifndef __DES_3DES_Algorithms__ExtendedWord__
#define __DES_3DES_Algorithms__ExtendedWord__

#include <stdio.h>
/*
 Принимает 64-битную последовательность
 Разбивает ее на 2 32-битных слова
 Выполняет расширение правого слова до 48 бит
 */
class ExtendedWord {
	
public:
	ExtendedWord();
	
	ExtendedWord(const long long &sourceDWord);
	virtual ~ExtendedWord();
	
	long extension32To48();
	
	inline long getExtendedWord() { return extendedRightWord; }
	
private:
	
	int leftWord;
	int rightWord;
	long extendedRightWord;
};

#endif /* defined(__DES_3DES_Algorithms__ExtendedWord__) */
