#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "map.h"

currentMap *world[401][401] = {NULL};
int x = 200;
int y = 200;

void generateLandingMap(currentMap *map);
void generateMap(currentMap *map);

int main(int argc, char *argv[]){
    srand(time(NULL));

    world[200][200] = malloc(sizeof(currentMap));
    generateLandingMap(world[200][200]); // Generate the initial map

    // Movement
    char input = '?';
    while(input != 'q'){
        printf("Enter a command: ");
        scanf(" %c", &input);

        if(input == 'n'){
            if(y > 0){
                y--;
                if(world[y][x] != NULL){ // If map has been generated.
                    printMap(world[y][x]);
                } else {
                    world[y][x] = malloc(sizeof(currentMap));
                    generateMap(world[y][x]);
                }
            } else {
                printf("You have reached the end of the world!");
            }
        } else if(input == 's'){
            if(y < 400){
                y++;
                if(world[y][x] != NULL){
                    printMap(world[y][x]);
                } else {
                    world[y][x] = malloc(sizeof(currentMap));
                    generateMap(world[y][x]);
                }
            } else {
                printf("You have reached the end of the world!");
            }
        } else if(input == 'w'){
            if(x > 0){
                x--;
                if(world[y][x] != NULL){
                    printMap(world[y][x]);
                } else {
                    world[y][x] = malloc(sizeof(currentMap));
                    generateMap(world[y][x]);
                }
            } else {
                printf("You have reached the end of the world!");
            }
        } else if(input == 'e'){
            if(x < 400){
                x++;
                if(world[y][x] != NULL){
                    printMap(world[y][x]);
                } else {
                    world[y][x] = malloc(sizeof(currentMap));
                    generateMap(world[y][x]);
                }
            } else {
                printf("You have reached the end of the world!");
            }
        }
    }

    // Free the memory of the assigned maps
    for (int i = 0; i < 401; i++){
        for(int j = 0; j < 401; j++){
            if(world[i][j] != NULL){
                free(world[i][j]);
            }
        }
    }

    return 0;
}

void generateLandingMap(currentMap *map){
    setupMap(map);
    generatePaths(map, -1, -1, -1, -1);
    generateBuildings(map);
    generateTerrain(map);
    printMap(map);
}

void generateMap(currentMap *map){
    int n = (y > 0   && world[y-1][x] != NULL) ? world[y-1][x]->sGate : -1;
    int s = (y < 400 && world[y+1][x] != NULL) ? world[y+1][x]->nGate : -1;
    int w = (x > 0   && world[y][x-1] != NULL) ? world[y][x-1]->eGate : -1;
    int e = (x < 400 && world[y][x+1] != NULL) ? world[y][x+1]->wGate : -1;
    setupMap(map);
    generatePaths(map, n, s, w, e);
    generateBuildings(map);
    generateTerrain(map);
    printMap(map);
}