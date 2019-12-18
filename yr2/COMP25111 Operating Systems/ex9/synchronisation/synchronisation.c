// A simple sequencing exercise

#define  FAMILY     10		// A text-substitution definition
#define  FAVOURITE   5          // Should be 0 <= FAVOURITE < FAMILY

#include <stdlib.h>		// Libraries which may be needed
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int find_child(pid_t);

/*----------------------------------------------------------------------------*/

/* Global array variable */
pid_t child_PID[FAMILY];	// PID return value from fork

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int main (int argc, char *argv[])
{
  pid_t new_PID;		// PID return value from fork
  int parent;			// C has no explicit inbuilt 'boolean' type
  int i;

  printf("Hello from %s ", argv[0]);// My name
  printf("with PID %d ", getpid()); // My Unix identity
  printf("whose parent is %d\n", getppid()); // My mum's PID

  parent = (0==0);		// i.e. 'TRUE'
  for (i = 0; i < FAMILY; i++)
    {
    new_PID = fork();		// Make another process
    parent = (new_PID != 0);
    if (parent) child_PID[i] = new_PID;
    else break;			// Children esape from loop
    }

  if (parent)			// Parent runs this branch
    {
    int status;			// A place for status return

    printf("[%d]: I now have %d children:\n", getpid(), FAMILY);
    for (i = 0; i < FAMILY; i++)
      printf("   %d\n", child_PID[i]);
/*
    waitpid(child_PID[FAVOURITE], &status, 0);// Wait for one child state change
*/
for (i = 0; i < FAMILY; i++)// Barrier
  {
  new_PID = wait(&status);	// Wait for one child state change
  printf("\n[%d]: Child %d (PID %d) has exited ",
           getpid(), find_child(new_PID), new_PID);
  if (status == NULL) printf("normally.\n");
  else                printf("badly.\n");
  }		// See "man 2 wait" if you want to diagnose any problems.

    printf("\n[%d]: Goodbye from parent process.\n", getpid());
    }//if

  else

    {				// Child runs this branch (i set by 'break')
    printf("I am %d, child %d of %d\n", getpid(), i, getppid());

    sleep(i);			// Take a nap!  Don't rush ...

    printf("\nGoodbye from child %d with PID %d\n", i, getpid());
    }//else

  exit(0);			// A way of stopping this process
				// The '0' indicates "okay"
}//main

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* Nasty bit of C programming.  Returns -1 if not found. (Shouldn't happen!)  */
/* May be used in later parts of exercise.                                    */

int find_child(pid_t pid)
{
int j;

for (j = (FAMILY - 1); j >= 0; j--) if (child_PID[j] == pid) break;
return j;
}

/*----------------------------------------------------------------------------*/
