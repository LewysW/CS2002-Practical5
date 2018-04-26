#include "msg_queue.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

bool threadDoubleEnqueueSizeTest() {
    pthread_t tid[2];
    void *tr1, *tr2;
    MQueue* queue = makeQueue();

    pthread_create(&(tid[0]), NULL, threadEnqueueOnce, queue);
    pthread_create(&(tid[1]), NULL, threadEnqueueOnce, queue);

    pthread_join(tid[0], &tr1);
    pthread_join(tid[1], &tr2);

    pthread_mutex_destroy(&(queue->access));
    return (queue->size == 2);
}

void* threadEnqueueOnce(void *arg) {
    send_msg((MQueue*)arg, 1);
    pthread_exit((void*)"Goodbye!");
}

bool threadEnqueueOrderTest() {
    pthread_t tid[2];
    MQueue* queue = makeQueue();
    pthread_create(&(tid[0]), NULL, threadIterativeEnqueue, queue);
    pthread_create(&(tid[1]), NULL, threadIterativeEnqueue, queue);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    for (int i = 0; i < 1000; i++) if ((read_msg(queue)->data) != i) return false;

    return true;
}

void* threadIterativeEnqueue(void* arg) {
    while(true) {
        pthread_mutex_lock(&mutex);
        if ((((MQueue*) arg)->size) < 1000) {
            send_msg((MQueue*)arg, ((MQueue*) arg)->size);
        } else {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit((void*)"Goodbye!");
}

bool threadDequeueOrderTest() {
    pthread_t tid[2];
    bool *passed;
    passed = malloc(sizeof(bool));
    MQueue* queue = makeQueue();
    for (int i = 0; i < 1000; i++) send_msg(queue, i);
    printMQueue(queue);
    pthread_create(&(tid[0]), NULL, threadIterativeDequeue, queue);
    pthread_create(&(tid[1]), NULL, threadIterativeDequeue, queue);
    pthread_join(tid[0], (void*) passed);
    pthread_join(tid[1], (void*) passed);
    printMQueue(queue);
    printf("Passed: %d\n", *passed);

    return (passed);
}

void* threadIterativeDequeue(void* arg) {
    int counter = 0;
    bool* passed = malloc(sizeof(bool));
    *passed = true;
    while(true) {
        pthread_mutex_lock(&mutex);
        int element = 0;
        counter++;
        if ((((MQueue*) arg)->size) > 0) {
            printf("Counter: %d\n", counter);
            if ((element = (read_msg((MQueue*)arg)->data)) != counter) {
                printf("Counter1: %d\n", counter);
                printf("Dequeued Value: - %d\n", element);
                *passed = false;
            } else {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            pthread_mutex_unlock(&mutex);
        }

    }
    pthread_exit((void*) passed);
}
