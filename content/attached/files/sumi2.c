#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <omp.h>

typedef __int128_t int128_t;
#define SR 30
int64_t gmin[SR];
int numdigits(int128_t x)
{
    int s=0;
    while(x>0){
	    s+=x%10;
	    x/=10;
    }
    return s;
}
int lendigit(int64_t x, int64_t *muls)
{
    int s=0;
    int64_t lm=1;
    while(x>0){
	    s++;
	    x/=10;
	    lm*=10;
    }
    *muls=lm;
    return s;
}
int main()
{
#pragma omp parallel
    {
	int tnums = omp_get_num_threads();
	int tid = omp_get_thread_num();
	int64_t minint[SR];
	int64_t i;
	int ilen;
	int64_t nextt=10;
	for(i=0;i<SR;i++)minint[i]=-1;
	int64_t initi=(tid+1)*3;
	ilen = lendigit(initi, &nextt);
        for(i=initi;i<0x1000000000000L;i+=3*tnums){
	    int128_t x = (int128_t)i;
	    while(i>=nextt){
		    ilen++;
		    nextt*=10;
	    }
	    x*=x;
	    int v=numdigits(x)/9;
	    if(v<SR&&v>ilen){
		if(minint[v-ilen]<0){
			minint[v-ilen]=i;
#pragma omp critical
			{
			    if(gmin[v-ilen]==0||minint[v-ilen]<gmin[v-ilen]){
				  gmin[v-ilen]=minint[v-ilen];
				  printf("%d %ld\n",v-ilen,minint[v-ilen]);
				  fflush(stdout);
			    }
			}
		}
	    }
        }
    }
    return 0;
}

