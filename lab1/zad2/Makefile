CC = gcc -Wall
LIB=library
MAIN=main.c
MAINDLL=main.c dllmanager.h

all:
	make lib_static
	make lib_shared
	make main_static
	make main_shared
	make main_dlopen
	make clean
lib_static: $(LIB).c
	$(CC) -c $(LIB).c
	ar cr $(LIB).a $(LIB).o

lib_shared: $(LIB).c
	$(CC) -c -fPIC $(LIB).c
	$(CC) -shared -o $(LIB).so $(LIB).o

main_static: $(LIB).c $(MAINDLL)
	$(CC) $(MAIN) -o main_static -L . $(LIB).a

main_shared: $(LIB).c $(MAINDLL)
	$(CC) $(MAIN) -o main_shared -L . $(LIB).so

main_dlopen: $(LIB).c $(MAINDLL)
	$(CC) $(MAIN) -o main_dlopen -ldl -D USE_DLL

clean:
	rm -f *.o *.out *.a *.so