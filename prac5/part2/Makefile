msg_queue: msg_queue.o msg_queue_test.o
	gcc -Wall -Wextra -g msg_queue_test.o msg_queue.o -o msg_queue -lpthread

msg_queue.o: msg_queue.c msg_queue.h
		 gcc -Wall -Wextra -g msg_queue.c -c -o msg_queue.o

msg_queue_test.o: msg_queue_test.c msg_queue.h
	 	 gcc -Wall -Wextra -g msg_queue_test.c -c -o msg_queue_test.o

clean:
	rm -f *.o msg_queue
