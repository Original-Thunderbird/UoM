#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>				// For ctime()
#include <errno.h>				// Needed to identify any error

int main(int argc, char *argv[]){
  struct stat buffer;
  if (argc < 2) printf("Needs an argument.\n");
  else{
    if (lstat(argv[1], &buffer) == -1){
      if(errno == ENOENT){
        printf("No such directory.\n");
        return 0;
      }
    }
    else if(!S_ISDIR(buffer.st_mode)){
      printf("Not a directory.\n");
      return 0;
    }
  }
  FILE *f_out;				// File handle
  char filename[200]="\0";
  strcpy(filename,argv[1]);
  strcat(filename,"/out.txt");
  printf("%s\n",filename );
  const char test_string[]   = "Test string\nAnother line\n";
  char *p_char;
  f_out = fopen(filename, "w");	// Open output file (if possible)
  if (f_out == NULL) fprintf(stderr, "Failed to open output file\n");
  fprintf(f_out, "%s\n",test_string );
  fclose(f_out);
}
