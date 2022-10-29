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

#define P  29
#define INV (-1)

//using curve:
//x^4+C2*y^4+A*x^3+B*xy^2+C*y^3+D*x^2+E*y^2+F=0
int C2;
int A,B,C,D,E,F;
int sqroot[P];
int recip[P];
int pindex[P];
int pass[P+1][P+1];
int plist[P+1][P+1][4];

std::vector<Line> lines;
std::vector<Point> points;
std::vector<Point> tangents;
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

int get_point_index(int x, int y)
{
	int idx = pindex[y];
	if(idx==INV)return INV;
	int i;
	for(i=idx;i<points.size();++i){
		if(points[i].y!=y)return INV;
		if(points[i].x==x)return i;
	}
	return INV;
}

void setpindex()
{
	int x,y;
	points.clear();
	for(y=0;y<P;y++)pindex[y]=INV;
	for(y=0;y<P;y++){//x^4+C2*y^4+A*x^3+B*xy^2+C*y^3+D*x^2+E*y^2+F=0
		for(x=0;x<P;x++){
			int z=x*x*x*x+C2*y*y*y*y+A*x*x*x+B*x*y*y+C*y*y*y+D*x*x+E*y*y+F;
			z%=P;if(z<0)z+=P;
			if(z==0){
				if(pindex[y]==INV) pindex[y]=points.size();
				Point pp;pp.x=x;pp.y=y;
				points.push_back(pp);
			}
		}
	}
}

char itochar(int x)
{
	if(x<26)return 'A'+x;
	else return 'a'+(x-26);
}

void outputresults()
{
	//	printf("%d nodes %d lines found\n\t",(int)points.size(),(int)lines.size());fflush(stdout);
	int i;
	printf("{N%dE%d}",(int)points.size(),(int)lines.size());
	for(i=0;i<lines.size();++i){
		printf("%c%c%c%c",itochar(lines[i].p1),itochar(lines[i].p2),itochar(lines[i].p3),itochar(lines[i].p4));
	}
	printf("\n");
}

void outputlines()
{
	int n=(int)points.size();
	int i,j;
	lines.clear();
	tangents.clear();
	int allp=n;
	memset(pass,0,sizeof(pass));
	memset(plist,-1,sizeof(plist));
	for(i=0;i<n;i++){
		int xi,yi;
		xi=points[i].x;
		yi=points[i].y;//x^4+C2*y^4+A*x^3+B*xy^2+C*y^3+D*x^2+E*y^2+F=0
		//dF/dx = 4x^3+3Ax^2+By^2+2Dx
		//dF/dy = 4C2y^3+2Bxy+3Cy^2+2Ey
		int dFx = 4*xi*xi*xi+3*A*xi*xi+B*yi*yi+2*D*xi;
		int dFy = 4*C2*yi*yi*yi+2*B*xi*yi+3*C*yi*yi+2*E*yi;
		dFx%=P;dFy%=P;if(dFx<0)dFx+=P; if(dFx<0)dFy+=P;
		//dFx(x-xi)+dFy(y-yi)=0 //tangent line
		if(dFx==0&&dFy==0){
			Point pp; pp.x=P; pp.y = 0;
			tangents.push_back(pp);
			allp--;//the tangent line is skipped
			continue;//skip sigular point
		}
		int z=P-dFx*xi-dFy*yi;z%=P;if(z<0)z+=P;
		if(z!=0){
			dFx*=recip[z];dFy*=recip[z];
			dFx%=P;dFy%=P; if(dFx<0)dFx+=P; if(dFy<0)dFy+=P;
			Point pp;pp.x=dFx; pp.y=dFy;
			tangents.push_back(pp);
			for(j=0;j<i;j++){
				if(tangents[j].x==dFx && tangents[j].y==dFy)break;
			}
			if(j<i)continue;//skip the line since it is comment tangent line
			if(dFy!=0){//dFx*x+dFy*y+1=0;
				for(j=0;j<P;j++){
					int cy = (P-(dFx*j+1)*recip[dFy])%P;
					if(cy<0)cy+=P;
					pass[j][cy]++;
					if(pass[j][cy]<=4){
						plist[j][cy][pass[j][cy]-1]=i;
					}
				}
				int cx = dFx*recip[dFy]; cx%=P; if(cx<0)cx+=P;
				pass[cx][P]++;
				if(pass[cx][P]<=4){
					plist[cx][P][pass[cx][P]-1]=i;
				}
			}else{//dFx*x+1=0;
				int cx = P-recip[dFx]; if(cx>=P)cx-=P;
				for(j=0;j<P;j++){
					pass[cx][j]++;
					if(pass[cx][j]<=4){
						plist[cx][j][pass[cx][j]-1]=i;
					}
				}
				pass[P][P]++;
				if(pass[P][P]<=4){
					plist[P][P][pass[P][P]-1]=i;
				}
			}
		}else if(dFy!=0){//dFx*x+dFy*y=0;
			if(dFy<0)dFy+=P;
			dFx*=recip[dFy];dFx%=P; if(dFx<0)dFx+=P;//dFx*x+y=0
			Point pp; pp.x = dFx; pp.y= P;
			tangents.push_back(pp);
			for(j=0;j<i;j++){
				if(tangents[j].x==dFx && tangents[j].y==P)break;
			}
			if(j<i)continue;
			for(j=0;j<P;j++){
				int cy = P-dFx*j; cy%=P; if(cy<0)cy+=P;
				pass[j][cy]++;
				if(pass[j][cy]<=4){
					plist[j][cy][pass[j][cy]-1]=i;
				}
			}
			pass[dFx][P]++;
			if(pass[dFx][P]<=4){
				plist[dFx][P][pass[dFx][P]-1]=i;
			}
		}else{//x=0
			Point pp; pp.x = P; pp.y = P;
			tangents.push_back(pp);
			for(j=0;j<i;j++){
				if(tangents[j].x==P&&tangents[j].y==P)break;
			}
			if(j<i)continue;
			for(j=0;j<P;j++){
				pass[0][j]++;
				if(pass[0][j]<=4){
					plist[0][j][pass[0][j]-1]=i;
				}
			}
			pass[P][P]++;
			if(pass[P][P]<=4){
				plist[P][P][pass[P][P]-1]=i;
			}
		}
	}
	int lc=0;
	for(i=0;i<=P;i++)for(j=0;j<=P;j++){
		if(pass[i][j]==4)lc++;
	}
	int tc = (allp*allp)/24-3;
	if(lc>=tc){
	printf("{T%dU%dL%d}",(int)points.size(),allp,lc);
	for(i=0;i<=P;i++)for(j=0;j<=P;j++){
		if(pass[i][j]==4){
			int k;
			for(k=0;k<4;k++)
				printf("%c",itochar(plist[i][j][k]));
		}
	}
	printf("\n");
#if 0
	for(i=0;i<=P;i++)for(j=0;j<=P;j++){
		if(pass[i][j]==4){
			fprintf(stderr,"{%d,%d}",i,j);
			int k;
			for(k=0;k<4;k++)fprintf(stderr,"%c",itochar(plist[i][j][k]));
		}
	}
	fprintf(stderr,"\n");
	for(i=0;i<points.size();++i){
		fprintf(stderr,"\t%c(%x,%x)[%d,%d]\n",itochar(i), points[i].x,points[i].y,tangents[i].x,tangents[i].y);
	}
#endif
	}
}

int main()
{
	srand(time(NULL));
	setsqroot();
	for(C2=1;C2<P;C2++){
		if(sqroot[P-C2]>=0)continue;
		for(A=0;A<P;A++)for(B=0;B<P;B++)for(C=0;C<P;C++)for(D=0;D<P;D++){
			for(E=0;E<P;E++)for(F=0;F<P;F++){
				setpindex();
				if(points.size()>=25&&points.size()<=80){
					outputlines();
				}
			}
		}
	}
}

