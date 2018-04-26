#include "msg_queue.h"

/**
CUSTOM UNIT TESTS FOR MULTI THREADED CASES OF MQUEUE
*/

//Mutex for use in unit tests
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Tests that two threads enqueuing a node to the MQueue results in a size of two.
bool threadDoubleEnqueueSizeTest() {
    pthread_t tid[2];
    void *tr1, *tr2;
    MQueue* queue = makeQueue();

    //Creates each thread and calls threadEnqueueOnce
    pthread_create(&(tid[0]), NULL, threadEnqueueOnce, queue);
    pthread_create(&(tid[1]), NULL, threadEnqueueOnce, queue);

    //Merge threads back with main program.
    pthread_join(tid[0], &tr1);
    pthread_join(tid[1], &tr2);

    //Destroy mutex
    pthread_mutex_destroy(&(queue->access));

    //Check that size is 2.
    return (queue->size == 2);
}

//Allows a thread to enqueue a value of 1 and exits. @arg - MQueue to enqueue to
void* threadEnqueueOnce(void *arg) {
    send_msg((MQueue*)arg, 1);
    pthread_exit((void*)"Enqueued!");
}

/**Tests that the enqueue function of an MQueue is thread safe and can correctly
handle multiple threads attempting to enqueue at once by utilising mutexes.*/
bool threadEnqueueOrderTest() {
    //Array of thread IDs
    pthread_t tid[2];

    //Initialise queue
    MQueue* queue = makeQueue();

    //Creates two threads which call threadIterativeEnqueue
    pthread_create(&(tid[0]), NULL, threadIterativeEnqueue, queue);
    pthread_create(&(tid[1]), NULL, threadIterativeEnqueue, queue);

    //Terminates thread execution.
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    /*For loop and condition manually check whether values dequeued are FIFO and
    therefore have been added correctly by the threads.*/
    for (int i = 0; i < 1000; i++) if ((read_msg(queue)->data) != i) return false;

    return true;
}

/**Iterates until the size of the given MQueue is 1000.
Uses mutexes to avoid race conditions between the threads.
*/
void* threadIterativeEnqueue(void* arg) {
    //Loop continuously (condition cannot be outside of a mutex lock)
    while(true) {
        //Lock mutex
        pthread_mutex_lock(&mutex);
        //If the size of the MQueue is less than 1000...
        if ((((MQueue*) arg)->size) < 1000) {
            //...then enqueue another node.
            send_msg((MQueue*)arg, ((MQueue*) arg)->size);
        } else {
            //Otherwise unlock the mutex and return
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        //Unlock mutex after thread has enqueued
        pthread_mutex_unlock(&mutex);
    }
    //Exit
    pthread_exit((void*)"Iterative Enqueue!");
}

/**Tests that the dequeue function of an MQueue is thread safe and can correctly
handle multiple threads attempting to dequeue at once by utilising mutexes.*/
bool threadDequeueOrderTest() {
    pthread_t tid[2];

    //Test data structure to allow values to be passed to thread function
    TestMQueue* testMQueue = malloc(sizeof(TestMQueue));
    testMQueue->queue = makeQueue();
    testMQueue->counter = 0;
    testMQueue->passed = true;

    //For loop used to manually include the numbers 0 - 999
    int i = 0;
    for (i = 0; i < 1000; i++) send_msg(testMQueue->queue, i);

    //Two threads created and function threadIterativeDequeue called, passing in testMQueue
    pthread_create(&(tid[0]), NULL, threadIterativeDequeue, testMQueue);
    pthread_create(&(tid[1]), NULL, threadIterativeDequeue, testMQueue);

    //Terminates thread execution.
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    /*Updated value of testMQueue->counter is compared with the sum of all of the
    values enqueued (given by Gaussian formula n(n+1)/2). If values match then
    test passed.
    */
    int n = i - 1; //Upperbound of iteration.
    int result = (n * (n + 1))/2; //Gaussian formula for sum of 1 to n
    return (result == testMQueue->counter);
}

/**Iterates and dequeues while the size of the queue is greater than 0.
Uses mutexes to avoid race conditions between the threads.
*/
void* threadIterativeDequeue(void* arg) {
    //Iterates continuously
    while(true) {
        //Mutex lock is engaged
        pthread_mutex_lock(&mutex);
        //If size of queue is greater than zero
        if ((((TestMQueue*) arg)->queue->size) > 0) {
            //Add the current dequeued value to the testMQueue counter.
            ((TestMQueue*) arg)->counter += read_msg(((TestMQueue*) arg)->queue)->data;
        //Otherwise unlock the mutex and return NULL.
        } else {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        /*Unlock mutex after thread has finished dequeuing to allow other thread
        to dequeue*/
        pthread_mutex_unlock(&mutex);

    }
    //Exit
    pthread_exit((void*) "Iterative Dequeue!");
}
