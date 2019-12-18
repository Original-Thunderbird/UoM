#include<stdio.h>
#include<stdlib.h>

//running time is O(p).
//for every pow of g, see if it has the same module with y.
unsigned long dl(unsigned long y,unsigned long g,unsigned long p) {
    unsigned long i=0, t=1;
    while(i<p){
        if(t%p==y)
            return i;
        t = t*g%p;
        i++;
    }
    return -1;
}
//A one-way function is one for which it's easy to go from a domain element to its image, but hard to go from a range element to its pre-image.
//in Modular exponentiation, domain element is power x, image is y, it's time complexity is O(log(p)), yet for discrete logarithm,
//domain element is y and image is x, the reverse of that of Modular exponentiation, and time complexity if O(p), thus being harder, and proving that Modular exponentiation is a one-way function.
int main(){
    unsigned long y,g,p;
    scanf("%lu%lu%lu",&y,&g,&p);
        unsigned long dove=dl(y,g,p);
        if(dove!=-1) printf("%lu\n",dove);
        else printf("no solution\n");

    return 0;
}
