msg_queue: msg_queue.o mq_test.o single_threaded_tests.o multi_threaded_tests.o
	gcc -Wall -Wextra -g mq_test.o msg_queue.o single_threaded_tests.o multi_threaded_tests.o -o msg_queue -lpthread

msg_queue.o: msg_queue.c msg_queue.h
		 gcc -Wall -Wextra -g msg_queue.c -c -o msg_queue.o

mq_test.o: mq_test.c msg_queue.h
	 	 gcc -Wall -Wextra -g mq_test.c -c -o mq_test.o

single_threaded_tests.o: single_threaded_tests.c msg_queue.h
		 gcc -Wall -Wextra -g single_threaded_tests.c -c -o single_threaded_tests.o

multi_threaded_tests.o: multi_threaded_tests.c msg_queue.h
		 gcc -Wall -Wextra -g multi_threaded_tests.c -c -o multi_threaded_tests.o

clean:
	rm -f *.o msg_queue
