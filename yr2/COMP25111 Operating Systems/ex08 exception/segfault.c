//------------------------------------------------------------------------------
// A simplified demonstration of signal handling; part 3

#include <stdlib.h>			// Libraries which may be needed
#include <stdio.h>
#include <signal.h>

struct sigaction sigaction_structure;	// A data structure, filled in below

void broken();				// Declaration only - definition below


//------------------------------------------------------------------------------

int num;				// A global variable
int *ptr;				//  and a pointer

//------------------------------------------------------------------------------

int main (int argc, char *argv[])	// The 'root' programme; execution start
{
  
  // Set three fields in a predefined 'struct'
  sigaction_structure.sa_handler = &broken;	// Pointer to a -function-
  sigemptyset(&sigaction_structure.sa_mask);	// Call defines 'empty' field
  sigaction_structure.sa_flags   = 0;		// No flags

  // Link the structure to signal "SIGSEGV"
  sigaction(SIGSEGV, &sigaction_structure, NULL);	// Link


  num = 0x13579;				// A reminder about pointers
  ptr = &num;					// Point at the variable
  printf("See if this pointer %016X", ptr);
  printf(" works: %08X\n", *ptr);	// Dereferencing, getting the value that
					// is stored in the memory location
					// pointed to by the pointer.
   printf("It must be okay.\n");

  ptr = NULL;				// SHOULDN'T be dereferenced ***
  printf("This will still work %016X", ptr); // Print the address that the ptr
					// points to. ptr is NULL, which,
					// In C99 can be portably expressed as
					// the integer value 0.
  printf("but this bit won't: %08X\n", *ptr);// Dereferencing NULL causes a
					// segmentation fault.
  exit(0);				// Won't get to here!
}//main

//------------------------------------------------------------------------------

void broken(int signum)	// The argument indicates which signal was activated
{
  printf(" but you are not allowed to go there.\n");

  switch (signum)	// signum values are defined in "signal.h"
  {
    case SIGINT:  printf("You must restore something to print this.\n"); break;
    case SIGSEGV: printf("You must not trespass.\n"); break;
    default:      printf("You'll have to write some more traps.\n"); break;
  }

  exit(1);
}//broken

//------------------------------------------------------------------------------
