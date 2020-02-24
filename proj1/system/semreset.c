/* semreset.c - semreset */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  semreset  --  reset a semaphore's count and release waiting processes
 * @param sem	ID of semaphore to reset
 * @param count	new count (must be >= 0)
 */
syscall	semreset(sid32 sem, int32 count)
{
	intmask	mask;			/* saved interrupt mask		*/
	struct	sentry *semptr;		/* ptr to semaphore table entry */
	pid32	pid;			/* ID of a waiting process	*/
	struct queue *semqueue; 	/* semaphore's process queue */

	mask = disable();

	if (count < 0 || isbadsem(sem) || semtab[sem].sstate==S_FREE) {
		restore(mask);
		return SYSERR;
	}

	semptr = &semtab[sem];
	semqueue = semptr->squeue;	/* free any waiting processes */
	while ((pid=getfirst(semqueue)) != EMPTY)
		ready(pid, RESCHED_NO);
	semptr->scount = count;		/* reset count as specified */
	resched();
	restore(mask);
	return OK;
}
