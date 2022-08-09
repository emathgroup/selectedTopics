#include <stdio.h>

#define W 14
#define DD 100
#define SCNT (59049*81) //3**W
#define WHITE 0
#define BLACK 1


typedef int MC[SCNT];
int mcb[W*DD+1][SCNT];
#define mc1 mcb[0]

void initmc1()
{
	int i,s;s=0;
	for(i=0;i<SCNT;i++)mc1[i]=-1;
	for(i=0;i<W;i++){
		s*=3;s+=1;
	}
	mc1[s]=0;
}

void decode(int x, char v[])
{
	int i;
	for(i=0;i<W;i++){
		v[i]=x%3;
		x/=3;
	}
}
int encode(const char v[])
{
	int i,s=0;
	for(i=W-1;i>=0;i--){
		s*=3;s+=v[i];
	}
	return s;
}

void update_mc(const MC& old, MC& newp, int step)
{
	int i,j,k,ns;
	for(i=0;i<SCNT;i++)newp[i]=-1;
	for(i=0;i<2;i++){//try new status to be i
		for(j=0;j<SCNT;j++){
			if(old[j]<0)continue;
			char v[W];
			decode(j,v);
			if(i==0){
				v[step]=0;
				if(step>0&&v[step-1]==2){
					v[step-1]=1;
				}
				ns=encode(v);
				if(old[j]>newp[ns]){
					newp[ns]=old[j];
				}
			}else{
				if(v[step]==2)continue;
				if(v[step]==0||step>0&&v[step-1]==0){
					v[step]=1;
				}else{
					v[step]=2;
				}
				ns=encode(v);
				if(old[j]+1>newp[ns]){
					newp[ns]=old[j]+1;
				}
			}
		}
	}
}

int bitcount(int x)
{
	int c=0;
	while(x>0){
		if(x&1)c++;
		x>>=1;
	}
	return c;
}

void outputstatus(int x)
{
	int i;
	for(i=0;i<W;i++){
		printf("%d",x%3>0);
		x/=3;
	}
	printf("\n");
}

int main()
{
	int i,j;
	initmc1();
	for(i=0;i<W*DD;i++){
		update_mc(mcb[i], mcb[i+1], i%W);
	}
	int maxv=0,mc=-1;
	int prevmax;
	long maxcnt=-1;
	int col;
	for(col=DD;col>0;col--){
		maxv=0,mc=-1,maxcnt=-1;
//		printf("\nRow=%d, col=%d\n",W,col);
	for(i=0;i<SCNT;i++){
		char v[W];
		decode(i,v);
		for(j=0;j<W;j++)if(v[j]==2)break;
		if(j<W)continue;
		if(mcb[col*W][i]>maxv){
			maxv=mcb[col*W][i];
			mc=i;
		}
	}
	if(col<DD){
		printf(" %d",prevmax-maxv);
	}
//	printf("maxv=%d(%f),maxcnt=%ld\n",maxv,maxv*1.0/(col*W),maxcnt);
	prevmax=maxv;
#if 0
	outputstatus(mc);
	int ns = prev[col-1][mc];
	for(i=col-2;i>=0;i--){
		outputstatus(ns);
		ns = prev[i][ns];
	}
#endif
	}
	return 0;
}
