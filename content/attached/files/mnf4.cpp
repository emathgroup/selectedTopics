#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;
#ifndef TP
#define TP 2435
#endif

struct POW{
	int o2;
	int o5;
	int min_p_index;
	int max_p_index;
	long value;
	bool operator<(const POW& p)const{
		return value<p.value;
	}
	bool operator==(const POW& p)const{
		return value==p.value;
	}
};
vector<POW> cf;
int p2o, p5o, pto;
int C;
double F,T;

int twocount;
int fivecount;
vector<int> stacks;
#define PCOUNT 20000
#define PBOUND 500000
char pmask[PBOUND];
double maxv=-1;
int plist[PCOUNT]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101};
double logp[PCOUNT];
vector<double> lbvs;
vector<double> upvs;

//k should be no less than 6
double pup(double k)
{
	if(k>=688383){
		double x=k*(log(k)+log(log(k))-1+(log(log(k))-2)/log(k));
		return x;
	}else{
		return k*log(k*log(k));
	}
}

double plb(double k)
{
	return k*(log(k)+log(log(k))-1+(log(log(k))-2.1)/log(k));
}


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
	C=floor(pow(pup(2*TP),1.0/4));
	if(C>=PCOUNT){
		fprintf(stderr, "PCOUNT %d is too small, it should be more than %d\n",PBOUND, C);
	}
}

void setm25c()
{
	int i;
	double logp2TP = log(pup(2*TP));
	for(i=0;i<C;i++){
		if(i>=PCOUNT){
			printf("Prime count too small\n");
			exit(-1);
		}
		double v=logp2TP/logp[i];
		int m2 = floor(log(2.0*v)/log(2));
		int m5 = floor(log(5.0*v)/log(5));
		if(m2<=0)m2=1;
		if(m5<=0)m5=1;
		p2o+=m2;
		p5o+=m5;
	}
	F=C+TP-p5o;
	T=F+TP-p2o;
	printf("N=%f,C=%d, F=%f, T=%f\n",(double)TP, C,F,T);
	for(i=0;i<C;i++){
		int ph=plist[i];
		double lb = log(plb(T))/logp[i];
		double ub =5*log(pup(TP))/logp[i];
		lb=log(lb)/log(5),ub=log(ub)/log(5);
		double ub2=log(pup(2*TP))/logp[i];
		ub2=log(ub2)/log(5);
		if(ub2<1.0)continue;
		printf("%d^{ ",ph);
		int j=ceil(lb);
		while(j<ub){
			printf("5^%d ",j);
			j++;
		}
		lb = log(plb(F))/logp[i];
		ub = log(pup(2*TP))/logp[i];
		lb=log(lb)/log(5),ub=ub2;
		j=ceil(lb);
		while(j<ub){
			printf("2*5^%d ",j);
			j++;
		}
		printf("} ");
	}
	printf("\n");
}

int main()
{
	initlogp();
	setm25c();
#if 0
	initcf();
	search(cf.size()-1);
#endif
	return 0;
}
