CC=gcc
LIB=library

all:
	make lib_static
	make lib_shared
	make main_static
	make main_shared
	make clean
lib_static: $(LIB).c
	$(CC) -Wall -c $(LIB).c
	ar cr $(LIB).a $(LIB).o

lib_shared: $(LIB).c
	$(CC) -Wall -c -fPIC $(LIB).c
	$(CC) -Wall -shared -o $(LIB).so $(LIB).o

main_static: $(LIB).c
	$(CC) main.c -o main_static -L . $(LIB).a

main_shared: $(LIB).c
	$(CC) main.c -o main_shared -L . $(LIB).so
clean:
	rm -f *.o *.a *.so