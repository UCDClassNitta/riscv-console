#ifndef QUEUES_H
#define QUEUES_H
#include <stdint.h>
#include <stdlib.h>
#include "RVCOS.h"
/**
 * @brief The queue uses a doubly linked list implementation
 * Treat it as only exposing the PriorityQueue interface and the 2 functions
 * Avoid directly accessing the PQ nodes
 */
typedef struct _PriorityQueueNode
{
  uint32_t tcb_id;
  struct _PriorityQueueNode *prev;
  struct _PriorityQueueNode *next;
} PriorityQueueNode;

typedef struct
{
  uint32_t size;
  PriorityQueueNode *head;
  PriorityQueueNode *tail;
} PriorityQueue;


/**
 * @brief Enqueues a tcb id to the back of a queue
 *
 * @param target_prio queue we want to put
 * @param tcb_id_ref the tcb id
 */
void enqueue(PriorityQueue *target_prio, uint32_t *tcb_id_ref);

/**
 * @brief Removes the head element of a queue and pass it back by tcb_id_ref
 *
 * @param target_prio queue we want to remove from
 * @param tcb_id_ref the "out" param
 */
void dequeue(PriorityQueue *target_prio, uint32_t *tcb_id_ref);

#endif