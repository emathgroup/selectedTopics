#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <omp.h>

typedef __int128_t int128_t;
#define SR 30
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


int main()
{
    initdigs();
#pragma omp parallel
    {
	int tnums = omp_get_num_threads();
	int tid = omp_get_thread_num();
	int64_t minint[SR];
	int64_t i;
	for(i=0;i<SR;i++)minint[i]=-1;
        for(i=(tid+1)*3;i<0x1000000000000L;i+=3*tnums){
	    int128_t x = (int128_t)i;
	    x*=x;
	    int v=numdigits(x)/9;
	    if(v<SR){
		if(minint[v]<0){
			minint[v]=i;
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
    return 0;
}

