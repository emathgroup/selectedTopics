#include <map>
#include <iostream>

typedef long mtype;
typedef std::map<mtype, long> DMAP;
#define ONE ((mtype)1)
DMAP dmap1,dmap2,dmap3;
DMAP *pmap, *qmap, *tmap, *hmap;
#ifndef N
#define N 37
#endif

#define IS ((N*(N+1))/2)
#define ISR 14
#define ISL 14

void initLN()
{
    int i;
    for(i=1;i<=N;i++){
	mtype x=1;
	if(IS%i!=0)continue;
	x<<=i;
	dmap1.insert(std::make_pair(x,1L));
    }
    pmap=&dmap1;
}

void initL1()
{
    int i;
    for(i=2;i<=N;i++){
	mtype x=1;
	x<<=i;
	dmap3.insert(std::make_pair(x,1L));
    }
    hmap=&dmap3;
    qmap=&dmap2;
}


int get_sum(mtype x)
{
    int i,s=0;
    mtype h=2;
    for(i=1;i<=N;i++){
	if(x&h)s+=i;
	h<<=1;
    }
    return s;
}

void update_map()
{
    qmap->clear();
    DMAP::iterator it;
    int i;
    for(it=pmap->begin();it!=pmap->end();++it){
	int sum=IS-get_sum(it->first);
	mtype h=2;
	for(i=1;i<=N;i++,h<<=1){
	    if(h&it->first)continue;
	    if((sum%i)!=0)continue;
	    (*qmap)[it->first|h]+=it->second;
	}
    }
    tmap=qmap;qmap=pmap;pmap=tmap;
}

void update_hmap()
{
    qmap->clear();
    DMAP::iterator it;
    int i;
    for(it=hmap->begin();it!=hmap->end();++it){
	int sum = get_sum(it->first);
	mtype h=2;
	for(i=1;i<=N;i++,h<<=1){
	    if(h&it->first)continue;
	    if((sum%i)!=0)continue;
	    (*qmap)[it->first|h]+=it->second;
	}
    }
    tmap=qmap;qmap=hmap;hmap=tmap;
}

long tcount;
void search(long used,long lcount, int level, int sum)
{
    int i;
    if(level<=ISR){
	long left = ((2L<<N)-2)&(~used);
	DMAP::iterator dit = hmap->find(left);
	if(dit!=hmap->end()){
	    long r=lcount*dit->second;
#pragma omp atomic update
	    tcount+=r;
	}
	return;
    }
    for(i=N;i>=1;i--){
	if(used&(1L<<i))continue;
	if(sum%i!=0)continue;
	used|=1L<<i;
	search(used,lcount, level-1,sum-i);
	used&=~(1L<<i);
    }
}

int main()
{
    int i;
    initL1();
    initLN();
    for(i=2;i<=ISR;i++){
	update_hmap();
    }
    for(i=2;i<=ISL;i++){
	update_map();
    }
    DMAP::iterator it = pmap->begin();
#pragma omp parallel
    {
	DMAP::iterator the_it;
	bool is_valid=true;
	long used = 0;
	do{
#pragma omp critical
	    {
		if(it == pmap->end()){
		    is_valid=false;
		}else{
		    the_it=it++;
		}
	    }
	    if(!is_valid)break;
	    search(the_it->first, the_it->second, N-ISL, IS-get_sum(the_it->first));
	}while(1);
    }
    printf("a[%d]=%ld\n",N,tcount);
    return 0;
}
