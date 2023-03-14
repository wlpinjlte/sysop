CC=gcc
all:
	make main BLOCK_SIZE=1
	make main BLOCK_SIZE=1024
main:
	$(CC) -Wall main.c -o main_$(BLOCK_SIZE) -D BLOCK_SIZE=$(BLOCK_SIZE)
report:
	make all
	rm -f pomiar_zad2_1.txt pomiar_zad2_1024.txt
	head -c 11242 </dev/urandom >test.in
	./main_1 test.in tmp1>>pomiar_zad2_1.txt
	./main_1024 test.in tmp1024>>pomiar_zad2_1024.txt
clean:
	rm main_* test.in tmp1 tmp1024