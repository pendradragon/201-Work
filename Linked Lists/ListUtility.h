#include "node.h"

typedef struct LinkedList {
	int size;
	struct Node* head;
} LinkedList;

//functions that should be written in the c file -- writing them all here so I don't forget to do all of them 
void intialize(struct LinkedList* list) {};

//from the lecture videos
void prepend(struct LinkedList* list, int data) {};
void printList(struct LinkedList* list) {};

//need to be written for the assignment and turned in

void append(struct LinkedList* list, int value) {
	//should add a value to the BACK of the linked list with the value that was saved in parameter
};

void deleteNode(struct LinkedList* list, int nodeValue) {
	//Deletes the first node who's value matches the nodeValue parameter
};

void deleteAt(struct LinkedList* list, int index) {
	//deletes the node at the speified index
};

void wipeList(struct LinkedList* list) {
	//wipes all of the values from the list -- if the list is called again, all it should return is NULL
}