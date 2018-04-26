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
    TestMQueue* testMQueue = malloc(sizeof(TestMQueue));
    testMQueue->queue = makeQueue();
    testMQueue->counter = 0;
    testMQueue->passed = true;

    int i = 0;
    for (i = 0; i < 1000; i++) send_msg(testMQueue->queue, i);
    pthread_create(&(tid[0]), NULL, threadIterativeDequeue, testMQueue);
    pthread_create(&(tid[1]), NULL, threadIterativeDequeue, testMQueue);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    int n = i - 1; //Upperbound of iteration.
    int result = (n * (n + 1))/2; //Gaussian formula for sum of 1 to n
    return (result == testMQueue->counter);
}

void* threadIterativeDequeue(void* arg) {
    while(true) {
        pthread_mutex_lock(&mutex);
        if ((((TestMQueue*) arg)->queue->size) > 0) {
            ((TestMQueue*) arg)->counter += read_msg(((TestMQueue*) arg)->queue)->data;
        } else {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }

        pthread_mutex_unlock(&mutex);

    }
    pthread_exit((void*) "Goodbye!");
}
