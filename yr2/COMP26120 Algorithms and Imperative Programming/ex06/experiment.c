#include "search.h"
int getNextString(char *buffer, int size, FILE* stream) {

    int ch = fgetc(stream);
    // find the left quote
    while (ch != EOF && ch != '"')
        ch = fgetc(stream);

    // EOF or parsing error
    if (ch != '"')
        return -1;

    int nbytes = 0;
    const int maxSz = size-1;
    ch = fgetc(stream);
    while (nbytes != maxSz && ch != '"') {
        buffer[nbytes] = ch;
        ch = fgetc(stream);
        ++nbytes;
    }

    buffer[nbytes] = 0;
    return nbytes;
}


int main(){
  int num_db,num_qy,i,j;
  FILE *in=fopen("data.txt","r+");
  fscanf(in, "%d %d", &num_db, &num_qy);
  num_qy = 100;
  char *str_base = (char*)malloc(num_db*26), *str_qury = (char*)malloc(num_qy*26);
  for(i=0;i<num_db;i++){
    getNextString(str_base+i*26, 26, in);
  }
  for(i=0;i<num_qy;i++){
    getNextString(str_qury+i*26, 26, in);
  }
  time_t start,end;

  start =time(NULL);
  for(i=0;i<num_qy;i++){
    for(j=0;j<num_db;j++){
      if(!strcmp(str_base+j*26,str_qury+i*26)){
        break;
      }
    }
  }
  end =time(NULL);
  printf("linear:time=%f\n",difftime(end,start));

  start =time(NULL);//or time(&start);
  mergesort(str_base,0,num_db-1);
  for(i=0;i<num_qy;i++){
    binSearch(str_base, str_qury+i*26, 0, num_db-1);
  }
  end =time(NULL);
  printf("binary:time=%f\n",difftime(end,start));
}
