#include "PQueue.h"
#include <stdlib.h>

/**
 * @brief Initialize priority queue
 * 
 * @param queue 
 * @return int 
 */
int pq_init(pqueue_t *queue) {
    queue->size = 0;
    if (!(queue->array = (pqnode_t *)malloc(PQMAX * sizeof(pqnode_t)))) return 1;

    return 0;
}

/**
 * @brief Destroys priority queue and all items in it
 * 
 * @param queue 
 * @return int 
 */
int pq_destroy(pqueue_t* queue) {
    for(int i = 0; i < pq_size(queue); i++) {
        free(queue->array[i].data);
    }

    free(queue->array);
    queue->size = 0;
    return 0;
}

/**
 * @brief Destroys priority queue but doesnt free data
 * 
 * @param queue 
 * @return int 
 */
int pq_destroy_static(pqueue_t* queue) {
    free(queue->array);
    queue->size = 0;
    return 0;
}

/**
 * @brief Queues data into the priority queue
 * 
 * @param queue 
 * @param data 
 * @param priority 
 * @return int 
 */
int pq_enqueue(pqueue_t *queue, void *data, int priority) {
    pqnode_t node = { .data = data, .priority = priority };

    queue->array[pq_size(queue)] = node;

    for(int i = pq_size(queue) - 1; i >= 0; i--) {
        if(priority < (queue->array[i]).priority)
            break;

        pq_swap(queue, i + 1, i);
    }

    queue->size++;

    return 0;
}

/**
 * @brief Swap values in priority queue
 * 
 * @param queue 
 * @param a 
 * @param b 
 * @return int 
 */
int pq_swap(pqueue_t *queue, int a, int b) {
    if(pq_isEmpty(queue) || a > pq_size(queue) || b > pq_size(queue) || a < 0 || b < 0 || a == b) 
        return 1;

    pqnode_t tempNode;
    tempNode = queue->array[a];
    queue->array[a] = queue->array[b];
    queue->array[b] = tempNode;

    return 0;
}

/**
 * @brief Dequeue data in queue with smallest priority value
 * 
 * @param queue 
 * @param data Puts data dequeued into data ptr
 * @return int 
 */
int pq_dequeue(pqueue_t *queue, void **data) {
    if(pq_size(queue) == 0) return 1;

    *data = queue->array[pq_size(queue) - 1].data;
    queue->size--;

    return 0;
}

/**
 * @brief Dequeues a specific node at given index
 * 
 * @param queue 
 * @param index 
 * @return int 
 */
int pq_dequeue_node(pqueue_t *queue, int index) {
    if(pq_size(queue) == 0 || index >= pq_size(queue)) return 1;

    queue->size--;

    for(int i = index; i < pq_size(queue); i++) {
        pq_swap(queue, i, i + 1);
    }

    return 0;
}

/**
 * @brief See the next item to dequeue
 * 
 * @param queue 
 * @param data Puts data dequeued into data ptr
 * @return int 
 */
int pq_peek(pqueue_t *queue, void **data) {
    if(pq_size(queue) == 0) return 1;

    *data = queue->array[pq_size(queue) - 1].data;

    return 0;
}

/**
 * @brief Get size of priority queue
 * 
 * @param queue 
 * @return int 
 */
int pq_size(pqueue_t *queue) {
    return queue->size;
}

/**
 * @brief Checks if priority queue is empty
 * 
 * @param queue 
 * @return int 
 */
int pq_isEmpty(pqueue_t *queue) {
    return queue->size == 0;
}
