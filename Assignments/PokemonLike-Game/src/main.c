#include <time.h>
#include <stdlib.h>

#include "map.h"

int main(int argc, char *argv[]){
    srand(time(NULL));

    setupMap();
    generatePaths();
    generateBuildings();
    generateTerrain();

    printMap();

    return 0;
}