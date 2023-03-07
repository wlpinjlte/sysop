CC = gcc -Wall -std=c11 -ggdb


all: static shared clean

static:
	make clean
	$(CC) -c mylib.c
	ar crs libmylib.a mylib.o
	$(CC) main.c -o main -L. -lmylib

shared:
	make clean
	$(CC) -fPIC -c mylib.c
	$(CC) -shared -fPIC -o libmylib.so mylib.o
	$(CC) main.c -o main -L. -lmylib -Wl,-rpath=`pwd`

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.txt