#include "queues.h"
/**
 *
 * @brief Enqueues a tcb id to the back of a queue
 *
 * @param target_prio queue we want to put
 * @param tcb_id the tcb id
 */
void enqueue(PriorityQueue *target_prio, uint32_t tcb_id) {
  PriorityQueueNode *head_node = target_prio->head;
  if (head_node == NULL) {
    head_node = (PriorityQueueNode *)malloc(sizeof(PriorityQueueNode));
    head_node->next = NULL;
    head_node->prev = NULL;
    head_node->tcb_id = tcb_id;

    target_prio->head = head_node;
    target_prio->tail = head_node;
  } else {
    PriorityQueueNode *tail_node = target_prio->tail;
    tail_node->next = (PriorityQueueNode *)malloc(sizeof(PriorityQueueNode));
    tail_node->next->tcb_id = tcb_id;
    tail_node->next->prev = tail_node;
    tail_node->next->next = NULL;
    target_prio->tail = tail_node->next;
  }
  target_prio->size++;
}

/**
 * @brief Removes the head element of a queue and pass it back by tcb_id_ref
 *
 * @param target_prio queue we want to remove from
 * @param tcb_id_ref the "out" param
 */
void dequeue(PriorityQueue *target_prio, uint32_t *tcb_id_ref) { 
  PriorityQueueNode *head_node = target_prio->head;
  if (head_node == NULL || target_prio == NULL) {
    return;
  }
  if (head_node->next == NULL) {
    target_prio->head = target_prio->tail = NULL;
  } else {
    head_node->next->prev = NULL;
    target_prio->head = head_node->next;
  }
  *tcb_id_ref = head_node->tcb_id;
  target_prio->size--;
  free(head_node);
}