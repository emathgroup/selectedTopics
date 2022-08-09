#include <stdio.h>

#define W 11
#define DD 200
#define SCNT (59049*3) //3**W
#define WHITE 0
#define BLACK 1


typedef int MC[SCNT];
long cnt[DD][SCNT];
int mcb[DD][SCNT];
int prev[DD][SCNT];
#define mc1 mcb[0]

void initmc1()
{
	int i,j;
	for(i=0;i<1<<W;i++)mc1[i]=-1;
	for(i=0;i<1<<W;i++){
		int b=0;
		int bc=0;
		for(j=W-1;j>=0;j--){
			b*=3;
			if((1<<j)&i){//a black block
				bc++;
				if(j>0&&(((1<<(j-1))&i)==0)||j<W-1&&(((1<<(j+1))&i)==0)){//if any white neighbour found
					b+=1;
				}else{
					b+=2;
				}
			}
		}
		if(mc1[b]<bc){
			mc1[b]=bc;
			cnt[0][b]=1;
		}else if(mc1[b]==bc){
			cnt[0][b]++;
		}
	}
}

void decode(int x, char v[])
{
	int i;
	for(i=0;i<W;i++){
		v[i]=x%3;
		x/=3;
	}
}
void update_mc(MC *old, MC *newp, MC *pprev, long pcount[], long count[])
{
	int i,j,k;
	for(i=0;i<SCNT;i++)(*newp)[i]=-1;
	for(i=0;i<1<<W;i++){//try new status to be i
		for(j=0;j<SCNT;j++){
			if((*old)[j]<0)continue;
			char v[W];
			decode(j,v);
			int ns=0,bc=0;
			for(k=W-1;k>=0;k--){
				if(v[k]==2&&(i&(1<<k)))break;
				ns*=3;
				if(i&(1<<k)){
					if(v[k]==0||k>0&&(i&(1<<(k-1)))==0||k<W-1&&(i&(1<<(k+1)))==0){
						ns+=1;
					}else{
						ns+=2;
					}
					bc++;
				}
			}
			if(k>=0)continue;//checking failed
			if((*old)[j]+bc>(*newp)[ns]){
				(*newp)[ns]=(*old)[j]+bc;
				(*pprev)[ns]=j;
				count[ns]=pcount[j];
			}else if((*old)[j]+bc==(*newp)[ns]){
				count[ns]+=pcount[j];
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
	for(i=2;i<=DD;i++){
		update_mc(&mcb[i-2], &mcb[i-1], &prev[i-1], cnt[i-2],cnt[i-1]);
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
		if(mcb[col-1][i]>maxv){
			maxv=mcb[col-1][i];
			maxcnt= cnt[col-1][i];
			mc=i;
		}else if(mcb[col-1][i]==maxv){
			maxcnt+=cnt[col-1][i];
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
