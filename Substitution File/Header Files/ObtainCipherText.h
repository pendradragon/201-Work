#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//global variables

extern cipherTextSize;
extern char cipherText[cipherTextSize];

typedef struct {
	char cipherTextFilePath[260];
}FilePath;

void getUserInput(FilePath* storage) {};
void setCipherText(FilePath* path) {};
char* returnCipherText() {};