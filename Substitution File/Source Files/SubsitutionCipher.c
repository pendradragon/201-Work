//custom header imports
#include "ObtainCipher.h"
#include "ObtainCipherText.h"


#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 1000

// Function to decrypt a single character using the substitution key
char decryptChar(char c, const char* key) {
    // Convert character to lowercase for case insensitivity
    c = tolower(c);

    // If the character is a letter, decrypt it
    if (c >= 'a' && c <= 'z') {
        // Find the position of the character in the key
        for (int i = 0; i < 26; i++) {
            if (key[i] == c) {
                return 'a' + i;  // Return the decrypted character
            }
        }
    }
    // If it's not a letter, return it as it is (spaces, punctuation, etc.)
    return c;
}

// Function to decrypt a ciphertext using a substitution cipher
void decrypt(const char* ciphertext, char* plaintext, const char* key) {
    int length = strlen(ciphertext);

    // Decrypt each character of the ciphertext
    for (int i = 0; i < length; i++) {
        plaintext[i] = decryptChar(ciphertext[i], key);
    }

    plaintext[length] = '\0';  // Null-terminate the plaintext string
}

int main() {
    char ciphertext[MAX_LENGTH] = returnCipherText();
    char key[26] = returnCipher();
    char plaintext[MAX_LENGTH];

    // Get the ciphertext input
    printf("Enter the ciphertext: ");
    fgets(ciphertext, MAX_LENGTH, stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';  // Remove newline character

    // Get the substitution key input
    printf("Enter the 26-character decryption key: ");
    fgets(key, 27, stdin);
    key[strcspn(key, "\n")] = '\0';  // Remove newline character

    // Ensure the key is 26 characters long
    if (strlen(key) != 26) {
        printf("Error: The key must be exactly 26 characters long.\n");
        return 1;
    }

    // Decrypt the ciphertext
    decrypt(ciphertext, plaintext, key);

    // Output the decrypted plaintext
    printf("Decrypted text: %s\n", plaintext);

    return 0;
}


int main() {
	//arrays used throughout the decryption
	char cipher[26] = returnCipher();
	char cipherText[100] = returnCipherText();
}