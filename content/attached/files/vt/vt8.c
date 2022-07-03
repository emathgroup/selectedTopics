#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define NUM 11 //正方形方块数目
#define MAXE 25 //内部小正方形最大边长
#define U 5 //横向裁剪次数
#define V 4 //纵向裁剪次数

typedef struct rec{
    short x,y;
}rec_t;
int s[NUM];
int ss=20;
char bb[MAXE][MAXE];
char bs[NUM][MAXE][MAXE];

int gcd(int a, int b)
{
    if(b==0)return a;
    return gcd(b,a%b);
}

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
int num[NUM+1];

int cmpd(const void *p, const void *q)
{
    const rec_t *pi=(const rec_t*)p;
    const rec_t *qi=(const rec_t*)q;
    int s1=pi->x*pi->y, s2=qi->x*qi->y;
    if(s1!=s2)return s1-s2;
    return pi->y-qi->y;
}

int trysearch(int level);
int search(int level, int target, int counti, int start)
{
    int i;
    if(target==0){
	num[level+1]=counti;
	if(trysearch(level+1))return 1;
	return 0;
    }
    for(i=start;i<(U+1)*(V+1);i++){
	if(tdata[i].x*tdata[i].y>target)return 0; 
	if(used[i])continue;
	if(tdata[i].y>s[level])continue;
	if(i>0&&used[i-1]==0&&tdata[i-1].x==tdata[i].x&&tdata[i-1].y==tdata[i].y&&counti>num[level])continue;
	idata[counti]=i;
	used[i]=1;
#ifdef OUT
	printf("A %d*%d\n",tdata[i].x,tdata[i].y);
#endif
	if(search(level, target-tdata[i].x*tdata[i].y, counti+1,i+1))
	    return 1;
#ifdef OUT
	printf("R %d*%d\n",tdata[i].x,tdata[i].y);
#endif
	used[i]=0;
    }
    return 0;
}


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
    if(searchfill(edge,f,e,e-1)){
	return 1;
    }
    return 0;
}

int rcc;
int trysearch(int level)
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
	      return 1;
	    }
	}
	return 0;
    }
    int target = s[level]*s[level];
#ifdef OUT
    printf("try L%d T%d\n",level,target);
#endif
    return search(level, target, num[level], 0);
}

int check(int a[], int b[])
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
    return trysearch(0);
}
#define MAXN MAXE
int nextai(int a[NUM], int mv){
    int i,j;
    if(a[NUM-1]<mv-1){a[NUM-1]++;return 1;}
    for(i=NUM-2;i>=0;i--){
	    if(a[i]<a[i+1]-1){
		    a[i]++;
		    for(j=i+1;j<NUM;j++)a[j]=a[j-1]+1;
		    return 1;
	    }
    }
    return 0;
}
int main()
{
    int a1[NUM];
    int i;
    for(i=0;i<NUM;i++)a1[i]=i+1;
    do{
		   int r=0;
		   for(i=0;i<NUM;i++)r+=a1[i]*a1[i];
		   double h=sqrt(r); int g=(int)round(h);
		   if(fabs(h-round(h))<1E-8&&r==g*g){
			   ss=g;for(i=0;i<NUM;i++)s[i]=a1[i];
			   int cf=a1[0];
			   for(i=1;i<NUM;i++)cf=gcd(cf,a1[i]);
			   if(cf>1)continue;
			   printf("Search: %d^2",s[0]);
			   for(i=1;i<NUM;i++)printf("+%d^2",s[i]);
			   printf("=%d^2\n",g);
    inita(a,U+1,ss);
    do{
	if(a[0]>s[0])break;
	inita(b,V+1,ss);
	do{
	    if(b[0]>s[0])break;
	    if(check(a,b))break;
//	}while(0);
	}while(nexta(b,V+1));
//    }while(0);
    }while(nexta(a,U+1));
	}
    }while(nextai(a1,MAXN));
    return 0;
}
