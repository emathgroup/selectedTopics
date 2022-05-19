#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
using namespace std;
         //1 2 3 4 5 6 1 2 3 4 5 6 1 2 3 4 8 9
int oo2[]={0,1,0,2,0,1,0,1,0,2,0,1,0,1,0,2,3,0};
int oo3[]={0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,2};
int oo5[]={0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0};
double logp[3];
#define M2 12
#define M3 7
#define M5 4

int determine(int o[18], int A[],int B[], int C[], int r[15], int *first_row_sum)
{
	int a,b,c,d,e,f,g,h,i,j,k,l,m;
	a=o[A[0]]+o[B[0]+6]+o[C[0]+12];
	b=o[C[1]+12]+o[A[1]]+o[B[1]+6];
	c=o[C[2]+12]+o[B[2]+6]+o[A[2]];
	d=o[A[3]]+o[B[3]+6]+o[C[3]+12];
	e=o[A[4]]+o[C[4]+12]+o[B[4]+6];
	f=o[B[5]+6]+o[C[5]+12]+o[A[5]];
	g=o[C[2]+12]+o[A[3]]+o[B[5]+6];
	h=o[C[1]+12]+o[B[3]+6]+o[A[4]];
	i=o[A[1]]+o[B[2]+6]+o[C[4]+12];
	j=o[A[0]]+o[B[4]+6]+o[C[5]+12];
	k=o[B[0]+6]+o[A[2]]+o[C[3]+12];
	l=o[C[1]+12]+o[B[2]+6]+o[A[5]];
	m=o[C[0]+12]+o[A[4]]+o[B[5]+6];
	*first_row_sum=a;
	a=-a;b=-b;c=-c;d=-d;e=-e;f=-f;g=-g;h=-h;i=-i;j=-j;k=-k,l=-l,m=-m;
#ifdef TRACE
	printf("a=%d, b=%d, c=%d, d=%d,e=%d,f=%d,g=%d,h=%d,i=%d,j=%d,k=%d,l=%d,m=%d\n",
			a,b,c,d,e,f,g,h,i,j,k,l,m);
#endif
	{
		r[0]=2*a + (10*b + (-3*d + (e + (-f + (-13*g + (h + (-2*i + (-9*j + (-16*k + (12*l + 6*m))))))))));
		r[1]=2*a + (-8*b + (3*d + (e + (-f + (11*g + (h + (4*i + (9*j + (14*k + (-12*l - 6*m))))))))));
		r[2]= 6*b + (-3*d + (-3*e + (3*f + (-9*g + (-3*h + (-3*j + (-6*k + 6*l)))))));
		r[3]= -2*a + (2*b + (2*e + (-2*f + (4*g + (2*h + (2*i - 2*k))))));
		r[4]=2*a + (-2*b + (3*d + (e + (-f + (5*g + (h + (-2*i + (3*j + (8*k - 6*l)))))))));
		r[5]=-2*a + (-10*b + (3*d + (-e + (-5*f + (13*g + (5*h + (2*i + (9*j + (16*k + (-12*l - 6*m))))))))));
		r[6]=-6*b + (6*g + (6*j + (6*k - 6*l)));
		r[7]=2*a + (16*b + (6*c + (-3*d + (e + (5*f + (-19*g + (-5*h + (-2*i + (-15*j + (-22*k + (18*l + 6*m)))))))))));
		r[8]=-2*a + (8*b + (-3*d + (-e + (f + (-11*g + (-h + (2*i + (-9*j + (-14*k + (12*l + 6*m))))))))));
		r[9]=2*a + (-8*b + (6*d + (-2*e + (2*f + (8*g + (-2*h + (-2*i + (6*j + (14*k + (-12*l - 6*m))))))))));
		r[10]=3*d + (3*e + (-3*f + (3*g + (3*h + 3*j))));
		r[11]=2*a + (-2*b + (3*d + (7*e + (-f + (5*g + (h + (-2*i + (3*j + (2*k - 6*l)))))))));
		r[12]=-2*a + (2*b + (-3*d + (-e + (f + (-5*g + (-h + (2*i + (-3*j + (-2*k + 6*l)))))))));
		r[13]=6*f;
		r[14]=2*a + (-2*b + (-2*e + (2*f + (2*g + (-2*h + (-2*i + 2*k))))));
		int x;
		for(x=0;x<15;x++){
#ifdef TRACE
			printf("r[%d]=%d,",x,r[x]);
#endif
		}
#ifdef TRACE
		printf("\n");
#endif
		return 1;
	}
	return 0;
}

int g1[6]={0,3,6,9,12,15};
int g2[6]={1,4,7,10,13,16};
int g3[6]={2,5,8,11,14,17};
#define UB 1000000
#define LB -1000000
#define BAC 22
int balist[BAC][6]={
	{0,0,0,0,0,0},//x1
	{0,0,0,1,0,0},//x2
	{0,0,0,0,1,0},//x2
	{0,0,0,0,0,1},//x2
	{0,0,1,0,0,0},//x3
	{0,1,0,0,0,0},//x3
	{1,0,0,0,0,0},//x3
	{0,0,0,1,1,0},//x4
	{0,0,0,1,0,1},//x4
	{0,0,0,0,1,1},//x4
	{1,0,0,0,1,0},//x6
	{0,1,0,0,0,1},//x6
	{0,0,1,1,0,0},//x6
	{0,0,0,1,2,0},//x8
	{0,0,0,0,1,2},//x8
	{0,0,0,2,1,0},//x8
	{0,0,0,0,2,1},//x8
	{0,0,0,2,0,1},//x8
	{0,0,0,1,0,2},//x8
	{1,0,0,0,1,2},//x24
	{0,1,0,2,0,1},//x24
	{0,0,1,1,2,0}
};

int verifygroups(int o2[18],int o3[18],int o5[18], int oo2[18], int oo3[18])
{
	int i,j;
	for(i=0;i<6;i++)for(j=i+1;j<6;j++){
		if(o2[g1[i]]==o2[g1[j]]&&
				o3[g1[i]]==o3[g1[j]]&&
				o5[g1[i]]==o5[g1[j]]){
			return -1;
		}
	}
	for(i=0;i<6;i++)for(j=i+1;j<6;j++){
		if(o2[g2[i]]==o2[g2[j]]&&
				o3[g2[i]]==o3[g2[j]]&&
				o5[g2[i]]==o5[g2[j]]){
			return -1;
		}
	}
	for(i=0;i<6;i++)for(j=i+1;j<6;j++){
		if(o2[g3[i]]==o2[g3[j]]&&
				o3[g3[i]]==o3[g3[j]]&&
				o5[g3[i]]==o5[g3[j]]){
			return -1;
		}
	}
	int h;
	for(h=0;h<BAC;h++){
		for(i=0;i<6;i++){
			oo2[g1[i]]=o2[g1[i]]+balist[h][3];
			oo3[g1[i]]=o3[g1[i]]+balist[h][0];
			oo2[g2[i]]=o2[g2[i]]+balist[h][4];
			oo3[g2[i]]=o3[g2[i]]+balist[h][1];
			oo2[g3[i]]=o2[g3[i]]+balist[h][5];
			oo3[g3[i]]=o3[g3[i]]+balist[h][2];
		}
		int pass=1;
		for(i=0;i<18;i++)for(j=i+1;j<18;j++){
			if(oo2[i]==oo2[j]&&oo3[i]==oo3[j]&&o5[i]==o5[j]){
				pass=0;
				goto endtest;
			}

		}
endtest:
		if(pass)return h;
	}
	return -1;
}

int getsorder(int v[18], int g[6], int order[6])
{
	int i,j;
	for(i=0;i<6;i++)order[i]=i;
	for(i=0;i<6;i++)for(j=i+1;j<6;j++){
		if(v[g[order[i]]]>v[g[order[j]]]){
			int t=order[i];
			order[i]=order[j];
			order[j]=t;
		}
	}
	return 0;
}

void adjustorder(int v[18],int g[6])
{
	int order[6],i,mv;
	getsorder(v,g,order);
	mv=v[g[order[0]]];
	for(i=0;i<6;i++){
		v[g[order[i]]]-=mv;
	}
}

int getrange(int v[18], int g[6])
{
	int order[6];
	getsorder(v,g,order);
	return v[g[order[5]]]-v[g[order[0]]];
}

#define ASSERT(x) if(!(x)){fprintf(stderr,"ec %d\n", __LINE__);printf("%d %d %d %d %d %d %d %d %d %d\n",y5,z4,u,v,to4,x5,cv,r[3],H,r[0]);exit(-1);}
#define BC 10
int findmin(int r[15], int o[BC][18], int ks,int V[BC])
{
	int x5,y5,z4,S,u,v,to4;
	int to[18];
	int cc=0;
	for(y5=-1;y5<=1;y5++)for(z4=-1;z4<=1;z4++)for(u=0;u<6;u++)for(v=0;v<6;v++)for(to4=-1;to4<=1;to4++){
		int H=r[0]-u+3*v+3*y5;H%=6;
		if(H<0)H+=6;
		for(x5=H;x5<6;x5+=6){
			int cv = r[3]-2*u-6*z4+4*x5;
			ASSERT(cv % 6 ==0);
			S=to4-cv/6;
			to[4]=to4;
			cv = r[0]+(-u + (3*v + (12*z4 + (17*x5 + 3*y5))));
			ASSERT(cv%6==0);cv/=6;
			to[1]=cv-2*S;
			cv=r[1]+(-u + (-3*v + (-12*z4 + (-13*x5 - 3*y5))));
			ASSERT(cv%6==0);cv/=6;
			to[2]=3*S+cv;
			cv=r[2]+(3*u + (3*v + (12*z4 + (3*x5 - 3*y5))));
			ASSERT(cv%6==0);cv/=6;
			to[3]=-2*S + cv;
			cv=r[4]+(-u + (-3*v + (-6*z4 + (-7*x5 + 3*y5))));
			ASSERT(cv%6==0);cv/=6;
			to[5]=+2*S +cv;
			cv=r[5]+(u + (-3*v + (-12*z4 + (-11*x5 + 3*y5))));
			ASSERT(cv%6==0);cv/=6;
			to[6]=+2*S +cv;
			cv=r[6];
			ASSERT(r[6]%6==0);cv/=6;
			to[7]=cv+S - z4 -x5;
			cv=r[7]+(-u + (3*v + (18*z4 + (17*x5 - 3*y5))));
			ASSERT(cv%6==0);cv/=6;
			to[8]=-2*S + cv;
			cv=r[8]+(u + (3*v + (12*z4 + (13*x5 - 3*y5))));
			ASSERT(cv%6==0);cv/=6;
			to[9]=-2*S + cv;
			cv=r[9]+(2*u + (-6*z4 - 16*x5));

			ASSERT(cv%6==0);cv/=6;
			to[10]=+S +cv;
			cv = r[10]+(-3*u + (-3*v + (-6*z4 + (3*x5 + 3*y5))));
			ASSERT(cv%6==0);cv/=6;
			to[11]=+2*S + cv;
			cv=r[11]+(-u + (-3*v + (-12*z4 + (-x5 + 3*y5))));
			ASSERT(cv%6==0);cv/=6;
			to[12]=+2*S +cv;
			cv=r[12]+(u + (3*v + (6*z4 + (x5 - 3*y5))));
			ASSERT(cv%6==0);cv/=6;
			to[13]=-S + cv;
			cv=r[13];
			ASSERT(cv%6==0);cv/=6;
			to[17]=cv+S -x5 -y5;
			cv = r[14]+2*u - 4*x5;
			ASSERT(cv%6==0);cv/=6;
			to[0]=cv;
			to[14]=z4;
			to[15]=x5;
			to[16]=y5;
#ifdef TRACE
			printf("to4=%d, z4=%d\n",to4,z4);
			int x;
			for(x=0;x<18;x++){
				printf("to[%d]=%d,",x,to[x]);
			}
			printf("\n");
#endif
			adjustorder(to,g1);
			adjustorder(to,g2);
			adjustorder(to,g3);
#ifdef TRACE
			printf("after adjust:\n");
			for(x=0;x<18;x++){
				printf("to[%d]=%d,",x,to[x]);
			}
			printf("\n");
#endif
			int ko=to[0]+to[1]+to[2];
			if(ko>ks)continue;
			int i,j;
			for(i=0;i<cc;i++){
				if(ko<V[i])break;
			}
			if(i>=BC)continue;
#ifdef TRACE
			printf("data added ko=%d\n",ko);
#endif
			if(cc<BC)cc++;
			for(j=cc-1;j>i;j--){
				V[j]=V[j-1];
				memcpy(o[j],o[j-1],sizeof(int)*18);
			}
			V[i]=ko;
			memcpy(o[i],to,sizeof(int)*18);

		}
	}
	return cc;
}

int ma[]={1,2,3,4,5,6};
int mb[]={1,2,3,4,5,6};
int mc[]={1,2,3,4,8,9};
#define A0 (long)(ma[l1[0]]*7)
#define A1 (long)(ma[l1[1]]*7)
#define A2 (long)(ma[l1[2]]*7)
#define A3 (long)((ma[l1[3]])*7)
#define A4 (long)((ma[l1[4]])*7)
#define A5 (long)((ma[l1[5]])*7)
#define B0 (long)((mb[l2[0]])*11)
#define B1 (long)((mb[l2[1]])*11)
#define B2 (long)((mb[l2[2]])*11)
#define B3 (long)((mb[l2[3]])*11)
#define B4 (long)((mb[l2[4]])*11)
#define B5 (long)((mb[l2[5]])*11)
#define C0 (long)(mc[l3[0]]*13)
#define C1 (long)(mc[l3[1]]*13)
#define C2 (long)(mc[l3[2]]*13)
#define C3 (long)(mc[l3[3]]*13)
#define C4 (long)(mc[l3[4]]*13)
#define C5 (long)(mc[l3[5]]*13)
#define X0 num(o2,o3,o5,0)
#define Y0 num(o2,o3,o5,1)
#define Z0 num(o2,o3,o5,2)
#define X1 num(o2,o3,o5,3)
#define Y1 num(o2,o3,o5,4)
#define Z1 num(o2,o3,o5,5)
#define X2 num(o2,o3,o5,6)
#define Y2 num(o2,o3,o5,7)
#define Z2 num(o2,o3,o5,8)
#define X3 num(o2,o3,o5,9)
#define Y3 num(o2,o3,o5,10)
#define Z3 num(o2,o3,o5,11)
#define X4 num(o2,o3,o5,12)
#define Y4 num(o2,o3,o5,13)
#define Z4 num(o2,o3,o5,14)
#define X5 num(o2,o3,o5,15)
#define Y5 num(o2,o3,o5,16)
#define Z5 num(o2,o3,o5,17)

long num(int o2[],int o3[], int o5[], int ind)
{
	long r=1;
	int i;
	for(i=0;i<o2[ind];i++)r*=2;
	for(i=0;i<o3[ind];i++)r*=3;
	for(i=0;i<o5[ind];i++)r*=5;
	return r;
}

double best_value;

void output(int l1[],int l2[],int l3[],int o2[],int o3[],int o5[], double cv)
{
	int v[6][6];
	long mul=1L;
	v[0][0]=X0;v[0][1]=Y0;v[0][2]=Z0;v[0][3]=A0;v[0][4]=B0;v[0][5]=C0;
	v[1][0]=Y1,v[1][1]=C1,v[1][2]=A1,v[1][3]=X1,v[1][4]=Z1,v[1][5]=B1;
	v[2][0]=C2,v[2][1]=X2,v[2][2]=B2,v[2][3]=Y2,v[2][4]=A2,v[2][5]=Z2;
	v[3][0]=A3,v[3][1]=B3,v[3][2]=X3,v[3][3]=Z3,v[3][4]=C3,v[3][5]=Y3;
	v[4][0]=Z4,v[4][1]=A4,v[4][2]=C4,v[4][3]=B4,v[4][4]=Y4,v[4][5]=X4;
	v[5][0]=B5,v[5][1]=Z5,v[5][2]=Y5,v[5][3]=C5,v[5][4]=X5,v[5][5]=A5;
	int i,j;
#if 0
	printf("l1:");
	for(i=0;i<6;i++)printf("%d ",l1[i]);
	printf("\nl2:");for(i=0;i<6;i++)printf("%d ",l2[i]);
	printf("\nl3:");for(i=0;i<6;i++)printf("%d ",l3[i]);
#endif
	for(i=0;i<6;i++)mul*=v[0][i];
	printf("\n%ld(%f):\n",mul,cv);
	for(i=0;i<6;i++){
		for(j=0;j<6;j++)printf("%d\t",v[i][j]);
		printf("\n");
	}
	printf("\n");
	fflush(stdout);
	best_value = cv;
	return;
}

int main()
{
	int l1[6]={0,1,2,3,4,5};
	int l2[6]={0,1,2,3,4,5};
	//int l3[6]={5, 0, 2, 3, 1, 4};
	int l3[6]={0,1,2,3,4,5};
	int r2[15],r3[15],r5[15];
	int o2[BC][18],o3[BC][18],o5[BC][18];
	int v2[BC],v3[BC],v5[BC];
	int co2,co3,co5;
	logp[0]=log(2);logp[1]=log(3);logp[2]=log(5);
	best_value =1000.0;
	do{
		do{
			do{
				int s2,s3,s5;
				int d2=determine(oo2,l1,l2,l3,r2,&s2);
				int d3=determine(oo3,l1,l2,l3,r3,&s3);
				int d5=determine(oo5,l1,l2,l3,r5,&s5);
				int x;
#ifdef TRACE
				printf("l1=");
				for(x=0;x<6;x++)printf("%d,",l1[x]);
				printf("\nl2=");
				for(x=0;x<6;x++)printf("%d,",l2[x]);
				printf("\nl3=");
				for(x=0;x<6;x++)printf("%d,",l3[x]);
				printf("\nr2=");
				for(x=0;x<15;x++)printf("%d,",r2[x]);
				printf("\nr3=");
				for(x=0;x<15;x++)printf("%d,",r3[x]);
				printf("\nr5=");
				for(x=0;x<15;x++)printf("%d,",r5[x]);
				printf("\n");
#endif
				if(d2&&d3&&d5){
#ifdef TRACE
					printf("f2:\n");
#endif
					co2=findmin(r2,o2,M2-s2,v2);
#ifdef TRACE
					printf("f3:\n");
#endif
					co3=findmin(r3,o3,M3-s3,v3);
#ifdef TRACE
					printf("f5:\n");
#endif
					co5=findmin(r5,o5,M5-s5,v5);
					int i,j,k;
					for(i=0;i<co2;i++)for(j=0;j<co3;j++)for(k=0;k<co5;k++){
						double cv = (s2+v2[i])*logp[0]+(s3+v3[j])*logp[1]+(s5+v5[k])*logp[2];
						if(cv<best_value){
							int oo2[18],oo3[18];
							int h =verifygroups(o2[i],o3[j],o5[k],oo2,oo3);
							if(h<0)continue;
							cv+=(balist[h][0]+balist[h][1]+balist[h][2])*logp[1]+(balist[h][3]+balist[h][4]+balist[h][5])*logp[0];
							if(cv<=best_value){
								output(l1,l2,l3,oo2,oo3,o5[k],cv);
							}
						}
					}
				}
			}while(next_permutation(l3,l3+6));
		}while(next_permutation(l2,l2+6));
	}while(next_permutation(l1,l1+6));
	return 0;
}
