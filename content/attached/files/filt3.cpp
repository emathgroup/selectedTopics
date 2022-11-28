#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <algorithm>
#include <vector>
#include <set>
#include <string>


#define NN 13
#if  NN==14
#define MAXWEIGHT 22
#elif NN==13
#define MAXWEIGHT 19
#elif NN==12
#define MAXWEIGHT 16
#elif NN==11
#define MAXWEIGHT 12
#elif NN==10
#define MAXWEIGHT 10
#elif NN==9
#define MAXWEIGHT 7
#elif NN==8
#define MAXWEIGHT 6
#else
#error "NN not supported yet"
#endif
#define TOTALMAX  ((MAXWEIGHT*NN)/4)
#define MOD 0

#define GPCMD "gp"
#define GPSF  "/home/zdu/"
#define SINGULAR "/home/zdu/singular/bin/Singular"
#define LINEDF "data"
#define OUTF   "sd13.1"

const char *share1=
"proc inonecv(list nodes, list index)\n"
"{\n"
"    matrix m[6][6];\n"
"    int kk;\n"
"    list n;\n"
"    for(kk=1;kk<=4;kk++){\n"
"        n=nodes[ index[kk] ];\n"
"        m[kk,1]=n[1]*n[1];\n"
"        m[kk,2]=n[1]*n[2];\n"
"        m[kk,3]=n[1]*n[3];\n"
"        m[kk,4]=n[2]*n[2];\n"
"        m[kk,5]=n[2]*n[3];\n"
"        m[kk,6]=n[3]*n[3];\n"
"    }\n"
"    m[5,1]=a2-b2;\n    m[6,1]=2ab;\n"
"    m[5,2]=ac-bd;\n    m[6,2]=ad+bc;\n"
"    m[5,3]=a;\n    m[6,3]=b;\n"
"    m[5,4]=c2-d2;\n    m[6,4]=2cd;\n"
"    m[5,5]=c;\n     m[6,5]=d;\n"
"    m[5,6]=1;\n     m[6,6]=0;\n"
"    poly p=det(m);\n"
"    return (p);\n"
"}\n"
"proc ptinline(list n)\n"
"{\n"
"    matrix m[3][3];\n"
"    m[1,1]=n[1];\n"
"    m[1,2]=n[2];\n"
"    m[1,3]=n[3];\n"
"    m[2,1]=a;\n    m[2,2]=c;\n     m[2,3]=1;\n"
"    m[3,1]=b;\n    m[3,2]=d;\n     m[3,3]=0;\n"
"    poly p=det(m);\n"
"    return (p);\n"
"}\n";

#define MAX_TOTAL_EDGES TOTALMAX
#define NODES_PER_EDGE 4
char chessboard[NN][NN][NN];
int line_count;
char used[NN];
char line_buf[MAX_TOTAL_EDGES][NODES_PER_EDGE];

char test(int x,int y,int z, int w)
{
    if(chessboard[x][y][z]||chessboard[x][y][w]||
            chessboard[x][z][w]||chessboard[y][z][w])
        return 0;
    return 1;
}

char iscircle(int x,int y,int z,int w)
{
	if(chessboard[x][y][z]!=chessboard[x][y][w])return 0;
	if(chessboard[x][y][z]==0)return 0;
	return 1;
}

void set_to(int x,int y, int z,int w, int value)
{
    chessboard[x][y][z]=chessboard[x][z][w]=chessboard[x][y][w]=
        chessboard[y][z][w]=value;
}

void patch_circle(int x, int y, int z, int w)
{
	int value=chessboard[x][y][z];
	if(chessboard[x][y][w]>value)value=chessboard[x][y][w];
	if(chessboard[x][z][w]>value)value=chessboard[x][z][w];
	if(chessboard[y][z][w]>value)value=chessboard[y][z][w];
	chessboard[x][y][z]=chessboard[x][y][w]=chessboard[x][z][w]=chessboard[y][z][w]=chessboard[x][y][z]=value;
}

void init()
{
	line_count=0;
	memset(chessboard,0,sizeof(chessboard));
	memset(used,0,sizeof(used));
}

void setv(int x,int y,int z,int w)
{
    set_to(x,y,z,w,line_count+1);
    used[x]++;
    used[y]++;
    used[z]++;
    used[w]++;
    line_buf[line_count][0]=x;
    line_buf[line_count][1]=y;
    line_buf[line_count][2]=z;
    line_buf[line_count][3]=w;
    line_count++;
}
void unsetv(int x,int y,int z,int w)
{
    set_to(x,y,z,w,0);
    used[x]--;
    used[y]--;
    used[z]--;
    used[w]--;
    line_count--;
}

int fc;


int getinteger(const char *& p)
{
	if(*p=='-'){
		p++;
		return - getinteger(p);
	}else if(*p=='+'){
		p++;
		return getinteger(p);
	}else if(isdigit(*p)){
		int a =atoi(p);
		while(isdigit(*p))p++;
		return a;
	}else{
		return 1;
	}
}

bool ispositive(const char *s, std::vector<char>& symbols)
{
    int with_const=0;
    symbols.clear();
    while(1){
        int c = getinteger(s);
        if(c<0)return false;
	if(*s=='*')s++;
        if(*s>='a'&&*s<='z'){
	    symbols.push_back(*s);
	    s++;
	    int e=1;
	    if(isdigit(*s)){
	        e = getinteger(s);
	    }
	    if((e&1))return false;
	    if(*s=='\0'){
		    return true;
	    }
        }else if(*s=='\0'&&c>0){
		symbols.push_back('1');
		return true;
	}else{
		return false;
	}
    }
}


bool singlevar(const std::string& s, char letter, char& var, int& horder)
{
	int i;
	const char *p=s.c_str();
	int len = s.size();
	var = -1;horder=0;
	for(i=0;i<len;i++){
		if('a'<=p[i]&&p[i]<='z'&&p[i]!=letter){
			if(var>=0&&var!=p[i]){
				return false;
			}
			var=p[i];
			if(i<len-1&&p[i+1]=='^')i++;
			if(i<len-1&&isdigit(p[i+1])){
				int order = atoi(p+i+1);
				if(order>horder)horder=order;
			}else{
				if(horder<1)horder=1;
			}
		}
	}
	return true;
}

void collectsymbols(const char *p, std::set<char>& symbols)
{
	while(*p!='\0'){
		if('a'<=*p&&*p<='z')
			symbols.insert(*p);
		p++;
	}
}

void togpstr(const char *p, std::string& s)
{
	bool withd=false;
	s="";
start:
	withd=false;
	if(*p=='(')p++;
	while(isdigit(*p)||*p=='/'){withd=true;s+=*p;p++;}
	if(*p=='\0')return;
charloop:
	if(*p=='+'||*p=='-'){
		s+=*p;p++;
		goto start;
	}
	if(*p=='\0')return;
	if(*p>='a'&&*p<='z'){
		if(withd)s+='*';
	}
	if(*p=='*'){s+=*p;p++;}
	if(*p>='a'&&*p<='z'){
		s+=*p;p++;
	}else if(*p==')'){
		p++;
		goto charloop;
	}else{
		fprintf(stderr, "Invalid string\n");
		exit(-1);
	}
	if(isdigit(*p)||*p=='^'){
		if(isdigit(*p))s+='^';
		else{
			s+='^';p++;
		}
		while(isdigit(*p)){
			s+=*p;p++;
		}
	}
	withd=true;
	goto charloop;
}

char line[1000];
bool extra_circle_passed(const char *fname, int x, int y, int z, int w)
{
	char sname[200];
	sprintf(sname, OUTF "/%s",fname);
	FILE *f = fopen(sname, "r");
	if(f==NULL){
		fprintf(stderr, "fail to open file %s\n",sname);
		exit(-1);
	}
	FILE *fo = fopen("tmp2.sg","w");
	while(fgets(line,1000,f)){
		if(strncmp(line,"ring ",5)==0){
			char *p=line+5;
			while(*p!='=')p++;p++;
			if(*p=='('){
				p++;
			}
			*p='\0';
			char *q=p+1;
			while(isdigit(*q))q++;
			fprintf(fo,"%s%d%s",line,0,q);//set ring to rational ring
		}else if(strncmp(line, "ideal i=",8)==0){
			fprintf(fo,"list lll1=%d,%d,%d,%d;\npoly fff1=inonecv(allnodes,lll1);\n",x+1,y+1,z+1,w+1);
			char *p=line+strlen(line);
			while(p>line&&isspace(p[-1]))p--;
			if(p[-1]!=';'){
				fprintf(stderr, "invalid in file %s\n",sname);
				exit(-1);
			}
			p--;*p='\0';
			fprintf(fo,"%s,fff1;\n",line);
		}else{
			fprintf(fo,"%s",line);
		}
	}
	fclose(fo);
	fclose(f);
	sprintf(line, SINGULAR " < tmp2.sg >sg.out");
	system(line);
	f=fopen("sg.out","r");
	int pass=0,fail=0;
	while(fgets(line,1000,f)){
		if(strncmp(line,"pass",4)==0){
			pass=1;break;
		}else if(strncmp(line,"fail",4)==0){
			fail=1;break;
		}
	}
	return fail==0;
}

void process_one_file(const char *fname)
{
	char sname[200];
	char list[NN];
	init();
	sprintf(sname,OUTF "/%s",fname);
	int pass=0,fail=0;
	FILE *f = fopen(sname,"r");
	if(f==NULL){
		fprintf(stderr,"fail to open file %s\n",sname);
		return;
	}
	if(fgets(line,1000,f)==NULL){
		fprintf(stderr,"fail to read file %s\n",sname);
		fclose(f);
		return;
	}
	if(line[0]!='/'||line[1]!='/'){
		fprintf(stderr,"error in file\n");fclose(f);return;
	}
	char *p=line+2;
	while(*p!=',')p++; *p='\0';p++;
	std::string m(line+2);
	while(isspace(*p))p++;
	char *q=p;while(*q!=':')q++;*q='\0';
	std::string n(p);
	char letter = -1;
	std::string minpoly;
	while(fgets(line,1000,f)){
		if(strncmp(line,"ring ",5)==0){
			char *p=line+5;
			while(*p!='=')p++;p++;
			if(*p=='('){
				while(*p!=',')p++;
				p++;
				letter = *p;
				fgets(line,1000,f);
				if(strncmp(line,"minpoly=",8)!=0){
					char *q=line+strlen(line);
					while(q>line&&isspace(q[-1]))q--;
					*q='\0';
					minpoly=line+8;
				}
			}
			break;
		}
	}

	fclose(f);

	p=q+1;
	int i;
	for(i=0;i<NN;i++){
		int f=atoi(p);
		while(isdigit(*p))p++;
		if(p[0]!='='||p[1]!='>'){
			fprintf(stderr,"error in file 4\n");return;
		}
		p+=2;
		int t=atoi(p);
		while(isdigit(*p))p++;
		while(isspace(*p))p++;
		if(f!=i){
			fprintf(stderr,"error in file 5\n");return;
		}
		list[i]=t;
	}

	std::vector<std::string> polys;
	std::vector<std::string> sopolys;
	sprintf(sname,OUTF "/%s.out",fname);
	f=fopen(sname,"r");
	if(f==NULL){
		fprintf(stderr, "Fail to read data file %s\n",sname);
		return;
	}
	bool filtered=false;
	std::set<char> symbols;
	while(fgets(line,1000,f)){
		if(strncmp(line,"succ",4)==0){
			while(fgets(line,1000,f)){
				if(strncmp(line,"si[",3)!=0)break;
				char *p=line+3;
				while(*p&&*p!=']')p++;
				if(*p==']'&&p[1]=='=')p+=2;
				char *q=p+strlen(p);
				while(q>p&&isspace(q[-1]))q--;
				*q='\0';
				std::string s;
				togpstr(p,s);q=p;
				collectsymbols(p,symbols);
				std::vector<char> psymbols;
				if(ispositive(q, psymbols)){
					int j;
					for(j=0;j<psymbols.size();j++){
						if('a'<=psymbols[j]&&psymbols[j]<='z'){
							std::string x;x+=psymbols[j];
							polys.push_back(x);
						}else if(psymbols[j]=='1'){
							filtered=true;
							break;
						}
					}
				}else if(s=="a2+b2-2a+1"){
					polys.push_back("a-1");
					polys.push_back("b");
				}else if(s=="a2+b2-4a+4"){
					polys.push_back("a-2");
					polys.push_back("b");
				}else if(s=="2c2+2d2-2c+1"){
					filtered=true;
					break;
				}else{
					polys.push_back(s);
				}
			}
			break;
		}
	}
	fclose(f);
	if(filtered){
		fail=1;
	}else{
		FILE *fg=fopen("tmp.gp","w");
		int first=1,i;
                fprintf(fg,"\\r " GPSF "suball.gp\n");
                fprintf(fg,"print(\"begin\");\n");
                fprintf(fg,"aa=suball([");
		for(i=0;i<polys.size();i++){
			if(!first)fprintf(fg,",");
			first=0;
			fprintf(fg,"%s",polys[i].c_str());
		}
		fprintf(fg,"],[");
                std::set<char>::iterator sit;
                first=1;
                for(sit=symbols.begin();sit!=symbols.end();++sit){
                    if(*sit==letter)continue;
                    if(!first)fprintf(fg,",");
                    first=0;
                    fprintf(fg,"%c",*sit);
                }
                fprintf(fg,"])[1];\n");
		fprintf(fg,"for(u=1,length(aa), if(type(aa[u])!=\"t_VEC\",print(aa[u])));\n");
		fprintf(fg,"print(\"end\");\n");
		fclose(fg);
		sprintf(line,"rm -rf tmp.out;" GPCMD " < tmp.gp > tmp.out");
		system(line);
		fg =fopen("tmp.out","r");
		if(fg==NULL){
			printf( "unknown %s\n",fname);
		}else{
			polys.clear();
			symbols.clear();
			while(fgets(line,1000,fg)){
				if(strncmp(line,"begin",5)==0){
					while(fgets(line,1000,fg)){
						if(strncmp(line,"end",3)!=0){
							char *p=line+strlen(line);
							while(p>line&&isspace(p[-1]))p--;
							*p='\0';
							collectsymbols(line,symbols);
							polys.push_back(line);
						}else break;
					}
					break;
				}
			}
			fclose(fg);
			if(polys.size()>0){
			  fg =fopen("tmp.gp","w");
			  fprintf(fg,"\\r " GPSF "rroot.gp\n");
			  fprintf(fg,"print(\"begin\");\n");
			  fprintf(fg,"if(rroot([");
			  if(minpoly==""){
				fprintf(fg,"1");
			  }else{
				std::string gps;
				togpstr(minpoly.c_str(), gps);
				fprintf(fg,"%c,%s",letter,gps.c_str());
			  }
			  for(i=0;i<polys.size();i++){
				  char var;int ho;
				  if(singlevar(polys[i], letter, var,ho)&&ho>0&&(ho&1)==0){
					  fprintf(fg,",%s",polys[i].c_str());
				  }
			  }
			  fprintf(fg,"]), print(\"pass\"),print(\"fail\"));\n");
			  fclose(fg);
			  sprintf(line,"rm -rf tmp.out; " GPCMD " <tmp.gp > tmp.out");
			  system(line);
			  fg = fopen("tmp.out", "r");
			  if(fg==NULL){
				  printf("error in %s\n",fname);
			  }else{
				  while(fgets(line,1000,fg)){
					  if(strncmp(line,"pass",4)==0){
						  pass=1;break;
					  }else if(strncmp(line,"fail",4)==0){
						  fail=1;break;
					  }
				  }
				  fclose(fg);
			  }
			}else{
				pass=1;
			}
		}
	}
	if(pass){
		sprintf(line, OUTF "/%s.out",fname);
		FILE *fo = fopen(line,"r");
		if(fo==NULL){
			fprintf(stderr,"fail to open file %s\n",line);
			return;
		}

		char cmd[100];sprintf(cmd,"rm -rf result.all");system(cmd);
		FILE *sa = fopen("result.all","a");
		int count = m.size()/3;
		for(i=0;i<count;i++){
			fprintf(sa,"%c%c%c%c",m[3*i+0],m[3*i+1],m[3*i+2],'A'+NN-1);
		}
		int cc=count;
		int knownv=-1, maxv=-1;
		while(fgets(line,1000,fo)){
			if(strncmp(line,"known",5)==0){
				if(sscanf(line,"known %d, upbound %d",&knownv,&maxv)!=2){
					fprintf(stderr, "invalid in reading %s.out\n",fname);
					return;
				}
			}else if(line[0]=='['){
				int x,y,z,w;
				sscanf(line,"[%d,%d,%d,%d]",&x,&y,&z,&w);
				fprintf(sa,"%c%c%c%c",x-1+'A',y-1+'A',z-1+'A',w-1+'A');
				if(test(x-1,y-1,z-1,w-1)){
					setv(x-1,y-1,z-1,w-1);
					cc++;
				}else{
					patch_circle(x-1,y-1,z-1,w-1);//multiple points in one circle
				}
			}
		}
		fseek(fo,0,SEEK_SET);
		int pc=cc;
		while(fgets(line,1000,fo)){
			if(line[0]=='{'){
				int x,y,z,w;
				sscanf(line,"{%d,%d,%d,%d}",&x,&y,&z,&w);
				if(test(x-1,y-1,z-1,w-1)){
					if(extra_circle_passed(fname,x-1,y-1,z-1,w-1)){
						printf("\t{%d,%d,%d,%d}\n",x,y,z,w);
						pc++;
					}
				}
			}
		}
		fprintf(sa,"\n");fclose(sa);
		printf("pass %s, m=%s,n=%s, known %d, max %d, realknown %d, realmax %d\n",fname,m.c_str(),n.c_str(),knownv, maxv, cc, pc);
		fclose(fo);
	}else if(fail){
		printf("filter %s\n",fname);
	}else{
		printf("unknown %s\n",fname);
	}
	fflush(stdout);

}

int main(int argc, char *argv[])
{
    char sname[100];
    if(argc>=2){
	    process_one_file(argv[1]);
    }else{
    DIR *dirs=opendir(OUTF);
    if(dirs==NULL){
        fprintf(stderr,"Failed to open dir %s\n",sname);
        return -1;
    }
    struct dirent *flist;
    while(flist=readdir(dirs)){
        if(flist->d_name[0]!='f')continue;
        if(flist->d_name[1]<'0'||flist->d_name[1]>'9')
            continue;
        const char *sn = flist->d_name;
        while(*sn!='\0'&&*sn!='.')sn++;
        if(*sn!='\0')continue;
        process_one_file(flist->d_name);
    }
    closedir(dirs);
    }
    return 0;
}
