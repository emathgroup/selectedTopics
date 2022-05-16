#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <set>

#define MAXSTEP 1260
#define C2 80
#define C3 50
#define C5 40
#define C7 30
#define NU 36
#define NUMP 4

int plist[]={2,3,5,7};
double logp[NUMP];
int decode(int x, int cnt[])
{
	int i;
	for(i=0;i<NUMP;i++)cnt[i]=0;
	for(i=0;i<NUMP;i++){
		while(x%plist[i]==0){
			cnt[i]++;
			x/=plist[i];
		}
	}
	if(x==1)return 1;
	return 0;
}

struct PROD{
	unsigned char count[NUMP];
	bool operator<(const PROD& prod)const{
		int i;
		for(i=0;i<NUMP;i++){
			if(count[i]<prod.count[i])return true;
			if(count[i]>prod.count[i])return false;
		}
		return false;
	}
	bool operator==(const PROD& prod)const{
		int i;
		for(i=0;i<NUMP;i++){
			if(count[i]!=prod.count[i])return false;
		}
		return true;
	}
};

std::set<PROD> prods[NU];
int main()
{
	int i,j,n;
	int cnt[NUMP];
	for(i=0;i<NUMP;i++)logp[i]=log(plist[i]);
	for(i=1;i<=MAXSTEP;i++){
		if(decode(i,cnt)){
			double v=0;
			PROD prod;
			printf("#Step: %d\n",i);
			for(n=NU-2;n>=0;n--){
				std::set<PROD>::iterator cit;
				for(cit = prods[n].begin(); cit != prods[n].end(); ++cit){
					for(j=0;j<NUMP;j++){
						if(cit->count[j]+cnt[j]>=256){
							fprintf(stderr, "overflow\n");
							break;
						}
					}
					if(j==NUMP){
						for(j=0;j<NUMP;j++){
							prod.count[j]=cit->count[j]+cnt[j];
						}
						std::set<PROD>::iterator sit = prods[n+1].find(prod);
						if(sit == prods[n+1].end()){//new elements found
							prods[n+1].insert(prod);
							if(n==NU-2){
							for(j=0;j<NUMP;j++)if(prod.count[j]%6!=0)break;
								if(j==NUMP){
									double v=0;
									for(j=0;j<NUMP;j++)v+=prod.count[j]/6*logp[j];
									printf("%f\t2^%d*3^%d*5^%d*7^%d\n",v,prod.count[0]/6,prod.count[1]/6,prod.count[2]/6,prod.count[3]/6);
									fflush(stdout);
								}
							}
						}
					}

				}
			}
			for(j=0;j<NUMP;j++)prod.count[j]=cnt[j];
			prods[0].insert(prod);
			fprintf(stderr,"step %d\n",i);
		}
	}
	return 0;
}
