/* newqueue.c - newqueue */

#include <xinu.h>
#include <stdlib.h>

/**
 * Allocate and initialize a queue
 * @return pointer to queue structure
 */
struct queue *newqueue(void)
{
	// TODO - allocate memory for a new Queue structure (required to be on heap)
	struct queue* q = (struct queue*) malloc(sizeof(struct queue));
	// TODO - initialize the structure
	q->size=0;
	q->head=NULL;
	q->tail=NULL;
	// TODO - return pointer to the structure
	return q;
}
