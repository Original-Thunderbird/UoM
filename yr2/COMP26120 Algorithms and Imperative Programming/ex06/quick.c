#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void swap(int *arr, int a, int b){
  int temp=arr[a];
  arr[a]=arr[b];
  arr[b]=temp;
}

void dealpivot(int *arr, int le, int ri) {
  int md = (le+ri)/2;
  if(arr[le]>arr[md]){
    swap(arr,le,md);
  }
  if(arr[le]>arr[ri]){
    swap(arr,le,ri);
  }
  if(arr[md]>arr[ri]){
    swap(arr,md,ri);
  }
  swap(arr,ri-1,md);
}

void quick(int *arr,int le,int ri){
    if(le < ri){
        dealpivot(arr,le,ri);
        int pivot = ri-1, i = le, j = ri - 1;
        while(1){
          while(arr[++i]<arr[pivot]);
          while(j > le && arr[--j] > arr[pivot]);
          if(i<j){
            swap(arr,i,j);
          }
          else{
            break;
          }
        }
        if (i < ri){
          swap(arr, i, ri-1);
        }
        quick(arr, le, i-1);
        quick(arr, i+1, ri);
    }
}


int main(int argc, char** argv) {
    int i, len;
    scanf("%d", &len);
    int *arr = (int*)malloc(len*sizeof(int));
    for(i=0; i<len; i++ ){
        scanf("%d", &arr[i]);
    }
    quick(arr, 0, len-1);
    for(i=0; i<len; i++ ){
        printf("%d ", arr[i]);
    }
}
