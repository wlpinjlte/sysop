CC=gcc
all:
	make main_std
	make main_sys
main_std:
	$(CC) -Wall -o main_std main.c
main_sys:
	$(CC) -Wall -o main_sys main.c -D SYS
report:
	make all
	rm -f pomiar_zad_1_sys.txt pomiar_zad_1_std.txt
	head -c 312424 </dev/urandom >test.in
	./main_std a b test.in tmp_std>>pomiar_zad_1_std.txt
	./main_sys a b test.in tmp_sys>>pomiar_zad_1_sys.txt
	rm -f tmp_std tmp_sys test.in
clean:
	rm main_*