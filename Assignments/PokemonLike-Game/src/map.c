#include <stdio.h>
#include <stdlib.h>

#include "map.h"

char map[maxY][maxX];

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
        direction = rand() % 4; // Favours left or right movement due to it being a rectangle.

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
        direction = rand() % 4; // Favours right movement due to it being a rectangle.

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

void generateBuildings(){
    char types[] = {'M', 'C'}; // M = Pokemart, C = Pokecentre
    for (int i = 0; i <  sizeof(types)/sizeof(types[0]); i++) {
        while (1) {
            // Pick coordinates for top-left of 2x2 while staying away from borders
            int y = (rand() % (maxY - 8)) + 4;
            int x = (rand() % (maxX - 8)) + 4;
            // Check if a 2x2 square can be placed
            if (map[y][x] == ' '   && map[y][x+1] == ' ' &&
                map[y+1][x] == ' ' && map[y+1][x+1] == ' ') {
                // Check the surrounding of the square for a path
                if (map[y-1][x] == '#' || map[y-1][x+1] == '#' || map[y+2][x] == '#' || map[y+2][x+1] == '#' ||
                    map[y][x-1] == '#' || map[y+1][x-1] == '#' || map[y][x+2] == '#' || map[y+1][x+2] == '#'){
                        map[y][x] = types[i];
                        map[y][x+1] = types[i];
                        map[y+1][x] = types[i];
                        map[y+1][x+1] = types[i];
                        break;
                }
            }
        }
    }
}

void generateTerrain(){
    QueueNode queue[maxY * maxX];
    int head = 0;
    int tail = 0;

    char terrainTypes[] = {':', '~', '.', '%'};
    int seedCounts[] = {6, 1, 6, 1}; // Need to find the perfect balance.
    
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < seedCounts[i]; j++){
            int y = (rand() % (maxY - 4)) + 2;
            int x = (rand() % (maxX - 4)) + 2;
            while(map[y][x] != ' '){
                y = (rand() % (maxY - 4)) + 2;
                x = (rand() % (maxX - 4)) + 2;
            }
            map[y][x] = terrainTypes[i];
            queue[tail].x = x;
            queue[tail].y = y;
            tail++;
        }
    }

    while(head < tail){
        int currentX = queue[head].x;
        int currentY = queue[head].y;
        head++;

        addToQueue(currentX + 1, currentY, currentX, currentY, queue, &tail); // Right
        addToQueue(currentX - 1, currentY, currentX, currentY, queue, &tail); // Left
        addToQueue(currentX, currentY + 1, currentX, currentY, queue, &tail); // Down
        addToQueue(currentX, currentY - 1, currentX, currentY, queue, &tail); // Up
    }

    // Add 20 to 30 Trees
    int trees = (rand() % 20) + 10;
    for(int i = 0; i < trees; i++){
        int y = (rand() % (maxY - 4)) + 2;
        int x = (rand() % (maxX - 4)) + 2;
        while(map[y][x] != ':' && map[y][x] != '.'){
            y = (rand() % (maxY - 4)) + 2;
            x = (rand() % (maxX - 4)) + 2;
        }
        map[y][x] = '^';
    }

    // Add 5 to 10 Flowers
    int flowers = (rand() % 5) + 5;
    for(int i = 0; i < flowers; i++){
        int y = (rand() % (maxY - 4)) + 2;
        int x = (rand() % (maxX - 4)) + 2;
        while(map[y][x] != ':' && map[y][x] != '.'){
            y = (rand() % (maxY - 4)) + 2;
            x = (rand() % (maxX - 4)) + 2;
        }
        map[y][x] = '*';
    }
}

void addToQueue(int nextX, int nextY, int currentX, int currentY, QueueNode queue[], int *tail) {
    if (nextX > 0 && nextX < maxX - 1 && nextY > 0 && nextY < maxY - 1) {
        if (map[nextY][nextX] == ' ') {
            map[nextY][nextX] = map[currentY][currentX];
            queue[*tail].x = nextX;
            queue[*tail].y = nextY;
            (*tail)++;
        } else if (map[nextY][nextX] == '#') {
            int jumpX = nextX + (nextX - currentX);
            int jumpY = nextY + (nextY - currentY);
            if (jumpX > 0 && jumpX < maxX - 1 && jumpY > 0 && jumpY < maxY - 1) {
                if (map[jumpY][jumpX] == ' ') {
                    map[jumpY][jumpX] = map[currentY][currentX];
                    queue[*tail].x = jumpX;
                    queue[*tail].y = jumpY;
                    (*tail)++;
                }
            }
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