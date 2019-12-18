#include <stdio.h>
#include <string.h>
int f2c(float raw){
    float ret;
    /*absolut zero in Ferenheit*/
    if(raw<-459.67){
        printf("Temperatures below absolute zero\n");
        return 4;
    }
    ret=(raw-32)*5/9;
    printf("%.2f°C = %.2f°F\n",ret,raw);
    return 0;
}
int c2f(float raw){
    float ret;
    /*absolut zero in Celcius*/
    if(raw<-273.15){
        printf("Temperatures below absolute zero\n");
        return 4;
    }
    ret=raw*9/5+32;
    printf("%.2f°C = %.2f°F\n",raw,ret);
    return 0;
}

int main(int argc, char* argv[]){
    float raw=0;
    int ret=0;
    /*Error:wrongnumber of parameters*/
    if(argc!=3){
        printf("The wrongnumber of parameters have been supplied\n");
        return 1;
    }
    /*Error:-f or -c missing*/
    if(strcmp(argv[1],"-f")&strcmp(argv[1],"-c")){
        printf("Failed to supply either -f or -c as the first option\n");
        return 2;
    }
    /*Error:Malformed number*/
    if(strspn(argv[2],".-0123456789")!=strlen(argv[2])){
        printf("Malformed number in second parameter\n");
        return 3;
    }
    sscanf(argv[2],"%f",&raw);
    /*Fahrenheit to Celcius*/
    if(!strcmp(argv[1],"-f")){
        ret=f2c(raw);
	if(ret==4){
		return 4;
	}
    }
    /*Celcius to Fahrenheit*/
    else if(!strcmp(argv[1],"-c")){
        ret=c2f(raw);
	if(ret==4){
		return 4;
	}
    }
    return 0;
}
