/**
 * @brief Priority queue that sets the minimum value to dequeue first. o(n) for enqueue o(1) for dequeue or peeking.
 * @author phun
 * @date 2/8/24
 */

#ifndef PQUEUE_H
#define PQUEUE_H

#define PQMAX 2000

typedef struct PQueueNode {
    int priority;
    void *data;
} pqnode_t;

typedef struct PQueue {
    pqnode_t *array;
    int size;
} pqueue_t;

int pq_init(pqueue_t *queue);
int pq_destroy(pqueue_t *queue);
int pq_destroy_static(pqueue_t *queue);
int pq_enqueue(pqueue_t *queue, void *data, int priority);
int pq_swap(pqueue_t *queue, int a, int b);
int pq_dequeue(pqueue_t *queue, void **data);
int pq_dequeue_node(pqueue_t *queue, int index);
int pq_peek(pqueue_t *queue, void **data);
int pq_size(pqueue_t *queue);
int pq_isEmpty(pqueue_t *queue);

#endif