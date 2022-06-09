#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;
#ifndef TP
#define TP 1000000
#endif

#ifndef TARGET
#define TARGET 1E17
#endif
int p2o, p5o, pto;

#ifdef DEBUG
#define TRACE(...) fprintf(stderr,__VA_ARGS__)
#else
#define TRACE(...)
#endif
int twocount;
int fivecount;
int mor;
vector<int> stacks;
vector<double> leftN;
vector<double> accuT;
#define TPM 100
#define PCOUNT 20000
#if TPM > PCOUNT
#error TPM must be no more than PCOUNT
#endif
#if PCOUNT < 2*TPM
#error PCOUNT is too small
#endif
#define PBOUND 1000000
char pmask[PBOUND];
int plist[PCOUNT];
double logs;
double logp[PCOUNT];
double logps[TPM];
double sumllp[TPM];
double curbest;
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
	logs=0;
	for(i=0;i<TPM;i++)logs+=logp[i];
	sumllp[TPM-1]=log(logp[TPM-1]);
	logps[0]=logp[0];
	for(i=1;i<TPM;i++){
		logps[i]=logps[i-1]+logp[i];
	}
	for(i=TPM-2;i>=0;i--){
		sumllp[i]=sumllp[i+1]+log(logp[i]);
	}
}
int search_for_must_one_range(int ub, double ln)
{
	double s=sumllp[0]; 
	double lamb = exp((s+(ln))/(ub+1));
	if(lamb/logp[ub]<1){
		int ne=ub;
		while(lamb/logp[ne]<1&&ne>=0){ne--;}
		if(ne<0){
			return 0;
		}
		if(ne==ub){
			return ne;
		}
		return search_for_must_one_range(ne,ln);
	}
	return ub;
}

double estimate_in_range(int lb, int ub, int maxv, double ln, double acc, int recursive=0)
{
	int si=stacks.size();
	double s=sumllp[lb];if(ub+1<TPM)s-=sumllp[ub+1];
	double lamb = exp((s+(ln))/(ub-lb+1));
	if(curbest>0&&lamb*(ub-lb+1)+acc>curbest*1.001){
		return -1.0;
	}
	if(lamb/logp[lb]>maxv||lamb/logp[ub]<1){
		int ns, ne;
		ns=lb;ne=ub;
		double nacc = accuT[si];
		while(lamb/logp[ns]>maxv&&ns<=ne){nacc+=maxv*logp[ns];ns++;}
		while(lamb/logp[ne]<1&&ne>=ns){nacc+=logp[ne];ne--;}
		if(curbest>0&&nacc>curbest*1.001)return -1.0;
		if(ne<ns){
			if(lamb/logp[lb]>maxv){return (double)maxv*logp[si];
			}else{
				return logp[si];
			}
		}
		double nln;
		nln = ln - (ns-lb)*log(maxv);
		if(recursive==0){
			return lamb;
		}
		if(recursive>0&&estimate_in_range(ns,ne,maxv,nln,nacc,recursive-1)<0)return -1;
	}
	return lamb;
}
double estimate_lambda()
{
	return estimate_in_range(stacks.size(), mor>=TPM?TPM-1:mor, stacks.size()==0?TPM:stacks.back(), leftN.back(), accuT.back());
}
int search();
int push(int x)
{
	int pi=stacks.size();
	double lv=leftN[pi]-log(x);
	if(lv<-0.0001)return -1;
	stacks.push_back(x);
	leftN.push_back(lv);
	accuT.push_back(accuT[pi]+x*logp[pi]);
	return 0;
}
void pop()
{
	stacks.pop_back();
	leftN.pop_back();
	accuT.pop_back();
}

void tracedepth(int d);
#ifdef DEBUG
#define TD() tracedepth((int)stacks.size())
#else
#define TD()
#endif

void try_two_only()
{
	int pi=stacks.size();
	double lv=leftN[pi];
	int pc = ceil(lv/log(2));
	if(pc<0)pc=0;
	if(pc>mor-pi){
		TD();TRACE("pc=%d too large,pi=%d\n",pc,pi);
		return;//two much primes required
	}
	double mv = accuT[pi];
	int i;
#if 0
	for(i=pi;i<pi+pc;i++){
		mv+=2*logp[i];
	}
#else
	if(pi+pc>=1){
		mv+=2*logps[pi+pc-1];

	}
	if(pi>=1){
		mv-=2*logps[pi-1];
	}

#endif
#if 0
	for(i=pi+pc;i<mor;i++){
		mv+=logp[i];
	}
#else
	mv+=logps[mor-1];
	if(pi+pc>=1)mv-=logps[pi+pc-1];
#endif
	if(curbest<0||mv<curbest){
		curbest=mv;
		TD();TRACE("best %f found\n",(curbest-logs)/log(10));
		printf("Cur best %f:",(curbest-logs)/log(10));
		for(i=0;i<stacks.size();i++){
			printf(" %d",stacks[i]-1);
		}
		printf(" 1*%d\n",pc);
	}else{
		TD();TRACE("%f found\n",(mv-logs)/log(10));
	}
}

void tracedepth(int d)
{
	int i;
	for(i=0;i<d;i++){
		TRACE("  ");
	}
}

int search()
{
	double lamb = estimate_lambda();
	try_two_only();
	if(lamb<0){
		TD();
		TRACE("PRUNED lamb=%f,accu=%f,curbest=%f\n",lamb,accuT[stacks.size()],curbest);
		return -1;
	}
	double ei= lamb/logp[stacks.size()];
#if 0
	if(ei<2){
		TD();TRACE("PRUNED ei=%f\n", ei);
		return -1;
	}
#endif
	int s = (int)round(ei);
	int pos_required=1, neg_required=1;
	if(stacks.size()==0||s<=stacks.back()){
		if(s<=2){
			s=2;
			neg_required=0;
		}else if(push(s)<0){
			TD();TRACE("push %d failed\n",s);
			pos_required=0;
		} else {
			TD();TRACE("push %d\n",s);
			if(search()<0){
				pos_required=neg_required=0;
			}
			pop();
		}
	}else{
		pos_required=0;
	}
	int i;
	for(i=1;pos_required||neg_required;++i){
		if(neg_required&&i<s-2){
			if(push(s-i)==0){
				TD();TRACE("push %d\n",s-i);
				if(search()<0){
					neg_required=0;
				}
				pop();
			}else{
				TD();TRACE("push %d failed\n",s-i);
			}
		}else{
			neg_required=0;
		}
		if(pos_required){
			if(stacks.size()==0||s+i<=stacks[0]){
				if(push(s+i)==0){
					TD();TRACE("push %d\n",s+i);
					if(search()<0){
						pos_required=0;
					}
					pop();
				}else{
					TD();TRACE("push %d failed\n",s+i);
					pos_required=0;
				}
			}else{
				pos_required=0;
			}
		}
	}
	return 0;
}


int main()
{
	initlogp();
	curbest=-1;
	mor=search_for_must_one_range(TPM-1,log(TARGET));
	logs=0;int i;
	for(i=0;i<mor;i++)logs+=logp[i];
	leftN.push_back(log(TARGET));
	accuT.push_back(0.0);
	search();
	return 0;
}
