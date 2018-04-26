#include "msg_queue.h"

//makeNode test
bool initialiseNodeTest() {
    Node* node = makeNode(0);
    return (node != NULL);
}

//makeNode test
bool initialiseNodeValueTest() {
    Node* node = makeNode(5);
    return (node->data == 5);
}

//makeNode test
bool initialiseNodeNextTest() {
    Node* node = makeNode(0);
    return (node->next == NULL);
}

//initMQueue test
//Tests that the MQueue is correctly initialised by makeQueue/initMQueue
bool intialiseMQueueTest() {
    MQueue* queue = makeQueue();
    return (queue != NULL);
}

//initMQueue test
bool intialiseMQueueHeadTest() {
    MQueue* queue = makeQueue();
    return (queue->head == NULL);
}

//initMQueue test
bool intialiseMQueueTailTest() {
    MQueue* queue = makeQueue();
    return (queue->tail == NULL);
}

//is_empty test
bool newMQueueEmptyTest() {
    MQueue* queue = makeQueue();
    return(queue->head == NULL);
}

//size test
bool newMQueueSizeZeroTest() {
    MQueue* queue = makeQueue();
    return(queue->size == 0);
}

bool enqueueHeadTailTest() {
    MQueue* queue = makeQueue();
    send_msg(queue, 1);
    return ((queue->head != NULL)
        && (queue->tail != NULL)
        && (queue->head == queue->tail));
}

bool enqueueSizeTest() {
    MQueue* queue = makeQueue();
    send_msg(queue, 1);
    return (queue->size == 1);
}

bool doubleEnqueueSizeTest() {
    MQueue* queue = makeQueue();
    send_msg(queue, 1);
    send_msg(queue, 2);
    return(queue->size == 2);
}

bool enqueueDequeueTest() {
    MQueue* queue = makeQueue();
    send_msg(queue, 1);
    return ((read_msg(queue))->data == 1);
}

bool doubleEnqueueDequeueTest() {
    MQueue* queue = makeQueue();
    send_msg(queue, 1);
    send_msg(queue, 2);

    int one = read_msg(queue)->data;
    int two = read_msg(queue)->data;
    return (one == 1 && two == 2);
}

bool doubleEnqueueDequeueSizeTest() {
    MQueue* queue = makeQueue();
    send_msg(queue, 1);
    send_msg(queue, 2);

    read_msg(queue);
    read_msg(queue);
    return (queue->size == 0);
}

bool dequeueEmptyQueueTest() {
    MQueue* queue = makeQueue();
    return (read_msg(queue) == NULL);
}
