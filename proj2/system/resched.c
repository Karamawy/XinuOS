/* resched.c - resched, resched_cntl */

#include <xinu.h>
#include<stdlib.h>
struct defer Defer;
/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
pid32 tobechoosen;
uint32 last[NPROC];
int16 asfrequency=50;
void sfrequency(int16 freq){
	asfrequency=freq;
}
void aging(void)
{
	//kprintf("AGING\n");
	pid32 tmppid = firstid(readylist); // return pid
	struct procent *pttmp;
	while (tmppid != lastid(readylist))
	{
		pttmp = &proctab[tmppid];
		if (pttmp->spolicy == AGINGSCHED)
		{
			//kprintf("AGING increments\n");
			pttmp->prprio++;
			queuetab[tmppid].qkey = pttmp->prprio;
		}
		tmppid = queuetab[tmppid].qnext;
	}
	// after increminting the values we now want to choose the first aging process
	tmppid= firstid(readylist);
	while (tmppid != lastid(readylist))
	{
		pttmp = &proctab[tmppid];
		if (pttmp->spolicy == AGINGSCHED)
		{
			//kprintf("TO BE CHOOSEN %d\n",tobechoosen);
			tobechoosen=tmppid;
			break;
		}
		tmppid = queuetab[tmppid].qnext;
	}
	return;
}
void prop(void)
{
	pid32 tmppid = firstid(readylist); // return pid
	struct procent *pttmp;
	while (tmppid != lastid(readylist))
	{
		pttmp = &proctab[tmppid];
		if (pttmp->spolicy == PROPORTIONALSHARE){
			tobechoosen=tmppid;
			break;
		}
		tmppid = queuetab[tmppid].qnext;
	}
	return;
}

bool8 agingflag = FALSE, propflag = FALSE;
void check()
{
	pid32 tmppid = firstid(readylist); // return pid
	struct procent *pttmp;
	while (tmppid != lastid(readylist))
	{
		pttmp = &proctab[tmppid];
		kprintf("%s\n",pttmp->prname);
		if (pttmp->spolicy == AGINGSCHED)
			agingflag = TRUE;
		else if (pttmp->spolicy == PROPORTIONALSHARE)
			propflag = TRUE;
		tmppid = queuetab[tmppid].qnext;
	}
	return;
}

void resched(void) /* Assumes interrupts are disabled	*/
{
	struct procent *ptold; /* Ptr to table entry for old process	*/
	struct procent *ptnew; /* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0)
	{
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */
	ptold = &proctab[currpid];
	/*Updating Current Priority*/
	//kprintf("%s\n",ptold->prname);
	if (ptold->spolicy == AGINGSCHED)
	{
		//chprio(currpid, ptold->intprio);
		ptold->prprio=ptold->intprio;
	}
	else if (ptold->spolicy == PROPORTIONALSHARE)
	{
		ptold->pi = ptold->pi + (100 * (clktimems - last[currpid]) / ptold->prprio);
	}

	//check if all process have same policy or not and update the flage
	check();
	if (agingflag && propflag)
	{ //randomization
		int16 schedpolicy=rand()%100;
		if (schedpolicy < asfrequency)
		{
			aging();
		}
		else
		{
			prop();
		}
	}
	else if (agingflag && !propflag)
	{
		aging();
	}
	else if (!agingflag && propflag)
	{
		prop();
	}
	else
	{
		tobechoosen=0; //NULL PROCESS
	}
	struct procent *tobechoosenptr;
	tobechoosenptr=&proctab[tobechoosen];
	if (ptold->prstate == PR_CURR)
	{ 
		//if(ptold->prprio==0)return;
		if(ptold->pi<tobechoosenptr->pi&&tobechoosenptr->spolicy==PROPORTIONALSHARE&&ptold->spolicy==PROPORTIONALSHARE)
			return;
		if(ptold->prprio>tobechoosenptr->prprio&&tobechoosenptr->spolicy==AGINGSCHED&&ptold->spolicy==AGINGSCHED)
			return;
		/* Old process will no longer remain current */
		ptold->prstate = PR_READY;
		ptold->prvstate = PR_CURR;
		if(ptold->spolicy==PROPORTIONALSHARE)
		insert(currpid,readylist,0xFFFFFFFF-ptold->pi);
		else insert(currpid, readylist, ptold->intprio);
	}
	/* Force context switch to highest priority ready process */
	currpid = getitem(tobechoosen);// remove the choosen process from the queue and prepare it for ctxsw
	ptnew = &proctab[currpid];
	if (ptnew->spolicy == PROPORTIONALSHARE){
	if(!last[currpid]||ptnew->prvstate>2)ptnew->pi=ptnew->pi>clktimems?ptnew->pi:clktimems; // if first sched or was blocked
		last[currpid] = clktimems;//set its last sched time
	}
	ptnew->prstate = PR_CURR;		
	preempt = QUANTUM; /* Reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status resched_cntl(			/* Assumes interrupts are disabled	*/
					int32 defer /* Either DEFER_START or DEFER_STOP	*/
)
{
	switch (defer)
	{

	case DEFER_START: /* Handle a deferral request */

		if (Defer.ndefers++ == 0)
		{
			Defer.attempt = FALSE;
		}
		return OK;

	case DEFER_STOP: /* Handle end of deferral */
		if (Defer.ndefers <= 0)
		{
			return SYSERR;
		}
		if ((--Defer.ndefers == 0) && Defer.attempt)
		{
			resched();
		}
		return OK;

	default:
		return SYSERR;
	}
}
