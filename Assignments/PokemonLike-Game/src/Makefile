all: main

main: main.o map.o
	gcc -g main.o map.o -o main

main.o: main.c map.h
	gcc -g -c main.c -o main.o

map.o: map.c map.h
	gcc -g -c map.c -o map.o

clean:
	rm main main.o map.o