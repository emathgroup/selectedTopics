#include <stdio.h>
#include <gmpxx.h>
#include <set>
#include <vector>
#include <list>
#include <bitset>
#include <iostream>
#include <algorithm>

#define MAXR 1024
#ifndef N
#define N 2
#endif
typedef std::bitset<MAXR> dtype;

#define ONE ((dtype)1)
int isp[MAXR];
int pc;
int plist[MAXR];

bool operator<(const dtype& d1, const dtype& d2)
{
    int i;
    for(i=0;i<MAXR;i++){
	bool b1=d1[i];
	bool b2=d2[i];
	if(!b1&&b2)return true;
	if(b1&&!b2)return false;
    }
    return false;
}

struct Data{
    mpz_class mul;
    dtype nlist;
    dtype sqrinfo;
    bool operator<(const Data& d)const{
	if(mul<d.mul)return true;
	if(mul>d.mul)return false;
	if(nlist<d.nlist)return true;
	if(d.nlist<nlist)return false;
	return sqrinfo<d.sqrinfo;
    }
};


typedef std::set<Data> MySet;
dtype dec[MAXR];
MySet alldata;

void initp()
{
    int i,j;
    for(i=2;i<MAXR;i++)isp[i]=1;
    for(i=2;i<MAXR;i++){
	if(isp[i]){
	    plist[pc++]=i;
	    for(j=i*i;j<MAXR;j+=i){
		isp[j]=0;
	    }
	}
    }
    for(i=1;i<MAXR;i++){
	int x=i;
	dtype r;
	for(j=0;j<pc;j++){
	    if(plist[j]>x)break;
	    int jc=0;
	    while(x%plist[j]==0){
		jc++;x/=plist[j];
	    }
	    if(jc&1){
		r.set(j);
	    }
	}
	dec[i]=r;
    }
}

mpz_class gprod;
std::vector<int> ilist;
mpz_class target;
std::vector<int> tlist;

bool find(int starti, int level)
{
    int i;
    if(level==N){
	    if(gprod==target){
		std::vector<int> v1,v2;
		for(i=0;i<N;i++){
			v1.push_back(tlist[i]);
		}
		int j;
		for(i=0,j=0;i<2*N;i++){
		    if(j>=N||ilist[i]!=tlist[j]){
			v2.push_back(ilist[i]);
		    }else{
			j++;
		    }
		}
		std::sort(v1.begin(),v1.end());
		std::sort(v2.begin(),v2.end());
		std::cout<<"{";
		for(i=0;i<N;i++){
		    std::cout<<" "<<v1[i];
		}
		std::cout<<" }{";
		for(i=0;i<N;i++){
		    std::cout<<" "<<v2[i];
		}
		std::cout<<" }"<<std::endl;
		return true;
	    }
	    return false;
    }
    bool r=false;
    for(i=starti;i<2*N;i++){
	gprod*=ilist[i];
	tlist.push_back(ilist[i]);
	if(target%gprod==0){
	    r = find(i+1,level+1);
	}
	tlist.pop_back();
	gprod/=ilist[i];
	if(r)break;
    }
    return r;
}

bool findhalf(const Data& d)
{
    gprod=1;
    ilist.clear();
    tlist.clear();
    int i,j;
    int c=0;
    for(i=1;i<MAXR;i++){
	if(d.nlist.test(i)){
	    c++;
	    if(c==N)break;
	}
    }
    for(c=0,j=i+1;c<N;c++){
	while(!d.nlist.test(j))j++;
	while(!d.nlist.test(i))i--;
	ilist.push_back(i);
	ilist.push_back(j);
	j++;i--;
    }
    target=sqrt(d.mul);
    return find(0,0);
}

int main()
{
    int n,m;
    initp();

    Data d;
    d.mul=1;
    d.sqrinfo=0;
    d.nlist=0;
    for(m=1;m<=2*N;m++){
	d.mul*=m;
	d.nlist.flip(m);
	d.sqrinfo^=dec[m];
    }
    alldata.insert(d);

    auto it = alldata.begin();

    while(1){
	if(it->nlist==0){
		std::cerr<<"overflow\n";
		fflush(stderr);
		break;//end of search
	}
	if(it->sqrinfo==0){//find result
	    std::cout<<"N="<<N<<std::endl;
	    std::cout<<sqrt(it->mul)<<std::endl;
	    auto nit=it;
	    while(nit!=alldata.end()&&nit->mul==it->mul){
		if(findhalf(*nit))break;
		nit++;
	    }
	    fflush(stdout);
	    break;
	}
	for(n=MAXR-1;n>=0;n--){
	    if(it->nlist.test(n))break;//find last digit
	}
	if(n==MAXR-1){
            d.nlist=0;//flag for end of search
	    d.mul=(it->mul/n)*m;
	    d.sqrinfo=0;
	    alldata.insert(d);
	}else{
	    m=n+1;
	    d.nlist=it->nlist;
	    d.nlist.flip(n);d.nlist.flip(m);
	    d.mul=(it->mul/n)*m;
	    d.sqrinfo=it->sqrinfo^dec[n]^dec[m];
	    alldata.insert(d);
	}
	while(it->nlist.test(n))n--;
	if(n>=1 && it->nlist.test(n-1)){
	    m=n-1;
	    d.nlist=it->nlist;
	    d.nlist.flip(n);d.nlist.flip(m);
	    d.mul=(it->mul/m)*n;
	    d.sqrinfo=it->sqrinfo^dec[n]^dec[m];
	    alldata.insert(d);
	}
	++it;
    }

    return 0;
}
