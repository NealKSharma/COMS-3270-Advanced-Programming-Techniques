#ifndef QUEUE_H
#define QUEUE_H

typedef struct {
    int data;
    node *prev;
    node *next;
}node;

typedef struct {
    node *head;
    node *tail;
}queue;

int queue_init(queue *q);
int queue_enqueue(queue *q, int payload);
int queue_dequeue(queue *q);
int queue_length(queue *q);
int queue_isempty(queue *q);
int queue_destroy(queue *q);

#endif