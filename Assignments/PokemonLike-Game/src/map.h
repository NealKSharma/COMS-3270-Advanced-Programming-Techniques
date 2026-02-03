#ifndef MAP_H
#define MAP_H

#define maxY 21
#define maxX 80

extern char map[maxY][maxX];

typedef struct {
    int x;
    int y;
} QueueNode;

void setupMap();
void printMap();
void generatePaths();
void generateBuildings();
void generateTerrain();
void addToQueue(int nextX, int nextY, int currentX, int currentY, QueueNode queue[], int *tail);

#endif