CC=gcc
all:
	make main
main:
	$(CC) -Wall -o main main.c
report:
	make main
	./main 0.0000000005 1 >report.txt
	./main 0.0000000005 2 >>report.txt
	./main 0.0000000005 10 >>report.txt
clean:
	rm main report.txt