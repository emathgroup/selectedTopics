#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <omp.h>
#include <vector>

typedef __int128_t int128_t;
#define SR 40
int64_t gmin[SR];
#define STATM 10000
int digs[STATM];

int getdig(int x)
{
    int s=0;
    while(x>0){
	s+=x%10;
	x/=10;
    }
    return s;
}

void initdigs()
{
    int i;
    for(i=0;i<STATM;i++){
	digs[i]=getdig(i);
    }
}

int numdigits(int128_t x)
{
    int s=0;
    while(x>0){
	    s+=digs[x%STATM];
	    x/=STATM;
    }
    return s;
}


#define TL 100000000000L
#define LIM 73
std::vector<int64_t> lists;
int ltc;

void initlists()
{
    long i;
    for(i=0;i<TL;i++){
	int128_t x=i;
	x*=x;x%=TL;
	int r=numdigits(x);
	if(r>=LIM){
	    lists.push_back(i);
	}
    }
}

int main()
{
    initdigs();
    initlists();
    printf("Total %ld found\n",lists.size());
#pragma omp parallel
    {
	int tnums = omp_get_num_threads();
	int tid = omp_get_thread_num();
	int64_t minint[SR];
	int64_t i;
	int j;
	for(i=0;i<SR;i++)minint[i]=-1;
        for(i=(tid+1);i<0x1000000000000L;i+=tnums){
	  for(j=0;j<lists.size();j++){
	    int128_t x = ((int128_t)i*TL+lists[j]);
	    if(x%3!=0)continue;
	    int v=numdigits(x*x)/9;
	    if(v<SR){
		if(minint[v]<0){
			minint[v]=(int64_t)x;
#pragma omp critical
			{
			    if(gmin[v]==0||minint[v]<gmin[v]){
				  gmin[v]=minint[v];
				  printf("%d %ld\n",v,minint[v]);
				  fflush(stdout);
			    }
			}
		}
	    }
          }
	}
    }
    return 0;
}

