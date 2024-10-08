#include "ObtainCipher.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//global variable that can be used throughout the program
extern char cipher[26];

typedef struct {
	char cipherFilePath[260];
}FilePath;

static void obtainFilePath(FilePath* storage) {
	//user input
	printf("Enter the path to your cipher:\t");
	fgets(storage->cipherFilePath, 260, stdin);
}

static void setCipher(FilePath* path) {
	FILE* file = fopen(path->cipherFilePath, "r"); //opens the file in read mode

	char charFromFile;
	int index = 0;

	while ((charFromFile = fgetc(file)) != EOF && index < 25) { //EOF = End of File
		cipher[index] = charFromFile;
	}

	fclose(file);
}

//the only function that will be used by the cipher program
char* returnCipher() {
	FilePath cipherFilePath;

	obtainFilePath(&cipherFilePath);
	setCipher(&cipherFilePath);

	return cipher;
}