all: main

main: main.o map.o entity.o
	gcc -g main.o map.o entity.o -o main

main.o: main.c map.h entity.h
	gcc -g -c main.c -o main.o

entity.o: entity.c entity.h map.h
	gcc -g -c entity.c -o entity.o

map.o: map.c map.h
	gcc -g -c map.c -o map.o

clean:
	rm main main.o map.o entity.o