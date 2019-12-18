// A template for memory sharing between processes

#include <stdlib.h>		// Libraries which may be needed
#include <stdio.h>		// IO library
#include <unistd.h>
#include <sys/shm.h>		// Shared memory definitions
#include <string.h>		// String manipulation library

#define KEY  1278		// A 'magic' identifier (any -unique- value OK)
#define SIZE 100		// Size, in bytes
#define PERMISSION 0666		// rwx permissions, as in 'chmod'

unsigned char* create_shm(int, unsigned int, int*);
void release_shm(unsigned char*);
void destroy_shm(int);
int i;
/*----------------------------------------------------------------------------*/

main (int argc, char *argv[])	// The 'root' programme; execution start
{
int shm_id[10];
unsigned char* p_shm[10];
pid_t new_PID;				// PID return value from fork
for(i=0;i<10;i++){
    p_shm[i] = create_shm(KEY+i, SIZE, &(shm_id[i]));	// Create a shared memory
    printf("Receiver's shared memory is at address: %016X\n", p_shm[i]);
    p_shm[i][0] = 0;				// Flag that message has (not) arrived.
}
while (p_shm[1][0] == 0) sleep(1);		// Poll for message
i=0;
while(p_shm[i][0]){
    printf("Receiver picked up message:: \"%s\"\n", &p_shm[i][1]);
    i++;
}
/* "&p_shm[1]" ?!  The address of (&) character 1 ([1]) in array "p_shm".     */
for(i=0;i<10;i++){
  p_shm[i][0] = 0;				// Message is finished with
}
for(i=0;i<10;i++){
    release_shm(p_shm[i]);		   	// Free shared memory
    destroy_shm(shm_id[i]);			// ... and destroy it
}
exit(0);
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
