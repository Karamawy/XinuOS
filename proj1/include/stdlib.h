/**
 * @file stdlib.h
 *
 * $Id: stdlib.h 2051 2009-08-27 20:55:09Z akoehler $
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#define	RAND_MAX 077777

int	abs(int);
int32	labs(int32);
int	atoi(char *);
int32	atol(char *);
void	bzero(void *, int);
void	qsort(char *, unsigned int, int, int (*)(void));
uint32	rand(void);
void	srand(unsigned int);
