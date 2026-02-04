#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "map.h"

currentMap *world[401][401] = {NULL};
int y = 200;
int x = 200;

void generateLandingMap(currentMap *map);
void handleMovement(int changeY, int changeX);
void generateMap(currentMap *map);

int main(int argc, char *argv[]){
    srand(time(NULL));

    // Generate the initial map
    world[y][x] = malloc(sizeof(currentMap));
    generateLandingMap(world[y][x]);

    // Movement
    char input = '?';

    while(input != 'q'){
        printf("Current coordinates: (%d, %d)\n", y-200, x-200);
        printf("Move (n,s,e,w) or 'q':");
        scanf(" %c", &input);

        // To only read the first letter and ignore the rest
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (input == 'q') {
            printf("Thanks for playing!");
            break;
        }
        
        switch (input) {
            case 'n': handleMovement(-1, 0); break;
            case 's': handleMovement(1, 0);  break;
            case 'w': handleMovement(0, -1); break;
            case 'e': handleMovement(0, 1);  break;
            default:  printf("Invalid command!\n");
        }
    }

    // Free the memory of the created maps
    for (int i = 0; i < 401; i++){
        for(int j = 0; j < 401; j++){
            if(world[i][j] != NULL) free(world[i][j]);
        }
    }

    return 0;
}

void generateLandingMap(currentMap *map){
    setupMap(map);
    generatePaths(map, -1, -1, -1, -1); // Random gates
    generateBuildings(map, 'M', 'C'); // Always generates the buildings
    generateTerrain(map);
    printMap(map);
}

void handleMovement(int changeY, int changeX){
    int nextY = y + changeY;
    int nextX = x + changeX;

    if (nextX < 0 || nextX > 400 || nextY < 0 || nextY > 400) {
        printf("You have reached the end of the world!\n");
        return;
    }

    x = nextX;
    y = nextY;

    if (world[y][x] == NULL) { // If map has not been generated.
        world[y][x] = malloc(sizeof(currentMap));
        generateMap(world[y][x]);
    } else {
        printMap(world[y][x]);
    }
}

void generateMap(currentMap *map){
    setupMap(map);

    int n = (y > 0   && world[y-1][x] != NULL) ? world[y-1][x]->sGate : -1;
    int s = (y < 400 && world[y+1][x] != NULL) ? world[y+1][x]->nGate : -1;
    int w = (x > 0   && world[y][x-1] != NULL) ? world[y][x-1]->eGate : -1;
    int e = (x < 400 && world[y][x+1] != NULL) ? world[y][x+1]->wGate : -1;
    generatePaths(map, n, s, w, e);

    int distance = abs(x - 200) + abs(y - 200);
    int probability = 5;
    if(distance < 200) probability = ((-45 * distance) / 200) + 50;

    char pokeMart = ((rand() % 100) < probability) ? 'M' : ' ';
    char pokeCentre = ((rand() % 100) < probability) ? 'C' : ' ';
    generateBuildings(map, pokeMart, pokeCentre);

    generateTerrain(map);
    printMap(map);
}