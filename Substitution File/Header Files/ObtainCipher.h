#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//global variable that can be used throughout the program
extern char cipher[26];

typedef struct {
	char cipherFilePath[260];
}FilePath;

void obtainFilePath(FilePath* storage);

void setCipher(FilePath* path);

//the only function that will be used by the cipher program
char* returnCipher();
