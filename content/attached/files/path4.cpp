#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <iostream>
#include <map>
#include <vector>
#ifndef N
#define N 50
#endif

struct IS{
    char cnpair;//count of pairs
    char pairs[N];//list of pairs and single
    long mask;
    int  mbc(int x)const{
	int i,c=0;
	for(i=1;i<x;i++){
		if(mask&(1L<<i))c++;
	}
	return c;
    }
    bool is_connected(int x);
    bool merge_two_pairs(int a, int b);
    bool add_new_pair(int v1, int v2);
    bool change_pair_new_value(int pid, int v);
    void output()const{
	int i;
	for(i=0;i<cnpair;i++){
	    std::cout<<"{"<<(int)pairs[2*i]<<","<<(int)pairs[2*i+1]<<"}";
	}
	if(mask!=0){
	    std::cout<<"M< ";
	    int i;
	    for(i=0;i<64;i++){
		if(mask&(1L<<i)){
		    std::cout<<i<<" ";
		}
	    }
	    std::cout<<">";
	}
	std::cout<<std::endl;
    }
    int get_x_index(int x)const{
	int i;
	for(i=0;i<2*cnpair;i++){
	    if(pairs[i]==x)return i;
	}
	return -1;
    }
    bool operator<(const struct IS& is)const{
	if(mask<is.mask)return true;
	if(mask>is.mask)return false;
	if(cnpair<is.cnpair)return true;
	if(cnpair>is.cnpair)return false;
	int i;
	for(i=0;i<2*cnpair;i++){
	    if(pairs[i]<is.pairs[i])return true;
	    if(pairs[i]>is.pairs[i])return false;
	}
	return false;
    }
    bool operator==(const struct IS& is)const{
	if(mask!=is.mask)return false;
	if(cnpair!=is.cnpair)return false;
	int i;
	for(i=0;i<2*cnpair;i++){
	    if(pairs[i]!=is.pairs[i])return false;
	}
	return true;
    }
};

int sqrlist[N];
int sqrc;
typedef std::map<IS, long> ISMAP;
ISMAP is1, is2, *isp, *isn;

int update_map(char map[N], int j)
{
    if(map[j]==j)return j;
    if(j<=0||j>=N){
	    printf("Invalid found\n");
    }
    map[j]=update_map(map, map[j]);
    if(map[j]<=0){
	    printf("Invalid found in update_map\n");
    }
    return map[j];
}

void update_map_to(char map[N], int k, int kv)
{
    int n=map[k];
    map[k]=kv;
    if(n!=k&&n!=kv){
	update_map_to(map,n,kv);
    }
}

void merge_map(char map[N], int k, int j)
{
    int mj = update_map(map, j);
    if(mj<=0){
	    printf("Invalid value\n");
    }
    if(map[k]!=mj){
	update_map_to(map, k, mj);
    }
}
bool makenewis(IS& nis, const IS& ois, int c1, int c2, int step);

bool IS::is_connected(int x)
{
    char map[N+1];
    char deg[N+1];
    char pedge[N+1];
    int i,j;
    int nc=0;
    for(i=1;i<x;i++){
	pedge[i]=-1;
	deg[i]=0;
	if(mask&(1L<<i)){
	    map[i]=-1;
	}else{
	    map[i]=i;
	}
    }
    for(i=0;i<cnpair;i++){
	deg[pairs[2*i+1]]++;deg[pairs[2*i]]++;
	map[pairs[2*i+1]]=pairs[2*i];
	pedge[pairs[2*i]]=pairs[2*i+1];
	pedge[pairs[2*i+1]]=pairs[2*i];
    }
    for(i=1;i<x;i++){
	if((mask&(1L<<i))==0){
	    nc++;
	    for(j=0;j<sqrc;j++){
		if(sqrlist[j]<=2*i)continue;
		if(sqrlist[j]-i>=x)break;
		int k=sqrlist[j]-i;
		if(mask&(1L<<k))continue;
		//map k to j
		if(pedge[k]==i)continue;//useless edge
		merge_map(map, k,i);
		deg[k]++;deg[i]++;
	    }
	}
    }
    int ov=-1;
    int onec=0;
    for(i=1;i<x;i++){
	if((mask&(1L<<i))==0){
//	    if(deg[i]==0&&nc>1)return false;
	    if(deg[i]==1&&nc>2){
	        return false;
	    }
	    int nv=update_map(map, i);
	    if(ov==-1){
		ov=nv;
	    }else if(ov!=nv){//not connected graph
		    return false;
	    }
	}
    }
#if 1
    for(i=0;i<2*cnpair;i++){
	if(deg[pairs[i]]==2){//collapse on the node
	    break;
	}
    }
    if(i<2*cnpair){
            for(j=0;j<sqrc;j++){
                if(sqrlist[j]<=pairs[i])continue;
                if(sqrlist[j]- pairs[i]>=x)break;
                int k=sqrlist[j]- pairs[i];
		if(k==pairs[i])continue;
		if(k==pairs[i^1])continue;
                if(mask&(1L<<k))continue;
                //map k to pairs[i] found
		{
		    IS nis;
#ifdef DEBUG
		    printf("Connect %d to %d\n",pairs[i],k);
#endif
		    if(makenewis(nis, *this,pairs[i], k, x)&&nis.cnpair+nis.mbc(x)<x&&nis.is_connected(x)){
#ifdef DEBUG
			    printf("Simp to\n");
			    nis.output();
#endif
			*this = nis;
		    }else{
#ifdef DEBUG
			    printf("Filter\n");
			    nis.output();
#endif
			return false;
		    }
		    break;
		}
            }

    }
#endif
    return true;
}

void init_sqrs()
{
    int i;
    for(i=2;i*i<2*N;i++){
	sqrlist[sqrc++]=i*i;
    }
}

void init_for_N()
{
    std::vector<int> cands;
    int i,j;
    for(i=0;i<sqrc;i++){
	if(sqrlist[i]<=N)continue;
	int c=sqrlist[i]-N;
	if(c<N&&c>0){
	    cands.push_back(c);
	}
    }
    for(i=1;i<=N;i++)for(j=i+1;j<=N;j++){
	 IS is;
	 is.cnpair=1;
	 is.pairs[0]=i;//create pseudo edge link cands[i] & cands[j]
	 is.pairs[1]=j;
	 is.mask=0L;
	 is1.insert(std::make_pair(is,1L));
    }
    isn=&is1;
    isp=&is2;
}

bool IS::add_new_pair(int v1, int v2)
{
    if(v2<v1)return add_new_pair(v2,v1);
    char bkpairs[N];
    memcpy(bkpairs,pairs,N);
    int i,j=0;
    for(i=0;i<cnpair;i++){
	if(v1<bkpairs[i*2]&&(i==0||v1>=bkpairs[2*i-2])){
	    pairs[2*j]=v1;pairs[2*j+1]=v2;j++;
	}
	pairs[2*j]=bkpairs[2*i];pairs[2*j+1]=bkpairs[2*i+1];j++;
    }
    if(cnpair==0||v1>=bkpairs[2*cnpair-2]){//add at the end
	pairs[2*cnpair]=v1;pairs[2*cnpair+1]=v2;j++;
    }
    cnpair++;
    return true;
}

bool IS::merge_two_pairs(int a, int b)
{
    int index1 = a;
    int index2 = b;
    if(index1<0||index1>=2*cnpair)return false;
    if(index2<0||index2>=2*cnpair)return false;
    if(index1/2==index2/2){
	return false;
    }
    int newx1 = pairs[index1^1], newx2=pairs[index2^1];
    if(newx1>newx2){
	int tmp=newx1;newx1=newx2;newx2=tmp;
    }
    mask |= (1L<<pairs[a])|(1L<<pairs[b]);
    int i,j=0;
    char bkpairs[N];
    memcpy(bkpairs,pairs,N);
    for(i=0;i<cnpair;i++){
	if(newx1<=bkpairs[2*i]&&(i==0||newx1>bkpairs[2*i-2])){//found location to insert new item
	    pairs[2*j]=newx1;pairs[2*j+1]=newx2;j++;
	}
	if(i==index1/2||i==index2/2)continue;
	pairs[2*j]=bkpairs[2*i];pairs[2*j+1]=bkpairs[2*i+1];j++;
    }
    if(newx1>bkpairs[2*cnpair-2]){
	pairs[2*cnpair-4]=newx1;pairs[2*cnpair-3]=newx2;j++;
    }
    cnpair--;
    return true;
}

bool IS::change_pair_new_value(int pid, int v)
{
    int newx1 = pairs[pid^1];
    int newx2 = v;
    if(newx1>newx2){
	int tmp=newx1;newx1=newx2;newx2=tmp;
    }
    char oldpair[N];
    memcpy(oldpair,pairs,N);
    mask |= 1L<<pairs[pid];
    int i,j=0;
    for(i=0;i<cnpair;i++){
        if(newx1<=oldpair[2*i]&&(i==0||newx1>oldpair[2*i-2])){//found location to insert new item
            pairs[2*j]=newx1;pairs[2*j+1]=newx2;j++;
        }
	if(i==pid/2)continue;
	pairs[2*j]=oldpair[2*i];pairs[2*j+1]=oldpair[2*i+1];j++;
    }
    if(newx1>oldpair[2*cnpair-2]){
        pairs[2*cnpair-2]=newx1;pairs[2*cnpair-1]=newx2;j++;
    }//do not change single and count of pairs
    return true;
}

bool makenewis(IS& nis, const IS& ois, int c1, int c2, int step)//connect c1 & c2 into one group
{
    int index1 = ois.get_x_index(c1);
    int index2 = ois.get_x_index(c2);
    nis = ois;
    if(index1>=0&&index2>=0){
	//check for end status
	if((index1^1)==index2){//merge them together
	    if(ois.cnpair!=1)return false;
	    long val=(1L<<step)-2;
	    if(((ois.mask|(1L<<c1)|(1L<<c2))&val)!=val)return false;
	    nis.mask = val;
	    nis.cnpair=0;
	    return true;
	}
        return nis.merge_two_pairs(index1, index2);
    }else if(index1>=0&&index2<0){
        return nis.change_pair_new_value(index1,c2);
    }else if(index1<0&&index2>=0){
	return nis.change_pair_new_value(index2, c1);
    }else{//both minus
	return nis.add_new_pair(c1,c2);
    }
}

void update_for_x(int x)
{
    std::vector<int> cands;
    int i,j;
    for(i=0;i<sqrc;i++){
        if(sqrlist[i]<=x)continue;
        int c=sqrlist[i]-x;
        if(c<x&&c>0){
            cands.push_back(c);
        }else if(c>=x)break;
    }
#ifdef DEBUG
    std::cout<<"Update for level "<<x<<std::endl;
#endif
    ISMAP::iterator mit;
    isp->clear();
    for(mit = isn->begin(); mit!=isn->end();++mit){
	if(mit->first.mask &(1L<<x)){//The number has been masked, copy it to next level and do nothing
	    IS nis=mit->first; nis.mask&=~(1L<<x);//remove x from mask, it is not required any more
	    if(nis.cnpair+nis.mbc(x)<x){
#ifdef DEBUG
		std::cout<<"Copy from upper Level:"<<std::endl;
		nis.output();
#endif
	        (*isp)[nis]+=mit->second;
	    }
	    continue;
	}
	int xindex = mit->first.get_x_index(x);
	if(xindex<0){//number x has never been used
	    for(i=0;i<cands.size();i++){//try to connect it to two cands
		if(mit->first.mask&(1L<<cands[i]))continue;//The number has been fully consumed
		for(j=i+1;j<cands.size();j++){
		    if(mit->first.mask&(1L<<cands[j]))continue;
		    IS nis;
		    if(makenewis(nis, mit->first, cands[i],cands[j],x)&&nis.cnpair+nis.mbc(x)<x&&nis.is_connected(x)){
#ifdef DEBUG
			std::cout<<"From "<<std::endl;
			mit->first.output();
			std::cout<<"Add "<<cands[i]<<"=>"<<cands[j]<<std::endl;
			nis.output();
#endif
		        (*isp)[nis]+=mit->second;
		    }
		}
	    }
	}else{//number x has been connected to one group, try to connect it to another cands
	    for(i=0;i<cands.size();i++){
		if(mit->first.mask&(1L<<cands[i]))continue;
		IS nis;
		if(makenewis(nis, mit->first, cands[i],x, x)&&nis.cnpair+nis.mbc(x)<x&&nis.is_connected(x)){
		    nis.mask&=~(1L<<x);
#ifdef DEBUG
		    std::cout<<"From "<<std::endl;
		    mit->first.output();
		    std::cout<<"Add "<<cands[i]<<"=>"<<x<<std::endl;
		    nis.output();
#endif
		    (*isp)[nis]+=mit->second;
		}
	    }
	}
    }
    ISMAP *tmp = isp;isp=isn;isn=tmp;//swap isp and isn;
}

long tcount;

void search(const IS& is, long weight, int x)
{
    while(x>=1&&is.mask&(1L<<x)){x--;}
    if(x<=0){//end of search, a result found
	 if(is.cnpair==0){
#pragma omp atomic update
		 tcount+=weight;
	 }
	 return;
    }
    std::vector<int> cands;
    int i,j;
    for(i=0;i<sqrc;i++){
        if(sqrlist[i]<=x)continue;
        int c=sqrlist[i]-x;
        if(c<x&&c>0){
            cands.push_back(c);
        }else if(c>=x)break;
    }
    int xindex = is.get_x_index(x);
        if(xindex<0){//number x has never been used
            for(i=0;i<cands.size();i++){//try to connect it to two cands
                if(is.mask&(1L<<cands[i]))continue;//The number has been fully consumed
                for(j=i+1;j<cands.size();j++){
                    if(is.mask&(1L<<cands[j]))continue;
                    IS nis;
                    if(makenewis(nis, is, cands[i],cands[j],x)&&nis.cnpair+nis.mbc(x)<x&&nis.is_connected(x)){
			search(nis, weight, x-1);
                    }
                }
            }
        }else{//number x has been connected to one group, try to connect it to another cands
            for(i=0;i<cands.size();i++){
                if(is.mask&(1L<<cands[i]))continue;
                IS nis;
                if(makenewis(nis, is, cands[i],x, x)&&nis.cnpair+nis.mbc(x)<x&&nis.is_connected(x)){
                    nis.mask&=~(1L<<x);
		    search(nis, weight, x-1);
                }
            }
        }
}

#define D 6
int main()
{
    int i;
    init_sqrs();
    init_for_N();
    for(i=N;i>=N-D&&i>=1;i--){
	update_for_x(i);
    }

    size_t tsize = (*isn).size();
    printf("search size %lu\n",tsize);fflush(stdout);
    ISMAP::iterator it = (*isn).begin();
#pragma omp parallel
    {
	while(1){
	    ISMAP::iterator cit;
	    bool isend=false;
#pragma omp critical
	    {
		 if(it==(*isn).end()){
		    isend=true;
		 }else{
		    cit = it++;
		 }
	    }
	    if(isend)break;
	    search(cit->first,cit->second, N-D-1);
	}
    }

    printf("%ld\n",tcount);
    return 0;
}
