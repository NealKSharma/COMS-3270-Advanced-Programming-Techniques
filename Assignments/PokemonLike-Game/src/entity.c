#include <stdlib.h>
#include "entity.h"
#include "map.h"

void initializePlayer(Player *pc, singleMap *map){
    pc->entity.type = player;
    pc->entity.mapY = map->mapY;
    pc->entity.mapX = map->mapX;
    // Can only be placed on a path
    int y, x;
    while(1){
        y = rand() % (maxY - 2) + 1;
        x = rand() % (maxX - 2) + 1;
        if(map->terrain[y][x] == '#'){
            map->terrain[y][x] = '@';
            pc->entity.y = y;
            pc->entity.x = x;
            break;
        }
    }
}