/* resume.c - resume */

#include <xinu.h>

/**
 * Unsuspend a process, making it ready
 * @param pid  ID of process to unsuspend
 *
 */
status	resume(pid32 pid)
{
	intmask	mask;			/* saved interrupt mask		*/
	struct	procent *prptr;		/* ptr to process' table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}
	prptr = &proctab[pid];
	if (prptr->prstate != PR_SUSP) {
		restore(mask);
		return SYSERR;
	}
	ready(pid, RESCHED_YES);
	restore(mask);
	return OK;
}
