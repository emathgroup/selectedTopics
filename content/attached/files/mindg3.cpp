#include <stdio.h>
#include <gmpxx.h>
#include <vector>
#include <iostream>
#include <algorithm>

std::vector<int> smalllist;
std::vector<long> longlist;
#define BLOCKSIZE 10000
#define STARTS 6
#define MAXS   10
#define SBIT  MAXS
long GM;
long MAXV;
long MINV;
#define UBIT   20
#define EBIT   34
#ifndef D1
#define D1     4
#endif
#ifndef D2
#define D2     8
#endif
#ifndef D3
#define D3     9
#endif
//              0,    1 ,   2,   3,     4,    5,    6,    7,    8,   9
bool vset[10]={false,false,false,false,false,false,false,false,false,false};
char enc[3]={D1,D2,D3};
long gid=0;
void setvset()
{
    vset[enc[0]]=vset[enc[1]]=vset[enc[2]]=true;
}
bool allinset(const mpz_class& y)
{
    mpz_class x=y;
    while(x>0){
	mpz_class t=x%10;
	int d=t.get_si();
	x/=10;
	if(!vset[d])return false;
    }
    return true;
}
bool allinset(__int128_t y)
{
    while(y>0){
	int t=y%10;
	y/=10;
	if(!vset[t])return false;
    }
    return true;
}

int bestv=0;

#define SMALLR 1000000
#define LEVEL 6
void initsmalls()
{
    long i;
    int j;
    for(i=1;i<SMALLR;i++){
	int x=(i*i)%SMALLR;
	if(enc[0]==0&&i%10==0)continue;
	for(j=0;j<LEVEL;j++){
	    if(!vset[x%10])break;
	    x/=10;
	}
	if(j>=LEVEL){
	    smalllist.push_back(i);
	}
    }
}

struct SV{
    long v;
    long tens;
    int level;
    int sz;
};

struct GSV{
    long v;
    int level;
};

void searcht(SV *sv)
{
    if(sv->level==MAXS){
	    {
#pragma omp critical
	longlist.push_back(sv->v);
	    }
	return;
    }
#if 0
    mpz_class sqrv=sv->v*sv->v;
    if(!sv->sz&&allinset(sqrv)){
#pragma omp critical
	    {
		    std::cout<<"Found "<<sv->v<<std::endl;
		    fflush(stdout);
	    }
    }
#else
    __int128_t sqrv;
#endif
    int i;
    sv->level++;
    sv->tens*=10;
    for(i=0;i<=9;i++){
	sqrv = ((__int128_t)sv->v*sv->v)%sv->tens;
	sv->sz = (i==0);
	if(allinset(sqrv)){
            searcht(sv);
	}
	sv->v+=sv->tens/10;
    }
    sv->v-=sv->tens;
    sv->tens/=10;
    sv->level--;
}

bool getinitv(long cid, long& ret)
{
    int i;
    long mul=1;
    ret = 0;
    for(i=0;i<SBIT;i++){
	int x=cid%3;
	ret+=enc[x]*mul;mul*=10;
	cid/=3;
    }
    return true;
}

void verify(const mpz_class& x)
{
    mpz_class z=x*x;
    if(enc[0]==0&&x%10==0)return;
    if(allinset(z)){
#pragma omp critical
	    {
		    std::cout<<"Found "<<x<<std::endl;
		    fflush(stdout);
	    }
    }
}

void check(const mpz_class& start, const mpz_class& end, int k)
{
    int tlen = (k+1)/2;
    long mod = GM;
    long leftbit=tlen-SBIT;
    if(leftbit<=0)return;
    long mod2=1;
    int i;
    for(i=0;i<leftbit;i++)mod2*=10;
    if(start/mod == end/mod){
	    mpz_class s1=start%mod;
	    mpz_class e1=end%mod;
	    long ls1 = s1.get_si();
	    long le1 = e1.get_si();
	    std::vector<long>::iterator sit = std::lower_bound(longlist.begin(),longlist.end(),ls1);
	    mpz_class y=(start/mod)*mod;
	    while(sit!=longlist.end()){
		if(*sit>le1)break;
		if(*sit>=ls1&&*sit<=le1){
			mpz_class x=y + (*sit);
			verify(x);
		}
		++sit;
	    }
    }else{
	    mpz_class s1=start%mod;
	    long ls1 = s1.get_si();
	    std::vector<long>::iterator sit = std::lower_bound(longlist.begin(),longlist.end(),ls1);
	    mpz_class y=(start/mod)*mod;
	    while(sit!=longlist.end()){
		    mpz_class x= y + (*sit);
		    verify(x);
		    ++sit;
	    }
	    s1=end%mod;
	    ls1=s1.get_si();
	    sit = std::upper_bound(longlist.begin(),longlist.end(),ls1);
	    if(sit!=longlist.end()){
		    mpz_class y = (end/mod)*mod;
		    int endindex = sit-longlist.begin()+1;
		    int j;
		    for(j=0;j<endindex;j++){
			    mpz_class x =y+(longlist[j]);
			    verify(x);
		    }
	    }
    }
}

mpz_class startm,startm2;
void searchh(GSV *sv)
{
    long i;
    mpz_class x=sv->v;
    x*=(GM);
//    for(i=0;i<1L<<(SBIT);i++){
//	long j;
//	if(getinitv(i, j)){
//	    mpz_class y=x;y+=j;
	    mpz_class z=x+(GM);
	    int k;
	    mpz_class sx1 = sqrt(x*startm);
	    mpz_class sx2 = sqrt(x*startm2);
	    mpz_class xz1 = sqrt(z*startm);
	    mpz_class xz2 = sqrt(z*startm2);
	    for(k=EBIT;k>=UBIT;k-=2){
		check(sx1,xz1,k-1);
		check(sx2,xz2,k);
		sx1/=10;sx2/=10;
		xz1/=10;xz2/=10;
	    }
//	}
//    }
}

int main()
{
    setvset();
    initsmalls();
    startm=1;
    int i;
    GM=1;MINV=1;
    for(i=0;i<SBIT;i++)GM*=10;
    for(i=0;i<SBIT-1;i++)MINV*=3;
    MAXV=MINV*3;
    if(enc[0]!=0)MINV=0;
    for(i=2*SBIT;i<EBIT-1;i++){
	startm*=10;
    }
    startm2=startm*10;
    printf("size is %ld\n",smalllist.size());fflush(stdout);
#pragma omp parallel
    {
	 long cid;
	 long i;
      while(1){
	 #pragma omp critical
	    {
		cid = gid++;
	    }
	    if(cid>=smalllist.size())break;
	    SV sv;
	    sv.v = smalllist[cid];
	    sv.level=STARTS;
	    sv.tens=SMALLR;
	    sv.sz = cid<SMALLR/10;

	    searcht(&sv);
      }
    }
    std::cout<<"Search tail done\n";
    std::sort(longlist.begin(),longlist.end());
    std::cout<<"Total count "<<longlist.size()<<std::endl;
    if(longlist.size()==0)return 0;

    gid=MINV;
#pragma omp parallel
    {
	long cid=-1;
	long i;
	GSV gsv;
	while(1){
#pragma omp critical
		{
			cid = gid++;
/*			if(cid%10000==0){
				std::cerr<<cid<<std::endl;
				fflush(stderr);
			}*/
		}
		if(cid>=MAXV)break;
		long initv;
		if(getinitv(cid, initv)){
		    gsv.v=initv;
		    gsv.level=SBIT;
		    searchh(&gsv);
		}
	}
    }

    return 0;
}
