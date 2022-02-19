#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;
#define SRC "w5.txt"

#define MLEVEL 4
map<string, bool> allwords;
int tsize;
//int tindex[MLEVEL];
#define MAXLEN 100
char buf[MAXLEN];
int tcount[1<<10];

int getcount(const string& s)
{
	map<string, bool>::iterator mit;
	memset(tcount,0,sizeof(tcount));
	int i,j;
	int maxv=0;
	for(mit=allwords.begin();mit!=allwords.end();++mit){
		if(!mit->second)continue;
		const string& t=mit->first;
		int tv=0;
		for(i=0;i<5;i++){
			int v=0;
			if(t[i]==s[i]){
				v=0;
			}else{
				for(j=0;j<5;j++){
					if(s[i]==t[j]&&t[j]!=s[j])break;
				}
				if(j<5)v=1;
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

string gethint(bool useall)
{
    map<string, bool>::iterator mit;
    int bestcount=allwords.size();
    string rs;
    for(mit=allwords.begin();mit!=allwords.end();++mit){
	    if(useall||mit->second){
		    int curc=getcount(mit->first);
		    if(curc<bestcount){
			    bestcount=curc;
			    rs=mit->first;
		    }
	    }
    }
    return rs;
}


int filter_by(const string& s, const string& p)
{
	map<string, bool>::iterator mit;
	int c=0;
	int i,j;
	for(mit=allwords.begin();mit!=allwords.end();++mit){
		if(!mit->second)continue;
		const string& t=mit->first;
		for(i=0;i<5;i++){
			if(p[i]=='m'){
				if(t[i]!=s[i])break;
			}else if(p[i]=='s'){
				for(j=0;j<5;j++){
					if(s[i]==t[j]&&t[j]!=s[j])break;
				}
				if(j==5)break;
			}else{//'u'
				if(t[i]==s[i])break;
				for(j=0;j<5;j++){
					if(s[i]==t[j]&&t[j]!=s[j])break;
				}
				if(j<5)break;
			}
		}
		if(i<5){
			mit->second=false;
		}else{
			c++;
		}
	}
	return c;
}

void dump_candidates()
{
	map<string, bool>::iterator mit;
	for(mit=allwords.begin();mit!=allwords.end();++mit){
		if(mit->second){
			cout<<mit->first<<' ';
		}
	}
	cout<<"\n";
}

const char *dsc[]={"first","second","third", "fourth","fifth", "sixth"};

int main()
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
			allwords.insert(make_pair(s,true));
		}
	}
	fclose(f);
	for(i=0;i<6;i++){
		string s,p;
		cout <<"Please input the word for "<<dsc[i]<<" try: ";
		cin >> s;
		if(s=="quit")break;
		if(s=="show"){
			dump_candidates();
			i--;
			continue;
		}
		if(s=="help"){
			string rs=gethint(true);
			cout<<rs<<"("<<getcount(rs)<<")\n";
			i--;
			continue;
		}
		if(s=="hh"){
			string rs=gethint(false);
			cout<<rs<<"("<<getcount(rs)<<")\n";
			i--;
			continue;
		}
		cout <<"Please input response of the "<<dsc[i]<<" try [(m)atch, (s)hift, (u)nmatch]: ";
		cin >> p;
		if(p=="quit")break;
		if(s.length()!=5||p.length()!=5){
			cout << "input length invalid\n";
			i--;
			continue;
		}
		int j;
		for(j=0;j<5;j++){
			if(p[j]!='m'&&p[j]!='s'&&p[j]!='u'){
				cout <<"invalid response, all characters must be one of m,s,u\n";
				break;
			}
			if(s[j]<'a'||s[j]>'z'){
				cout<<"invalid input word, all characters must between a and z\n";
				break;
			}
		}
		if(j<5){
			i--;
			continue;
		}
		int r=filter_by(s,p);
		cout<<"Total "<<r<<" words left\n";
		if(r<=10){
			dump_candidates();
		}
	}
	return 0;
}
