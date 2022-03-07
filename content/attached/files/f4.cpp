#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <map>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <random>
using namespace std;

#define WORDNUM 13
#define SHARECOUNT (WORDNUM-1)
struct ShareInfo{
	int w1,w2;
	int c1,c2;
};
ShareInfo si[SHARECOUNT]={
	{0,1,3,0},
	{1,2,3,0},
	{2,3,3,0},
	{3,4,3,0},
	{4,5,3,0},
	{5,6,3,0},
	{6,7,3,0},
	{7,8,3,0},
	{0,9,0,0},
	{9,10,3,0},
	{10,11,3,0},
	{11,12,3,0}
};

int charid;
map<string, long> cmap;
#define SRC "data.txt"
#define SRC2 "input.txt"
#define UTF8_CHAR_LEN( byte ) ((( 0xE5000000 >> (( byte >> 3 ) & 0x1e )) & 3 ) + 1)
#define BUFLEN 1024
#define SLEN  100
char buf[BUFLEN];
int len[4];
char tbuf[4][SLEN];
char sbuf[BUFLEN];
void check_one_word(int curc)
{
    int i,t=0;
    for(i=0;i<4;i++){
	    if(len[(curc+i)&3]==0)return;
	    memcpy(sbuf+t,tbuf[(curc+i)&3],len[(curc+i)&3]);
	    t+=len[(curc+i)&3];
    }
    sbuf[t]='\0';
    map<string, long>::iterator mit;
    mit = cmap.find(sbuf);
    if(mit != cmap.end()){
	    mit->second++;
    }
}

void scanfile(const char *fname)
{
	FILE *f = fopen(fname,"r");
	if(f==NULL){
		fprintf(stderr, "Fail to open input file %s\n",fname);
		return;
	}
	int curc=0;
	len[0]=len[1]=len[2]=len[3]=0;
	while(fgets(buf,BUFLEN,f)!=NULL){
		int i=0;
		while(buf[i]!='\0'){
			int size = UTF8_CHAR_LEN(buf[i]);
			if(size>1 || !isspace(buf[i])){
				len[curc]=size;
				memcpy(tbuf[curc],buf+i,size);
				curc++;curc&=3;
				check_one_word(curc);
			}
			i+=size;
		}
	}
	fclose(f);
}

int main(int argc, const char *argv[])
{
	int seed;
	FILE *f=NULL;
	f=fopen(SRC,"r");
	if(f==NULL){
		fprintf(stderr, "Fail to open input idiom files %s\n", SRC);
		return -1;
	}
	int tc=1;
	int lc=0;
	while(fgets(buf,BUFLEN,f)!=NULL){
		int i=0,ccount=0;
		for(;buf[i]!='\0'&&ccount<4;){
			int size = UTF8_CHAR_LEN(buf[i]);
			ccount++;
			i+=size;
		}
		lc++;
		if(buf[i]!='\t')continue;
		buf[i]='\0';
		string s(buf);
		cmap.insert(make_pair(s,0L));
	}
	fclose(f);
	f=fopen(SRC2,"r");
	if(f==NULL){
		fprintf(stderr, "Fail to open input file " SRC2 "\n");
		return -1;
	}
	tc=1;lc=0;
	while(fgets(buf,BUFLEN,f)!=NULL){
		char *p=buf;
		while(*p!='\0'&&!isspace(*p))p++;
		*p='\0';
		scanfile(buf);
	}
	fclose(f);
	map<string, long>::iterator mit;
	for(mit=cmap.begin();mit!=cmap.end();++mit){
		if(mit->second>0){
			printf("%s,%ld\n",mit->first.c_str(),mit->second);
		}
	}
	return 0;
}
