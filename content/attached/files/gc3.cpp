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
int decc[MAXR][MAXR];
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
	    decc[i][j]=jc;
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

long find(int starti, int level)
{
    int i;
    if(level==N){
	    if(gprod==target){
#if 0
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
#endif
		return 1;
	    }
	    return 0;
    }
    long r=0;
    for(i=starti;i<2*N;i++){
	gprod*=ilist[i];
	tlist.push_back(ilist[i]);
	if(target%gprod==0){
	    r += find(i+1,level+1);
	}
	tlist.pop_back();
	gprod/=ilist[i];
    }
    return r;
}

int pcount[MAXR];
int maxdim;
long blen;
long *buf;

bool indexinrange(int c[MAXR])
{
    int i;
    for(i=0;i<maxdim;i++){
	if(c[i]>pcount[i])return false;
    }
    return true;
}

long count2index(int c[MAXR])
{
    int i;
    long index=0;
    for(i=0;i<maxdim;i++){
	index*=pcount[i]+1;
	index+=c[i];
    }
    return index;
}

bool gonext(int cindex[MAXR], int lc[MAXR])
{
    int i;
    for(i=maxdim-1;i>=0;i--){
	if(cindex[i]<pcount[i]){
	    cindex[i]++;
	    return true;
	}else{
	    cindex[i]=lc[i];
	}
    }
    return false;
}

void addreduce(int x, int level, int t)
{
    int lc[MAXR];
    int cindex[MAXR];
    int dindex[MAXR];
    int i;
    for(i=0;i<maxdim;i++){
	    lc[i]=decc[x][i]*t;
	    if(lc[i]>pcount[i])return;
	    cindex[i]=lc[i];
    }
    int sgn = -1;
    if(t&1)sgn= 1;
    do{
	    long cc = count2index(cindex);
	    for(i=0;i<maxdim;i++)dindex[i]=cindex[i]-lc[i];
	    long dd = count2index(dindex);
	    buf[level*blen+cc]+=sgn*buf[(level-t)*blen+dd];
    }while(gonext(cindex, lc));
}

void reducelast(int x, int level)
{
    int lc[MAXR];
    int i;
    for(i=0;i<maxdim;i++){
	lc[i]=decc[x][i]*(level+1);
	if(lc[i]>pcount[i])return;
    }
    int sgn = -1;
    if((level+1)&1)sgn=1;
    buf[level*blen+count2index(lc)]+=sgn;
}

long findhalf(const Data& d)
{
    int i,j;
    for(i=0;i<pc;i++)pcount[i]=0;
    for(i=1;i<MAXR;i++){
	if(d.nlist.test(i)){
	    for(j=0;j<pc;j++){
		pcount[j]+=decc[i][j];
	    }
	}
    }
    blen=1L;
    for(i=pc-1;i>=0;i--)if(pcount[i]>0)break;
    maxdim=i+1;
    for(i=0;i<maxdim;i++)pcount[i]/=2;//find square root
    for(i=0;i<maxdim;i++){
	    blen*=(pcount[i]+1);
	    if(blen>0x7FFFFFFFFFL){
		std::cerr<<"Buffer len overflow\n";
		exit(-1);
	    }
    }
    buf = new long[N*blen];
    if(NULL == buf){
	    std::cerr<<"Out of memory\n";
	    exit(-1);
    }
    memset(buf,0,N*blen*sizeof(long));

    gprod=1;
    ilist.clear();
    tlist.clear();
    for(i=1;i<MAXR;i++){
	if(d.nlist.test(i)){
		ilist.push_back(i);
	}
    }

    if(ilist.size()!=2*N){
	   std::cerr<<"Invalid count of integer\n";
	   exit(-1);
    }
    for(i=0;i<2*N;i++){
	    int v=ilist[i];
	    if(indexinrange(decc[v])){
		    long ind = count2index(decc[v]);
		    buf[ind]=1L;
	    }
    }
    for(j=1;j<N;j++){//cal for j+1 numbers
	int t;
	for(t=1;t<=j;t++){
	   for(i=0;i<2*N;i++){
	      addreduce(ilist[i],j,t);
	   }
	}
	for(i=0;i<2*N;i++){
	    reducelast(ilist[i],j);
	}
	for(i=0;i<blen;i++){
	    if(buf[j*blen+i]%(j+1)!=0){
		std::cerr << "Invalid buffer data, not multiple of "<<j+1<<std::endl;
		exit(-1);
	    }
	    buf[j*blen+i]/=j+1;
	}
    }

    long r = buf[N*blen-1];

    delete []buf;
    return r;
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
	    std::cout<<sqrt(it->mul)<<"<";
	    auto nit=it;
	    long c=0;
	    while(nit!=alldata.end()&&nit->mul==it->mul){
	//	if(findhalf(*nit));
		c++;
		nit++;
	    }
	    std::cout<<c<<">"<<std::endl;
	    nit=it;
	    while(nit!=alldata.end()&&nit->mul==it->mul){
		std::cout<<"{ ";
		int ii;
		for(ii=0;ii<MAXR;ii++){
		    if(nit->nlist.test(ii)){
			std::cout<<ii<<" ";
		    }
		}
		std::cout<<"}<";
		std::cout<<findhalf(*nit)/2;
		std::cout<<">";
		std::cout<<std::endl;
		nit++;
	    }
	    std::cout<<std::endl;
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
	{
            auto nit=it;
	    ++nit;
	    alldata.erase(it);
	    it=nit;
	}
    }

    return 0;
}
