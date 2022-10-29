#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <time.h>
#include <algorithm>

struct Line{
	short p1,p2,p3,p4;
	bool operator<(const Line& l)const{
		if(p1<l.p1)return true;
		if(p1>l.p1)return false;
		if(p2<l.p2)return true;
		if(p2>l.p2)return false;
		if(p3<l.p3)return true;
		if(p3>l.p3)return false;
		return p4<l.p4;
	}
};
struct Point{
	short x,y;
	bool operator<(const Point& p)const{
		if(x<p.x)return true;
		if(x>p.x)return false;
		return y<p.y;
	}
};

#define P  17
#define C1 3   //both -C1 and -C3 must not be square root mod P so that no infinity points in curves
#define INV (-1)

//using two curves:
//x^2+C1*y^2+A*x+B*y+C=0
//x^2+C2*y^2+D*x+E*y+F=0
int C2;
int A,B,C,D,E,F;
int sqroot[P];
int recip[P];
int pindex1[P];
int pindex2[P];

std::vector<Line> lines;
std::vector<Point> points1;
std::vector<Point> points2;
int maxlines[100];
void setsqroot()
{
	int i,j;
	for(i=1;i<=P-1;i++){
		int v=(i*i)%P;
		if(sqroot[v]==0){
			sqroot[v]=i;
		}
	}
	for(i=1;i<=P-1;i++){
		if(sqroot[i]==0)sqroot[i]=INV;//mark for INVALID
	}
	recip[0]=INV;
	for(i=1;i<=P-1;i++){
		for(j=1;j<=P-1;j++){
			if((i*j)%P==1){
				recip[i]=j;
				break;
			}
		}
	}
}

int get_point_index1(int x, int y)
{
	int idx = pindex1[y];
	if(idx==INV)return INV;
	int i;
	for(i=idx;i<points1.size();++i){
		if(points1[i].y!=y)return INV;
		if(points1[i].x==x)return i;
	}
	return INV;
}
int get_point_index2(int x, int y)
{
	int idx = pindex2[y];
	if(idx==INV)return INV;
	int i;
	for(i=idx;i<points2.size();++i){
		if(points2[i].y!=y)return INV;
		if(points2[i].x==x)return i;
	}
	return INV;
}

void setpindex(int c1, int a, int b, int c, std::vector<Point>& pset, int pindex[])
{
	int x,y;
	pset.clear();
	for(y=0;y<P;y++)pindex[y]=INV;
	for(y=0;y<P;y++){//x^2+c1*y^2+a*x+b*y+c=0
		int z=c1*y*y+b*y+c;z%=P;
		//x^2+a*x+z=0;
		int delta = a*a-4*z;
		delta%=P; if(delta<0)delta+=P;
		delta=sqroot[delta];
		if(delta>0){
			pindex[y]=pset.size();
			int x1=-a+delta;
			int x2=-a-delta;
			x1%=P;x2%=P;if(x1<0)x1+=P;if(x2<0)x2+=P;
			if(x1&1){x1=(x1+P)/2;}else{x1=x1/2;}
			if(x2&1){x2=(x2+P)/2;}else{x2=x2/2;}
			Point pp;pp.x=x1;pp.y=y;
			pset.push_back(pp);
			pp.x=x2;
			pset.push_back(pp);
		}else if(delta==0){
			pindex[y]=pset.size();
			int x=(P-a)%P; if(x<0)x+=P;
			if(x&1){x=(x+P)/2;}else{x/=2;}
			Point pp;pp.x=x;pp.y=y;
			pset.push_back(pp);
		}
	}
}

char itochar(int x)
{
	if(x<26)return 'A'+x;
	else return 'a'+(x-26);
}

void outputresults(int tc, std::map<int,int> &p2map)
{
	//	printf("%d nodes %d lines found\n\t",(int)points.size(),(int)lines.size());fflush(stdout);
	int i;
	printf("{N%dE%d}",tc,(int)lines.size());
	for(i=0;i<lines.size();++i){
		printf("%c%c%c%c",itochar(lines[i].p1),itochar(lines[i].p2),itochar(p2map[lines[i].p3]),itochar(p2map[lines[i].p4]));
	}
	printf("\n");
}

void outputlines()
{
	int n1=(int)points1.size();
	int n2=(int)points2.size();
	int i,j;
	lines.clear();
	int tcount=n1+n2;
	std::map<int,int> p2map;
	for(i=0;i<n1;i++){
		int xi,yi;
		xi=points1[i].x;
		yi=points1[i].y;
		for(j=0;j<n2;j++){
			int xj,yj;
			xj=points2[j].x;
			yj=points2[j].y;
			if(xi==xj&&yi==yj){p2map[j]=i;tcount--;continue;}
			int detx=xi-xj; if(detx<0)detx+=P;
			int dety=yi-yj; if(dety<0)dety+=P;
			if(detx==0){
				//x^2+C1*y^2+A*x+B*y+C=0; has point (xi,yi)
				int a=C1;a%=P;if(a<0)a+=P;
				int b=-B*recip[a]-yi; b%=P;if(b<0)b+=P; //point (xi,b) is also in curve 1
				int y1=b;
				if(yi==yj)continue;
				if(y1==yi)continue;
				if(y1==yj)continue;
				//x^2+C2*y^2+D*x+E*y+F=0 has point (xj,yj)
				a=C2;a%=P;if(a<0)a+=P;
				b=-E*recip[a]-yj; b%=P; if(b<0)b+=P; // point (xj, b) is also in curve 2 
				int y2=b;
				if(y2==yj)continue;
				int id1,id2;
				id1=get_point_index1(xi,y1);id2=get_point_index2(xj,y2);
				if(id1==INV||id2==INV){
					fprintf(stderr,"Internal error\n");
					exit(-1);
				}
				if(id1<=i||id2<=j){//not minimal representation
					continue;
				}
				if(y1==y2)continue;
				if(yi==y2||yj==y2)continue;
				Line l1;
				l1.p1=i;l1.p2=id1;l1.p3=j;l1.p4=id2;
				lines.push_back(l1);
				continue;
			}
			int k=dety*recip[detx];k%=P;
			int m=yi-k*xi;m%=P;if(m<0)m+=P;//line y=k*x+m

			//x^2+C1*y^2+A*x+B*y+C=0; has point (xi,yi)
			int k2 = (k*k)%P;
			int a= (k2*C1+1)%P;a%=P;if(a<0)a+=P;
			int b=2*C1*k*m+A+B*k;
			b%=P;if(b<0)b+=P;
			b*=recip[a]; b%=P; 
			int x1 = P-b-xi; x1%=P;if(x1<0)x1+=P;
			int y1 = k*x1+m; y1%=P; if(y1<0)y1+=P;

			a=(k2*C2+1)%P; a%=P;if(a<0)a+=P;
			b=2*C2*k*m+D+E*k;
			b%=P;if(b<0)b+=P;
			b*=recip[a]; b%=P;
			int x2 = P-b-xj; x2%=P; if(x2<0)x2+=P;
			int y2 = k*x2+m; y2%=P; if(y2<0)y2+=P;
			int id1,id2;
			id1=get_point_index1(x1,y1);id2=get_point_index2(x2,y2);
			if(id1==INV||id2==INV){
				fprintf(stderr,"Internal error\n");
				exit(-1);
			}
			if(id1<=i||id2<=j){//not minimal representation
				continue;
			}
			if(x1==x2&&y1==y2)continue;
			if(x1==xj&&y1==yj)continue;
			if(x1==xi&&y1==yi)continue;
			if(x2==xi&&y2==yi)continue;
			if(x2==xj&&y2==yj)continue;
			Line l1;
			l1.p1=i;l1.p2=id1;l1.p3=j;l1.p4=id2;
			lines.push_back(l1);
		}
	}
	if(maxlines[tcount]<=lines.size()){
		int i=0;
		for(j=0;j<n2;j++){
			if(p2map.find(j)==p2map.end()){
				p2map[j]=n1+i;
				i++;
			}
		}
		maxlines[tcount]=lines.size();
		outputresults(tcount,p2map);
	}
}

int main()
{
	srand(time(NULL));
	setsqroot();
	for(C2=1;C2<P;C2++){
		if(sqroot[P-C2]>=0)continue;
		if(C2==C1)continue;
	for(A=0;A<P;A++)for(B=0;B<P;B++)for(C=0;C<P;C++)for(D=0;D<P;D++){
		for(E=0;E<P;E++)for(F=0;F<P;F++){
			setpindex(C1,A,B,C,points1,pindex1);
			setpindex(C2,D,E,F,points2,pindex2);
			int n1=points1.size(),n2=points2.size();
			int dif=n1-n2;if(dif<0)dif=-dif;
			if(dif<=4&&30<=n1+n2&&n1+n2<=50){
				outputlines();
			}
		}
	}
	}
}

