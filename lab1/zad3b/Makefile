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
o:
	make all OPT="-O"
o0:
	make all OPT="-O0"
os:
	make all OPT="-Os"
test:
	sudo mv library.so /usr/lib
	make create_test
	./main_static<test.in>result_static_$(OPT).txt
	./main_shared<test.in>result_shared_$(OPT).txt
	./main_dlopen<test.in>result_dynamic_$(OPT).txt
	rm -f test.in

create_test:
	echo "init 20">test.in
	(ls -p | grep -v / | sed 's/^/count /')>>test.in
	echo "show 0">>test.in
	echo "show 1">>test.in
	echo "show 2">>test.in
	echo "show 3">>test.in
	echo "show 4">>test.in
	echo "show 5">>test.in
	echo "delete 5">>test.in
	echo "delete 4">>test.in
	echo "delete 3">>test.in
	echo "delete 2">>test.in
	echo "delete 1">>test.in
	echo "delete 0">>test.in
	echo "destroy">>test.in
	echo "exit">>test.in

report:
	sudo ls > /dev/null
	make o
	make test OPT="-O"
	make o0
	make test OPT="-O0"
	make os
	make test OPT="-Os"
	make clean
lib_static: $(LIB).c
	$(CC) -c $(LIB).c
	ar cr $(LIB).a $(LIB).o

lib_shared: $(LIB).c
	$(CC) -c -fPIC $(LIB).c
	$(CC) -shared -o $(LIB).so $(LIB).o

main_static: $(LIB).c $(MAINDLL)
	$(CC) $(OPT) $(MAIN) -o main_static -L . $(LIB).a

main_shared: $(LIB).c $(MAINDLL)
	$(CC) $(OPT) $(MAIN) -o main_shared -L . $(LIB).so

main_dlopen: $(LIB).c $(MAINDLL)
	$(CC) $(OPT) $(MAIN) -o main_dlopen -ldl -D USE_DLL

clean:
	rm -f *.o *.out *.a *.so