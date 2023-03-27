#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bucketSort(int *array, int len){
  int i, j, temp[32768];
  memset(temp,0,sizeof(temp));
  for(i=0; i<len; i++){
    temp[array[i]]++;
  }
  for(i=j=0; i<32768; i++){
    while (temp[i]) {
      array[j]=i;
      temp[i]--;
      j++;
    }
  }
}

int main()
{
  int len, i, *array;
  FILE *in=fopen("in","r+"), *out=fopen("out","w+");
  fscanf(in, "%d", &len);
  array=(int*)malloc(len*sizeof(int));
  if(array){
    for (i=0; i<len; i++){
      fscanf(in, "%d", &array[i]);
    }
    bucketSort(array,len);
    for(i=0;i<len;i++){
      fprintf(out, "%d ",array[i] );
    }
  }
  fclose(out);
  fclose(in);
  printf("%d\n", len);
  return 0;
}
