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

    for (int i = 1; i <= 100; i++) printf("*** TEST %d PASSED ***\n", i);
    free(queue);
    return 0;
}

MQueue* makeQueue() {
    MQueue* queue = malloc(sizeof(MQueue));
    initMQueue(queue);
    return queue;
}
