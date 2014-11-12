
#ifndef __DES_3DES_Algorithms__ExtendedWord__
#define __DES_3DES_Algorithms__ExtendedWord__

#include <stdio.h>

typedef unsigned long long DWord;

/*
 Принимает 64-битную последовательность
 Разбивает ее на 2 32-битных слова
 Выполняет расширение правого слова до 48 бит
 */
class ExtendedWord {
	
public:
	ExtendedWord();
	
	ExtendedWord(const DWord &sourceDWord);
	
	long extension32To48();
	
	inline DWord getExtendedWord() { return extendedRightWord; }
	
private:
	
	int leftWord;
	int rightWord;
	DWord extendedRightWord;
};

#endif /* defined(__DES_3DES_Algorithms__ExtendedWord__) */
