#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void hcf(mpz_t a, mpz_t b, mpz_t res){
	mpz_t c, d, temp, t1;
  mpz_inits(c, d, temp, t1, NULL);
  if(mpz_cmp(a, b)>=0){
    mpz_set(c, a);
    mpz_set(d, a);
  }
  else{
    mpz_set(c, b);
    mpz_set(d, a);
  }
	//c=(a>=b)?a:b;
    //d=(a<b)?a:b;
  mpz_mod(t1, c, d);
	while(mpz_cmp_ui(t1, 0)>0){
		//c %= d;
    mpz_mod(c, c, d);
		//temp = c;
    mpz_set(temp, c);
		//c = d;
    mpz_set(c, d);
		//d = temp;
    mpz_set(d, temp);
    mpz_mod(t1, c, d);
	}
	mpz_set(res, d);
	mpz_clear(c);
	mpz_clear(d);
	mpz_clear(temp);
}

void mpz_pom(mpz_t a,mpz_t p, mpz_t n, mpz_t res) {
    mpz_t t0, t1, t2, t3;
    mpz_inits(t0, t1, t2, t3, NULL);
    //if(p==0)    return 1;
    if(mpz_cmp(p, 0)==0){
        //return 1;
        mpz_set_ui(res,1);
        mpz_clears(t0, t1, t2, t3, NULL);
        return ;
    }
    //if(p%2==0){
    mpz_mod_ui(t0, p, 2);
    if(mpz_cmp(t0, 0)==0){
        //t0 = fme(a, p/2, n);
        mpz_fdiv_q_ui(t0, p, 2); //t0=p/2
        mpz_pom(a, t0, n, t1); //t0=fme(a, t0/2, n);
        //return t1*t1%n
        mpz_mul(t0, t1, t1); //t0 = t1*t1;
        mpz_mod(res, t0, n);  //t0 = t0%n;
    }
    //t0 = fme(a, (p-1)/2, n);
    mpz_fdiv_q_ui(t0, p-1, 2); //t0=(p-1)/2
    mpz_pom(a, t0, n, t1); //t0=fme(a, t0/2, n);
    //return ((a%n)*(t1*t1%n))%n;
    mpz_mul(t0,t1,t1); //t0=t0*t0
    mpz_mod(t1, a, n); //t1=a%n
    mpz_mod(t0, t0, n); //t0=t0%n=t0*t0%n
    mpz_mul(t0,t1,t0);  //t0=t1*t0=(a%n)*(t0*t0%n)
    mpz_mod(res, t0, n); //res=t0%n=(a%n)*(t0*t0%n)%n
    mpz_clears(t0, t1, t2, t3, NULL);
}

void mpz_imvert(mpz_t y, mpz_t p, mpz_t res) {
    //myfme(y, p-2, p, res);
    mpz_pom(res, y, p-2, p);
    gmp_printf("res=%Zd\n",res);
}

void elGamal(){
    unsigned long t4;
    mpz_t x, m, y, k, a, b, p, g, t1, t2, t3, t5;
    //mpz_inits(x, m, y, k, a, b, t1, t2, t3, t5, NULL);
    mpz_init(x);  mpz_init(m);  mpz_init(y);  mpz_init(k);  mpz_init(a);  mpz_init(b);  mpz_init(t1);  mpz_init(t2);  mpz_init(t3);  mpz_init(t5);
    mpz_init_set_str(g, "43", 10);
    mpz_init_set_str(p, "170141183460469231731687303715884105727", 10);
    gmp_printf("Prime modulus is %Zd\nPrimitive root %Zd is %Zd\nChoose: e (encrypt) | d (decrypt) | k (get public key) | x (exit)?", p, p, g);
    char c = getchar(); getchar();
    while(c!='x'){
        switch(c){
            case 'k':
                printf("Type private key: ");
                gmp_scanf("%Zd",&x);   getchar();
                mpz_powm(y,g,x,p);//y=fme(g,x,p);
                gmp_printf("Public key is: %Zd\n",y);
                break;
            case 'e':
                srand((unsigned)time(NULL));
                t4 = rand();
                mpz_set_ui(t5, t4);
                //mpz_set_ui(t5, 9);

                mpz_mod(k, t5, p);//k = rand()%p;
                gmp_printf("%Zd\n",k);
                printf("Type secret number to send: ");
                gmp_scanf("%Zd",&m);   getchar();

                printf("Type recipient's public key: ");
                gmp_scanf("%Zd",&y);  getchar();

                mpz_mod(t1, m, p);//t1 = m%p;
                mpz_powm(t2, y, k, p);//t2 = fme(y,k,p);
                mpz_mul(t3, t1, t2);//t1=m%p * fme(y,k,p);
                mpz_powm(a, g, k, p);//a=fme(g,k,p);
                mpz_mod(b, t3, p);//b=( m%p * fme(y,k,p) ) %p;
                gmp_printf("\nThe encrypted secret is: (%Zd, %Zd)\n", a, b);
                break;
            case 'd':
                printf("Type in received message :");
                gmp_scanf("%Zd %Zd", &a,&b);    getchar();

                printf("Type in your private key: ");
                gmp_scanf("%Zd", &x);    getchar();

                mpz_mod(t1, b, p);//t1=b%p;
                gmp_printf("t1=%Zd\n",t1);
                mpz_powm(t2, a, x, p);//t2=fme(a,x,p);
                gmp_printf("t2=%Zd\n",t2);
                mpz_invert(t3, t2, p);//t3=imp(t2,p);
                gmp_printf("t3=%Zd\n",t3);
                mpz_mul(t5, t1, t3);//t5 = ( b%p * imp(fme(a,x,p), p) );
                gmp_printf("t5=%Zd\n",t5);
                mpz_mod(m, t5, p);//m=( b%p * imp(fme(a,x,p), p) )%p;
                gmp_printf("\nThe decrypted secret is: %Zd\n", m);
        }
        printf("Choose: e (encrypt) | d (decrypt) | k (get public key) | x (exit)?");
        c = getchar(); getchar();
        //printf("%c\n",c);
    }
    mpz_clears(x, m, y, k, a, b, t1, t2, t3, t5, NULL);
}

int main(){
    elGamal();
}
