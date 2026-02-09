#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "map.h"

singleMap *world[401][401] = {NULL};
int y = 200, x = 200;

void generateMap(singleMap *map){
    setupMap(map);

    // Either coordinate of the connecting gate / -1 for randomly generated gate / -2 for no gate (Gates cant be at a -ve value)
    int n = (world[y-1][x] != NULL) ? world[y-1][x]->sGate : -1;
    int s = (world[y+1][x] != NULL) ? world[y+1][x]->nGate : -1;
    int w = (world[y][x-1] != NULL) ? world[y][x-1]->eGate : -1;
    int e = (world[y][x+1] != NULL) ? world[y][x+1]->wGate : -1;
    // If the map is on the border dont generate gates at all
    if(y == 0) n = -2; 
    if(y == 400) s = -2;
    if (x == 0) w = -2;
    if (x == 400) e = -2;
    generatePaths(map, n, s, w, e);

    int distance = abs(x - 200) + abs(y - 200);
    int probability = 5; // Default value is 5%
    if(distance < 200) probability = ((-45 * distance) / 200) + 50;
    char pokeMart = ((rand() % 100) < probability || (x == 200 && y == 200)) ? 'M' : ' ';
    char pokeCentre = ((rand() % 100) < probability || (x == 200 && y == 200)) ? 'C' : ' ';
    generateBuildings(map, pokeMart, pokeCentre);

    generateTerrain(map);
    printMap(map);
}

void handleMovement(int changeY, int changeX){
    if (x + changeX < 0 || x + changeX > 400 || y + changeY < 0 || y + changeY > 400) {
        printf("You have reached the edge of the world!\n\n");
        return;
    }

    x += changeX; y += changeY;
    if (world[y][x] == NULL) {
        world[y][x] = malloc(sizeof(singleMap));
        generateMap(world[y][x]);
    } else {
        printMap(world[y][x]);
    }
}

int main(int argc, char *argv[]){
    srand(time(NULL));

    // Generate the initial map
    world[y][x] = malloc(sizeof(singleMap));
    generateMap(world[y][x]);

    // Movement
    char ch, input[13]; // Max: "f -200 -200"
    while(1){
        printf("Current coordinates (x, y): (%d, %d)\n", x-200, y-200);
        printf("Move (n,s,w,e,f) or 'q': ");
        int i = 0;
        do{
            ch = getchar();
            if(i < 12) {  // Only store if there's room
                input[i] = ch;
                i++;
            }
        } while (ch != '\n');
        input[i-1] = '\0'; // Add Null terminator

        if(input[0] == 'q'){
            printf("Thanks for playing!");
            break;
        } else if(input[0] == 'f'){
            int flyX, flyY;
            if(sscanf(input+2, "%d %d", &flyX, &flyY) != 2){
                printf("Invalid fly command! Use format: f x y\n\n");
                continue;
            }
            if (flyX < -200 || flyX > 200 || flyY < -200 || flyY > 200) {
                printf("Invalid coordinates! Must be between -200 and 200.\n\n");
                continue;
            }

            y = flyY + 200; x = flyX + 200;
            if (world[y][x] == NULL) {
                world[y][x] = malloc(sizeof(singleMap));
                generateMap(world[y][x]);
            } else {
                printMap(world[y][x]);
            }
        } else {
            switch (input[0]) {
            case 'n': handleMovement(-1, 0); break;
            case 's': handleMovement(1, 0);  break;
            case 'w': handleMovement(0, -1); break;
            case 'e': handleMovement(0, 1);  break;
            default:  printf("Invalid command!\n\n");
            }
        }
    }

    for (int i = 0; i < 401; i++){
        for(int j = 0; j < 401; j++){
            if(world[i][j] != NULL) free(world[i][j]); // Free the memory of the created maps
        }
    }

    return 0;
}