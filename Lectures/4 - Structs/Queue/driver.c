#include "queue.h"

int main(int argc, char *argv[]){
    queue myqueue;

    queue_init(&myqueue);

    queue_destroy(&myqueue);

    return 0;
}