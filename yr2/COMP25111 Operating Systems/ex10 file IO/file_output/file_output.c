// A file operation demonstrator.

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*----------------------------------------------------------------------------*/

int main (int argc, char *argv[])
{
FILE *f_out;				// File handle
const char test_filename[] = "test_output.txt";
const char test_string[]   = "Test string\nAnother line\n";
char *p_char;


f_out = fopen(test_filename, "w");	// Open output file (if possible)

if (f_out == NULL) fprintf(stderr, "Failed to open output file\n");
else
  {
  p_char = (char*) test_string;		// Initialise working pointer

  while (*p_char != '\0')		// Loop until string terminator found
    {
//  fputc(*p_char, f_out);		// Put character into file
    fputc(*p_char, stdout);		// This line and the next one ...
//  printf("%c", *p_char);		// ... do the same thing.
//  fflush(stdout);

    sleep(1);				// Pause for visibility
    p_char++;				// Advance along string
    }

  fclose(f_out);
  }

exit(0);
}

/*----------------------------------------------------------------------------*/
