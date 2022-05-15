#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>

#ifdef DEBUG
#define TRACE(fmt,...) printf(fmt, __VA_ARGS__);fflush(stdout)
#else
#define TRACE(fmt, ...)
#endif

#define ROW 6
#define TNUM (64)
#if TNUM >64 
#error "TNUM should be no more than 64"
#endif
#define NUMP 4
int target[NUMP]={6,5,3,3};//target product 2^6*3^5*5^3*7^3
int list[TNUM][NUMP];
time_t starttime;
void initlist()
{
	int i,j,s,t,k;
	k=0;
	for(i=0;i<2;i++)for(j=0;j<2;j++)for(s=0;s<3;s++)for(t=0;t<3;t++){//push 2^i*3^j*5^s*5^t, total 2*2*3*3=36 elements
		list[k][0]=i;list[k][1]=j;list[k][2]=s;list[k][3]=t;
		k++;
	}
	for(i=0;i<2;i++)for(j=2;j<4;j++){
		for(s=0;s<2;s++)for(t=0;t<2;t++){ //total 3*2*2=12 elements
			if(s==1&&t==1)continue;//skip
			list[k][0]=i;list[k][1]=j;list[k][2]=s;list[k][3]=t;
			k++;
		}
	}
	for(i=2;i<4;i++)for(j=0;j<2;j++){
		for(s=0;s<2;s++)for(t=0;t<2;t++){ //total 3*2*2=12 elements
			if(s==1&&t==1)continue;//skip
			list[k][0]=i;list[k][1]=j;list[k][2]=s;list[k][3]=t;
			k++;
		}
	}
	for(i=2;i<4;i++)for(j=2;j<4;j++){//total 2*2=4 elements
		s=t=0;
		list[k][0]=i;list[k][1]=j;list[k][2]=s;list[k][3]=t;
		k++;
	}
	if(k!=TNUM){
		fprintf(stderr, "invalid initialization count\n");
		exit(-1);
	}
	for(i=0;i<TNUM;i++){
		printf("%02d:{",i);
		for(j=0;j<NUMP;j++){
			printf(" %d",list[i][j]);
		}
		printf(" }\n");
	}
}

int step;
char used[TNUM];
char v[ROW];
long numweights[TNUM];

long tv;
std::vector<long> tvs;

void staticnums()
{
	int i;
	for(i=0;i<tvs.size();i++){
		int j;
		for(j=0;j<TNUM;j++){
			if(tvs[i]&(1L<<j)){
				numweights[j]++;
			}
		}
	}
	long rmmask=0L;
	int bitleft=64;
	for(i=0;i<TNUM;i++){
		printf("w[%d]=%ld\n",i,numweights[i]);
		if(numweights[i]<3700){
			bitleft--;
			rmmask|=1L<<i;
		}
	}
	std::vector<long> tvs2;
	for(i=0;i<tvs.size();i++){
		if((tvs[i]&rmmask)==0){
			tvs2.push_back(tvs[i]);
		}

	}
	tvs=tvs2;
	printf("Total %d bit left, %ld rows after opt\n",bitleft, tvs.size());
}

void output()
{
	int i,j;
	int sum[NUMP];
	for(i=0;i<NUMP;i++)sum[i]=0;
	for(i=0;i<ROW;i++){
		int x=v[i];
		for(j=0;j<NUMP;j++){
			sum[j]+=list[x][j];
		}
	}
	for(j=0;j<NUMP;j++){
		if(sum[j]!=target[j])break;
	}
	if(j==NUMP){
		tvs.push_back(tv);
	}
}

void searchrow(int start)
{
	int i;
	if(step==ROW){
		output();
		return;
	}
	for(i=start;i<TNUM;i++){
		tv|=1l<<i;
		v[step]=i;
		step++;
		searchrow(i+1);
		tv&=~(1l<<i);
		step--;
	}
}

long rowid[ROW];
long columnid[ROW];
long rowmask, colmask;
long cmbcount;
int firstbit(long x){
	int i;
	for(i=0;i<TNUM;i++){
		if(x&(1l<<i))return i;
	}
	return -1;
}
void outputrows()
{
	char x[ROW],y[ROW];
	int i;
	for(i=0;i<ROW;i++){x[i]=y[i]=i;}
	int sum1[NUMP],sum2[NUMP];
	do{
		do{
			int i,j;
			long v1=0,v2=0;
			int failed = 0;
			for(i=0;i<ROW;i++){
				if(v1&rowid[x[i]]&columnid[y[i]]){
					failed=1;break;
				}
				v1|=rowid[x[i]]&columnid[y[i]];
				if(v2&rowid[x[i]]&columnid[y[ROW-1-i]]){
					failed=1;break;
				}
				v2|=rowid[x[i]]&columnid[y[ROW-1-i]];
			}
			for(i=0;i<tvs.size();i++){
				if(v1==tvs[i])break;
			}
			if(i==tvs.size())continue;
			for(i=0;i<tvs.size();i++){
				if(v2==tvs[i])break;
			}
			if(i==tvs.size())continue;
			printf("Sol:\n");
			for(i=0;i<ROW;i++){
				for(j=0;j<ROW;j++){
					long h=rowid[x[i]]&columnid[y[j]];
					printf("%02d,",firstbit(h));
				}
				printf("\n");
			}
			printf("\n");
			fflush(stdout);
			return;

		}while(std::next_permutation(x,x+6));
	}while(std::next_permutation(y,y+6));

}

int is_single_bit(long x)
{
	if(x==0)return 0;
	return (x&(x-1))==0;
}

void searchrows(int rit, int cit)
{
	int i;
	int rc=0;
	int hit,lit;
	if(step==ROW){
		cmbcount++;
		outputrows();
		return;
	}
	for(hit = rit; hit!= tvs.size();++hit){
		rc++;
		if(rowmask&tvs[hit])continue;
		if(step==1)printf("\t");
		if(step<=1){
			int tdiff=time(NULL)-starttime;
			fprintf(stderr,"Search row %d for step %d(%d seconds)\n",rc,step, tdiff);
		}
		for(i=0;i<step;i++){
			if(!is_single_bit(tvs[hit]&columnid[i]))
				break;
		}
		if(i<step)continue;
		if(step>=ROW-1){
			for(i=0;i<step;i++){
				TRACE("row %d v %lx ", i, rowid[i]);
			}
			TRACE("row %d v %lx\n",step, tvs[hit]);
		}
		rowid[step]=tvs[hit];
		rowmask|=tvs[hit];
		for(lit = cit; lit!=tvs.size(); ++lit){
			if(colmask&tvs[lit])continue;
			for(i=0;i<=step;i++){
				if(!is_single_bit(tvs[lit] & rowid[i]))
					break;
			}
			if(i<=step)continue;
			if(step>=ROW-1){
				for(i=0;i<step;i++){
					TRACE("col %d v %lx",i,columnid[i]);
				}
				TRACE("col %d v %lx\n",step, tvs[lit]);
			}
			columnid[step]=tvs[lit];
			colmask|=tvs[lit];
			step++;
			searchrows(hit+1,lit+1);
			colmask&=~tvs[lit];
			step--;
		}
		rowmask &= ~tvs[hit];
	}
}

int main()
{
	starttime = time(NULL);
	initlist();
	searchrow(0);
	printf("Total %ld rows found\n",tvs.size());fflush(stdout);
	staticnums();
#if 1
	searchrows(0,0);
	printf("%ld\n",cmbcount);
#endif
	return 0;
}
