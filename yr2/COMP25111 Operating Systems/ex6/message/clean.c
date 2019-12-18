// A utility to remove a known area of shared memory which has accidentally
// been left behind.

#include <stdlib.h>		// Libraries which may be needed
#include <stdio.h>		// IO library
#include <unistd.h>
#include <sys/shm.h>		// Shared memory definitions

#define KEY  1278		// A 'magic' identifier (any -unique- value OK)
#define SIZE 100		// Size, in bytes
#define PERMISSION 0666		// rwx permissions, as in 'chmod'

/*----------------------------------------------------------------------------*/

main (int argc, char *argv[])	// The 'root' programme; execution start
{
int shm_id;                       /* The 'handle' of the shared memory block. */

/* Find the ID of the memory block if it currently exists.                    */
shm_id = shmget(KEY, SIZE, PERMISSION);

if (shm_id == -1) printf("No shared memory was left.\n");
else
  {                          /* There was a remnant: schedule for destruction */
  printf("Shared memory found with key %016X\n", shm_id);

  if (shmctl(shm_id, IPC_RMID, NULL) < 0)
    {
    perror("ERROR - cannot destroy shared memory");
    exit(1);
    }
  else printf("Destroyed.\n");
  }

exit(0);
}

/*----------------------------------------------------------------------------*/
