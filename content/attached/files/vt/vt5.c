#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define NUM 3

typedef struct rec{
    short x,y;
}rec_t;
int s[NUM];
int ss=20;
//int s[NUM]={1,2,3,5,7,9};
//int ss=13;
#define MAXE 30
#define MAXF 80
char bb[MAXE][MAXE];
char bs[NUM][MAXE][MAXE];

#define U 2
#define V 1

int nexta(int a[], int c)
{
    int k,h;
    int ts=a[c-1];
    for(k=c-2;k>=0;k--){
	if(ts>=(a[k]+1)*(c-1-k)+1){
	    a[k]++;
	    for(h=k+1;h<c-1;h++){
		a[h]=a[k];ts-=a[k];
	    }
	    a[c-1]=ts-1;
	    return 1;
	}
	ts+=a[k];
    }
    return 0;
}

void output(int a[], int c)
{
    int i;
    for(i=0;i<c;i++)printf("%d ",a[i]);
    printf("\n");
}

void inita(int a[], int c, int s)
{
    int i;
    for(i=0;i<c-1;i++)a[i]=1;
    a[i]=s-(c-1);
}
int a[U+1],b[V+1];
rec_t tdata[(U+1)*(V+1)];
int used[(U+1)*(V+1)];
int idata[(U+1)*(V+1)];
int num[7];

int cmpd(const void *p, const void *q)
{
    const rec_t *pi=(const rec_t*)p;
    const rec_t *qi=(const rec_t*)q;
    int s1=pi->x*pi->y, s2=qi->x*qi->y;
    if(s1!=s2)return s1-s2;
    return pi->y-qi->y;
}

void trysearch(int level);
void search(int level, int target, int counti, int start)
{
    int i;
    if(target==0){
	num[level+1]=counti;
	trysearch(level+1);
    }
    for(i=start;i<(U+1)*(V+1);i++){
	if(tdata[i].x*tdata[i].y>target)return;
	if(used[i])continue;
	if(tdata[i].y>s[level])continue;
	if(i>0&&used[i-1]==0&&tdata[i-1].x==tdata[i].x&&tdata[i-1].y==tdata[i].y&&counti>num[level])continue;
	idata[counti]=i;
	used[i]=1;
#ifdef OUT
	printf("A %d*%d\n",tdata[i].x,tdata[i].y);
#endif
	search(level, target-tdata[i].x*tdata[i].y, counti+1,i+1);
#ifdef OUT
	printf("R %d*%d\n",tdata[i].x,tdata[i].y);
#endif
	used[i]=0;
    }
}


char fill[MAXF];
int searchfill(int edge, int sti, int edi, int id)
{
    int i,j,k,s,t;
    if(id<sti){//search succ;
	    return 1;
    }
    int w=tdata[idata[id]].x, h=tdata[idata[id]].y;
    int ew=edge-w,wh=edge-h;
    if(id==edi-1){
	    ew=(ew+1)/2;wh=(wh+1)/2;
    }
    for(i=0;i<=edge-w;i++)for(j=0;j<=edge-h;j++){
	int pass=1;
	for(s=0;s<w;s++)for(t=0;t<h;t++){
		if(bb[i+s][j+t]!=0){pass=0;goto exitloop1;}
	}
exitloop1:
	if(pass){
		for(s=0;s<w;s++)for(t=0;t<h;t++)bb[i+s][j+t]=id-sti+1;
		if(searchfill(edge, sti, edi, id-1))return 1;
		for(s=0;s<w;s++)for(t=0;t<h;t++)bb[i+s][j+t]=0;
	}
    }
    for(i=0;i<=edge-h;i++)for(j=0;j<=edge-w;j++){
	 int pass=1;
   	 for(s=0;s<w;s++)for(t=0;t<h;t++){
		if(bb[i+t][j+s]!=0){pass=0;goto exitloop2;}
	 }
exitloop2:
         if(pass){
		for(s=0;s<w;s++)for(t=0;t<h;t++)bb[i+t][j+s]=id-sti+1;
		if(searchfill(edge, sti, edi, id-1))return 1;
		for(s=0;s<w;s++)for(t=0;t<h;t++)bb[i+t][j+s]=0;
	}
    }
    return 0;
}

int tryfill(int edge, int f, int e)
{
    memset(bb,0,sizeof(bb));
    memset(fill,0,sizeof(fill));
    if(e-f>MAXF){
	    fprintf(stderr,"overflow in buffer fill\n");
	    exit(-1);
    }
    if(searchfill(edge,f,e,e-1)){
	return 1;
    }
    return 0;
}

int rcc;
void trysearch(int level)
{
    if(level>=NUM){
	if(num[level]!=(U+1)*(V+1)){
	    fprintf(stderr, "Invalid total num %d\n",num[level]);
	}else{
#if 1
	    int u;
	    for(u=0;u<NUM;u++){
		if(tryfill(s[u],num[u],num[u+1])){
		    memcpy(bs[u],bb,sizeof(bb));
		}else break;
	    }
	    if(u>=NUM){
#endif
	      int i,j,k;
	      printf("a:");output(a,U+1);
	      printf("b:");output(b,V+1);
	      for(i=0;i<NUM;i++){
		printf("%d*%d=",s[i],s[i]);
		for(j=num[i];j<num[i+1];j++){
		    printf("+%d*%d",tdata[idata[j]].x,tdata[idata[j]].y);
		}
		printf(";\n");
#if 1
		for(j=0;j<s[i];j++){
		  for(k=0;k<s[i];k++){
		    printf("%c",bs[i][j][k]+'A'-1);
		  }
		  printf("\n");
		}
	      }
#endif
	      printf("\n");
	      fflush(stdout);
	    }
	}
	return;
    }
    int target = s[level]*s[level];
#ifdef OUT
    printf("try L%d T%d\n",level,target);
#endif
    search(level, target, num[level], 0);
}

void check(int a[], int b[])
{
    int i,j,c=0;
    for(i=0;i<U+1;i++)for(j=0;j<V+1;j++){
	used[c]=0;
	if(a[i]<b[j]){
	   tdata[c].x=a[i];
	   tdata[c].y=b[j];
	}else{
	   tdata[c].x=b[j];
	   tdata[c].y=a[i];
	}
	c++;
    }
    qsort(tdata,(U+1)*(V+1),sizeof(int),cmpd);
#ifdef OUT
    printf("Cand:");
    for(i=0;i<(U+1)*(V+1);i++)printf("%d %d",tdata[i].x,tdata[i].y);
    printf("\n");
#endif
    num[0]=0;
    trysearch(0);
}
#define MAXN MAXE
int main()
{
    int a1,a2,a3;
    for(a1=1;a1<MAXN;a1++)for(a2=a1+1;a2<MAXN;a2++)for(a3=a2+1;a3<MAXN;a3++){
		   int r=a1*a1+a2*a2+a3*a3;
		   double h=sqrt(r); int g=(int)round(h);
		   if(fabs(h-round(h))<1E-8&&r==g*g){
			   ss=g;s[0]=a1;s[1]=a2;s[2]=a3;
			   printf("Search: %d^2+%d^2+%d^2=%d^2\n",a1,a2,a3,g);
    inita(a,U+1,ss);
    do{
	if(a[0]>s[0])break;
	inita(b,V+1,ss);
	do{
	    if(b[0]>s[0])break;
	    check(a,b);
//	}while(0);
	}while(nexta(b,V+1));
//    }while(0);
    }while(nexta(a,U+1));
}}
    return 0;
}
