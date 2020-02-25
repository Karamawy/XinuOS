/* queue.c - enqueue, dequeue, isempty, nonempty, et al. */

#include <xinu.h>
#include <stdlib.h>

/**
 * Prints out contents of a queue
 * @param q	pointer to a queue
 */
void	printqueue(struct queue *q)
{
	//TODO - print all contents from head to tail
	//TODO - format should be [(pid=p1), (pid=p2), ...]
	struct qentry *entry = q->head;
	struct qentry *last = q->tail;
	if(entry==last&&entry==NULL)return;
	kprintf("[");
	while(entry!=last){
		kprintf("(pid=%d), ",entry->pid);
		entry=entry->next;
	}
	kprintf("(pid=%d)]",last->pid);
}

/**
 * Checks whether queue is empty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	isempty(struct queue *q)
{
	//TODO
	return (!q||!q->size||!q->head);
	//kprintf("is empty sh3'ala\n");
}

/**
 * Checks whether queue is nonempty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	nonempty(struct queue *q)
{
	//TODO - don't just check q's size because q could be NULL
	//kprintf("non empty sh3'ala\n");
	return (q&&q->size&&q->head);
}


/**
 * Checks whether queue is full
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8	isfull(struct queue *q)
{
	//TODO - check if there are at least NPROC processes in the queue
	return (q->size==NPROC);
}


/**
 * Insert a process at the tail of a queue
 * @param pid	ID process to insert
 * @param q	Pointer to the queue to use
 *
 * @return pid on success, SYSERR otherwise
 */
pid32 enqueue(pid32 pid, struct queue *q)
{
        //TODO - check if queue is full and if pid is illegal, and return SYSERR if either is true
		if(isfull(q)||isbadpid(pid)) return SYSERR;
        //TODO - allocate space on heap for a new QEntry
		struct qentry* new = (struct qentry*)malloc(sizeof(struct qentry));
        //TODO - initialize the new QEntry
		new->pid=pid;
		new->next=NULL;
		new->prev=q->tail;
		q->tail->next=new;
        //TODO - insert into tail of queue
		if(isempty(q)){
			q->head=q->tail=new;
		}
		else {
			q->tail=new;
		}
        //TODO - return the pid on success
		//kprintf("enqueue sh3'ala\n");
		q->size++;
		return pid;
}


/**
 * Remove and return the first process on a list
 * @param q	Pointer to the queue to use
 * @return pid of the process removed, or EMPTY if queue is empty
 */
pid32 dequeue(struct queue *q)
{		
        //TODO - return EMPTY if queue is empty
		if(isempty(q))return EMPTY;
        //TODO - get the head entry of the queue
        //TODO - unlink the head entry from the rest
        //TODO - free up the space on the heap
        //TODO - return the pid on success
		//kprintf("pid el dequee %d\n",pid);
		return getfirst(q);
}


/**
 * Finds a queue entry given pid
 * @param pid	a process ID
 * @param q	a pointer to a queue
 * @return pointer to the entry if found, NULL otherwise
 */
struct qentry *getbypid(pid32 pid, struct queue *q)
{
	//TODO - return NULL if queue is empty or if an illegal pid is given
	if(isempty(q)||isbadpid(pid))return NULL;
	//TODO - find the qentry with the given pid
	//TODO - return a pointer to it
	struct qentry* tmp = q->head;
	while(tmp){
		if(tmp->pid==pid)return tmp;
		tmp=tmp->next;
	}
	//kprintf("get by id sh3'ala\n");
	return NULL;
}

/**
 * Remove a process from the front of a queue (pid assumed valid with no check)
 * @param q	pointer to a queue
 * @return pid on success, EMPTY if queue is empty
 */
pid32	getfirst(struct queue *q)
{
	//TODO - return EMPTY if queue is empty
	if(isempty(q))return EMPTY;
	//TODO - remove process from head of queue and return its pid
	struct qentry* tmp = q->head;
	pid32 pid= tmp->pid;
	if(q->size==1){
			q->tail=q->head=NULL;
			free(tmp,sizeof(tmp));
			return pid;
		}
	q->head=tmp->next;
	q->head->prev=NULL;
	free(tmp,sizeof(tmp));
	q->size--;
	//kprintf("get first sh3'ala\n");
	return pid;
}	

/**
 * Remove a process from the end of a queue (pid assumed valid with no check)
 * @param q	Pointer to the queue
 * @return pid on success, EMPTY otherwise
 */
pid32	getlast(struct queue *q)
{
	//TODO - return EMPTY if queue is empty
	if(isempty(q))return EMPTY;
	//TODO - remove process from tail of queue and return its pid
	struct qentry* tmp = q->tail;
	pid32 pid= tmp->pid;
	if(q->size==1){
			q->tail=q->head=NULL;
			free(tmp,sizeof(tmp));
			return pid;
		}
	q->tail=tmp->prev;
	q->tail->next=NULL;
	free(tmp,sizeof(tmp));
	q->size--;
	//kprintf("get last sh3'ala\n");
	return pid;
}



/**
 * Remove a process from an arbitrary point in a queue
 * @param pid	ID of process to remove
 * @param q	Pointer to the queue
 * @return pid on success, SYSERR if pid is not found
 */
pid32	remove(pid32 pid, struct queue *q)
{
	//TODO - return EMPTY if queue is empty
	if(isempty(q))return EMPTY;
	//TODO - return SYSERR if pid is illegal
	if(isbadpid(pid)) return SYSERR;
	//TODO - remove process identified by pid parameter from the queue and return its pid
	if(q->head->pid==pid)return getfirst(q);
	if(q->tail->pid==pid)return getlast(q);
	struct qentry* tmp = q->head;
	while(tmp){
		if(tmp->pid==pid){
			tmp->prev->next=tmp->next;
			tmp->next->prev=tmp->prev;
			free(tmp,sizeof(tmp));
			q->size--;
			return pid;
		}
		tmp=tmp->next;
	}
	//kprintf("remove sh3'ala\n");
	//TODO - if pid does not exist in the queue, return SYSERR
	return SYSERR;
}
