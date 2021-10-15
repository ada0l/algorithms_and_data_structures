#include <stdbool.h>
#include <stdio.h>

#include "queue.h"
#include "random.h"
#include <wchar.h>

QueueNode* queue_node_new(
    QueueType* data, QueueNode* prev, QueueNode* next)
{
    QueueNode* result = (QueueNode*)malloc(sizeof(QueueNode));
    result->data = data;
    result->prev = prev;
    result->next = next;
    return result;
}

bool queue_node_is_dereferenable(QueueNode* node)
{
    return node != NULL;
}

bool queue_is_node_dereferenable(Queue* queue, QueueNode* node)
{
    return node != NULL && queue->beg != node && queue->end != node;
}

void queue_node_free(QueueNode* node)
{
    free(node->data);
    free(node);
}

Queue* queue_new()
{
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    // There are always two elements in the queue,
    // which explicitly point to the beginning and
    // the end. It seems to me that this is the best
    // solution that simplifies implementation
    queue->beg = queue_node_new(NULL, NULL, NULL);
    queue->end = queue_node_new(NULL, queue->beg, NULL);
    queue->beg->next = queue->end;
    queue->size = 0;
    return queue;
}

QueueNode* queue_get_begin(Queue* queue) { return queue->beg->next; }

QueueNode* queue_get_end(Queue* queue) { return queue->end->prev; }

size_t queue_get_size(Queue* queue) { return queue->size; }

void queue_push_after_node(
    Queue* queue, QueueNode* node, QueueType* new_element)
{
    QueueNode* new_node
        = queue_node_new(new_element, node, node->next);
    node->next->prev = new_node;
    node->next = new_node;
    ++queue->size;
}

void queue_push_back(Queue* queue, QueueType* new_element)
{
    return queue_push_after_node(
        queue, queue->end->prev, new_element);
}

void queue_push_front(Queue* queue, QueueType* new_element)
{
    return queue_push_after_node(queue, queue->beg, new_element);
}

QueueType* queue_top(Queue* queue) { return queue->end->prev->data; }

QueueType* queue_remove_node(Queue* queue, QueueNode* node)
{
    QueueType* tmp = node->data;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    --queue->size;
    free(node);
    return tmp;
}

QueueType* queue_pop_back(Queue* queue)
{
    return queue_remove_node(queue, queue->end->prev);
}

QueueType* queue_pop_front(Queue* queue)
{
    return queue_remove_node(queue, queue->beg->next);
}

void queue_for_each(Queue* queue, void (*func)(QueueNode* node))
{
    QueueNode* node = queue->beg->next;
    while (node != queue->end) {
        QueueNode* node_for_next = node->next;
        func(node);
        node = node_for_next;
    }
}

void queue_free_without_content(Queue* queue)
{
    free(queue->beg);
    free(queue->end);
    free(queue);
}

void queue_free(Queue* queue)
{
    queue_for_each(queue, queue_node_free);
    queue_free_without_content(queue);
}

void queue_shuffle(Queue* queue)
{
    // This code isn't optimal for two reasons:
    // 1. Finding an element that will be placed at the beginning or
    // end occurs in O(N)
    // 2. Each time it deletes the node and creates it again, and so
    // on 10,000 times. And as you know, memory allocation is usually
    // slow.
    //
    // Why isn't the code written differently?
    // I'm too lazy to rewrite it, and I don't know how.
    //
    // Klenin is very sad.
    for (int i = 0; i < 10000; ++i) {
        int random_nodes_count
            = random_get_num_in_range(0, queue->size);
        int j = 0;
        QueueNode* node = queue->beg->next;
        while (queue_is_node_dereferenable(queue, node)
            && j <= random_nodes_count) {
            node = node->next, ++j;
        }
        if (queue_is_node_dereferenable(queue, node)) {
            QueueType* type = queue_remove_node(queue, node);
            if (random_get_num_in_range(0, 1)) {
                queue_push_back(queue, type);
            } else {
                queue_push_front(queue, type);
            }
        }
    }
}
