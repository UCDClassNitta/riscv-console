#include "queues.h"
/**
 *
 * @brief Enqueues a tcb id to the back of a queue
 *
 * @param target_prio queue we want to put
 * @param tcb_id_ref the tcb id
 */
void enqueue(PriorityQueue *target_prio, uint32_t *tcb_id_ref)
{
    PriorityQueueNode *head_node = target_prio->head;
    if (head_node == NULL)
    {
        head_node = (PriorityQueueNode *)malloc(sizeof(PriorityQueueNode));
        head_node->tcb_id = *tcb_id_ref;
        target_prio->tail = head_node;
    }
    else
    {
        PriorityQueueNode *tail_node = target_prio->tail;
        tail_node->next = (PriorityQueueNode *)malloc(sizeof(PriorityQueueNode));
        tail_node->next->tcb_id = *tcb_id_ref;
        tail_node->next->prev = tail_node;
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
void dequeue(PriorityQueue *target_prio, uint32_t *tcb_id_ref)
{

    PriorityQueueNode *head_node = target_prio->head;
    if (head_node == NULL)
    {
        return;
    }
    if (head_node->next == NULL)
    {
        target_prio->head = target_prio->tail = NULL;
    }
    else
    {
        head_node->next->prev = NULL;
        target_prio->head = head_node->next;
    }

    *tcb_id_ref = head_node->tcb_id;

    free(head_node);
}