/* getmem.c - getmem */

#include <xinu.h>

/**
 * Allocate heap storage, returning lowest word address
 * @param nbytes	size of memory requested
 * @return pointer to first byte of allocated block
 */
void	*malloc(uint32 nbytes)
{
	return (void*) getmem(nbytes);
}


/**
 * Allocate heap storage, returning lowest word address
 * @param nbytes	size of memory requested
 * @return pointer to first byte of allocated block
 */
char  	*getmem(uint32 nbytes)
{
	intmask	mask;			/* saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* use memblk multiples	*/

	prev = &memlist;
	curr = memlist.mnext;
	while (curr != NULL) {			/* search free list	*/

		if (curr->mlength == nbytes) {	/* block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;
			restore(mask);
			return (char *)(curr);

		} else if (curr->mlength > nbytes) { /* split big block	*/
			leftover = (struct memblk *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;
			restore(mask);
			return (char *)(curr);
		} else {			/* move to next block	*/
			prev = curr;
			curr = curr->mnext;
		}
	}
	restore(mask);
	return (char *)SYSERR;
}
