#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define maxX 80
#define maxY 21

char map[maxY][maxX];

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
    for(int i = 0; i < maxY; i++){
        for(int j = 0; j < maxX; j++){
            if(i == 0 || i == maxY - 1 || j == 0 || j == maxX - 1){
                map[i][j] = '%';
            } else {
                map[i][j] = ' ';
            }
        }
    }
}

void generatePaths(){
    // Generating the random Enterances
    int northX = (rand() % (maxX - 4)) + 2;
    int southX = (rand() % (maxX - 4)) + 2;
    while(abs(northX - southX) < 10){ southX = (rand() % (maxX - 4)) + 2; } // Makes it so they arent aligned too closed

    int westY = (rand() % (maxY - 4)) + 2;
    int eastY = (rand() % (maxY - 4)) + 2;
    while(abs(westY - eastY) < 5){ eastY = (rand() % (maxY - 4)) + 2; }

    map[0][northX] = '#';
    map[maxY - 1][southX] = '#';
    map[westY][0] = '#';
    map[eastY][maxX - 1] = '#';

    // Connecting the enterances
    int currentX, currentY, direction;

    // North to South path
    currentX = northX;
    currentY = 1;
    while(currentX != southX || currentY != maxY - 1){
        map[currentY][currentX] = '#';
        direction = rand() % 5; // Favours left or right movement due to it being a rectangle.

        if((direction == 0) && currentY < maxY - 2){
            currentY++;
        } else if(currentX != southX){
            if(currentX > southX) currentX--;
            else currentX++;
        } else if(currentX == southX && currentY < maxY - 1){
            currentY++;
        }
    }

    // West to East path
    currentX = 1;
    currentY = westY;
    while(currentY != eastY || currentX != maxX - 1){
        map[currentY][currentX] = '#';
        direction = rand() % 5; // Favours right movement due to it being a rectangle.

        if((direction != 0) && currentX < maxX - 2){
            currentX++;
        } else if(currentY != eastY){
            if(currentY > eastY) currentY--;
            else currentY++;
        } else if(currentY == eastY && currentX < maxX - 1){
            currentX++;
        }
    }
}

void printMap(){
    for(int i = 0; i < maxY; i++){
        for(int j = 0; j < maxX; j++){
           printf("%c", map[i][j]);
        }
        printf("\n");
    }
}