#include <algorithm>
#include <map>
#include <iostream>
#include <vector>
#include <string.h>

typedef struct MD{
    char d[9];
    bool operator<(const struct MD& md)const{
	int i;
	for(i=0;i<9;i++){
		if(d[i]<md.d[i])return true;
		if(d[i]>md.d[i])return false;
	}
	return false;
    }
    bool operator==(const struct MD& md)const{
	int i;
	for(i=0;i<9;i++)if(d[i]!=md.d[i])return false;
	return true;
    }
}MD;

#define HENG 0
#define SHU  1

char dirs[9]={HENG,HENG,SHU,SHU,HENG,SHU,HENG,SHU,HENG};
void output(const MD& d)
{
    int i,j;
    for(i=0;i<3;i++){
	for(j=0;j<3;j++){
	    if(d.d[i*3+j]==0){
		 std::cout<<"0";
	    }else{
		 std::cout<<(int)(d.d[i*3+j]);
	    }
	}
	std::cout<<"\n";
    }
    std::cout<<"\n";
}

std::map<MD, int> dmap;
std::vector<MD> mds;
void init(){
    char d[9]={0,1,2,3,4,5,6,7,8};
    int c=0;
    do{
	MD md;
	memcpy(md.d,d,sizeof(d));
	mds.push_back(md);
	dmap[md]=c++;
    }while(std::next_permutation(d,d+9));
}

char fillorder[]={0,2,6,8,1,3,5,7,4};
char board[17][17];
MD md;
bool tryfill(int depth)
{
    int startx,endx, starty,endy;
    if(depth>=9)return true;//end of finding
    int loc=fillorder[depth];
    int value=md.d[loc];
    if(value==0){
	    return tryfill(depth+1);
    }
    if(loc%3==0){//always left most
	startx=endx=0;
    }else if(loc%3==1){
	startx=5,endx=6;
	if(dirs[value]==SHU){
		endx=7;
	}
    }else{//always right most
	if(dirs[value]==SHU){
		startx=endx=12;
	}else{
		startx=endx=11;
	}
    }
    if(loc<3){
	starty=endy=0;
    }else if(loc<6){
	starty=5,endy=6;
	if(dirs[value]==HENG){
		endy=7;
	}
    }else{
	if(dirs[value]==HENG){
		starty=endy=12;
	}else{
		starty=endy=11;
	}
    }
    int x,y;
    for(x=startx;x<=endx;x++)for(y=starty;y<=endy;y++){
	int i,j;
	if(dirs[value]==HENG){
	    int pass=1;
	    for(i=x;i<=x+5&&pass;i++)for(j=y;j<=y+4;j++){
		if(board[j][i]){
		    pass=0;break;
		}
	    }
	    bool r = false;
	    if(pass){
		 for(i=x;i<=x+5;i++)for(j=y;j<=y+4;j++)board[j][i]=value;
	         r=tryfill(depth+1);
	         for(i=x;i<=x+5;i++)for(j=y;j<=y+4;j++)board[j][i]=0;
		 if(r)return r;
	    }
	}else{
	    int pass=1;
	    for(i=x;i<=x+4&&pass;i++)for(j=y;j<=y+5;j++){
		if(board[j][i]){
		    pass=0;break;
		}
	    }
	    bool r = false;
	    if(pass){
		for(i=x;i<=x+4;i++)for(j=y;j<=y+5;j++)board[j][i]=value;
		r=tryfill(depth+1);
		for(i=x;i<=x+4;i++)for(j=y;j<=y+5;j++)board[j][i]=0;
		if(r)return r;
	    }
	}
    }
    return false;
}



#define SS 362880
int mvs[SS][4];
int dst[SS];
char fillstat[SS];
#define INVF ((char)0xFF)

std::vector<int> path;

int pc=0;
void dumpallpath(int from)
{
     path.push_back(from);
     int depth = dst[from];
     if(depth==0){
          int i;
	  std::cout<<"Cand "<<pc<<"\n";
	  for(i=path.size()-1;i>=0;i--){
	      output(mds[path[i]]);
	  }
	  pc++;
     }else{
	 int j;
	 for(j=0;j<4;j++){
	    if(mvs[from][j]<0)break;
	    if(fillstat[mvs[from][j]]==INVF)continue;
	    if(dst[mvs[from][j]]==depth-1){
		 dumpallpath(mvs[from][j]);
	    }
	 }
     }
     path.pop_back();
}

int main()
{
    int i,j;
    init();
    if(mds.size()!=SS){
	    std::cerr<<"Internal error 1\n";
	    return -1;
    }
    for(i=0;i<mds.size();++i){
	    md=mds[i];
	    for(j=0;j<3;j++){
		if(dirs[md.d[j*3+0]]==HENG&&dirs[md.d[j*3+1]]==HENG&&dirs[md.d[j*3+2]]==HENG&&
				md.d[j*3+0]>0&&md.d[j*3+1]>0&&md.d[j*3+2]>0)break;
	    }
	    if(j<3){fillstat[i]=INVF;continue;}
	    for(j=0;j<3;j++){
		if(dirs[md.d[0*3+j]]==SHU&&dirs[md.d[1*3+j]]==SHU&&dirs[md.d[2*3+j]]==SHU)break;
	    }
	    if(j<3){fillstat[i]=INVF;continue;}
	    if(!tryfill(0)){
		fillstat[i]=INVF;
	    }
    }
    for(i=0;i<mds.size();++i){
	    if(fillstat[i]==INVF)continue;
	    md=mds[i];
	    MD md2;
	    for(j=0;j<9;j++)if(md.d[j]==0)break;
	    if(j>=9){
		    std::cerr<<"Internal error 2\n";
		    return -1;
	    }
	    int cc=0;
	    if(j>=3){//up
		    int p=j-3;
		    md2=md;
		    md2.d[p]=md.d[j];
		    md2.d[j]=md.d[p];
		    int k=dmap[md2];
		    if(fillstat[k]!=INVF){
			if(dirs[md.d[p]]==SHU ||j%3!=1||dirs[md.d[p-1]]==dirs[md.d[j-1]]||dirs[md.d[p+1]]==dirs[md.d[j+1]])
		            mvs[i][cc++]=k;
		    }
	    }
	    if(j%3>0){//left
		    int p=j-1;
		    md2=md;
		    md2.d[p]=md.d[j];
		    md2.d[j]=md.d[p];
		    int k=dmap[md2];
		    if(fillstat[k]!=INVF){
			if(dirs[md.d[p]]==HENG||j<3||j>=6||dirs[md.d[p-3]]==dirs[md.d[j-3]]||dirs[md.d[p+3]]==dirs[md.d[j+3]])
		        	mvs[i][cc++]=k;
		    }
	    }
	    if(j<6){//down
		    int p=j+3;
		    md2=md;
		    md2.d[p]=md.d[j];md2.d[j]=md.d[p];
		    int k=dmap[md2];
		    if(fillstat[k]!=INVF){
			if(dirs[md.d[p]]==SHU||j%3!=1||dirs[md.d[p-1]]==dirs[md.d[j-1]]||
					dirs[md.d[p+1]]==dirs[md.d[j+1]])
		            mvs[i][cc++]=k;
		    }
	    }
	    if(j%3<2){//right
		    int p=j+1;
		    md2=md;
		    md2.d[p]=md.d[j];md2.d[j]=md.d[p];
		    int k=dmap[md2];
		    if(fillstat[k]!=INVF){
			if(dirs[md.d[p]]==HENG||j<3||j>=6||dirs[md.d[p-3]]==dirs[md.d[j-3]]||
					dirs[md.d[p+3]]==dirs[md.d[j+3]])
		            mvs[i][cc++]=k;
		    }
	    }
	    if(cc<4){
		    mvs[i][cc]=-1;
	    }
    }
    char dsrc[9]={1,2,0,3,4,5,6,7,8};
    char ddst[9]={1,5,0,3,6,2,4,7,8};
    MD md3;
    memcpy(md3.d,dsrc,sizeof(dsrc));
    int sid=dmap[md3];
    memcpy(md3.d,ddst,sizeof(ddst));
    int did = dmap[md3];
    for(i=0;i<SS;i++)dst[i]=-1;
    dst[sid]=0;
    while(1){
	bool changed=false;
	for(i=0;i<SS;i++){
	    if(dst[i]<0)continue;
	    for(j=0;j<4;j++){
		    if(mvs[i][j]<0)break;
		    int k=mvs[i][j];
		    if(dst[k]<0||dst[k]>dst[i]+1){
			    dst[k]=dst[i]+1;
			    changed=true;
		    }
	    }
	}
	if(!changed)break;
    }
    std::cout<<"Total "<<dst[did]<< " moves from "<<sid<<" to "<<did<<"\n";
    int from=did;
#if 0
    for(i=0;i<SS;i++){
	    std::cout<<"F"<<i<<" "<<dst[i]<<" "<<(int)fillstat[i]<<"\n";
	    std::cout<<"\tT{ ";
	    for(j=0;j<4;j++){
		    if(mvs[i][j]<0)break;
		    std::cout<<mvs[i][j]<<" ";
	    }
	    std::cout<<"}\n";
	    output(mds[i]);
    }
#endif
   dumpallpath(from);
   return 0;
}
