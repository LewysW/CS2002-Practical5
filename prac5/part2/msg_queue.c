#include "msg_queue.h"

/** enqueue
Creates a node initialised with a given integer value and attempts to enqueue it.
@MQueue - queue to enqueue node (send message) to.
@int - value to initialise node with.
*/
void send_msg( MQueue *s, int value ) {
    //Thread safety for node creation - threads wait while node is being created.
    pthread_mutex_lock(&(s->access));
    //Exit if queue is NULL
    if (s == NULL) exit(1);
    //Makes a new node with initialised with the given int value.
    Node* node = makeNode(value);

    //If queue is empty point head and tail to node to be enqueued
    if (s->head == NULL) {
        s->tail = s->head = node;

    //Otherwise add node to queue by pointing tail node of queue to new node and
    //updating the tail to be the new node.
    } else {
        s->tail->next = node;
        s->tail = node;
    }

    s->size++;
    //Unlock mutex
    pthread_mutex_unlock(&(s->access));
}

/** dequeue
Dequeues and returns the first value added the queue (pointed to by the head).
@MQueue - queue to dequeue (read message) from.
*/
Node *read_msg(MQueue* s) {
    //Thread safety - mutex locks access to queue manipulation while dequeuing
    pthread_mutex_lock(&(s->access));
    //Exit if queue is NULL
    if (s == NULL) exit(1);

    //If the queue is empty the head node is returned (should be NULL)
    if (s->head == NULL) return s->head;

    //Otherwise if not empty then point a new node to the head of the queue...
    Node* node = s->head;
    //...and point head of queue to second element in queue (thereby removing the first node)
    s->head = s->head->next;
    s->size--;

    //Unlock mutex
    pthread_mutex_unlock(&(s->access));

    //returns dequeued node
    return node;
}

/** initialises MQueue
Initialises MQueue by setting mutex as well as head and tail pointers.
@MQueue - queue to be initialised.
*/
void initMQueue(MQueue* s) {
    /**Initialises mutex.
    If initialisation fails then an error message is printed and program exits.
    */
    if (pthread_mutex_init(&(s->access), NULL) != 0) {
        printf("Mutex initialisation has failed.\n");
        exit(1);
    }

    //Sets tail equal to NULL and head equal to tail
    s->head = s->tail = NULL;
    s->size = 0;

    //NULL value is used to check if MQueue is empty.
}

/** prints values of MQueue elements
Prints "Empty queue" if MQueue is empty, otherwise prints values of each queue
element from oldest to newest element added.
@MQueue - queue to be printed.
*/
void printMQueue(MQueue* s) {
    //Mutex locked to ensure thread safety.
    pthread_mutex_lock(&(s->access));
    //Exit if queue is NULL
    if (s == NULL) exit(1);

    //If queue is empty then print "Empty queue" and return from function.
    if (s->head == NULL) {
        printf("Empty queue.\n");
        return;
    }

    //Otherwise set current node to the head of the queue...
    Node* current = s->head;

    //...and iterate over the remaining queue elements...
    while(current != NULL) {
        //...printing the value of the current element..
        printf("%d\n", current->data);
        //...and updating the current element to the next node.
        current = current->next;
    }

    //Unlocks mutex.
    pthread_mutex_unlock(&(s->access));
}

/** makes a new node
Creates a new node by mallocing space and setting initial values.
@int - value to assign to data of node.
*/
Node* makeNode(int value) {
    //Allocates memory for node.
    Node* node = malloc(sizeof(Node));
    //Sets data of node to given value.
    node->data = value;
    //Sets next to NULL (indicating no current next node).
    node->next = NULL;
    return node;
}
