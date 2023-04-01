// A template for memory sharing between processes

#include <stdlib.h>		// Libraries which may be needed
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>		// Shared memory definitions

//#include <signal.h>		// Can probably lose this one later @@@

#define KEY  1278		// A 'magic' identifier (any -unique- value OK)
#define SIZE 100		// Size, in bytes
#define PERMISSION 0666		// rwx permissions, as in 'chmod'

unsigned char* create_shm(int, unsigned int, int*);
void release_shm(unsigned char*);
void destroy_shm(int);

/*----------------------------------------------------------------------------*/

main (int argc, char *argv[])	// The 'root' programme; execution start
{
int shm_id;
unsigned char* p_shm;
pid_t new_PID;				// PID return value from fork
int private;

printf("\nHello from \"%s\"!\n", argv[0]);	// My name

p_shm = create_shm(KEY, SIZE, &shm_id);	// Create a shared memory
printf("Shared memory begins at address %016X\n", p_shm);

private = 99;				// Initialise two variables: one local
*p_shm = 99;				//  and one referenced in shared memory.

new_PID = fork();			// Clone another process

if (new_PID < 0)
  {
  printf("ERROR: fork() failed\n");	
  release_shm(p_shm);			// Tidy up if fork failed.
  destroy_shm(shm_id);
  exit(1);				// ... and give up here.
  }
else if (new_PID != 0)

  {					// Parent process
  printf("Parent - Addresses: private %016X   shared %016X\n", &private, p_shm);

  printf("Parent - Data 1: private %d   shared %d\n", private, *p_shm);

  sleep(1);				// Give child time to get ahead

  printf("Parent - Data 2: private %d   shared %d\n", private, *p_shm);

  release_shm(p_shm);		   	// Free shared memory
  destroy_shm(shm_id);			// Clear up before finishing
  exit(0);
  }

else

  {					// Child process
  printf("Child  - Addresses: private %016X   shared %016X\n", &private, p_shm);

  printf("Child  - Data 1: private %d   shared %d\n", private, *p_shm);

  private = 55;				// Change a private value
  *p_shm = 55;				// Change a shared value

  printf("Child  - Data 2: private %d   shared %d\n", private, *p_shm);

  release_shm(p_shm);		   	// Free shared memory
  exit(0);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*             You don't -need- to puzzle through the code below.             */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* Find the specified shared memory - creating it if it does not already      */
/* exist - then attach it to the current process' address space.              */

unsigned char* create_shm(int shm_key, unsigned int shm_size, int *p_shm_id)
{
unsigned char *p_shm;       /* The pointer to (virtual address of) the shared */
                            /* memory block in the process' address space.    */
int shm_id;                 /* An identifier for the particular shared memory.*/

/*   Create or link to the shared memory buffer                               */
shm_id = shmget((key_t)shm_key, shm_size, PERMISSION);
//printf("Existing key value %016X\n", shm_id);

if (shm_id == -1)
  {
  shm_id = shmget((key_t)shm_key, shm_size, IPC_CREAT | IPC_EXCL | PERMISSION);
//printf("New key value %016X\n", shm_id);
  }

if (shm_id == -1)             /* Abject failure to sort out the shared memory */
  {
  perror("ERROR - cannot create shared memory");
  exit(1);
  }

/* There is now some sharable memory; it now must be added to the current     */
/* process' address space.  ("shmat()" = SHared Memory ATtach)                */

if ((p_shm = (unsigned char*) shmat(shm_id, NULL, 0)) == (unsigned char*) -1)
  {
  perror("ERROR - cannot attach shared memory");
  if (shmctl(shm_id, IPC_RMID, NULL) < 0)
    perror("ERROR - cannot destroy shared memory");
  exit(1);
  }

*p_shm_id = shm_id;          /* Copy 'shm_id' (local variable) into caller's  */
                             /* variable using pointer to reference it.       */
return(p_shm);               /* Return virtual address of shared memory.      */
}

/*----------------------------------------------------------------------------*/
/* Retach process from the specified shared memory.                           */

void release_shm(unsigned char *p_shm)
{
if (shmdt(p_shm) < 0)                      /* Detach the shared memory buffer */
  {
  perror("ERROR - cannot release shared memory");
  exit(1);
  }
return;
}

/*----------------------------------------------------------------------------*/
/* Dispose of the shared memory                                               */

void destroy_shm(int shm_id)
{
/* Schedule the shared memory buffer for destruction.  OS removes memory once */
/* all processes have detached.                                               */

if (shmctl(shm_id, IPC_RMID, NULL) < 0)
  {
  perror("ERROR - cannot destroy shared memory");
  exit(1);
  }
return;
}

/*----------------------------------------------------------------------------*/
