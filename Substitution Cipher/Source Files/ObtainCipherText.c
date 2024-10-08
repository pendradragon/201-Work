#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//header import
#include "ObtainCipherText.h"

//int cipherTextSize;
char ciperText[cipherTextSize];

typedef struct {
	char filePath[260];
}FilePath;

void getUserInput(FilePath* storage) {
	//input for the path to the cipherText
	fprint("Insert the path to the ciphetext:\t");
	fgets(storage->filePath, 260, stdin);
}

void setCipherText(FilePath* path) {
	FILE* file = fopen(path->filePath, "r"); //opens the file in read mode

	char charFromFile;
	int index = 0;

	while ((charFromFile = fgetc(file)) != EOF && index < 100) { //EOF = End of File
		cipherText[index] = charFromFile;
	}

	fclose(file);
	}

char* returnCipherText() {
	FilePath cipherFilePath;

	obtainFilePath(&cipherFilePath);
	setCipher(&cipherFilePath);

	return cipherText;
}