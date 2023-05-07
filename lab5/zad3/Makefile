CC=gcc
all:
	make main
main:
	$(CC) -Wall -o main main.c
	$(CC) -Wall -o singleIntegral singleIntegral.c
report:
	make main
	./main 0.00000000005 1 >report.txt
	./main 0.00000000005 2 >>report.txt
	./main 0.00000000005 10 >>report.txt

clean:
	rm main singleIntegral