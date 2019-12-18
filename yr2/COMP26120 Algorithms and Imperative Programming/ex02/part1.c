#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
int main(){
    char ch;
    int sum=0, tup=0, tlw=0;
    while(1){
        ch=getchar();
        if (ch=='\n')    break;
        if (isupper(ch)){
            ch=tolower(ch);
            tlw++;
        }
        else if(islower(ch)){
            ch=toupper(ch);
            tup++;
        }
        sum++;
        putchar(ch);
    }
    putchar('\n');
    printf("Read %d characters in total, %d converted to upper-case, %d to lower-case\n", sum, tup, tlw);
    return 0;
}
*/
/*
int main(){
    char ch;
    FILE *in=fopen("input.txt","r+");
    if(in==NULL){
        perror("Cannot find target file");
        return -1;
    }
    int sum=0, tup=0, tlw=0;
    //freopen("input.txt","r",stdin);
    //while(fread(&ch,1,sizeof(char),in)){
    while((ch=fgetc(in))!=EOF){
        if (isupper(ch)){
            ch=tolower(ch);
            tlw++;
        }
        else if(islower(ch)){
            ch=toupper(ch);
            tup++;
        }
        sum++;
        putchar(ch);
    }
    putchar('\n');
    printf("Read %d characters in total, %d converted to upper-case, %d to lower-case\n", sum, tup, tlw);
    fclose(in);
    return 0;
}
*/
/*
int main(){
    char ch;
    FILE *in;
    in = fopen("input.txt","r");
    if(in==NULL){
        perror("Cannot find target file");
        return -1;
    }
    freopen("output.txt","w",stdout);
    int sum=0, tup=0, tlw=0;
    while((ch=fgetc(in))!=EOF){
        if (isupper(ch)){
            ch=tolower(ch);
            tlw++;
        }
        else if(islower(ch)){
            ch=toupper(ch);
            tup++;
        }
        sum++;
        putchar(ch);
    }
    putchar('\n');
    printf("Read %d characters in total, %d converted to upper-case, %d to lower-case\n", sum, tup, tlw);
    fclose(in);
    return 0;
}
*/

int main(){
    char ch,input[40],output[40];
    scanf("%s",input);
    scanf("%s",output);
    /*open input file*/
    FILE *in, *out;
    in = fopen(input,"r");
    /*error check*/
    if(in==NULL){
        perror("Cannot find target file");
        return -1;
    }
    /*alter stdout to a file*/
    out = fopen(output,"w");
    /*sum:total char number; tlw: character turned to lower-case; tup: character turned to upper-case*/
    int sum=0, tup=0, tlw=0;
    while((ch=fgetc(in))!=EOF){
    	/*character in upper-case*/
        if (isupper(ch)){
            ch=tolower(ch);
            tlw++;
        }
	/*character in lower-case*/
        else if(islower(ch)){
            ch=toupper(ch);
            tup++;
        }
        sum++;
        fputc(ch,out);
    }
    fclose(out);
    printf("Read %d characters in total, %d converted to upper-case, %d to lower-case\n", sum, tup, tlw);
    fclose(in);
    return 0;
}

