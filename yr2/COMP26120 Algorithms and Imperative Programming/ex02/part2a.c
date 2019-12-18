#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]){
    /*no argument*/
    if(argc==1){
        printf("Error: expected some command-line parameters\n");
        return 0;
    }
    short i, max=0, sub_max=0;
    /*in each loop max and sub_max updates if a string more longer is discovered, otherwise it remains at current value */
    for(i=1;i<argc;i++){
        if(strlen(argv[i])>max){
            max=strlen(argv[i]);
            sub_max=i;
        }
    }
    /*print the longest string parameter. Since sub_max updates iif a LONGER string is found, the first longest string will be print if many strings are at the same length*/
    printf("%s\n",argv[sub_max]);
    return 0;
}
