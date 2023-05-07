CC=gcc
all:
	make main
main:
	$(CC) -Wall -o simulation simulation.c semaphore_helper.c share_memory.c queue_manager.c -lrt -lpthread
	$(CC) -Wall -o barber barber.c semaphore_helper.c share_memory.c queue_manager.c -lrt -lpthread
	$(CC) -Wall -o client client.c semaphore_helper.c share_memory.c queue_manager.c -lrt -lpthread
clean:
	rm simulation barber client