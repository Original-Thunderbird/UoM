#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

unsigned long long hcf(unsigned long long a, unsigned long long b){
	unsigned long long c, d, temp;
	c=(a>=b)?a:b;
    d=(a<b)?a:b;
	while(c%d){
		c %= d;
		temp = c;
		c = d;
		d = temp;
	}
	return d ;
}

unsigned long long fme(unsigned long long a,unsigned long long p, unsigned long long n) {
    unsigned long long t;
    if(p==0)    return 1;
    if(p%2==0){
        t = fme(a, p/2, n);
        return (t*t)%n;
    }
    t = fme(a, (p-1)/2, n);
    return ((a%n)*(t*t%n))%n;
}

unsigned long long imp(unsigned long long y, unsigned long long p) {
    return fme(y, p-2, p);
}

void elGamal(){
    unsigned long long x, m, y, k, a, b, p=257, g=3;
    printf("Prime modulus is %llu\nPrimitive root %llu is %llu\nChoose: e (encrypt) | d (decrypt) | k (get public key) | x (exit)?", p, p, g);
    char c = getchar(); getchar();
    while(c!='x'){
        switch(c){
            case 'k':
                printf("Type private key: ");
                scanf("%llu",&x);   getchar();
                printf("Public key is: %llu\n",y=fme(g,x,p));
                break;
            case 'e':
                srand((unsigned)time(NULL));
                k = 9;
                printf("Type secret number to send: ");
                scanf("%llu",&m);   getchar();
                printf("Type recipient's public key: ");
                scanf("%llu",&y);  getchar();
                //printf("y=%lu\n", y);
                //printf("k=%lu\n", k);
                //printf("%lu\n", fme(y,k,p));

                printf("\nThe encrypted secret is: (%llu, %llu)\n",  a=fme(g,k,p), b=((m%p)*fme(y,k,p))%p );
                break;
            case 'd':
                printf("Type in received message :");
                scanf("%llu %llu", &a,&b);    getchar();
                printf("Type in your private key: ");
                scanf("%llu", &x);    getchar();
								//unsigned long long t1=b%p; t2
								printf("t1=%llu\nt2=%llu\nt3=%llu\nt4=", b%p, fme(a,x,p), imp(fme(a,x,p),p), b%p*imp(fme(a,x,p),p));
                printf("\nThe decrypted secret is: %llu\n", m = (b%p*imp(fme(a,x,p),p))%p);
        }
        printf("Choose: e (encrypt) | d (decrypt) | k (get public key) | x (exit)?");
        c = getchar(); getchar();
        //printf("%c\n",c);
    }

}

int main(){
/*
    unsigned long long a, b, g, x, p;
    scanf("%llu", &g);    getchar();
    printf("a=%lu\n",g);
    scanf("%llu", &x);    getchar();
    printf("x=%lu\n",x);
    scanf("%llu", &p);    getchar();
    printf("p=%lu\n",p);
    //printf("hcf:%lu\n", hcf(a,b));
    printf("hcf:%llu\n", fme(g,x,p));
*/
    elGamal();
}
