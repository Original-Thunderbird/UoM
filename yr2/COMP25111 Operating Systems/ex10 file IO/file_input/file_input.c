// Another file operation demonstrator

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*----------------------------------------------------------------------------*/

int main (int argc, char *argv[])
{
FILE *f_input;
char c;
char *filename;
int i, start, length;

if (argc < 4) printf("You need three arguments:\n  a filename\n"
                     "  two decimal values: start and length.\n");
else
  {					// With the suggested file (Hamlet.txt)
  filename = argv[1];			// For clarity
  start  = atoi(argv[2]);		// Try 88840
  length = atoi(argv[3]);		// and 382

  printf("Excerpt from %s -- start position %d for %d characters:\n\n",
          filename, start, length);

  f_input = fopen(filename, "r");	// Open the specified file (for reading)

  if (f_input == NULL)
    printf("I'm sorry Dave, I'm afraid I can't do that.\n");
  else
    {
    fseek(f_input, start, SEEK_SET);	// Move to a chosen location

    for (i = 0; i < length; i++)	// Loop to read and output
      {
      c = fgetc(f_input);		// Get next character
      if (feof(f_input)) break;		// Escape from loop if EOF
//    if (c == EOF) break;		// An -alternative- test
      printf("%c", c);
      }

    printf("\n\nFile position now %ld\n", ftell(f_input));

    fclose(f_input);			// Close the file again
    }

  }

exit(0);
}

/*----------------------------------------------------------------------------*/
