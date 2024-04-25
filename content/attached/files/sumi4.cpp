#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <omp.h>
#include <gmpxx.h>
#include <iostream>

#define TLEN 47
#define FIRSTD 2
#define SLEN 23
#define TSIZE 16
#define TO 379
#define WITHED 1

int getbitones(long x)
{
    int s=0;
    while(x){
	if(x&1)s++;
	x>>=1;
    }
    return s;
}

int numdigits(const mpz_class& x)
{
    mpz_class y=x;
    int s=0;
    while(y>0){
	    mpz_class z=y%10;
	    s+=z.get_si();
	    y=y/10;
    }
    return s;
}

mpz_class getinitnum()
{
    mpz_class x=FIRSTD;
    int i;
    for(i=0;i<TLEN-1;i++){
	x*=10;x+=9;
    }
    return x;
}

bool getX(const mpz_class& I, long i, mpz_class& X)
{
    int j;
    int t,c=0;
    mpz_class M=1;
    X=I;
    for(j=0;j<TLEN-1;j++)M*=10;
    for(j=0,t=0;j<SLEN+TSIZE;j++){
	if(i&(1L<<j)){
	    c++;
	}else{
	    //X(t)=c;
	    if(t==0&&c>FIRSTD)return false;
	    if(c>9)return false;
	    if(c>0)X-=c*M;
	    M/=10;
	    t++;
	    c=0;
	}
    }
    return true;
}

mpz_class curbest;
int main()
{
    mpz_class I = getinitnum();
    long i;
    mpz_class localbest=0;
    mpz_class LD =1;
    for(i=0;i<(TLEN-SLEN);i++){
	    LD*=10;
    }
#pragma omp parallel for private(localbest)
    for(i=0;i<(1L<<(SLEN+TSIZE));i++){
	if(getbitones(i)>TSIZE)continue;
	mpz_class X;
	if(getX(I, i, X)){
	    mpz_class Y=sqrt(X);
	    while(1){
	    mpz_class Z=Y*Y;
	    if(Z>=X+LD)break;
	    int dg = numdigits(Z);
	    if(dg==TO){
		    if(localbest==0||Y<localbest){
		       localbest=Y;
#pragma omp critical
		    if(curbest==0||localbest<curbest)
		    {
			curbest=localbest;
		        std::cout<<"Found "<<TO*9<<" with "<<Y<<"^2="<<Z<<std::endl;
		        fflush(stdout);
		    }
		    }
	    }
	    Y=Y+1;
	    }
	}
    }

    return 0;
}

