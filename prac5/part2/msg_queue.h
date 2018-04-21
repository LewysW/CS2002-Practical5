#include <pthread.h>

#ifndef MSG_QUEUE_H
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
} MQueue;

void send_msg( MQueue *s, int value ); // enqueue
Node *read_msg( MQueue *s );            // dequeue
void initMQueue(MQueue *);
void printMQueue(MQueue *);

#endif //MSG_QUEUE_H
