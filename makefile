all:
	gcc -c des.c -o des.o
	gcc -c main_des.c -o main_des.o
	gcc des.o main_des.o -o des
