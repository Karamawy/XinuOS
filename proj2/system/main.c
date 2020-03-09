/*  main.c  - main */

#include <xinu.h>

int a, b, c;
process inc4ever();

process	main(void)
{
	pid32 prA, prB, prC;
	int t;

	a = b = c = 0;

	prA = create(inc4ever, 2000, 14, 0, "proc A", 1, 0);
	prB = create(inc4ever, 2000, 12, 0, "proc B", 1, 1);
	prC = create(inc4ever, 2000, 10, 0, "proc C", 1, 2);
	resume(prA);
	resume(prB);
	resume(prC);
	kprintf(" Main sleep(10) ...\n");
	sleep(10);
	kprintf(" Main Woke Up\n");
	kill(prA);
	kill(prB);
	kill(prC);

	t = a + b + c;
	kprintf("Test results: pr0 = %d%%, pr1 = %d%%, pr2 = %d%%  (57%%, 28%%, 14%%).\n",
		a*100/t, b*100/t, c*100/t);

	return OK;
}



process inc4ever(int i)
{
	int j;
	kprintf("  Starting pr%d.\n", i);
	while(1) {
		switch(i) {
			case 0: a++; break;
			case 1: b++; break;
			case 2: c++; break;
		}
	for(j=0; j<100; j++)
		;
	}
}

