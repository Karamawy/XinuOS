/* resched.c - resched */

#include <xinu.h>

/**
 * Reschedule processor to next ready process
 *
 */
void	resched(void)		// assumes interrupts are disabled
{
	struct procent *ptold;	// ptr to table entry for old process
	struct procent *ptnew;	// ptr to table entry for new process

	// If rescheduling is deferred, record attempt and return
	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	// Point to process table entry for the current (old) process
	ptold = &proctab[currpid];


	// TODO - check ptold's state. If it's running, put it on the ready queue and change state to ready
	if(ptold->prstate==PR_CURR){
		enqueue(currpid,readyqueue);
		ptold->prstate=PR_READY;
	}
	// TODO - dequeue next process off the ready queue and point ptnew to it
	pid32 pid =dequeue(readyqueue);
	ptnew = &proctab[pid];
	// TODO - change its state to "current" (i.e., running)
	ptnew->prstate=PR_CURR;
	// TODO - set currpid to reflect new running process' PID
	currpid=pid;
	// Context switch to next ready process
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);
	//kprintf("RESCHED SH3'ala");
	// Old process returns here when resumed
	return;
}
