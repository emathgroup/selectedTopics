#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmpxx.h>
#include <iostream>

#define MAXP 1000
int plist[MAXP];
int pc;
char isp[MAXP];

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
}

int main()
{
	int i,j,s,t,u,v;
	long c=0;
	init();
	for(i=0;i<pc;i++)for(j=i+1;j<pc;j++)for(s=j+1;s<pc;s++)for(u=s+1;u<pc;u++)for(v=u+1;v<pc;v++){
		if(plist[i]+plist[j]+plist[s]+plist[u]+2*plist[v]>=MAXP)break;
		for(t=v+1;t<pc;t++){
			if(plist[i]+plist[j]+plist[s]+plist[t]+plist[u]+plist[v]>MAXP)break;
			if(plist[i]+plist[j]+plist[s]+plist[t]+plist[u]+plist[v]==MAXP){
//				printf("%d+%d+%d+%d+%d+%d\n",plist[i],plist[j],plist[s],plist[u],plist[v],plist[t]);
				c++;
			}
		}
	}
	printf("total %lld\n",c);
	return 0;
}
