#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// A struct to record pairs of strings
typedef struct pair {
  char* first;
  char* second;
} pair;


// processes a work pair by first storing in
// temp a version of work.first that is all
// lowercase and then storing in work.second
// a version where the first character is
// made uppercase 
void process(pair *work){
  work->second = (char*)malloc((strlen(work->first)+1)*sizeof(char));
  int i;
  for(i=0;i<strlen(work->first);i++){
    char c = work->first[i];
    if(isupper(c)){
      work->second[i] = tolower(c);
    }
    else{
      work->second[i] = c; 
    }
  }
  work->second[i]='\0';
  int last_space = 1;
  char* ptr = work->second;
  while(*ptr){
    if(last_space){
      *ptr = toupper(*ptr);
    }
    last_space = *ptr == ((char) ' '); 
    ptr++;
  }
}

// Takes a single string and writes the result to stdout
int main(int argc, char **argv){
  pair work;
  work.first = argv[1];
  process(&work);

  printf("%s becomes %s\n",work.first,work.second);
  free(work.second);
}
