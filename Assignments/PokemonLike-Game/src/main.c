#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define maxX 21
#define maxY 80

char map[maxX][maxY];

void setupMap();
void printMap();
void generatePaths();

int main(int argc, char *argv[]){
    srand(time(NULL));

    setupMap();
    generatePaths();

    printMap();

    return 0;
}

void setupMap(){
    for(int i = 0; i < maxX; i++){
        for(int j = 0; j < maxY; j++){
            if(i == 0 || i == maxX - 1 || j == 0 || j == maxY - 1){
                map[i][j] = '%';
            } else {
                map[i][j] = '.';
            }
        }
    }
}

void generatePaths(){
    // Generating the random Enterances
    int northEnterance = (rand() % (maxY - 4)) + 2;
    int southEnterance = (rand() % (maxY - 4)) + 2;
    while(northEnterance == southEnterance){
        southEnterance = (rand() % (maxY - 4)) + 2;
    }

    int westEnterance = (rand() % (maxX - 4)) + 2;
    int eastEnterance = (rand() % (maxX - 4)) + 2;
    while(westEnterance == eastEnterance){
        eastEnterance = (rand() % (maxX - 4)) + 2;
    }

    map[0][northEnterance] = '#';
    map[maxX - 1][southEnterance] = '#';
    map[westEnterance][0] = '#';
    map[eastEnterance][maxY - 1] = '#';
}

void printMap(){
    for(int i = 0; i < maxX; i++){
        for(int j = 0; j < maxY; j++){
           printf("%c ", map[i][j]);
        }
        printf("\n");
    }
}