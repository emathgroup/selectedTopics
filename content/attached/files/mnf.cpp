#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;
#ifndef TP
#define TP 8
#endif

struct POW{
	int o2;
	int o5;
	long value;
	bool operator<(const POW& p)const{
		return value<p.value;
	}
	bool operator==(const POW& p)const{
		return value==p.value;
	}
};
vector<POW> cf;
void initcf()
{
	int i,j;
	long v2,v;
	v2=1;
	for(i=0;i<=TP;i++){
		v=v2;//pow(2,i)
		for(j=0;j<=TP;j++){
			if(i!=0||j!=0){
				POW p;
				p.o2=i;p.o5=j;p.value=v;
				cf.push_back(p);
			}
			v*=5;//pow(2,i)*pow(5,j)
		}
		v2*=2;
	}
	sort(cf.begin(),cf.end());
}

int twocount;
int fivecount;
vector<int> stacks;
#define PCOUNT 26
double maxv=10E20;
int plist[PCOUNT]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101};
double logp[PCOUNT];
void initlogp()
{
	int i;
	for(i=0;i<PCOUNT;i++)logp[i]=log(plist[i]);
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
		s+=(cf[stacks[i]].value-1)*logp[i];
	}
	if(s<maxv){
		maxv=s;
	}
	if(s<maxv+0.001){
		printf("Found log to %f:\n\t",s);
		for(i=0;i<stacks.size();++i){
			if(i>0)printf("*");
			if(stacks[i]==0){//order 1
				printf("%d",plist[i]);
			}else{
				printf("%d^%ld",plist[i],cf[stacks[i]].value-1);
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
	for(i=0;i<=index;++i){
		POW& p = cf[i];
		if(twocount+p.o2<=TP&&fivecount+p.o5<=TP){
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
