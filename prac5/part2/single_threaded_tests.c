#include "msg_queue.h"

/**
CUSTOM UNIT TESTS FOR SINGLE THREADED CASES OF MQUEUE
*/

/* Tests whether a node has been correctly
initialised and is therefore not NULL*/
bool initialiseNodeTest() {
    Node* node = makeNode(0);
    return (node != NULL);
}

/* Tests whether a node's value has been correctly
initialised and is therefore equal to the value passed into makeNode*/
bool initialiseNodeValueTest() {
    Node* node = makeNode(5);
    return (node->data == 5);
}

/* Tests whether a node has been correctly
initialised and therefore that its next value is NULL*/
bool initialiseNodeNextTest() {
    Node* node = makeNode(0);
    return (node->next == NULL);
}

/* Tests whether an MQueue has been correctly
initialised and is therefore not NULL*/
bool intialiseMQueueTest() {
    MQueue* queue = makeQueue();
    return (queue != NULL);
}

/* Tests whether an MQueue has been correctly
initialised as empty and therefore that its head value is NULL*/
bool intialiseMQueueHeadTest() {
    MQueue* queue = makeQueue();
    return (queue->head == NULL);
}

/* Tests whether an MQueue has been correctly
initialised as empty and therefore that its head value is NULL*/
bool intialiseMQueueTailTest() {
    MQueue* queue = makeQueue();
    return (queue->tail == NULL);
}

/* Tests whether an MQueue has been correctly
initialised and has a size of zero*/
bool newMQueueSizeZeroTest() {
    MQueue* queue = makeQueue();
    return(queue->size == 0);
}

/* Tests that when a value is enqueued the head and tail are no
longer NULL and that the head points to the same value as the tail.
*/
bool enqueueHeadTailTest() {
    MQueue* queue = makeQueue();
    send_msg(queue, 1);
    return ((queue->head != NULL)
        && (queue->tail != NULL)
        && (queue->head == queue->tail));
}

//Tests that enqueuing one value to the MQueue results in a size of one.
bool enqueueSizeTest() {
    MQueue* queue = makeQueue();
    send_msg(queue, 1);
    return (queue->size == 1);
}

//Tests that enqueuing two values to the MQueue results in a size of two.
bool doubleEnqueueSizeTest() {
    MQueue* queue = makeQueue();
    send_msg(queue, 1);
    send_msg(queue, 2);
    return(queue->size == 2);
}

/*Tests that enqueuing a value of one will return the same value of one when
the value is dequeued
*/
bool enqueueDequeueTest() {
    MQueue* queue = makeQueue();
    send_msg(queue, 1);
    return ((read_msg(queue))->data == 1);
}

/** Tests that enqueuing two values (of one and two respectively) and dequeuing
them results in the same values of one and two to be returned in FIFO ordering.*/
bool doubleEnqueueDequeueTest() {
    MQueue* queue = makeQueue();
    send_msg(queue, 1);
    send_msg(queue, 2);

    int one = read_msg(queue)->data;
    int two = read_msg(queue)->data;
    return (one == 1 && two == 2);
}

/**
Tests that enqueuing two values (of one and two respectively) and dequeuing them
results in a queue size of 0.
*/
bool doubleEnqueueDequeueSizeTest() {
    MQueue* queue = makeQueue();
    send_msg(queue, 1);
    send_msg(queue, 2);

    read_msg(queue);
    read_msg(queue);
    return (queue->size == 0);
}

//Tests that NULL is returned when attempting to dequeue from an empty queue
bool dequeueEmptyQueueTest() {
    MQueue* queue = makeQueue();
    return (read_msg(queue) == NULL);
}
