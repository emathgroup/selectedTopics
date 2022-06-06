#include <stdio.h>
#include <iostream>
#include <gmpxx.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;
#ifndef TP
#define TP 1000
#endif

struct POW{
	int o2;
	int o5;
	mpz_class value;
	bool operator<(const POW& p)const{
		return value<p.value;
	}
	bool operator==(const POW& p)const{
		return value==p.value;
	}
};
vector<POW> cf;


int twocount;
int fivecount;
vector<int> stacks;
#define PCOUNT 2000
#if PCOUNT<2*TP
#error PCOUNT is too small
#endif
#define PBOUND 100000
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

void initcf()
{
	int i,j;
	mpz_class v2,v;
	v2=1;
	double ev=0.0;
	for(i=0;i<TP;i++){
		ev+=4*logp[i]+logp[TP+i];
	}
	double dv2,dv;
	dv2=0;
	for(i=0;i<=TP;i++){
		v=v2;//pow(2,i)
		dv=dv2;
		if(dv>ev)break;
		for(j=0;j<=TP;j++){
			if(i!=0||j!=0){
				POW p;
				p.o2=i;p.o5=j;p.value=v;
				cf.push_back(p);
			}
			v*=5;//pow(2,i)*pow(5,j)
			dv+=logp[2];//log(5);
			if(dv>ev)break;
		}
		v2*=2;
		dv2+=logp[0];//log(2);
	}
	sort(cf.begin(),cf.end());
}

void checkresult()
{
	int i;
	double s=0;
	if(stacks.size()>PCOUNT){
		fprintf(stderr, "More prime required\n");
		exit(-1);
	}
	for(i=0;i<stacks.size();++i){
		s+=(cf[stacks[i]].value.get_d()-1)*logp[i];
	}
	if(maxv<0||s<maxv){
		maxv=s;
	}
	if(s<maxv*1.0001){
		printf("Found log to %f:\n\t",s);
		for(i=0;i<stacks.size();++i){
			if(i>0)printf("*");
			if(stacks[i]==0){//order 1
				printf("%d",plist[i]);
			}else{
				printf("%d^%ld",plist[i],cf[stacks[i]].value.get_si()-1);
			}
		}
		printf("\n");
	}
}

void search(int index)
{
	int i;
	if(twocount==TP&&fivecount==TP){
		checkresult();
		return;
	}
	for(i=index;i>=0;--i){
		POW& p = cf[i];
		if(twocount+p.o2<=TP&&fivecount+p.o5<=TP){
			if(cf[i].o2>=1){//try change p^{2x-1} => p^{x-1} *lastP, and we know that total number of primes is no more than 2*TP
				if((cf[i].value.get_d()/2)*logp[stacks.size()]>logp[2*TP-1])
					continue;
			}
			if(cf[i].o5>=1){
				if((cf[i].value.get_d()/5)*logp[stacks.size()]>logp[2*TP-1])
					continue;
			}
#if 0
			if(maxv>0){//found any solution now
				int j;
				double tv=0;
				for(j=0;j<stacks.size();++j){
					tv+=(cf[stacks[j]].value.get_d()-1)*logp[j];
				}
				if(tv>=maxv*1.001)continue;//alreadly too large
			}
#endif
			stacks.push_back(i);
			twocount+=p.o2;
			fivecount+=p.o5;
			search(i);
			fivecount-=p.o5;
			twocount-=p.o2;
			stacks.pop_back();
		}
	}
}

int main()
{
	initlogp();
	initcf();
	search(cf.size()-1);
	return 0;
}
