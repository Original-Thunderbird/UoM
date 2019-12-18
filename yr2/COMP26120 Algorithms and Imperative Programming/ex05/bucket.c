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
  scanf("%d", &len);
  array=(int*)malloc(len*sizeof(int));
  if(array){
    for (i=0; i<len; i++){
      scanf("%d", &array[i]);
    }
    bucketSort(array,len);
    for(i=0;i<len;i++){
      printf("%d ",array[i] );
    }
  }
  return 0;
}
