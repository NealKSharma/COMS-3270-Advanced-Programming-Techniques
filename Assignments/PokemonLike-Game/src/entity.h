#ifndef ENTITY_H
#define ENTITY_H

#include "map.h"

typedef enum {
    player, // 0
    hiker, // 1
    rival, // 2
    swimmer // 3
} entityType;

typedef struct {
    int mapY, mapX, y, x;
    entityType type;
} Entity;

typedef struct {
    Entity entity;
} Player;

typedef struct {
    int x, y;
    int distance;
} heapNode;

typedef struct {
    heapNode *nodes;
    int size, capacity;
} heap;

extern int hikerDistance[maxY][maxX];
extern int rivalDistance[maxY][maxX];

void initializePlayer(Player *pc, singleMap *map);
int getCost(entityType type, char terrain);
void heapInitialize(heap *h);
void heapPush(heap *h, int y, int x, int distance);
heapNode heapPop(heap *h);
void pathFinding(singleMap *map, Player *pc, entityType type, int dist[maxY][maxX]);

#endif