// A template for investigation process forking

#include <stdlib.h>		// Libraries which may be needed
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

struct sigaction sigaction_structure;	// A data structure, filled in below

void goodbye();				// Declaration only - definition below

//------------------------------------------------------------------------------

int num;					// A global variable

//------------------------------------------------------------------------------

main (int argc, char *argv[])	// The 'root' programme; execution start
{
pid_t new_PID;			// PID return value from fork

printf("Hello from %s!\n", argv[0]);// My name
printf("My PID is %d\n", getpid()); // My Unix identity

new_PID = fork();		// The call we are exercising

printf("That call returned the value %d to process %d\n", new_PID, getpid());

//    The statement below may be useful later
//    if (new_PID == 0) execlp("./baby", "baby", (char*)NULL);

// Set three fields in a predefined 'struct'
sigaction_structure.sa_handler = &goodbye;	// Pointer to a -function-
sigemptyset(&sigaction_structure.sa_mask);	// Call defines 'empty' field
sigaction_structure.sa_flags   = 0;		// No flags

// Link the structure to signal "SIGINT" (as generated by ^C)
sigaction(SIGCHLD, &sigaction_structure, NULL); // Link

sleep(10);
printf("Goodbye from %s with PID %d\n", argv[0], getpid());

exit(0);			// A way of stopping this process
				// The '0' indicates "okay"
}

void goodbye(int signum)
{
  printf("\nGoodbye at count %d\n", num);

  exit(1);
}