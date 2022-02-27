#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

int charid;
map<string, int> cmap;
#define SRC "sid.sort"
#define UTF8_CHAR_LEN( byte ) ((( 0xE5000000 >> (( byte >> 3 ) & 0x1e )) & 3 ) + 1)
#define BUFLEN 1024
char buf[BUFLEN];
char tbuf[BUFLEN];
struct Idiom{
	int c[4];
	int used;
};

struct UseInfo{
	int word_id;
	int word_off;
};

struct CharInfo{
	string s;
	vector<UseInfo> uis;
};
vector<CharInfo> cis;

vector<Idiom> idms;
int getcid(const string& s){
	map<string, int>::iterator mit;
	mit = cmap.find(s);
	int r=0;
	if(mit == cmap.end()){
		r=charid++;
		cmap.insert(std::make_pair(s,r));
		CharInfo x;
		x.s=s;
		cis.push_back(x);
	}else{
		r=mit->second;
	}
	return r;
}

int getrandword()
{
	int id=rand()%idms.size();
	return id;
}

#define RANGE 16
#define WCOUNT 20
struct Loc{
	int row,col;
};
vector<Loc> locstack;
int wordadded;
int useinfo[2*RANGE+1][2*RANGE+1];
void dump_result()
{
	int i,j;
	int firstrow=2*RANGE+1, lastrow=0;
	int firstcol=2*RANGE+1, lastcol=0;
	for(i=0;i<2*RANGE+1;i++)for(j=0;j<2*RANGE+1;j++){
		if(useinfo[i][j]>=0){
			if(firstrow>i)firstrow=i;
			if(lastrow<i)lastrow=i;
			if(firstcol>j)firstcol=j;
			if(lastcol<j)lastcol=j;
		}
	}
	if(firstrow>0)firstrow--;
	if(lastrow<2*RANGE)lastrow++;
	if(firstcol>0)firstcol--;
	if(lastcol<2*RANGE)lastcol++;
	vector<int> vd;
	for(i=firstrow;i<lastrow+1;i++)for(j=firstcol;j<lastcol+1;j++){
		if(useinfo[i][j]>=0){
			vd.push_back(i*(2*RANGE+1)+j);
		}
	}
	int rshow=rand()%vd.size();

	int targetj=vd[rshow]%(2*RANGE+1);
	int targeti=vd[rshow]/(2*RANGE+1);
	for(i=firstrow;i<lastrow+1;i++){
		for(j=firstcol;j<lastcol+1;j++){
			if(useinfo[i][j]>=0){
				if(i==targeti&&j==targetj){
					printf("%s",cis[useinfo[i][j]].s.c_str());
				}else{
					printf(" *");
				}
			}else{
				printf("  ");
			}
		}
		printf("\n");
	}
	printf("\n");
	vd.clear();
	for(i=firstrow;i<lastrow+1;i++)for(j=firstcol;j<lastcol+1;j++){
		if(useinfo[i][j]>=0&&(i!=targeti||j!=targetj)){
			vd.push_back(useinfo[i][j]);
		}
	}
	sort(vd.begin(),vd.end());
	for(i=0;i<vd.size();++i){
			printf("%s",cis[vd[i]].s.c_str());
	}
	printf("\n\n");

	for(i=firstrow;i<lastrow+1;i++){
		for(j=firstcol;j<lastcol+1;j++){
			if(useinfo[i][j]>=0){
				printf("%s",cis[useinfo[i][j]].s.c_str());
			}else{
				printf("  ");
			}
		}
		printf("\n");
	}
	fflush(stdout);
}
int getleftfree(int row, int col)
{
	int c=0;
	while(col>0){
		if(useinfo[row][col-1]>=0)return c;
		if(row>0&&useinfo[row-1][col-1]>=0)return c;
		if(row<2*RANGE&&useinfo[row+1][col-1]>=0)return c;
		col--;
		c++;
		if(c>=3)return 3;
	}
	return c;
}
int getrightfree(int row, int col)
{
	int c=0;
	while(col<2*RANGE){
		if(useinfo[row][col+1]>=0)return c;
		if(row>0&&useinfo[row-1][col+1]>=0)return c;
		if(row<2*RANGE&&useinfo[row+1][col+1]>=0)return c;
		col++;
		c++;
		if(c>=3)return 3;
	}
	return c;
}
int getupfree(int row, int col)
{
	int c=0;
	while(row>0){
		if(useinfo[row-1][col]>=0)return c;
		if(col>0&&useinfo[row-1][col-1]>=0)return c;
		if(col<2*RANGE&&useinfo[row-1][col+1]>=0)return c;
		row--;
		c++;
		if(c>=4)return 4;
	}
	return c;
}
int getdownfree(int row, int col)
{
	int c=0;
	while(row>0){
		if(useinfo[row+1][col]>=0)return c;
		if(col>0&&useinfo[row+1][col-1]>=0)return c;
		if(col<2*RANGE&&useinfo[row+1][col+1]>=0)return c;
		row++;
		c++;
		if(c>=4)return 4;
	}
	return c;
}

int tryfillfrom()
{
	int i,j;
	if(wordadded>=WCOUNT){
		dump_result();
		return 1;
	}
	int startloc = rand()%locstack.size();
	i=startloc;
	do{
		Loc& curloc=locstack[i];
		int lc,rc,uc,dc;
		int row=curloc.row, col=curloc.col;
		lc=getleftfree(curloc.row, curloc.col);
		rc=getrightfree(curloc.row, curloc.col);
		uc=getupfree(curloc.row,curloc.col);
		dc=getdownfree(curloc.row,curloc.col);
		if(lc+rc<4&&uc+dc<4)continue;
		int curchar = useinfo[curloc.row][curloc.col];
		int tsize = cis[curchar].uis.size();
		if(tsize==0)continue;
		int startj=rand()%tsize;
		j=startj;
		do{
			UseInfo& ui = cis[curchar].uis[j];
			if(idms[ui.word_id].used)continue;
			int rowfirst[2]; rowfirst[0] = rand()%2;rowfirst[1]=1-rowfirst[0];
			int kk;
			for(kk=0;kk<2;kk++){
				if(rowfirst[kk]&&(lc>=ui.word_off+1&&rc>=4-ui.word_off||ui.word_off==0&&rc>=4||ui.word_off==3&&lc>=4)){
					//try fill lc
					int k;
					if(idms[ui.word_id].c[ui.word_off]!=useinfo[row][col]){
						fprintf(stderr, "Internal error: unmatched char\n");
						exit(-1);
					}
					for(k=0;k<4;k++){
						if(k!=ui.word_off){
							useinfo[row][col+k-ui.word_off]=idms[ui.word_id].c[k];
							Loc loc;loc.row=row;loc.col=col+k-ui.word_off;
							locstack.push_back(loc);
						}
					}
					wordadded++;
					idms[ui.word_id].used=1;
					if(tryfillfrom())return 1;
					idms[ui.word_id].used=0;
					wordadded--;
					for(k=0;k<4;k++){
						if(k!=ui.word_off){
							useinfo[row][col+k-ui.word_off]=-1;
							locstack.pop_back();
						}
					}
				}
				if(!rowfirst[kk]&&(uc>=ui.word_off+1&&dc>=4-ui.word_off||ui.word_off==0&&dc>=4||ui.word_off==3&&uc>=4)){
					int k;
					if(idms[ui.word_id].c[ui.word_off]!=useinfo[row][col]){
						fprintf(stderr, "Internal error: unmatched char\n");
						exit(-1);
					}
					for(k=0;k<4;k++){
						if(k!=ui.word_off){
							useinfo[row+k-ui.word_off][col]=idms[ui.word_id].c[k];
							Loc loc;loc.row=row+k-ui.word_off;loc.col=col;
							locstack.push_back(loc);
						}
					}
					wordadded++;
					idms[ui.word_id].used=1;
					if(tryfillfrom())return 1;
					idms[ui.word_id].used=0;
					wordadded--;
					for(k=0;k<4;k++){
						if(k!=ui.word_off){
							useinfo[row+k-ui.word_off][col]=-1;
							locstack.pop_back();
						}
					}
				}
			}
		}while(j++,j%=tsize,j!=startj);
	}while(i++,i%=locstack.size(),i!=startloc);
}

int main(int argc, const char *argv[])
{
	int seed;
	FILE *f=NULL;
	f=fopen(argv[1],"r");
	if(f==NULL){
		fprintf(stderr, "Fail to open input idiom files %s\n", argv[1]);
		return -1;
	}
	while(fgets(buf,BUFLEN,f)!=NULL){
		int i=0,ccount=0;
		Idiom idm;
		for(;buf[i]!='\0'&&ccount<4;){
			int size = UTF8_CHAR_LEN(buf[i]);
			memcpy(tbuf,buf+i,size);
			tbuf[size]='\0';
			int curchar = getcid(tbuf);
			idm.c[ccount++]=curchar;
			i+=size;
		}
		idm.used=0;
		if(ccount==4){
			idms.push_back(idm);
			for(i=0;i<4;i++){
				UseInfo ui;
				ui.word_id = idms.size()-1;
				ui.word_off = i;
				cis[idm.c[i]].uis.push_back(ui);
			}
		}
	}
	fclose(f);
	if(argc>2){
		seed=atoi(argv[2]);
	}else{
		seed=time(NULL);
	}
	srand(seed);
	printf("seed is %d\n",seed);
	memset(useinfo,-1,sizeof(useinfo));

	{
		int startword = getrandword();
		int i;
		for(i=0;i<4;i++){
			useinfo[RANGE][RANGE-1+i]=idms[startword].c[i];
			Loc loc;loc.row=RANGE;loc.col=RANGE-1+i;
			locstack.push_back(loc);
		}
		wordadded=1;
		idms[startword].used=1;
		(void)tryfillfrom();
	}
}
