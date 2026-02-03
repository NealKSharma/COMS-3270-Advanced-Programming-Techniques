all: main

main: main.o map.o
	gcc main.o map.o -o main

main.o: main.c map.h
	gcc -c main.c -o main.o

map.o: map.c map.h
	gcc -c map.c -o map.o

clean:
	rm main main.o map.o