#include <stdio.h>
#include <string.h>
#include <map>
#include <string>
#include <ctype.h>
using namespace std;

struct Wordle{
	string word1,word2;
	map<int, Wordle *> choose;
};
#define MAXLEVEL 7
Wordle * WStack[MAXLEVEL];
#define MAXLINE 1024
char line[MAXLINE];

int main(int argc, char *argv[])
{
	FILE *f=fopen(argv[1],"r");
	if(f==NULL){
		fprintf(stderr, "Fail to open input file %s\n",argv[1]);
		return -1;
	}
	int curtab=-1;
	while(fgets(line,MAXLINE, f)){
		char *skiptab=line;
		while(*skiptab=='\t')skiptab++;
		int tabcount=skiptab-line;
		if(tabcount==0&&isspace(skiptab[5])){//first line
			if(WStack[0]!=NULL||curtab!=-1){
				fprintf(stderr, "Invalid line encountered\n");
				return -1;
			}
			skiptab[5]='\0';
			WStack[0]=new Wordle;
			WStack[0]->word1=skiptab;
			curtab=0;
			continue;
		}else if(tabcount==0&&skiptab[5]!=':'){//end of data
			break;
		}
		if(skiptab[5]==':'){
			int v=0;
			if(curtab<tabcount){
				fprintf(stderr,"Invalid tab count\n");
				return -1;
			}
			curtab=tabcount;

			for(int i=0;i<5;i++){
				int c=0;
				if(skiptab[i]=='s')c=1;
				else if(skiptab[i]=='u')c=2;
				else if(skiptab[i]!='m'){
					fprintf(stderr, "Invalid data\n");
					return -1;
				}
				v|=c<<(2*i);
			}
			Wordle *pstack = new Wordle;
			WStack[curtab]->choose[v]=pstack;
			if(fgets(line,MAXLINE,f)==NULL){
				fprintf(stderr, "Invalid line\n");
				return -1;
			}
			skiptab=line;while(*skiptab=='\t')skiptab++;
			if(skiptab-line!=curtab+1){
				fprintf(stderr, "Invalid value\n");
				return -1;
			}
			if(isdigit(skiptab[0])){
				while(isdigit(skiptab[0]))skiptab++;
				if(*skiptab=='{')skiptab++;
				skiptab[5]='\0';
				pstack->word1 = skiptab;
				if(skiptab[6]!='}'){
					skiptab[11]='\0';
					pstack->word2 = skiptab+6;
				}
			}else if(isspace(skiptab[5])){
				skiptab[5]='\0';
				pstack->word1=skiptab;
				curtab++;
				WStack[curtab]=pstack;
			}else{
				fprintf(stderr,"Invalid line encountered\n");
				return -1;
			}
		}
	}
start_game:
	Wordle *curw = WStack[0];
	int count=0;
	char input[MAXLINE];
	while(1){
		int i, v=0;
		if(curw->choose.size()==0){
			if(curw->word2==""){
				printf("Final result is %s\n\n",curw->word1.c_str());
			}else{
				printf("Two words {%s %s} left\n\n", curw->word1.c_str(), curw->word2.c_str());
			}
			goto start_game;
		}
		printf("Next input %s\n",curw->word1.c_str());
input_result:
		v=0;
		printf("Please input response 5  {(m)atch, (s)hift or (u)match}: ");
		scanf("%s",input);
		if(strncmp(input,"quit",4)==0)return 0;
		if(strncmp(input,"restart",7)==0)goto start_game;
		if(strncmp(input,"help",4)==0){
			printf("quit: leave the code\n"
				"restart: restart the game\n"
				"help: display the help\n"
				"Other valid input should be 5 letters and each letter is one of m,s,u\n"
				"\tsuch as muusu\n"
					);
			goto input_result;
		}
		for(i=0;i<5;i++){
			int c=0;
			if(input[i]=='m')c=0;
			else if(input[i]=='s')c=1;
			else if(input[i]=='u')c=2;
			else {
				printf("Invalid input:\n");
				goto input_result;
			}
			v|=c<<(i*2);
		}
		map<int, Wordle *>::iterator it = curw->choose.find(v);
		if(it == curw->choose.end()){
			fprintf(stderr, "Internal error\n");
			return -1;
		}
		curw = it->second;
	}

}
