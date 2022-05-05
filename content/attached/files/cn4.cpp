#include <gmpxx.h>
#include <iostream>
#define N 10
#if 0
#define M  9765625 //(power(5,10))
#define C  4
#else
#define M 282475249 //(power(7,10))
#define C 6
#endif
#define HL (C*5)

mpz_class fact[HL+1];
void initfac()
{
	int i;
	fact[0]=fact[1]=1;
	for(i=2;i<=HL;i++)fact[i]=fact[i-1]*i;
}

int expand(int x, char d[10])
{
    int i,s=0;
    for(i=0;i<10;i++){
	    d[i]=x%(C+1);
	    s+=d[i];
	    x/=(C+1);
    }
    return s;
}

mpz_class tcount;
int tclass;

void update_tcount(char d[10])
{
	int i;
	mpz_class c1,c2;
	tclass++;
#if 0
	std::cout<<"class "<<tclass<<":";
	for(i=0;i<10;i++){
	     if(d[i]>0){
		std::cout<<"+"<<i<<"*"<<(int)d[i];
	     }
	}
	std::cout<<std::endl;
#endif
	c1=fact[HL-1]/fact[d[0]]/fact[HL-1-d[0]];//digit 0 could not be first digit
	c1*=fact[HL-d[0]];
	for(i=1;i<=9;i++){
		c1/=fact[d[i]];
	}
	c2=fact[HL];
	for(i=0;i<=9;i++){
		c2/=fact[C-d[i]];
	}
	mpz_class lc=c1;
	lc*=c2;
#if 0
	std::cout<<"\t"<<lc<<std::endl;
#endif
	tcount+=lc;
}
int TAG;

int enumall()
{
    int i,j;
    char d[10];
    int count;
    for(i=0;i<M;i++){
	    count = expand(i,d);
	    if(count == HL){//total HL digits 
		    int sum = 0;
		    for(j=0;j<10;j++){
			    sum += d[j]*j;
		    }
		    sum%=11;
		    if(sum == TAG){
			    update_tcount(d);
		    }
	    }
    }
}

int main()
{
	int ts=C%11;
	if(C&1){
		TAG = (ts+11)/2;
	}else{
		TAG = ts/2;
	}
	initfac();
	enumall();
	std::cout << "Total count: "<<tcount<<std::endl;
	std::cout<< "Total class:"<<tclass<<std::endl;
}
