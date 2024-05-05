#include <gmpxx.h>
#include <omp.h>
#include <iostream>
#include <stdio.h>
#include <vector>

unsigned long M=2022397710683865089UL;

std::vector<mpz_class> tailmul;
unsigned long MINV = 1277165569UL;
void initlist()
{
    int i,j,s,t;
    unsigned long p2=1L<<32;p2-=1;
    for(i=0;i<26;i++)for(j=0;j<26;j++)for(s=0;s<26;s++)for(t=0;t<26;t++){
	unsigned long v=i*256UL*65536UL+j*65536UL+s*256UL+t;
	v*=MINV;
	v&=p2;
	mpz_class m=M;
	m*=v;
	tailmul.push_back(m);
    }
}


int main()
{
	initlist();
#pragma omp parallel
	{
	    int nt = omp_get_num_threads();
	    unsigned long p2=1L<<32;
	    unsigned char decbuf[256];
	    int tid = omp_get_thread_num();
	    mpz_class mul = M;
	    mul*=nt*p2;
	    mpz_class v =M;
	    v*=tid*p2;//initial value
	    long count =0;
	    do{
		int h;
		for(h=0;h<tailmul.size();++h){
		    mpz_class w=v+tailmul[h];
		    size_t cnt;
		    mpz_export(decbuf, &cnt, -1,1,0, 0, w.get_mpz_t());
		    int i;
		    for(i=0;i<cnt;i++){
			if(decbuf[i]>=26)break;
		    }
		    if(i>=cnt){
#pragma omp critical
			    std::cout<<w<<std::endl;
			    fflush(stdout);
		    }
		}
		v+=mul;
		count++;
		if(count%10000==0){
#pragma omp critical
			    std::cerr<<"Reach "<<count << " in thread "<<tid<<std::endl;
			    fflush(stderr);
		}
	    }while(1);
	}
}

