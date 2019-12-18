// An extra example demonstration programme

#include <stdlib.h>		// Libraries which may be needed
#include <stdio.h>

pid_t new_PID;

main (int argc, char *argv[])	// The 'root' programme; execution start
{

printf("\n***************************\n");
printf(  "*  A different programme  *\n");
printf(  "***************************\n\n");

printf("Hello from %s!\n", argv[0]);
printf("My PID is %d\n", getpid());

printf("Goodbye from %s with PID %d\n", argv[0], getpid());

exit(0);
}

