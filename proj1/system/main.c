/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

void	printpid()
{
	int i;
	kprintf("Hello XINU WORLD!\r\n");

	for (i=0; i<10; i++)
	{
		kprintf("This is process %d\r\n", currpid);

		//uncomment the line below to see cooperative scheduling
		//resched();
	}
}

void	printargs(uint32 argc, uint32 *argv)
{
	printpid();

	int i;
	if (argc > 0)
	{
		for (i=0; i<argc; i++)
			kprintf("%d\n", argv[i]);
		kprintf("\r\n");
	}
}

int	main(uint32 argc, uint32 *argv)
{
	static uint32 main2args[] = {1, 2, 3};
	static uint32 main3args[] = {10, 20, 30, 40, 50, 60};

        // Create 5 processes
	ready(create((void*) printpid, INITSTK, "MAIN1", 2, 0, NULL), FALSE);
	ready(create((void*) printpid, INITSTK, "MAIN2", 2, 0, NULL), FALSE);
	ready(create((void*) printpid, INITSTK, "MAIN3", 2, 0, NULL), FALSE);
	ready(create((void*) printargs, INITSTK, "MAIN4", 2, 3, main2args), FALSE);
	ready(create((void*) printargs, INITSTK, "MAIN5", 2, 6, main3args), FALSE);

	return 0;
}
