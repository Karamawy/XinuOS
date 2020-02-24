/* suspend.c - suspend */

#include <xinu.h>

/**
 * Suspend a process, placing it in hibernation
 * @param pid	ID of process to suspend
 */
syscall	suspend(pid32 pid)
{
	intmask	mask;			// saved interrupt mask
	struct	procent *prptr;		// ptr to process' table entry

	mask = disable();
	if (isbadpid(pid) || (pid == NULLPROC)) {
		restore(mask);
		return SYSERR;
	}

	// Only suspend a process that is current or ready
	prptr = &proctab[pid];
	if ((prptr->prstate != PR_CURR) && (prptr->prstate != PR_READY)) {
		restore(mask);
		return SYSERR;
	}
	if (prptr->prstate == PR_READY) {
		remove(pid, readyqueue);    // remove a ready process from the ready list
		prptr->prstate = PR_SUSP;
	} else {
		prptr->prstate = PR_SUSP;   // mark the current process
		resched();		    // suspended and reschedule
	}
	restore(mask);
	return OK;
}
