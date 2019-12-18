#include <stdio.h>
#include <stdlib.h>

void swap (int *array, int j){
  int temp;
  temp=array[j];
  array[j]=array[j+1];
  array[j+1]=temp;
}

void bubbleSort(int *array, int len){
  int i, j;
  for(i=0; i<len-1; i++){
    for(j=0;j<len-1-i;j++){
      if(array[j]>array[j+1]){
        swap(array,j);
      }
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
    bubbleSort(array,len);
    for(i=0;i<len;i++){
      printf("%d ",array[i] );
    }
  }
  return 0;
}
