#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <gmpxx.h>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;
#ifndef TP
#define TP 10
#endif

#define PCOUNT 20000
#if PCOUNT<2*TP
#error PCOUNT is too small
#endif
#define PBOUND 1000000
char pmask[PBOUND];
double maxv=-1;
int plist[PCOUNT]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101};
double logp[PCOUNT];
void initlogp()
{
	long i,pc,j;
	pc=0;
	for(i=2;i<PBOUND;i++)pmask[i]=1;
	for(i=2;i<PBOUND;i++){
		if(pmask[i]){
			if(pc<PCOUNT){
				plist[pc++]=i;
			}
			for(j=i*i;j<PBOUND;j+=i)pmask[j]=0;
		}
	}
	if(pc<PCOUNT){
		fprintf(stderr, "PBOUND %d is too small\n",PBOUND);
		exit(-1);
	}
	for(i=0;i<PCOUNT;i++)logp[i]=log(plist[i]);
}

std::vector<mpz_class> vstack;
mpz_class prod;

void search(int index)
{
	int i;
	for(i=index;i<TP;i++){
		mpz_class a=vstack.back()*plist[i];
		if(a*a*a*2<prod){
			mpz_class m=prod/a;
			mpz_class v=a*a+4*m;
			mpz_class x;
			mpz_sqrt(x.get_mpz_t(), v.get_mpz_t());
			if(x*x==v){
				mpz_class b = (x-a);
				if(b%2==0){
					b=b/2;
					std::cout<<"a="<<a<<",b="<<b<<std::endl;
				}
			}
			vstack.push_back(a);
			search(i+1);
			vstack.pop_back();
		}else return;
	}
}

int main()
{
	int i;
	initlogp();
	mpz_class x(1);
	vstack.push_back(x);
	for(i=0;i<TP;i++){
		x*=plist[i];
	}
	prod=x;
	search(0);
	return 0;
}
