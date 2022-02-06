#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
using namespace std;
#define SRC "w5.txt"

#define MLEVEL 4
#define MAXS  50
set<string> allwords;
int tsize;
//int tindex[MLEVEL];
#define MAXLEN 100
char buf[MAXLEN];
int tcount[1<<10];
struct Part{
	typedef map<int, set<string> *> PMap;
	PMap parts;
	void clear(){
		PMap::const_iterator pit;
		for(pit=parts.begin(); pit!=parts.end();++pit){
			delete pit->second;
		}
		parts.clear();
	}
	~Part(){clear();}
	int insert(int code, const string& s){
		PMap::iterator it = parts.find(code);
		if(it==parts.end()){
			set<string> *nset = new set<string>;
			nset->insert(s);
			parts.insert(make_pair(code,nset));
			return 1;
		}else{
			set<string> *nset = it->second;
			nset->insert(s);
			return nset->size();
		}
	}
};

struct BestR{
	int score;
	string s;
};

int maxcount(int tsize){
//	int r=(int)round(5-30*(log(tsize)-8));
//	if(r<2)r=2;if(r>=MAXS)r=MAXS;
//	return r;
        return MAXS;
}

int getcount(const set<string>& allwords, const string& s, Part& parts)
{
	parts.clear();
	int i,j;
	int maxv=0;
	set<string>::const_iterator mit;
	for(mit=allwords.begin();mit!=allwords.end();++mit){
		const string& t=*mit;
		int tv=0;
		int used[5]={0,0,0,0,0};
		for(i=0;i<5;i++){
			if(t[i]==s[i]){
				used[i]=1;
			}
		}
		for(i=0;i<5;i++){
			int v=0;
			if(t[i]==s[i]){
				v=0;
			}else{
				for(j=0;j<5;j++){
					if(s[i]==t[j]&&!used[j])break;
				}
				if(j<5){v=1;used[j]=1;}
				else v=2;
			}
			tv|=(v<<(2*i));
		}
		int curs=parts.insert(tv, t);
		if(curs>maxv)maxv=curs;
	}
	return maxv;
}

int getcount(const set<string>& allwords, const string& s)
{
	int tcount[1<<10];
	int maxv=0,i,j;
	memset(tcount,0,sizeof(tcount));
	set<string>::const_iterator mit;
	for(mit=allwords.begin();mit!=allwords.end();++mit){
		const string& t=*mit;
		int tv=0;
		int used[5]={0,0,0,0,0};
		for(i=0;i<5;i++)if(t[i]==s[i])used[i]=1;
		for(i=0;i<5;i++){
			int v=0;
			if(t[i]==s[i]){
				v=0;
			}else{
				for(j=0;j<5;j++){
					if(s[i]==t[j]&&!used[j])break;
				}
				if(j<5){v=1;used[j]=1;}
				else v=2;
			}
			tv|=(v<<(2*i));
		}
		tcount[tv]++;
	}
	for(i=0;i<1<<10;i++){
		if(tcount[i]>maxv)maxv=tcount[i];
	}
	return maxv;
}

int getBestR(const set<string>& allwords, BestR br[], int count)
{
	int mc=0;
	int i,j;
	set<string>::const_iterator mit;
	for(mit=allwords.begin();mit!=allwords.end();++mit){
		const string& s=*mit;
		int c=getcount(allwords,s);
		for(i=0;i<mc;i++){
			if(c>=br[i].score)break;
		}
		j=count-1;if(j>mc)j=mc;
		for(;j>=i+1;j--){
			br[j]=br[j-1];
		}
		if(i<count){
		   br[i].score=c;
		   br[i].s=s;
		}
		if(mc<count)mc++;
	}
	return mc;
}

int getBestTry(const set<string>& allwords, string& result, int& the_total)
{
	BestR br[MAXS];
	int c=getBestR(allwords, br, maxcount(allwords.size()));
	int i;
	string ts;
	Part parts;
	int mtry = 1000;
	int mtotal=100000000;
	int sub_total;
	for(i=0;i<c;i++){
		int cc=getcount(allwords, br[i].s, parts);
		int maxtry = 0;
		int curtotal=0;
		Part::PMap::iterator pit;
		for(pit=parts.parts.begin();pit!=parts.parts.end();++pit){
			set<string> *pset = pit->second;
			int ssize=pset->size();
			int curt=0;
			if(ssize<=2){curt=ssize;ts=*pset->begin();if(ssize==1)sub_total=1;else sub_total=3;}
			else curt=getBestTry(*pset,ts,sub_total);
			if(curt>maxtry)maxtry=curt;
			curtotal+=sub_total;
		}
		if(maxtry+1<mtry||maxtry+1==mtry&&curtotal<mtotal){
			mtry=maxtry+1;
			mtotal=curtotal;
			result = br[i].s;
		}
	}
	the_total=mtotal+allwords.size();
	return mtry;
}

char code[4]={'m','s','u',' '};
void showcode(int c){
	int i;
	for(i=0;i<5;i++){
		cout<<code[(c>>(2*i))&3];
	}
}
void showtab(int tab){
	int i;
	for(i=0;i<tab;i++)cout<<'\t';
}
int ttcount;
int tsum;
int cstat[20];
void dump_result(const set<string>& allwords, int c, const string& s, int tab)
{
	showtab(tab);
	cout<<s<<'\n';
	Part parts;
	int cc=getcount(allwords, s, parts);
	Part::PMap::iterator pit;
	for(pit=parts.parts.begin();pit!=parts.parts.end();++pit){
		showtab(tab);showcode(pit->first);cout<<":\n";
		string result;
		if(pit->second->size()<=2){
		   set<string>::iterator sit;
		   ttcount+=pit->second->size();
		   showtab(tab+1);
		   if(pit->first==0){
			   tsum+=tab+1;cout<<tab+1;
			   cstat[tab+1]++;
		   }else {
			   cout<<tab+1+pit->second->size();cout<<"{";
			   if(pit->second->size()==2){
				   cstat[tab+2]++;
				   cstat[tab+3]++;
				   tsum+=2*(tab+1)+3;
			   }else{
				   tsum+=tab+2;
				   cstat[tab+2]++;
			   }
		   }
		   for(sit=pit->second->begin();sit!=pit->second->end();++sit){
			   cout<<*sit;cout<<' ';
		   }
		   cout<<"}\n";
		}else{
		   int ttotal;
		   int cc=getBestTry(*pit->second,result,ttotal);
		   dump_result(*pit->second, cc, result, tab+1);
		}
	}
}

int main(int argc, char *argv[])
{
	int i;
	FILE *f=fopen(SRC,"r");
	while(fgets(buf,MAXLEN,f)){
		int len=strlen(buf);
		while(len>0&&isspace(buf[len-1]))len--;
		if(len==5){
			buf[len]='\0';
			for(i=0;i<len;i++){
				if(isupper(buf[i]))buf[i]=tolower(buf[i]);
			}
			string s(buf);
			allwords.insert(s);
		}
	}
	fclose(f);
	string r;
	int total;
	dump_result(allwords,7,argv[1], 0);
	cout <<"Average cost "<< (double)tsum/ttcount<<"\n";
	for(i=0;i<20;i++){
		if(cstat[i]>0){
			cout <<cstat[i]<<" words need try " <<i <<" times\n";
		}
	}
	return 0;
}
