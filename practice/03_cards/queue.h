#ifndef QUEUE_HEADER_FILE_H

#define QUEUE_HEADER_FILE_H

#include <stdbool.h>
#include <stdlib.h>

typedef void QueueType;

typedef struct QueueNode {
    QueueType* data;
    struct QueueNode *prev, *next;
} QueueNode;

typedef struct Queue {
    int size;
    struct QueueNode *beg, *end;
} Queue;

QueueNode* queue_node_new(
    QueueType* data, QueueNode* prev, QueueNode* next);
bool queue_node_is_dereferenable(QueueNode* node);
bool queue_is_node_dereferenable(Queue* queue, QueueNode* node);
void queue_node_free(QueueNode* node);
Queue* queue_new();
QueueNode* queue_get_begin(Queue* queue);
QueueNode* queue_get_end(Queue* queue);
size_t queue_get_size(Queue* queue);
void queue_push_after_node(
    Queue* queue, QueueNode* node, QueueType* new_element);
void queue_push_back(Queue* queue, QueueType* new_element);
void queue_push_front(Queue* queue, QueueType* new_element);
QueueType* queue_top(Queue* queue);
QueueType* qeueue_remove_node(Queue* queue, QueueNode* node);
QueueType* queue_pop_back(Queue* queue);
QueueType* queue_pop_front(Queue* queue);
void queue_for_each(Queue* queue, void (*func)(QueueNode* node));
void queue_free_without_content(Queue* queue);
void queue_free(Queue* queue);
void queue_shuffle(Queue* queue);

#endif
