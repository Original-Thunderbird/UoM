#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void merge(int *arr, int lw, int md, int hh){
    int i, k, *temp = (int*)malloc((hh-lw +1)*sizeof(int));
    int le_l = lw, le_h = md, ri_l = md+1, ri_h = hh;
    for(k=0; le_l<=le_h&&ri_l<=ri_h; k++){
        if(arr[le_l]<=arr[ri_l]){
            temp[k]=arr[le_l++];
        }
        else{
            temp[k]=arr[ri_l++];
        }
    }
    for(i=le_l;i<=le_h;i++){
        temp[k++]=arr[i];
    }
    for(i=ri_l;i<=ri_h;i++){
        temp[k++]=arr[i];
    }
    for(i=0;i<hh-lw +1;i++){
      arr[lw+i]=temp[i];
    }
    free(temp);
}
void mergesort(int *arr, int hd, int tl){
    int md = 0;
    if(hd<tl){
      md = (hd+tl)/2;
      mergesort(arr, hd, md);
      mergesort(arr, md+1, tl);
      merge(arr, hd, md, tl);
    }
}
int main(int argc, char** argv) {
    int i, len;
    scanf("%d", &len);
    int *arr = (int*)malloc(len*sizeof(int));
    for(i=0; i<len; i++ ){
        scanf("%d", &arr[i]);
    }
    mergesort(arr, 0, len-1);
    for(i=0; i<len; i++ ){
        printf("%d ", arr[i]);
    }
}
