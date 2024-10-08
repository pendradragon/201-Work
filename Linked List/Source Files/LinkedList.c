#include <stdio.h>
#include <stdlib.h>

//structures used throughout the program 
typedef struct Node {
	int data;
	struct Node* next;
}Node;

typedef struct LinkedList {
	int size;
	struct Node* head;
}list;

void initialize(struct LinkedList* list) {
	list->size = 0;
	list->head = NULL;
}

void prepend(struct LinkedList* list, int value) {
	//makes a new node using the node defined in the header file
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	//adds the data to the node
	newNode->data = value;
	newNode->next = list->head;
	list->head = newNode;
	list->size++;
}

void printList(struct LinkedList* list) {
	struct Node* temp = list->head;
	while (temp != NULL) {
		printf("%d->", temp->data);
		temp = temp->next;
	}
	printf("\n");
}

void append(struct LinkedList* list, int value) {
	//makes a new node
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data=value;
	newNode->next = NULL;

	//if the head of the list is empty, make the head of the list the new node
	if (list->head == NULL) {
		list->head = newNode;
	}
	else {
		//traverses throught the end of the list
		struct Node* current = list->head;
		while (current->next != NULL) {
			current = current->next;
		}
		//adds the new node at the end of the list
		current->next = newNode;
		list->size++;
	}
}

void deleteNode(struct LinkedList* list, int nodeValue) {
	//temporary variables used to traverse the list
	struct Node* current = list->head;
	struct Node* previousNode = NULL;

	//if the head node is the value set to be deleted
	if (current == list->head && current->data == nodeValue){
		list->head = current->next; //changes the first node in the list
		free(current); //frees the memory that is being used to store the temporary variable
		return;
	}

	//searches for the node to be deleted
	while (current != NULL && current->data == nodeValue){
		previousNode = current;
		current = current->next;

		previousNode->next = current->next;
		list->size--;
	}

	//if the value to be deleted is not in the list
	if (current == NULL) {
		printf("Value %d was not found in the list.\n", nodeValue);
		return;
	}

	//free whatever memory is still in use 
	free(current);
}

void deleteAt(struct LinkedList* list, int index) {
	//if there are no values in the list to be deleted
	if (list->head == NULL) {
		printf("There are no values to be deleted in the list.\n");
		return;
	}

	//if the index is larger than the size of the list
	if (list->size < index) {
		printf("Index %d is out of range.\n", index);
		return;
	}
	
	//temporary variable go brrrr
	struct Node* current = list->head;

	//if the head node needs to be removed
	if (index == 0) {
		list->head = current->next;
		free(current);
		return;
	}

	//looping through the list
	struct Node* previousNode = NULL;
	for (int i = 0; current != NULL && i < index; i++) {
		previousNode = current;
		current = current->next;
	}

	//unlinks the node to be removed from the linked list
	previousNode->next = current->next;
	list->size--;

	//free up the memory for the node to be deleted
	free(current);
}

void wipeList(struct LinkedList* list) {
	struct Node* current = list->head;
	struct Node* next = NULL;

	//traverse the list and free each node from memory and set each node to NULL
	while (current != NULL) {
		next = current->next; //store the next node
		free(current); //free the current node
		current = next; //move on to the next node
	}

	//after the entire list is wiped, set the head of the list to NULL
	list->head = NULL;
}

int main() {
	//making the list
	struct LinkedList* list = (struct LinkedList*)malloc(sizeof(struct LinkedList));
	initialize(list);

	//appending values to the list
	append(list, 10);
	append(list, 20);
	append(list, 30);
	append(list, 40);
	append(list, 50);
	//printing for debugging
	printf("List elements:\t");
	printList(list);
	printf("Size of List:%d\n", list->size);

	//delete node 
	deleteNode(list, 30);
	printf("List of elements:\t");
	printList(list);
	printf("Size of List: %d\n", list->size);

	//delete at
	deleteAt(list, 2);
	printf("List of elements:\t");
	printList(list);
	printf("Size of List: %d\n", list->size);

	//wipeList
	wipeList(list);
}