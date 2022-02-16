#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
using namespace std;
#define SRC "w5.txt"
#define SRC2 "ta.txt"
#define SRC3 "in.txt"

#define CCOUNT 12972
#define MLEVEL 4
#define MAXS  100
vector<string> wordslist;
vector<string> choicelist;
int tsize;
typedef short ttype;
typedef short itype;
ttype mtable[CCOUNT][CCOUNT];
//int tindex[MLEVEL];
#define MAXLEN 100
char buf[MAXLEN];
int tcount[1<<10];
int charstat[26];
int charused[26];
int indt;
void addstat(const char *buf){
	int i;
#if 0
	for(i=0;i<5;i++){
		charstat[buf[i]-'a']++;
	}
#endif
}
int addscore(itype x){
	int i;
#if 0
	int t=0;
	for(i=0;i<5;i++){
		if(charused[buf[i]-'a']==0)
			t+=charstat[buf[i]-'a'];
		charused[buf[i]-'a']++;
	}
#endif
	indt++;
	return 0;
}
void removescore(itype x){
	int i;
#if 0
	for(i=0;i<5;i++){
		charused[buf[i]-'a']--;
	}
#endif
	indt--;
}
struct Part{
	typedef map<ttype, set<itype> *> PMap;
	PMap parts;
	PMap parts2;
	void clear(){
		PMap::const_iterator pit;
		for(pit=parts.begin(); pit!=parts.end();++pit){
			delete pit->second;
		}
		parts.clear();
		for(pit=parts2.begin(); pit!=parts2.end();++pit){
			delete pit->second;
		}
		parts2.clear();
	}
	~Part(){clear();}
	int insert(ttype code, itype s){
		PMap::iterator it = parts.find(code);
		if(it==parts.end()){
			set<itype> *nset = new set<itype>;
			nset->insert(s);
			parts.insert(make_pair(code,nset));
			return 1;
		}else{
			set<itype> *nset = it->second;
			nset->insert(s);
			return nset->size();
		}
	}
	int insert2(ttype code, itype s){
		PMap::iterator it = parts2.find(code);
		if(it==parts2.end()){
			set<itype> *nset = new set<itype>;
			nset->insert(s);
			parts2.insert(make_pair(code,nset));
			return 1;
		}else{
			set<itype> *nset = it->second;
			nset->insert(s);
			return nset->size();
		}
	}
};

struct BestR{
	float score;
	itype s;
};

int maxcount(int tsize){
	return MAXS;
	int r=(int)round(5-3*(log(tsize)-8));
	if(r<2)r=2;if(r>=MAXS)r=MAXS;
	return r;
	//	return 5;
}

void buildtable()
{
	int u,v,i,j;
	for(u=0;u<CCOUNT;u++){
		for(v=0;v<CCOUNT;v++){
			const string& t = choicelist[v];
			const string& s = choicelist[u];
			ttype tv=0;
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
			mtable[u][v]=tv;
		}
	}
}

int getcount(const set<itype>& allwords, const set<itype>& allchoice, itype s, Part& parts)
{
	parts.clear();
	int i,j;
	int maxv=0;
	set<itype>::const_iterator mit;
	for(mit=allwords.begin();mit!=allwords.end();++mit){
		itype t=*mit;
		ttype tv = mtable[s][t];
		int curs=parts.insert(tv, t);
		if(curs>maxv)maxv=curs;
	}
#if 0
	for(mit=allchoice.begin();mit!=allchoice.end();++mit){
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
		int curs=parts.insert2(tv, t);
	}
#endif
	return maxv;
}

int getcount(const set<itype>& allwords, itype s,double &the_score)
{
	int tcount[1<<10];
	int maxv=0,i,j;
	memset(tcount,0,sizeof(tcount));
	set<itype>::const_iterator mit;
	for(mit=allwords.begin();mit!=allwords.end();++mit){
		itype t=*mit;
		ttype tv = mtable[s][t];
		tcount[tv]++;
	}
	the_score = 0;
	int total_count=allwords.size();
	for(i=0;i<1<<10;i++){
		if(tcount[i]>0){
			double p=(double)tcount[i]/total_count;
			the_score -= p*log(p);
		}
		if(tcount[i]>maxv)maxv=tcount[i];
	}
	return maxv;
}

int getBestR(const set<itype>& allwords, const set<itype>& allchoice, BestR br[], int count)
{
	int mc=0;
	int i,j;
	set<itype>::const_iterator mit;
#if 0
	if(allwords.size()>=20){
		for(mit=allwords.begin();mit!=allwords.end();++mit){
			itype s=*mit;
			int ss = addscore(s);
			removescore(s);
			double dscore;
			int c=getcount(allwords,s,dscore);
			if(c==allwords.size())continue;
			float score=-dscore;
			for(i=0;i<mc;i++){
				if(score<br[i].score)break;
			}
			j=count-1;if(j>mc)j=mc;
			for(;j>=i+1;j--){
				br[j]=br[j-1];
			}
			if(i<count){
				br[i].score=score;
				br[i].s=s;
			}
			if(mc<count)mc++;
		}
	}else{
#endif
		{
			for(mit=allchoice.begin();mit!=allchoice.end();++mit){
				itype s=*mit;
				int ss=addscore(s);
				removescore(s);
				double dscore;
				int c=getcount(allwords,s,dscore);
				if(c==allwords.size())continue;
				float score=-dscore;
				for(i=0;i<mc;i++){
					if(score<br[i].score)break;
				}//insert at location i
				j=count-1;if(j>mc)j=mc;
				for(;j>=i+1;j--){
					br[j]=br[j-1];
				}
				if(i<count){
					br[i].score=score;
					br[i].s=s;
				}
				if(mc<count)mc++;
			}
		}
		return mc;
	}
	int getBestTry(const set<itype>& allwords, const set<itype>& allchoice, itype& result, int& the_total, bool dumpall=false);

	FILE *flog;
	int lcount;
	void logdata(itype name, int id, int s, int w)
	{
		return;
		if(s<=2)return;
		if(indt>3)return;
		int i;
		for(i=0;i<indt;i++)fprintf(flog,"\t");
		fprintf(flog,"%d %d %d %d\n",name, id,s,w);
		lcount++;
		if(lcount%128==0)fflush(flog);
	}

	int getStringResult(itype s, int& the_total)
	{
		Part parts;
		int ttcount=0;
		int tsum=0;
		int maxtry=0;
		set<itype> allwords, allchoice;
		itype i;
		for(i=0;i<wordslist.size();++i){
			allwords.insert(i);
		}
		for(i=0;i<CCOUNT;i++){
			allchoice.insert(i);
		}
		int cc=getcount(allwords, allchoice, s, parts);
		addscore(s);
		Part::PMap::reverse_iterator pit;
		for(pit=parts.parts.rbegin();pit!=parts.parts.rend();++pit){
			itype result;
			if(pit->second->size()<=2){
				set<itype>::iterator sit;
				if(pit->second->size()>maxtry)maxtry=pit->second->size();
				if(pit->first==0){
				}else {
					if(pit->second->size()==2){
						tsum+=3;
					}else{
						tsum+=1;
					}
				}
			}else{
				int ttotal;
				int cc=getBestTry(*pit->second,allchoice, result,ttotal);
				logdata(s, pit->first,pit->second->size(),cc);
				tsum+=ttotal;
				if(cc>maxtry)maxtry=cc;
				if(maxtry>=6)break;
			}
		}
		removescore(s);
		the_total=tsum+allwords.size();
		return maxtry+1;
	}

	int getBestTry(const set<itype>& allwords, const set<itype>& allchoice, itype& result, int& the_total, bool outall)
	{
		BestR br[MAXS];
		int c=getBestR(allwords, allchoice, br, maxcount(allwords.size()));
		int i;
#ifdef OUT
		printf("Get %d result:\n\t",c);
		for(i=0;i<c;i++){
			printf("%s ",br[i].s.c_str());
		}
		printf("\n");
#endif
		itype ts;
		Part parts;
		int mtry = 1000;
		int mtotal=100000000;
		int sub_total;
		for(i=0;i<c;i++){
			int cc=getcount(allwords,allchoice, br[i].s, parts);
#ifdef OUT
			printf("get %d count\n",cc);
#endif
			int maxtry = 0;
			int curtotal=0;
			addscore(br[i].s);
			Part::PMap::reverse_iterator pit;
			for(pit=parts.parts.rbegin();pit!=parts.parts.rend();++pit){
				set<itype> *pset = pit->second;
				int ssize=pset->size();
				int curt=0;
				if(ssize<=2){
					curt=ssize;
					ts=*pset->begin();
					if(ssize==1){
						if(pit->first!=0)sub_total=1;else sub_total=0;
					}
					else sub_total=3;
				}
				else{
					//	curt=getBestTry(*pset,*parts.parts2[pit->first],ts,sub_total);
					curt=getBestTry(*pset,allchoice,ts,sub_total);
					logdata(br[i].s, pit->first, pit->second->size(), curt);
				}
				if(curt>maxtry)maxtry=curt;
				curtotal+=sub_total;
				if(maxtry+indt>=7){curtotal=1000000000;break;}
			}
			removescore(br[i].s);
			if((curtotal<mtotal||curtotal==mtotal&&maxtry+1<mtry)){
				mtry=maxtry+1;
				mtotal=curtotal;
				result = br[i].s;
			}
			if(outall && curtotal<1000000){
				cout<<choicelist[br[i].s]<<' '<<maxtry+1<<' '<<curtotal+allwords.size()<<'\n';
				fflush(stdout);
			}
		}
		the_total=mtotal+allwords.size();
		return mtry;
	}

	char code[4]={'m','s','u',' '};
	void showcode(ttype c){
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
	void dump_result(const set<itype>& allwords, const set<itype>& allchoice, itype s, int tab)
	{
		showtab(tab);
		cout<<choicelist[s]<<'\n';
		Part parts;
		addscore(s);
		int cc=getcount(allwords,allchoice, s, parts);
		Part::PMap::iterator pit;
		for(pit=parts.parts.begin();pit!=parts.parts.end();++pit){
			showtab(tab);showcode(pit->first);cout<<":\n";
			itype result;
			if(pit->second->size()<=2){
				set<itype>::iterator sit;
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
					cout<<choicelist[*sit];cout<<' ';
				}
				cout<<"}\n";
			}else{
				int ttotal;
				//			int cc=getBestTry(*pit->second,*parts.parts2[pit->first],result,ttotal);
				int cc=getBestTry(*pit->second,allchoice,result,ttotal);
				//			dump_result(*pit->second,*parts.parts2[pit->first], cc, result, tab+1);
				dump_result(*pit->second,allchoice, result, tab+1);
			}
		}
		removescore(s);
	}

	int main(int argc, char *argv[])
	{
		int i;
		FILE *f=fopen(SRC,"r");
		map<string, itype> indexmap;
		itype cid=0;
		flog=fopen("slog","a");
		while(fgets(buf,MAXLEN,f)){
			int len=strlen(buf);
			while(len>0&&isspace(buf[len-1]))len--;
			if(len==5){
				buf[len]='\0';
				for(i=0;i<len;i++){
					if(isupper(buf[i]))buf[i]=tolower(buf[i]);
				}
				string s(buf);
				wordslist.push_back(s);
				indexmap[s]=cid++;
				addstat(buf);
			}
		}
		fclose(f);
		choicelist=wordslist;
		f = fopen(SRC2,"r");
		while(fgets(buf,MAXLEN,f)){
			int len=strlen(buf);
			while(len>0&&isspace(buf[len-1]))len--;
			if(len==5){
				buf[len]='\0';
				for(i=0;i<len;i++){
					if(isupper(buf[i]))buf[i]=tolower(buf[i]);
				}
				string s(buf);
				choicelist.push_back(s);
				indexmap[s]=cid++;
			}
		}
		fclose(f);
		fprintf(stderr, "Before build table\n");
		buildtable();
		fprintf(stderr,"Finished table building\n");

		set<itype> allwords, allindexes;
		for(i=0;i<wordslist.size();++i)allwords.insert(i);
		for(i=0;i<CCOUNT;i++)allindexes.insert(i);
		if(argc>1){
			for(i=0;i<CCOUNT;++i){
				if(choicelist[i]==argv[1])break;
			}
			if(i==CCOUNT){
				fprintf(stderr, "Invalid input string %s\n",argv[1]);
				return -1;
			}
			dump_result(allwords, allindexes,  i, 0);
			cout <<"Total count "<<tsum<<'\n';
			for(i=0;i<20;i++){
				if(cstat[i]>0){
					cout << cstat[i] <<" words tries "<<i<<" times\n";
				}
			}
		}else{
		    itype result;
		    int the_total;
		    (void)getBestTry(allwords, allindexes, result, the_total, true);
		}
		return 0;
	}
