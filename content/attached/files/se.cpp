#include <stdlib.h>
#include <iostream>
#include <list>
#include <set>

int getk(int n)
{
    int s=n/12;
    int d=n%12;
    int r=d/3;
    return 4*s+r;
}

int md0[]={4,16,28,36,48,60,68,80,92,100,112,124};
int md1[]={0,1,3,5,9,14,18,25,33,39,49,60};
long getmd(int n)
{
    long s=n/12;
    int d=n%12;
    return 64*s*s+md0[d]*s+md1[d];
}

int getsk(int n)
{
    int s=n/12;
    int d=n%12;
    switch(d){
    case 0:
	return 6*s;
    case 3:
	return 4*s+1;
    case 5:
	return 6*s+3;
    case 6:
	return 2*s+1;
    case 8:
	return 4*s+3;
    case 11:
	return 2*s+2;
    case 1:
    case 2:
    case 4:
    case 7:
    case 9:
    case 10:
	return 0;
    default:
	std::cerr<<"Internal error\n";
	exit(-1);
    }
}

int ml0[]={32,96,160,224,288,352,416,480,544,608,672,736};
int ml1[]={5,16,37,69,112,165,229,304,389,485,592,709};
int ml2[]={1,1,3,8,15,27,43,65,94,130,175,229};
long getL(int n)
{
    long s=n/12;
    int d=n%12;
    return ((256*s+ml0[d])*s+ml1[d])*s+ml2[d];
}

std::list<int> sklist;
int genlist(int n)
{
    int k=getk(n);
    int sk = getsk(n);
    long L=getL(n);
    long d=getmd(n);//from 1 to d
    long skint=-1;
    std::set<int> usedint;
    if(sk>0){
	d++;
	skint=d-sk;
    }
    int i,j;
    for(i=1;i<=d;i++){
	if(i==skint)continue;
	sklist.push_back(i);
    }
    std::cout<<n<<" Sum "<<L<<std::endl;
    for(i=1;i<=k;i++){
	    //using i number to form sum L;
	    int ave=L/i;
	    int start=ave-i/2, end=start+i-1;
	    long total=((start+end)*(long)(end-start+1))/2;
	    long diff = total-L;
	    if(diff>=i){
		    start--;end--;diff-=i;
	    }
	    if(diff<=-i){
		    start++;end++;diff+=i;
	    }
	    std::cout<<i<<": ";
	    if(diff>0){
		    //start-1..start+diff-2, start+diff..end
		    int j;
		    for(j=start-1;j<=end;j++){
			if(j==start+diff-1)continue;
			if(usedint.find(j)!=usedint.end()){
				std::cerr<<"number "<<j<<" has been used\n";
				exit(-1);
			}
			usedint.insert(j);
			std::cout<<j<<" ";
		    }
	    }else if(diff<0){
		    diff=-diff;
		    //start..end-diff end-diff+2..end+1
		    int j;
                    for(j=start;j<=end+1;j++){
                        if(j==end-diff+1)continue;
                        if(usedint.find(j)!=usedint.end()){
                                std::cerr<<"number "<<j<<" has been used\n";
                                exit(-1);
                        }
                        usedint.insert(j);
                        std::cout<<j<<" ";
                    }
	    }else{
		    //start to end
		    int j;
                    for(j=start;j<=end;j++){
                        if(usedint.find(j)!=usedint.end()){
                                std::cerr<<"number "<<j<<" has been used\n";
                                exit(-1);
                        }
                        usedint.insert(j);
                        std::cout<<j<<" ";
                    }
	    }
	    std::cout<<std::endl;
    }
    for(i=k+1;i<=n-1;i++){
	//sequence with length i
	std::list<int>::iterator lit,eit;
	lit = sklist.begin();eit=lit;
	long sum=0;
	for(j=1;j<=i;j++){
	    sum+=*eit;
	    ++eit;
	}
	while(sum<L&&eit!=sklist.end()){
	    sum+=*eit;sum-=*lit;
	    ++eit;++lit;
	}
	if(sum<L){
		std::cerr<<"Sum too small: Fail to find list with length "<<i<<std::endl;
		exit(-1);
	}
	std::list<int> cands;
	std::list<int>::iterator tmp;
	for(tmp=lit;tmp!=eit;++tmp){
	    cands.push_back(*tmp);
	}
	sklist.erase(lit,eit);
	while(sum>L){
	    long delta=sum-L;
	    std::list<int>::iterator pit=eit;
	    if(pit==sklist.begin()){
		    std::cerr<<"Sum too large: Fail to find list with length "<<i<<std::endl;
		    exit(-1);
	    }
	    --pit;//move one element forward
	    for(tmp=cands.begin();tmp!=cands.end();++tmp){
		if(*tmp-*pit>delta)break;
	    }
	    if(tmp==cands.end()){
		    std::cerr<<"Internal error in length "<<i<<std::endl;
		    exit(-1);
	    }
	    if(tmp==cands.begin()){//cannot remove any element any more
		    //move last element forward
		    if(eit==sklist.end()){
			    std::cerr<<"Fail in find list with length "<<i<<std::endl;
			    exit(-1);
		    }
		    long bv=cands.back();
		    long nv=*eit;
		    tmp = eit;
		    while(*tmp<bv){
			    ++tmp;if(tmp==sklist.end()){std::cerr<<"Fail in find list with length "<<i<<std::endl;exit(-1);}
			    nv = *tmp;
		    }
		    cands.pop_back();
		    cands.push_back(nv);
		    sum+=nv-bv;
		    std::list<int>::iterator tmp2=sklist.insert(eit,bv);
		    sklist.erase(tmp);
		    eit=tmp2;
		    continue;
	    }
	    --tmp;
	    long tmpvalue = *tmp;
	    sum-=(*tmp-*pit);
	    if(sum<0){
		    std::cerr<<"Internal error in length "<<i<<std::endl;
		    exit(-1);
	    }
	    cands.erase(tmp);
	    cands.push_front(*pit);
	    eit = sklist.insert(eit, tmpvalue);
	    sklist.erase(pit);
	}
	std::cout<<i<<": ";
	for(tmp=cands.begin();tmp!=cands.end();++tmp){
		std::cout<<*tmp<<" ";
	}
	std::cout<<"\n";
    }
    std::cout<<i<<": ";
    std::list<int>::iterator tmp;
    for(tmp=sklist.begin();tmp!=sklist.end();++tmp){
	    std::cout<<*tmp<<" ";
    }
    std::cout<<"\n";
    return 0;
}

int main(int argc, char *argv[])
{
    int n=atoi(argv[1]);
    genlist(n);
    return 0;
}
