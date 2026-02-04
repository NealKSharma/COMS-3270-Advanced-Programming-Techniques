#ifndef MAP_H
#define MAP_H

#define maxY 21
#define maxX 80

typedef struct {
    int nGate, sGate, eGate, wGate;
    char terrain[maxY][maxX];
} currentMap;

typedef struct {
    int x, y;
} queueNode;

void setupMap(currentMap *map);
void printMap(currentMap *map);
void generatePaths(currentMap *map, int n, int s, int e, int w);
void generateBuildings(currentMap *map, char pokeMart, char pokeCentre);
void generateTerrain(currentMap *map);
void addToQueue(currentMap *map, int nextX, int nextY, int currentX, int currentY, queueNode queue[], int *tail);

#endif