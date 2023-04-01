// A demonstration of C structs, malloc()/free() and a linked list

#include <stdlib.h>		// Libraries which may be needed
#include <stdio.h>

//------------------------------------------------------------------------------

typedef struct PCB_type		// Define a 'template' for a data structure
  {
  int PID;
  int priority;
  struct PCB_type *pNext;
  } PCB;			// End of 'typedef'

// Procedure declarations
PCB* add_entry(int, int, PCB*);
void print_list(PCB*);
void print_structure(PCB*);

//------------------------------------------------------------------------------
// ** Code starts here **

main (int argc, char *argv[])	// The 'root' programme; execution start
{
PCB  dummy;			// Dummy entry - only pointer used (start)
PCB *pEnd;			// End of list
PCB *pTemp;			// Working pointer

dummy.PID      = -1;		// Not used
dummy.priority = -1;		// Not used
dummy.pNext    = NULL;		// Initialise empty list
pEnd           = &dummy;


printf("Dynamic memory investigation programme\n");
printf("======================================\n");

printf("Size of integer: %d\n", sizeof(int));
printf("Size of pointer: %d\n", sizeof(PCB*));

printf("\nStructure 'dummy' at address: %016X\n", &dummy);
print_structure(&dummy);

print_list(dummy.pNext);

// Play with adding entries and seeing the list grow

pEnd = add_entry(1, 5, pEnd);

print_list(dummy.pNext);

pEnd = add_entry(2, 3, pEnd);
pEnd = add_entry(3, 8, pEnd);

print_list(dummy.pNext);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Because we are being well behaved, we clear up the dynamically allocated
// memory before exiting.

printf("\nDispose of list contents...\n");
int entries;
entries = 0;

while (dummy.pNext != NULL)		// Deallocate list items
  {
  pTemp = dummy.pNext;			// Keep pointer to 'struct'
  dummy.pNext = pTemp->pNext;		// Unlink 'struct'
  free(pTemp);				// Dispose of unlinked 'struct'
//printf("Disposed of %016X\n", pTemp);	// Uncomment to watch the destruction
  entries = entries + 1;
  }

printf("Disposed of %d entr", entries);	// We like to print tidily!
if (entries == 1) printf("y.\n"); else printf("ies.\n");

print_list(dummy.pNext);

exit(0);				// ... and relax.

}

//------------------------------------------------------------------------------
// Add an entry to the end of the list; return a pointer to the new last entry

PCB* add_entry(int PID, int priority, PCB* pEnd)
{
PCB *pTemp;

pTemp = malloc(sizeof(PCB));		// Allocate adequate memory

pTemp->PID      = PID;			// Insert the various data
pTemp->priority = priority;
pTemp->pNext    = NULL;			// This will terminate the extended list

pEnd->pNext     = pTemp;		// *Finally* link on the new item

return pTemp;				// Return the new 'pEnd' value
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Print out the list we have built

void print_list(PCB *pLocal)
{
int entries;

entries = 0;				// Commmenting left for the reader
printf("\n----------");
printf("\nDisplaying linked list\n");

while (pLocal != NULL)
  {
  entries = entries + 1;
  printf("\nEntry %d contains PID %d with priority %d\n", entries,
                                             pLocal->PID, pLocal->priority);
  print_structure(pLocal);
  pLocal = pLocal->pNext;
  }
if (entries == 1) printf("\n1 entry found.\n----------\n");
else              printf("\n%d entries found.\n----------\n", entries);
return;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Print the addresses of the different items in the instance

void print_structure(PCB *pLocal)
{
int entries;
printf("Address of entry: %016X\n", pLocal);
printf("Address of PID field:      %016X\n", &(pLocal->PID));
printf("Address of priority field: %016X\n", &(pLocal->priority));
printf("Address of pNext field:    %016X\n", &(pLocal->pNext));
return;
}

//==============================================================================
