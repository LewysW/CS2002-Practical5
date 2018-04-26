#include "msg_queue.h"

int main() {
    //TODO EXTENSION - add more security features to processes e.g. hashing the file, further encryption, diffie-helman key distribution.
    bool (*singleThreadedTests[NUM_SINGLE_THREADED_TESTS]) ();
    initTests(singleThreadedTests, 0);
    printf(GREEN "\n***RUNNING SINGLE-THREADED TESTS***\n");
    int numPassing = runTests(singleThreadedTests, NUM_SINGLE_THREADED_TESTS);

    printf(GREEN "\n***RUNNING MULTI-THREADED TESTS***\n");
    bool (*multiThreadedTests[NUM_MULTI_THREADED_TESTS]) ();
    initTests(multiThreadedTests, 1);
    numPassing += runTests(multiThreadedTests, NUM_MULTI_THREADED_TESTS);

    printf("\n***TOTAL TESTS PASSED %d/%d***\n", numPassing, (NUM_SINGLE_THREADED_TESTS + NUM_MULTI_THREADED_TESTS));
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

void initTests(bool (*testFunctions[]) (), int flag) {
    switch (flag) {
        case 0:
            testFunctions[0] = initialiseNodeTest;
            testFunctions[1] = initialiseNodeValueTest;
            testFunctions[2] = initialiseNodeNextTest;
            testFunctions[3] = intialiseMQueueTest;
            testFunctions[4] = intialiseMQueueHeadTest;
            testFunctions[5] = intialiseMQueueTailTest;
            testFunctions[6] = newMQueueEmptyTest;
            testFunctions[7] = newMQueueSizeZeroTest;
            testFunctions[8] = enqueueHeadTailTest;
            testFunctions[9] = enqueueSizeTest;
            testFunctions[10] = doubleEnqueueSizeTest;
            testFunctions[11] = enqueueDequeueTest;
            testFunctions[12] = doubleEnqueueDequeueTest;
            testFunctions[13] = doubleEnqueueDequeueSizeTest;
            testFunctions[14] = dequeueEmptyQueueTest;
            break;

        case 1: //Fall through intentional
        default:
            //Multithreaded tests.
            testFunctions[0] = threadDoubleEnqueueSizeTest;
            testFunctions[1] = threadEnqueueOrderTest;
            testFunctions[2] = threadDequeueOrderTest;
            break;

    }
}

int runTests(bool (*testFunctions[]) (), int numTests) {
    int numPassingTests = 0;
    int i;
    for (i = 0; i < numTests; i++) {
        if ((*testFunctions[i]) ()) {
            numPassingTests += 1;
            printf(GREEN "\n***TEST %d PASSED***\n", i + 1);
        } else {
            printf(RED "\n***TEST %d FAILED***\n", i + 1);
        }
    }

    printf("\n***%d/%d TESTS PASSED***\n", numPassingTests, i);

    return numPassingTests;
}
