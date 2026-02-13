#ifndef ENTITY_H
#define ENTITY_H

#include "map.h"

typedef enum {
    player, // 0
    hiker, // 1
    rival, // 2
    swimmer // 3
} Type;

typedef struct {
    int mapY, mapX, y, x;
    Type type;
} Entity;

typedef struct {
    Entity entity;
} Player;

void initializePlayer(Player *pc, singleMap *map);

#endif