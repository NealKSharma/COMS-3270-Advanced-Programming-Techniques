#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "entity.h"
#include "map.h"

int hikerDistance[maxY][maxX];
int rivalDistance[maxY][maxX];

static const int move_costs[7][8] = {
    // Terrain: 0:#, 1:M, 2:C, 3::, 4:., 5:^, 6:*, 7:Default
    {10, 50, 50, 20, 10, 20, 15, INT_MAX}, // Player
    {10, 50, 50, 15, 10, 15, 15, INT_MAX}, // Hiker
    {10, 50, 50, 20, 10, INT_MAX, 15, INT_MAX}, // Rival
    {10, 50, 50, 20, 10, INT_MAX, 15, INT_MAX}, // Pacer
    {10, 50, 50, 20, 10, INT_MAX, 15, INT_MAX}, // Wanderer
    {10, 50, 50, 20, 10, INT_MAX, 15, INT_MAX}, // Sentry
    {10, 50, 50, 20, 10, INT_MAX, 15, INT_MAX} // Explorer
};

void initializePlayer(Player *pc, singleMap *map){
    pc->entity.type = player;
    // Can only be placed on a path
    int y, x;
    while(1){
        y = rand() % (maxY - 2) + 1;
        x = rand() % (maxX - 2) + 1;
        if(map->terrain[y][x] == '#'){
            map->terrain[y][x] = '@';
            map->playerY = y;
            map->playerX = x;
            break;
        }
    }
}

int getCost(entityType type, char terrain) {
    int terrainIndex;
    // Map the character to a column index
    switch(terrain) {
        case '#': terrainIndex = 0; break; // Path
        case 'M': terrainIndex = 1; break; // Pokemart
        case 'C': terrainIndex = 2; break; // Pokecenter
        case ':': terrainIndex = 3; break; // Tall Grass
        case '.': terrainIndex = 4; break; // Clearing
        case '^': terrainIndex = 5; break; // Trees
        case '*': terrainIndex = 6; break; // Flower
        default:  terrainIndex = 7; break; // INT_MAX
    }

    if (type < 0 || type > 6) return INT_MAX;
    return move_costs[type][terrainIndex];
}

void heapInitialize(heap *h){
    h->capacity = 256; // Is increased further
    h->size = 0;
    h->nodes = malloc(sizeof(heapNode) * h->capacity);
}

void heapPush(heap *h, int y, int x, int distance){
    if (h->size >= h->capacity) {
        h->capacity *= 2;
        h->nodes = realloc(h->nodes, sizeof(heapNode) * h->capacity);
    }

    int i = h->size++;
    while(i > 0 && h->nodes[(i - 1) / 2].distance > distance){
        h->nodes[i] = h->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    h->nodes[i].y = y;
    h->nodes[i].x = x;
    h->nodes[i].distance = distance;
}

heapNode heapPop(heap *h) {
    heapNode min = h->nodes[0];
    h->size--; 
    
    if (h->size > 0) {
        heapNode last = h->nodes[h->size];
        int i = 0, child;
        while ((child = i * 2 + 1) < h->size) {
            if (child + 1 < h->size && h->nodes[child + 1].distance < h->nodes[child].distance) {
                child++;
            }
            if (last.distance <= h->nodes[child].distance) break;
            h->nodes[i] = h->nodes[child];
            i = child;
        }
        h->nodes[i] = last;
    }
    return min;
}

void pathFinding(singleMap *map, entityType type, int dist[maxY][maxX]){
    heap h;
    heapInitialize(&h);

    for(int i = 0; i < maxY; i++){
        for(int j = 0; j < maxX; j++){
            dist[i][j] = INT_MAX; // Set distances to Int Max.
        }
    }
    // Set player's location to 0 distance.
    dist[map->playerY][map->playerX] = 0;
    heapPush(&h, map->playerY, map->playerX, 0);

    while(h.size > 0){
        heapNode curr = heapPop(&h);
        if(curr.distance > dist[curr.y][curr.x]) continue;

        // Check the different directions
        for(int dy = -1; dy <= 1; dy++){
            for (int dx = -1; dx <= 1; dx++){
                if(dy == 0 && dx == 0) continue; // Skip the current tile
                int neighborY = curr.y + dy;
                int neighborX = curr.x + dx;

                if(neighborY > 0 && neighborY < maxY - 1 && neighborX > 0 && neighborX < maxX - 1){
                    char terrain = map->terrain[neighborY][neighborX];
                    int moveCost = getCost(type, terrain);
                    if(moveCost != INT_MAX){
                        int newDist = dist[curr.y][curr.x] + moveCost;
                        if(newDist < dist[neighborY][neighborX]){
                            dist[neighborY][neighborX] = newDist;
                            heapPush(&h, neighborY, neighborX, newDist);
                        }
                    }
                }
            }
        }
    }
    free(h.nodes);
}

void initializeEntities(singleMap *map, int numTrainers){
    map->entityList = malloc(numTrainers * sizeof(Entity *));
    if(!map->entityList) return;

    int index = 0;
    int random;
    if (numTrainers < 2) {
        random = rand() % 2;
        if(random == 0) placeEntity(map, hiker, index++);
        else placeEntity(map, rival, index++);
    } else {
        placeEntity(map, hiker, index++);
        placeEntity(map, rival, index++);
        numTrainers -= 2;
        while(numTrainers > 0){
            entityType type = (1 + rand() % 6);
            placeEntity(map, type, index++);
            numTrainers--;
        }
    }
}

void placeEntity(singleMap *map, entityType type, int index){
    Entity *npc = malloc(sizeof(Entity));
    if(!npc) return;

    int y = rand() % (maxY - 2) + 1;
    int x = rand() % (maxX - 2) + 1;
    while(getCost(type, map->terrain[y][x]) == INT_MAX){
        y = rand() % (maxY - 2) + 1;
        x = rand() % (maxX - 2) + 1;
    }
    npc->y = y;
    npc->x = x;
    npc->type = type;
    npc->standingOn = map->terrain[y][x];
    switch(type){
        case 1: map->terrain[y][x] = 'h'; break;
        case 2: map->terrain[y][x] = 'r';break;
        case 3: map->terrain[y][x] = 'p';break;
        case 4: map->terrain[y][x] = 'w';break;
        case 5: map->terrain[y][x] = 's';break;
        case 6: map->terrain[y][x] = 'e';break;
    }
    map->entityList[index] = (struct Entity *)npc;
}

void entityMovementLoop(){
    while(1){
        
    }
}