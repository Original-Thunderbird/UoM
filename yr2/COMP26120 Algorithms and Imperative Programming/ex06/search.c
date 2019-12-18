#include "search.h"

void merge(char *arr, int lw, int md, int hh){
    int i, k;
    char *temp = (char*)malloc((hh-lw +1)*26);
    int le_l = lw, le_h = md, ri_l = md+1, ri_h = hh;
    for(k=0; le_l<=le_h&&ri_l<=ri_h; k++){
        if(strcmp(arr+le_l*26,arr+ri_l*26)<=0){
            strcpy(temp+k*26,arr+le_l*26);
            le_l++;
        }
        else{
            strcpy(temp+k*26,arr+ri_l*26);
            ri_l++;
        }
    }
    for(i=le_l;i<=le_h;i++,k++){
        strcpy(temp+k*26,arr+i*26);
    }
    for(i=ri_l;i<=ri_h;i++,k++){
        strcpy(temp+k*26,arr+i*26);
    }
    for(i=0;i<hh-lw +1;i++){
      strcpy(arr+(lw+i)*26,temp+i*26);
    }
    free(temp);
}
void mergesort(char *arr, int hd, int tl){
    int md = 0;
    if(hd<tl){
      md = (hd+tl)/2;
      mergesort(arr, hd, md);
      mergesort(arr, md+1, tl);
      merge(arr, hd, md, tl);
    }
}

int binSearch(char *str_base, char *str_qury, int lw, int hg){
  /*
  int md=(lw+hg)/2;
  if(lw>hg){
    return 0;
  }
  if(strcmp(str_base+md*26,str_qury)==0){
    return 1;
  }
  else if(strcmp(str_base+md*26,str_qury)<0){
    return binSearch(str_base, str_qury, md+1, hg);
  }
  else{
    return binSearch(str_base, str_qury, lw, md-1);
  }
  */
  int md;
  while(lw<=hg){
    md = (lw+hg)/2;
    if(strcmp(str_base+md*26,str_qury)<0){
      lw = md+1;
    }
    else if(strcmp(str_base+md*26,str_qury)>0){
      hg = md -1;
    }
    else{
      return md;
    }
  }
  return -1;
}
