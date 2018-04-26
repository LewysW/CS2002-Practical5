#include "msg_queue.h"

int main(void) {

    //Creates list of bool functions that is the size of the number of single threaded unit tests.
    bool (*singleThreadedTests[NUM_SINGLE_THREADED_TESTS]) ();

    //Initialises the list of functions with the unit tests with the single threaded flag 0
    initTests(singleThreadedTests, 0);

    //Prints header for running single threaded tests
    printf(GREEN "\n***RUNNING SINGLE-THREADED TESTS***\n");

    //Gets number of single threaded queue tests that pass.
    int numPassing = runTests(singleThreadedTests, NUM_SINGLE_THREADED_TESTS);

    /*Creates a second list of bool functions that is the size of the number of
    multi-threaded unit tests*/
    bool (*multiThreadedTests[NUM_MULTI_THREADED_TESTS]) ();

    //Initialises the multi-threaded tests array with the appropriate unit tests to run (flag 1)
    initTests(multiThreadedTests, 1);

    //Prints header for running multi-threaded tests
    printf(GREEN "\n***RUNNING MULTI-THREADED TESTS***\n");
    numPassing += runTests(multiThreadedTests, NUM_MULTI_THREADED_TESTS);

    //Prints the number of tests passed as a fraction of the number of total tests
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

/** Initialises the array of tests to be run with the address of each unit test function.
@testFunctions - list of functions to be called, @flag - type of tests to be initialised...
... 0 for regular (single threaded) queue tests and a flag of 1 for multi-threaout of the total.ded tests
*/
void initTests(bool (*testFunctions[]) (), int flag) {
    switch (flag) {
        //If flag is zero then array is initialised with single threaded queue unit tests
        case 0:
            testFunctions[0] = initialiseNodeTest;
            testFunctions[1] = initialiseNodeValueTest;
            testFunctions[2] = initialiseNodeNextTest;
            testFunctions[3] = intialiseMQueueTest;
            testFunctions[4] = intialiseMQueueHeadTest;
            testFunctions[5] = intialiseMQueueTailTest;
            testFunctions[6] = newMQueueSizeZeroTest;
            testFunctions[7] = enqueueHeadTailTest;
            testFunctions[8] = enqueueSizeTest;
            testFunctions[9] = doubleEnqueueSizeTest;
            testFunctions[10] = enqueueDequeueTest;
            testFunctions[11] = doubleEnqueueDequeueTest;
            testFunctions[12] = doubleEnqueueDequeueSizeTest;
            testFunctions[13] = dequeueEmptyQueueTest;
            break;

        /*If flag is 1 (or other value) then the multi-threaded tests are assigned
        to the test function array */
        case 1: //Fall through intentional
        default:
            //Multithreaded tests.
            testFunctions[0] = threadDoubleEnqueueSizeTest;
            testFunctions[1] = threadEnqueueOrderTest;
            testFunctions[2] = threadDequeueOrderTest;
            break;

    }
}

/** Runs a given list of unit tests.
Iterates over each unit test in the list of test functions. If calling the
current function returns true then the test has passed, otherwise it has failed.
@testFunctions - list of unit tests to run. @numTests - number of tests to run.
*/
int runTests(bool (*testFunctions[]) (), int numTests) {
    //Number of tests that have passed
    int numPassingTests = 0;

    //Iterates over number of tests in test list.
    int i;
    for (i = 0; i < numTests; i++) {
        //If function returns true (i.e. passes)...
        if ((*testFunctions[i]) ()) {
            //... increment number of passing tests and print green message and test number
            numPassingTests += 1;
            printf(GREEN "\n***TEST %d PASSED***\n", i + 1);
        //Otherwise print red message and test number (to indicate a failed test)
        } else {
            printf(RED "\n***TEST %d FAILED***\n", i + 1);
        }
    }

    printf("\n***%d/%d TESTS PASSED***\n", numPassingTests, i);

    return numPassingTests;
}
