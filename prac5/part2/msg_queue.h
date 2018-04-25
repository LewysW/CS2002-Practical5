#ifndef MSG_QUEUE_H
#include <pthread.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define MSG_QUEUE_H


/* Interface for integer MQueue */
typedef struct node{
	int data;
	struct node *next;
} Node;

/* The multi-threaded version contain a mutex 'access */
typedef struct {
	Node *head;
	Node *tail;
	pthread_mutex_t access;
} MQueue;

//Provided function definitions
void send_msg( MQueue* s, int value ); // enqueue
Node *read_msg( MQueue* s );            // dequeue
void initMQueue(MQueue* s);
void printMQueue(MQueue* s);

//Additional function definitions
bool is_empty(MQueue* s);
MQueue* makeQueue();
Node* makeNode(int value);

#endif //MSG_QUEUE_H
