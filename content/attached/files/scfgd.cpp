#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
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
#define MOD 32003

#define GPCMD "gp"
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

void init(const char *fname)
{
	int i;
	int count =strlen(fname)/3;
	memset(chessboard,0,sizeof(chessboard));
	memset(used,0,sizeof(used));
	line_count=0;
        for(i=0;i<count;i++){
                setv(fname[i*3]-'A',fname[i*3+1]-'A',fname[i*3+2]-'A',NN-1);
        }
}

#define LSIZE 1024
char line[LSIZE];
int fc;
void gencfg(const char *fname1, const char *fname2, const char list[NN])
{
	int count=strlen(fname1)/3;
	int i;
	char localused[NN];
	memcpy(localused,used,sizeof(used));
	for(i=0;i<count;i++){
		int a[4];
		a[0]=list[fname2[i*3]-'A'];
		a[1]=list[fname2[i*3+1]-'A'];
		a[2]=list[fname2[i*3+2]-'A'];
		a[3]=list[NN-1];
		std::sort(a,a+4);
		if(a[3]==NN-1){
			if(!iscircle(a[0],a[1],a[2],a[3]))return;
		}else{
			if(!test(a[0],a[1],a[2],a[3]))return;
			localused[a[0]]++;localused[a[1]]++;localused[a[2]++];localused[a[3]++];
		}
	}
	for(i=0;i<NN;i++)if(localused[i]>MAXWEIGHT)return;
//Now the verification has passed
	char fname[200];sprintf(fname,OUTF "/f%d",fc++);
	FILE *f=fopen(fname,"w");
	if(f==NULL){
		fprintf(stderr, "Fail to create file %s\n",fname);
		exit(-1);
	}
	fprintf(f, "//%s, %s:",fname1,fname2);
	for(i=0;i<NN;i++){
		fprintf(f,"%d=>%d ",i,list[i]);
	}
	fprintf(f,"\n");
	char iname[1000];
	sprintf(iname,LINEDF "/ta%d/%s.sout",NN-1,fname1);
	FILE *fi1 = fopen(iname, "r");
	if(fi1==NULL){
		fprintf(stderr, "fail to open input configure file %s\n",iname);
		exit(-1);
	}
	while(fgets(line,LSIZE,fi1)){
		if(strncmp(line,"ring ",5)==0){//change MOD of ring
			char *p=line+5;
			while(*p!='=')p++;p++;
			char *zero;
			if(*p=='('){
				zero=p+1;
			}else{
				zero=p;
			}
			if(*zero!='0'){
				fprintf(stderr, "format of %s not correct\n",iname);
				exit(-1);
			}
			*zero='\0';
			zero++;
			fprintf(f,"%s%d%s",line,MOD, zero);
		}else if(strncmp(line, "ideal i",7)==0){
			break;//insert more command here
		}else{
			fprintf(f,"%s",line);//copy line
		}
	}
	fprintf(f,"%s", share1);

	int ff=0;
	for(i=0;i<count;i++){
                int a[4];
                a[0]=list[fname2[i*3]-'A'];
                a[1]=list[fname2[i*3+1]-'A'];
                a[2]=list[fname2[i*3+2]-'A'];
                a[3]=list[NN-1];
		std::sort(a,a+4);
		if(a[3]!=NN-1){
		    fprintf(f,"list l%d=%d,%d,%d,%d;\n",ff,a[0]+1,a[1]+1,a[2]+1,a[3]+1);
		    fprintf(f,"poly ff%d=inonecv(allnodes,l%d);\n",ff,ff);
		    ff++;
		}
	}
	char *p = line+strlen(line);//line for "ideal i"
	while(p>line&&isspace(p[-1]))p--;
	if(p[-1]!=';'){
		fprintf(stderr, "Invalid format of file %s\n",iname);
		exit(-1);
	}
	p--;*p='\0';
	int start=0;
	char *feq = line;
	while(*feq!='=')feq++;
	*feq='\0';
	feq++;
	if(p[-1]=='0'&&p[-2]=='='){//remove 0 polynomial
		p--;*p='\0';start=1;
	}
	std::string ringinit(feq);
	fprintf(f,"%s",line);//copy original polynomial for lines
	for(i=0;i<ff;i++){
		if(!start)ringinit+=",";
		start=0;
		char sbuf[20];
		sprintf(sbuf,"ff%d",i);
		ringinit+=sbuf;
	}
	fprintf(f,"=%s",ringinit.c_str());
	fprintf(f,";\n");//add all polynoimal for circles
	fprintf(f,"ideal si=std(i);\n");
	fclose(fi1);
	sprintf(iname,LINEDF "/ta%d/%s.flt",NN-1,fname1);
	fi1 = fopen(iname,"r");
	if(fi1==NULL){
		fprintf(stderr,"Warning: could not open file %s\n",iname);
	}else{
		std::vector<std::string> filterpolys;
		
		while(fgets(line,LSIZE,fi1)){
			char *p=line+strlen(line);
			while(p>line&&isspace(p[-1]))p--;
			*p='\0';//remove ending space
			filterpolys.push_back(line);
		}
		fclose(fi1);

		struct{
			bool operator()(const std::string& s1, const std::string& s2)const{
				if(s1.size()<s2.size())return true;
				if(s1.size()>s2.size())return false;
				return s1>s2;
			}
		}customLess;

		std::sort(filterpolys.begin(), filterpolys.end(), customLess);
		fprintf(f,"ideal j;poly g0,h0;\n");
		for(i=0;i<filterpolys.size();i++){
			fprintf(f,"g0=%s;\nj=g0;\ni=sat(si,j)[1];\nsi=std(i);\n",filterpolys[i].c_str());
		}
	}
	{
		fprintf(f,"link lk=\"\";list ll0;\nint countf,countm;\ncountf=%d;\ncountm=%d;\n",(int)strlen(fname1)/3,(int)strlen(fname1)/3);
                fprintf(f,"int x,y,z,w; \nideal i2,si2;");
		fprintf(f,"if(dim(si)==-1){printf(\"fail\");}else{\n");
		fprintf(f,"  for(x=0;x<%d;x++){for(y=x+1;y<%d;y++){for(z=y+1;z<%d;z++){for(w=z+1;w<%d;w++){\n",NN-1,NN-1,NN-1,NN-1);
                fprintf(f,"    ll0=x+1,y+1,z+1,w+1;\n");
        	fprintf(f,"    h0=inonecv(allnodes,ll0);\n");
        	fprintf(f,"    if(reduce(h0, si)==0){countf = countf+1;} \n");
		fprintf(f,"  }}}}\n");
		fprintf(f,"  if(countf>%d){printf(\"fail\");}else{\n",TOTALMAX);
		fprintf(f,"    fprintf(lk,\"succ %%s\",countf);\nsi;\n");
                fprintf(f,"   for(x=0;x<%d;x++){for(y=x+1;y<%d;y++){for(z=y+1;z<%d;z++){for(w=z+1;w<%d;w++){\n",NN-1,NN-1,NN-1,NN-1);
                fprintf(f,"    ll0=x+1,y+1,z+1,w+1;\n");
                fprintf(f,"    h0=inonecv(allnodes,ll0);\n");
                fprintf(f,"    if(reduce(h0, si)==0){fprintf(lk,\"[%%s,%%s,%%s,%%s]\",x+1,y+1,z+1,w+1);countm=countm+1;}else{ \n");
                fprintf(f,"    i2=si,h0;\n");
                fprintf(f,"    si2=std(i2);\n");
                fprintf(f,"    if(dim(si2)>=0){countm=countm+1; fprintf(lk,\"{%%s, %%s, %%s, %%s}\",x+1, y+1,z+1,w+1);}\n");
		fprintf(f,"    }\n");
                fprintf(f,"   }}}}\n");
		fprintf(f,"  }\n");
                fprintf(f,"  fprintf(lk, \"known %%s, upbound %%s\",countf, countm);\n");
 		fprintf(f,"}\n");
	}
	fclose(f);
	{
	   char cmdline[1000];
 	   sprintf(cmdline, SINGULAR " < " OUTF "/f%d > " OUTF "/f%d.out",fc-1,fc-1);
	   system(cmdline);
	   sprintf(fname, OUTF "/f%d.out",fc-1);
	   f = fopen(fname,"r");
	   int pass=0,fail=0;
	   if(f!=NULL){
		   while(fgets(cmdline,1000,f)){
			   if(strncmp(cmdline,"succ",4)==0){
				   pass=1;break;
			   }else if(strncmp(cmdline,"fail",4)==0){
				   fail=1;break;
			   }
		   }
		   fclose(f);
		   if(pass==0&&fail==1){
			   sprintf(cmdline,"rm " OUTF "/f%d;rm " OUTF "/f%d.out",fc-1,fc-1);
			   system(cmdline);
		   }
	   }
	}
}

int main(int argc, char *argv[])
{
	int i,j,k;
	char list[NN];
	char revo[NN];
	for(i=0;i<NN;i++)list[i]=i;
	if(argc<2){
		fprintf(stderr,"%s cfg1 [cfg2]\n",argv[0]);
		return 0;
	}
	char *fname1, *fname2;
	int samecfg=0;
	fname1 = argv[1];
	if(argc>2){
		fname2=argv[2];
		if(strcmp(fname1,fname2)==0){
			samecfg=1;
		}
	}else{
		fname2=argv[1];
		samecfg=1;
	}
	{
		init(fname1);
		{
			int lc=0;
			do{
				lc++;
				if(list[NN-1]==NN-1)continue;//The last node must be remapped
				if(samecfg){
					for(k=0;k<NN;k++){
						revo[list[k]]=k;
					}
					int cmdr=0;
					for(k=0;k<NN;k++){
						if(list[k]<revo[k]){
							cmdr=1;break;
						}else if(list[k]>revo[k]){
							cmdr=-1;break;
						}
					}
					if(cmdr<0)continue;
				}
				gencfg(fname1,fname2,list);
			}while(std::next_permutation(list,list+NN));
		}
	}
	return 0;
}
