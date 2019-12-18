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

//time complexity: O(log p)

//Justification: Constant number of arithmetic operations are performed, excluding those in the recursive call.
//Also, in each recursive call, the exponent p gets halved. Thus, the number of recursive calls and arithmetic operations is O(log p).
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

//time complexity: O(log p)

//Justification: The function is a single call to fme(a,p,n), since only one operation is performed(calling imp), plus p-2 is linear size with p, Thus
//the function is of the same complexity of fme.
unsigned long long imp(unsigned long long y, unsigned long long p) {
    return fme(y, p-2, p);
}

void elGamal(){
	printf("test\n");
    unsigned long long x, m, y, k, a, b, p=4093082899, g=2;
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
                k = rand()%p;
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
                printf("\nThe decrypted secret is: %llu\n", m = (b%p*imp(fme(a,x,p),p))%p);
        }
        printf("Choose: e (encrypt) | d (decrypt) | k (get public key) | x (exit)?");
        c = getchar(); getchar();
        //printf("%c\n",c);
    }

}

int main(){
		printf("1--highest common factor\n2--fast modular exponentation\n3--inverse modulo prime\n4--elGamal\nchoose mode:");
		int c;
		unsigned long long a, b, g, x, p;
		scanf("%d", &c); getchar();
		switch(c){
			case 1:
				printf("input a:");
				scanf("%llu\n", &a);
				printf("input a:");
				scanf("%llu\n", &b);
				printf("highest common factor\n", hcf(a,b));
				break;
			case 2:
				printf("input generator g:");
				scanf("%llu\n", &g);
				printf("input power x:");
				scanf("%llu\n", &x);
				printf("input prime p:");
				scanf("%llu\n", &p);
				printf("fast modular exponentation\n", fme(g,x,p));
				break;
			case 3:
				printf("input factor a:");
				scanf("%llu\n", &a);
				printf("input prime p:");
				scanf("%llu\n", &p);
				printf("highest common factor\n", imp(a,p));
				break;
			case 4:
				elGamal();
		}
}
