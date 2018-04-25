#include "msg_queue.h"

int main() {
    MQueue* queue = makeQueue();
    send_msg(queue, 1);
    send_msg(queue, 2);
    send_msg(queue, 3);
    send_msg(queue, 4);
    send_msg(queue, 5);
    printMQueue(queue);

    for (int i = 0; i < 5; i++) {
        read_msg(queue);
        printMQueue(queue);
    }

    printf("Mutex destruction code: %d\n", pthread_mutex_destroy(&(queue->access)));
    free(queue);
    return 0;
}

/** makes a new MQueue
Abstracts process of making MQueue away from main function.
*/
MQueue* makeQueue() {
    //Allocates space for MQueue
    MQueue* queue = malloc(sizeof(MQueue));
    //Initialises queue with default values.
    initMQueue(queue);
    return queue;
}
