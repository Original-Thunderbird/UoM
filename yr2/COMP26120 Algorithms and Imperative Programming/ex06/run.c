#include "search.h"
int getNextString(char *buffer, int size) {

    int ch = getchar();
    // find the left quote
    while (ch != EOF && ch != '"')
        ch = getchar();

    // EOF or parsing error
    if (ch != '"')
        return -1;

    int nbytes = 0;
    const int maxSz = size-1;
    ch = getchar();
    while (nbytes != maxSz && ch != '"') {
        buffer[nbytes] = ch;
        ch = getchar();
        ++nbytes;
    }

    buffer[nbytes] = 0;
    return nbytes;
}


int main(){
  int num_db,num_qy,i, res;
  scanf("%d %d", &num_db, &num_qy);
  char *str_base = (char*)malloc(num_db*26), *str_qury = (char*)malloc(num_qy*26);
  for(i=0;i<num_db;i++){
    getNextString(str_base+i*26, 26);
  }
  for(i=0;i<num_qy;i++){
    getNextString(str_qury+i*26, 26);
  }
  mergesort(str_base,0,num_db-1);
  for(i=0;i<num_qy;i++){
    res = binSearch(str_base, str_qury+i*26, 0, num_db-1);
    if(res>=0){
      printf("yes ");
    }
    else{
      printf("no ");
    }
  }
}
