#include <stdlib.h>
#include "entity.h"
#include "map.h"

int hikerDistance[maxY][maxX];
int rivalDistance[maxY][maxX];

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

int getCost(entityType type, char terrain){
    switch(type){
        case hiker:
            switch(terrain){
                case '#': return 10; // Path
                case 'M': return 50; // Pokemart
                case 'C': return 50; // Pokecenter
                case ':': return 15; // Tall Grass
                case '.': return 10; // Clearing
                case '^': return 20; // Trees
                case '*': return 15; // Flower
                default: return INT_MAX; // Boulders, Water, and Gates
            }
        case rival:
            switch(terrain){
                case '#': return 10; // Path
                case 'M': return 50; // Pokemart
                case 'C': return 50; // Pokecenter
                case ':': return 20; // Tall Grass
                case '.': return 10; // Clearing
                case '^': return 20; // Trees
                case '*': return 15; // Flower
                default: return INT_MAX; // Boulders, Water, and Gates
            }
        default: return -1;
    }
    return 0;
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

void pathFinding(singleMap *map, Player *pc, entityType type, int dist[maxY][maxX]){
    heap h;
    heapInitialize(&h);

    for(int i = 0; i < maxY; i++){
        for(int j = 0; j < maxX; j++){
            dist[i][j] = INT_MAX; // Set distances to Int Max.
        }
    }
    // Set player's location to 0 distance.
    dist[pc->entity.y][pc->entity.x] = 0;
    heapPush(&h, pc->entity.y, pc->entity.x, 0);

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