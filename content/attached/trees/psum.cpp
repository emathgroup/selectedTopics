#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmpxx.h>
#include <iostream>

#define MAXP 10000
#define RND 20
int plist[MAXP];
int pc;
char isp[MAXP];
mpz_class buf[RND][MAXP+1];

void init()
{
	int i,j;
	for(i=2;i<MAXP;i++)isp[i]=1;
	for(i=2;i<MAXP;i++){
		if(isp[i]){
			for(j=i*i;j<MAXP;j+=i){
				isp[j]=0;
			}
		}
	}
	for(i=3;i<MAXP;i++){
		if(isp[i]){
			plist[pc++]=i;
		}
	}
	for(i=0;i<pc;i++){
		buf[0][plist[i]]=1;//count of single number to reach the sum
	}
	for(i=0;i<pc;i++){
		for(j=i+1;j<pc;j++){
			if(plist[i]+plist[j]>MAXP)break;
			buf[1][plist[i]+plist[j]]++;//count of two different primes reach the sum
		}
	}
}

int main()
{
	int i,j,k,t;
	init();
	for(i=2;i<RND;i++){
		for(j=0;j<pc;j++){
			int v=plist[j];
			for(k=0;k<=MAXP-v;k++){
				buf[i][k+v]+=buf[i-1][k];
			}
		}
		int sgn=-1;
		for(t=2;t<=i+1;t++){
		    for(j=0;j<pc;j++){
			int v=t*plist[j];
			if(v<=MAXP){
			    if(t<=i){
				for(k=0;k<=MAXP-v;k++){
					buf[i][k+v]+=sgn*buf[i-t][k];//remove P+P + sum_of(i-2+1) primes and add back 3P+* etc
				}
			    }else{
				buf[i][v]+=sgn;//sum of t primes
			    }
			}else break;
		    }
		    sgn*=-1;
		}
		for(j=0;j<=MAXP;j++){
			if(buf[i][j]%(i+1)!=0){
				std::cerr << "Invalid value of buf["<<i<<","<<j<<"]="<<buf[i][j]<<"; not multiple of "<<i+1<<std::endl;
			}
			buf[i][j]/=i+1;
		}
		if(i%2==1){
			std::cout<<"Sum of "<<i+1<<" primes\n"; 
			for(j=1000;j<=MAXP;j+=1000){
				if(buf[i][j]>0){
					std::cout<<"S["<<j<<"]="<<buf[i][j]<<std::endl;
				}
			}
		}
	}
	return 0;
}
