// A template for investigation into shared memory

#include <stdlib.h>     // Libraries which may be needed
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>   // mmap functions
#include <sys/types.h>  // the flags used by mmap()

/*----------------------------------------------------------------------------*/

// run "man 3 mmap" for more information on these flags
#define SIZE 100                          // Size, in bytes
#define PERMISSION PROT_READ | PROT_WRITE // rw- permissions, as in 'chmod'
#define FLAGS MAP_SHARED | MAP_ANONYMOUS  // share the memory

/*----------------------------------------------------------------------------*/

void beParent();
void beChild();

/*----------------------------------------------------------------------------*/

unsigned char* sharedMemory;       // Pointer to our shared memory
int            private;            // memory private to each process

/*----------------------------------------------------------------------------*/

// The 'root' programme; execution start
main (int argc, char *argv[]) {
  // Create some shared memory
  sharedMemory = mmap(NULL, SIZE, PERMISSION, FLAGS, -1, 0);

  // print some info
  printf("Hello from %s!\n", argv[0]);	// My name
  printf("Pointer is: %016X\n", sharedMemory);

  // Initialise two variables
  private = 99;
  *sharedMemory = 99;

  // Clone this program into another process
  pid_t new_PID = fork();

  // make sure there wasn't any forking errors
  if (new_PID < 0) {
    printf("ERROR: fork() failed\n");
    munmap(sharedMemory, SIZE);
    exit(1);
  }

  // decide if we are the parent, or the child
  if (new_PID != 0)
    beParent(); // do parent process things
  else
    beChild(); // do child process things
}

/*----------------------------------------------------------------------------*/

void beParent() {
  // print out some data about the shared memory
  printf("Parent - Addresses: private %016p   shared %016p\n", &private, sharedMemory);
  printf("Parent - Test 1: private %d   shared %d\n", private, *sharedMemory);
  printf("Parent - going to sleep\n");

  // wait for the child to do it's thing
  sleep(1);

  printf("Parent - Test 2: private %d   shared %d\n", private, *sharedMemory);

  // Free shared memory & quit
  munmap(sharedMemory, SIZE);
  exit(0);
}

void beChild() {
  // print out some data about the shared memory
  printf("Child  - Addresses: private %016p   shared %016p\n", &private, sharedMemory);
  printf("Child  - Test 1: private %d   shared %d\n", private, *sharedMemory);

  // set the shared memory to new values
  private = 55;
  *sharedMemory = 55;
  printf("Child  - Test 2: private %d   shared %d\n", private, *sharedMemory);

  // quit
  exit(0);
}

/*----------------------------------------------------------------------------*/
