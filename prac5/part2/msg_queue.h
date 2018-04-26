#ifndef MSG_QUEUE_H
#include <pthread.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define MSG_QUEUE_H

//For Test printing
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"		/* Green */
#define NUM_SINGLE_THREADED_TESTS 14
#define NUM_MULTI_THREADED_TESTS 3

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
	int size;
} MQueue;

//Test queue struct to allow for value checking within thread-called functions
typedef struct {
	MQueue* queue;
	int counter;
	bool passed;
} TestMQueue;

//Provided function definitions
void send_msg( MQueue* s, int value ); // enqueue
Node *read_msg( MQueue* s );            // dequeue
void initMQueue(MQueue* s);
void printMQueue(MQueue* s);

//Additional function definitions
MQueue* makeQueue();
Node* makeNode(int value);
bool isNULLMQueue(MQueue* s);

//Test running functions
void initTests(bool (*testFunctions[]) (), int flag);
int runTests(bool (*testFunctions[]) (), int numTests);

//Multithreaded Queue Tests
bool threadDoubleEnqueueSizeTest();
bool threadEnqueueOrderTest();
bool threadDequeueOrderTest();
void* threadIterativeEnqueue(void* arg);
void* threadIterativeDequeue(void* arg);
void* threadEnqueueOnce(void* arg);

//Single Threaded Queue Tests
bool initialiseNodeTest();
bool initialiseNodeValueTest();
bool initialiseNodeNextTest();
bool intialiseMQueueTest();
bool intialiseMQueueMutexTest();
bool intialiseMQueueHeadTest();
bool intialiseMQueueTailTest();
bool newMQueueSizeZeroTest();
bool enqueueHeadTailTest();
bool enqueueSizeTest();
bool doubleEnqueueSizeTest();
bool enqueueDequeueTest();
bool doubleEnqueueDequeueTest();
bool doubleEnqueueDequeueSizeTest();
bool dequeueEmptyQueueTest();


#endif //MSG_QUEUE_H
