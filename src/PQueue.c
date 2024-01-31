#include "PQueue.h"
#include <stdlib.h>

int pq_init(pqueue_t *queue) {
    queue->size = 0;
    if (!(queue->array = (pqnode_t *)malloc(PQMAX * sizeof(pqnode_t)))) return 1;

    return 0;
}

int pq_destroy(pqueue_t *queue) {
    free(queue->array);
    queue->size = 0;
    return 0;
}

int pq_enqueue(pqueue_t *queue, void *data, int priority) {
    pqnode_t node = { .data = data, .priority = priority };

    queue->array[pq_size(queue)] = node;

    for(int i = pq_size(queue) - 1; i >= 0; i--) {
        if(priority <= (queue->array[i]).priority)
            break;

        pq_swap(queue, i + 1, i);
    }

    queue->size++;

    return 0;
}

int pq_swap(pqueue_t *queue, int a, int b) {
    if(pq_isEmpty(queue) || a > pq_size(queue) || b > pq_size(queue) || a < 0 || b < 0 || a == b) 
        return 1;

    pqnode_t tempNode;
    tempNode = queue->array[a];
    queue->array[a] = queue->array[b];
    queue->array[b] = tempNode;

    return 0;
}

int pq_dequeue(pqueue_t *queue, void **data) {
    if(pq_size(queue) == 0) return 1;

    *data = queue->array[pq_size(queue) - 1].data;
    queue->size--;

    return 0;
}

int pq_peek(pqueue_t *queue, void **data) {
    if(pq_size(queue) == 0) return 1;

    *data = queue->array[pq_size(queue) - 1].data;

    return 0;
}

int pq_size(pqueue_t *queue) {
    return queue->size;
}

int pq_isEmpty(pqueue_t *queue) {
    return queue->size == 0;
}
