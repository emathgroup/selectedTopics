// csolve.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ASSERT(x) assert(x)
const int P=2147483629;
signed extGCD(signed a, signed b, signed & ar, signed & br)
{
	signed x = 0, y = 1;
	ar = 1, br = 0;
	unsigned t, q;
	while (b != 0)
	{
		t = b; q = a / b; b = a % b; a = t;
		t = x; x = ar - q * x; ar = t;
		t = y; y = br - q * y; br = t;
	}
	return (a);
}

class fpn{
	unsigned v;
public:
	fpn(){v=0;}
	fpn(const fpn& n){v=n.v;}
	fpn(unsigned x){v=x%P;}
	fpn& operator+=(const fpn& n);
	fpn& operator-=(const fpn& n);
	fpn& operator*=(const fpn& n);
	fpn& operator/=(const fpn& n);
	fpn operator+(const fpn& n)const{fpn x(*this);return x+=n;}
	fpn operator-(const fpn& n)const{fpn x(*this);return x-=n;}
	fpn operator*(const fpn& n)const{fpn x(*this);return x*=n;}
	fpn operator/(const fpn& n)const{fpn x(*this);return x/=n;}
	fpn operator-()const{return neg();}
	bool isZero()const{return v==0;}
	bool isOne()const{return v==1;}
	bool operator==(const fpn& n)const;
	bool operator!=(const fpn& n)const;
	fpn operator++(int){fpn f(*this);*this+=fpn(1);return f;}
	fpn& operator++(){return *this+=fpn(1);}
	fpn operator--(int){fpn f(*this);*this-=fpn(1);return f;}
	fpn& operator--(){return *this-=fpn(1);}
	fpn inv()const;
	fpn neg()const{fpn n;if(v!=0)n.v=P-v;else n.v=0;return n;}
	fpn& operator=(const fpn& n){v=n.v;return *this;}
	int  getsv()const{int x=v;if(x<P/2)return x;else return -(P-x);}
};

fpn& fpn::operator+=(const fpn& n)
{
	if(v>=P-n.v){
		v=v+n.v-P;
	}else{
		v+=n.v;
	}
	return *this;
}

fpn& fpn::operator-=(const fpn& n)
{
	if(v>=n.v)
		v-=n.v;
	else
		v=v+P-n.v;
	return *this;
}

fpn& fpn::operator*=(const fpn& n)
{
	unsigned long long t = (unsigned long long)v * (unsigned long long) n.v;
	t%=P;
	v=(unsigned)t;
	return *this;
}

fpn& fpn::operator/=(const fpn& n)
{
	return (*this) *= n.inv();
}

fpn fpn::inv()const
{
	signed ar,br;
	extGCD(v, P, ar, br);
	if(ar<0)ar+=P;
	return ar;
}

bool fpn::operator==(const fpn& n)const
{
	return v==n.v;
}

bool fpn::operator!=(const fpn& n)const
{
	return v!=n.v;
}

typedef fpn num;

class constCoord{
	num x,y,z;
protected:
	void normalize(){}///require codes only for integer version to divid all elements by common dividor
public:
	constCoord(){}
	void trace()const{
		fprintf(stderr,"(%d,%d,%d)",x.getsv(),y.getsv(),z.getsv());
	}
	constCoord(const num& cx,const num& cy, const num& cz){
		x=cx;y=cy;z=cz;
		normalize();
	}
	bool isZero()const{return x==0&&y==0&&z==0;}
	constCoord(const num& cx, const num& cy){
		x=cx;y=cy;z=1;
	}
	constCoord linearEqu(const constCoord& c){
		num u=x*c.y-y*c.x;
		num v=y*c.z-z*c.y;
		num w=z*c.x-x*c.z;
		constCoord r(v,w,u);
		return r;
	}
	const num& getX()const{return x;}
	const num& getY()const{return y;}
	const num& getZ()const{return z;}
	bool operator==(const constCoord& c)const{
		if(x*c.y!=y*c.x)return false;
		if(y*c.z!=z*c.y)return false;
		return true;
	}
};

typedef num tnum[2];

class tCoord{
	tnum x,y,z;
protected:
	void normalize(){
		if(x[0]*y[1]==y[0]*x[1]&&y[0]*z[1]==z[0]*y[1]&&z[0]*x[1]==z[1]*x[0]){
			if(x[0]!=0||y[0]!=0||z[0]!=0){
				x[1]=y[1]=z[1]=0;
			}else{
				x[0]=x[1];y[0]=y[1];z[0]=z[1];
				x[1]=y[1]=z[1]=0;
			}
		}
	}
public:
	tCoord(){}
	tCoord(const constCoord& c, const constCoord& k){
		x[0]=c.getX();x[1]=k.getX();
		y[0]=c.getY();y[1]=k.getY();
		z[0]=c.getZ();z[1]=k.getZ();
		normalize();
	}
	void trace()const{
		fprintf(stderr,"(%d:%d %d:%d %d:%d)",x[0].getsv(),x[1].getsv(),y[0].getsv(),y[1].getsv(),z[0].getsv(),z[1].getsv());
	}
	tCoord linearEqu(const constCoord& c){
		num u0=x[0]*c.getY()-y[0]*c.getX();
		num v0=y[0]*c.getZ()-z[0]*c.getY();
		num w0=z[0]*c.getX()-x[0]*c.getZ();
		constCoord r0(v0,w0,u0);
		num u1=x[1]*c.getY()-y[1]*c.getX();
		num v1=y[1]*c.getZ()-z[1]*c.getY();
		num w1=z[1]*c.getX()-x[1]*c.getZ();
		constCoord r1(v1,w1,u1);
		tCoord t(r0,r1);
		return t;
	}
	bool isConst()const{
		return x[1]==0&&y[1]==0&&z[1]==0;
	}
	constCoord getConst()const{
		constCoord c(x[0],y[0],z[0]);
		return c;
	}
	constCoord getT()const{
		constCoord c(x[1],y[1],z[1]);
		return c;
	}
	constCoord replaceT(const num& t)const{
		constCoord c(x[0]+t*x[1],y[0]+t*y[1],z[0]+t*z[1]);
		return c;
	}
	const num& getX0()const{return x[0];}
	const num& getX1()const{return x[1];}
	const num& getY0()const{return y[0];}
	const num& getY1()const{return y[1];}
	const num& getZ0()const{return z[0];}
	const num& getZ1()const{return z[1];}
};

#define MAX_NODES 20
#define MAX_EDGES 26
#define NUM_NODES MAX_NODES
#define MAX_TOTAL_EDGES MAX_EDGES
#define NODES_PER_EDGE 4
enum ctType{unsetType, constType, tType};
enum tType{noT, varT, knownT,updateT};
constCoord cnodes[MAX_NODES], cedges[MAX_EDGES];
char cnodeId[MAX_NODES],cedgeId[MAX_EDGES];
tCoord tnodes[MAX_NODES], tedges[MAX_EDGES];
char tnodeId[MAX_NODES],tedgeId[MAX_EDGES];
ctType nodesType[MAX_NODES], edgesType[MAX_EDGES];
int nodesIndex[MAX_NODES], edgesIndex[MAX_EDGES];
char emask[MAX_EDGES];
int tStatus;
num tSValue[2];

int prevCNodeUsed, prevCEdgeUsed, prevTNodeUsed, prevTEdgeUsed;
int cNodeUsed, cEdgeUsed, tNodeUsed, tEdgeUsed;
int prevTTNodeUsed, prevTTEdgeUsed;

tCoord crossProd(const tCoord& ce1, const tCoord& ce2)
{
	ASSERT(tStatus==knownT);
	num u0,v0,w0;
	num u1,v1,w1;
	num u2,v2,w2;
	const num& a0=ce1.getX0();
	const num& a1=ce1.getX1();
	const num& b0=ce1.getY0();
	const num& b1=ce1.getY1();
	const num& c0=ce1.getZ0();
	const num& c1=ce1.getZ1();
	const num& a2=ce2.getX0();
	const num& a3=ce2.getX1();
	const num& b2=ce2.getY0();
	const num& b3=ce2.getY1();
	const num& c2=ce2.getZ0();
	const num& c3=ce2.getZ1();
	u0=c2*b0-b2*c0;
	u1=c3*b0+c2*b1-b3*c0-b2*c1;
	u2=c3*b1-b3*c1;
	v0=a2*c0-c2*a0;
	v1=a3*c0+a2*c1-c3*a0-c2*a1;
	v2=a3*c1-c3*a1;
	w0=b2*a0-a2*b0;
	w1=b3*a0+b2*a1-a3*b0-a2*b1;
	w2=b3*a1-a3*b1;
	u0+=u2*tSValue[0];
	u1+=u2*tSValue[1];
	v0+=v2*tSValue[0];
	v1+=v2*tSValue[1];
	w0+=w2*tSValue[0];
	w1+=w2*tSValue[1];
	constCoord cc1(u0,v0,w0);
	constCoord cc2(u1,v1,w1);
	tCoord t(cc1,cc2);
	return t;
}

void resetT()
{
	int i;
	tStatus=varT;
	for(i=0;i<tNodeUsed;i++){
		char n=tnodeId[i];
		nodesType[n]=unsetType;
	}
	for(i=0;i<tEdgeUsed;i++){
		char e=tedgeId[i];
		edgesType[e]=unsetType;
	}
	prevTNodeUsed=prevTEdgeUsed=tNodeUsed=tEdgeUsed=0;
	prevTTNodeUsed=prevTTEdgeUsed=0;
#ifdef TRACE
	fprintf(stderr, "resetT\n");
#endif
}

void updateTValue()
{
	int i;
	if(tStatus!=updateT)return;
	for(i=0;i<tNodeUsed;i++){
		cnodes[cNodeUsed]=tnodes[i].replaceT(tSValue[0]);
		int nid=tnodeId[i];
		cnodeId[cNodeUsed]=nid;
		nodesIndex[nid]=cNodeUsed;
		nodesType[nid]=constType;
#ifdef TRACE
		fprintf(stderr, "cnodes %d update to %c ",cNodeUsed, 'A'+nid); cnodes[cNodeUsed].trace();fprintf(stderr,"\n");
#endif
		cNodeUsed++;
	}
	tNodeUsed=prevTNodeUsed=0;
	for(i=0;i<tEdgeUsed;i++){
		cedges[cEdgeUsed]=tedges[i].replaceT(tSValue[0]);
		int eid=tedgeId[i];
		cedgeId[cEdgeUsed]=eid;
		edgesIndex[eid]=cEdgeUsed;
		edgesType[eid]=constType;
#ifdef TRACE
		fprintf(stderr, "cedges %d update to %d ", cEdgeUsed, eid); cedges[cEdgeUsed].trace();fprintf(stderr,"\n");
#endif
		cEdgeUsed++;
	}
	tEdgeUsed=prevTEdgeUsed=0;
	tStatus=noT;
}

bool compareTT(const tCoord& te1, const tCoord& te2)
{
	const num& x2=te2.getX0();
	const num& y2=te2.getY0();
	const num& z2=te2.getZ0();
	const num& x3=te2.getX1();
	const num& y3=te2.getY1();
	const num& z3=te2.getZ1();
	const num& x0=te1.getX0();
	const num& x1=te1.getX1();
	const num& y0=te1.getY0();
	const num& y1=te1.getY1();
	const num& z0=te1.getZ0();
	const num& z1=te1.getZ1();
	num b2=x3*y1-x1*y3,b1=x2*y1+x3*y0-x0*y3-x1*y2,b0=x2*y0-x0*y2;
	num a2=y3*z1-y1*z3,a1=y2*z1+y3*z0-y0*z3-y1*z2,a0=y2*z0-y0*z2;
	if(b2==0){
		if(b1==0){
			if(b0!=0){
				return false;
			}else{
				b2=a2;b1=a1;b0=a0;
				a2=a1=a0=0;
			}
		}else{
			num r=-b0/b1;
			if(a2*r*r+a1*r+a0!=0)
				return false;
			a2=a1=a0=0;
		}
	}
	a1=a1*b2-b1*a2;
	a0=a0*b2-b0*a2;
	ASSERT(tStatus==varT||tStatus==knownT||tStatus==updateT);
	if(a1==0&&a0==0){//get new equation b2*t^2+b1*t+b0=0
		if(tStatus==varT){
			if(b2!=0){
				tStatus=knownT;
				tSValue[0]=-b0/b2;
				tSValue[1]=-b1/b2;
				return true;
			}else if(b1!=0){
				tStatus=updateT;
				tSValue[0]=-b0/b1;
				tSValue[1]=0;
				return true;
			}else{
				return b1==0;
			}
		}else if(tStatus==knownT){
			a1=tSValue[1];a0=tSValue[0];
			a1=a1*b2+b1;
			a0=a0*b2+b0;
			if(a1==0&&a0==0){
				return true;
			}else if(a1==0){
				return false;
			}else{
				num r=-a0/a1;
				if(r*r-tSValue[1]*r-tSValue[0]!=0)
					return false;
				tStatus=updateT;
				tSValue[1]=0;
				tSValue[0]=r;
				return true;
			}
		}else{
			if(b2*tSValue[0]*tSValue[0]+b1*tSValue[0]+b0!=0)
				return false;
			return true;
		}
	}else if(a1==0){
		return false;
	}else{
		num r=-a0/a1;
		if(tStatus==varT){
			tStatus=updateT;
			tSValue[0]=r;
			tSValue[1]=0;
			return true;
		}else if(tStatus==knownT){
			if(r*r-tSValue[1]*r-tSValue[0]!=0)
				return false;
			tStatus=updateT;
			tSValue[0]=r;
			tSValue[1]=0;
			return true;
		}else{
			return r==tSValue[0];
		}
	}
}

bool compareTC(const tCoord& te,const constCoord& ce)
{
	const num& x2=ce.getX();
	const num& y2=ce.getY();
	const num& z2=ce.getZ();
	const num& x0=te.getX0();
	const num& x1=te.getX1();
	const num& y0=te.getY0();
	const num& y1=te.getY1();
	const num& z0=te.getZ0();
	const num& z1=te.getZ1();
	num p1=x1*y2-y1*x2;
	num p2=y0*x2-x0*y2;
	num q1=y1*z2-z1*y2;
	num q2=z0*y2-y0*z2;
	num tv;
	if(p1==0&&p2!=0)
		return false;
	if(q1==0&&q2!=0)
		return false;
	if(p1!=0&&q1!=0){
		if(p1*q2!=p2*q1)
			return false;
	}
	if(p1==0&&q1==0){
//		ASSERT(false);///should have been avoid
		return true;
	}
	if(p1!=0)
		tv=p2/p1;
	else
		tv=q2/q1;
	if(tStatus==varT){
		tStatus=updateT;
		tSValue[0]=tv;
		tSValue[1]=0;
		return true;
	}else if(tStatus==updateT){
		if(tv!=tSValue[0])
			return false;
		return true;
	}else{
		ASSERT(tStatus==knownT);
		if(tv*tv-tSValue[0]-tSValue[1]*tv!=0)
			return false;
		tStatus=updateT;
		tSValue[0]=tv;
		tSValue[1]=0;
		return true;
	}
}

bool setTEdge(int eid, const tCoord& te)
{
	emask[eid]=1;
	if(edgesType[eid]==unsetType){
		edgesType[eid]=tType;
		tedges[tEdgeUsed]=te;
		tedgeId[tEdgeUsed]=eid;
		edgesIndex[eid]=tEdgeUsed++;
#ifdef TRACE
		fprintf(stderr, "Set TEdge %d ",eid);te.trace();fprintf(stderr,"\n");
#endif
		return true;
	}else if(edgesType[eid]==constType){
		const constCoord& ce = cedges[edgesIndex[eid]];
#ifdef TRACE
                fprintf(stderr, "Compare TEdge %d", eid);te.trace();fprintf(stderr,"\n");
#endif
		return compareTC(te,ce);
	}else if(edgesType[eid]==tType){
		const tCoord& te2 = tedges[edgesIndex[eid]];
#ifdef TRACE
                fprintf(stderr, "Compare TEdge %d", eid);te.trace();fprintf(stderr,"\n");
#endif
		return compareTT(te,te2);
	}
}

bool setTNode(int nid, const tCoord& te)
{
	if(nodesType[nid]==unsetType){
		nodesType[nid]=tType;
		tnodes[tNodeUsed]=te;
		tnodeId[tNodeUsed]=nid;
#ifdef TRACE
                fprintf(stderr, "Set TNode %c ",nid+'A');te.trace();fprintf(stderr,"\n");
#endif
		nodesIndex[nid]=tNodeUsed++;
		return true;
	}else if(nodesType[nid]==constType){
		const constCoord& ce = cnodes[nodesIndex[nid]];
#ifdef TRACE
		fprintf(stderr, "Compare TNode %c", nid+'A');te.trace();fprintf(stderr,"\n");
#endif
		return compareTC(te,ce);
	}else if(nodesType[nid]==tType){
		const tCoord& te2 = tnodes[nodesIndex[nid]];
#ifdef TRACE
                fprintf(stderr, "Compare TNode %c", nid+'A');te.trace();fprintf(stderr,"\n");
#endif
		return compareTT(te,te2);
	}
}

bool setConstEdge(int eid, const constCoord& ce)
{
	if(ce.isZero())return false;
	if(edgesType[eid]==unsetType){
		edgesType[eid]=constType;
		cedges[cEdgeUsed]=ce;
		cedgeId[cEdgeUsed]=eid;
		edgesIndex[eid]=cEdgeUsed++;
#ifdef TRACE
		fprintf(stderr, "Edge %d => const ",eid);ce.trace();fprintf(stderr,"\n");
#endif
		return true;
	}else if(edgesType[eid]==constType){
		return cedges[edgesIndex[eid]]==ce;
	}else if(edgesType[eid]==tType){
		ASSERT(tStatus==varT||tStatus==knownT||tStatus==updateT);
		tCoord& te = tedges[edgesIndex[eid]];
		return compareTC(te,ce);
	}
}

bool setConstNode(int nid, const constCoord& ce)
{
	if(ce.isZero())return false;
	if(nodesType[nid]==unsetType){
		nodesType[nid]=constType;
		cnodes[cNodeUsed]=ce;
		cnodeId[cNodeUsed]=nid;
		nodesIndex[nid]=cNodeUsed++;
#ifdef TRACE
		fprintf(stderr, "Node %c => const", nid+'A');ce.trace();fprintf(stderr,"\n");
#endif
		return true;
	}else if(nodesType[nid]==constType){
		return cnodes[nodesIndex[nid]]==ce;
	}else if(nodesType[nid]==tType){
		ASSERT(tStatus==varT||tStatus==knownT||tStatus==updateT);
		tCoord& te = tnodes[nodesIndex[nid]];
		return compareTC(te,ce);
	}
}

char nvmask[MAX_NODES][MAX_NODES][MAX_NODES][MAX_NODES];
bool testEq(char line_buf[MAX_EDGES][NODES_PER_EDGE],const char nodesToEdges[MAX_NODES][MAX_NODES], const char edgesToNodes[MAX_EDGES][MAX_EDGES], char used[MAX_NODES], int numNodes, int numEdges,
			int n1,int n2, int n3, int n4)
{
	int i,j;
	int starte=0;
	tStatus=noT;
	prevCNodeUsed=prevCEdgeUsed=prevTNodeUsed=prevTEdgeUsed=0;
	prevTTEdgeUsed=prevTTNodeUsed=0;
	cNodeUsed=cEdgeUsed=tNodeUsed=tEdgeUsed=0;
	for(i=0;i<MAX_NODES;i++)nodesType[i]=unsetType;
	for(i=0;i<MAX_EDGES;i++)edgesType[i]=unsetType;
	for(i=0;i<MAX_EDGES;i++)emask[i]=0;
	nodesType[n1]=nodesType[n2]=nodesType[n3]=nodesType[n4]=constType;
	nodesIndex[n1]=0,nodesIndex[n2]=1,nodesIndex[n3]=2,nodesIndex[n4]=3;
	cnodeId[0]=n1;cnodeId[1]=n2;cnodeId[2]=n3;cnodeId[3]=n4;
#ifdef TRACE
	fprintf(stderr,"cnode 0: %c (0,0)\n",n1+'A');
	fprintf(stderr,"cnode 1: %c (1,0)\n",n2+'A');
	fprintf(stderr,"cnode 2: %c (1,1)\n",n3+'A');
	fprintf(stderr,"cnode 3: %c (0,1)\n",n4+'A');
#endif
	cNodeUsed=4;
	cnodes[0]=constCoord::constCoord(0,0);
	cnodes[1]=constCoord::constCoord(1,0);
	cnodes[2]=constCoord::constCoord(1,1);
	cnodes[3]=constCoord::constCoord(0,1);
	do{
		if(cNodeUsed>prevCNodeUsed||cEdgeUsed>prevCEdgeUsed){
			if(cNodeUsed>prevCNodeUsed){
				for(i=prevCNodeUsed;i<cNodeUsed;i++){
					for(j=0;j<i;j++){
						int node1=cnodeId[i];
						int node2=cnodeId[j];
						int e=nodesToEdges[node1][node2];
						if(e<0)continue;
						constCoord ce=cnodes[i].linearEqu(cnodes[j]);
						if(!setConstEdge(e,ce)){
#ifdef TRACE
							fprintf(stderr, "Fail to set edge%d: %c%c to const at line %d\n\t",e, node1+'A',node2+'A', __LINE__);ce.trace();fprintf(stderr,"\n");
#endif
							return false;
						}
					}
					prevCNodeUsed++;
				}
			}
			if(cEdgeUsed>prevCEdgeUsed){
				for(i=prevCEdgeUsed;i<cEdgeUsed;i++){
					for(j=0;j<i;j++){
						int edge1=cedgeId[i];
						int edge2=cedgeId[j];
						int n=edgesToNodes[edge1][edge2];
						if(n<0)continue;
						constCoord ce=cedges[i].linearEqu(cedges[j]);
						if(!setConstNode(n,ce)){
#ifdef TRACE
							fprintf(stderr,"Fail to set node%c by edge %d,%d at line %d\n\t", 'A'+n, edge1, edge2, __LINE__);ce.trace();fprintf(stderr,"\n");
#endif
							return false;
						}
					}
					prevCEdgeUsed++;
				}
			}
		}
		updateTValue();
		if(cNodeUsed>prevCNodeUsed)continue;
		if(cNodeUsed==numNodes||cEdgeUsed==numEdges)
			return true;
		if(tStatus==knownT&&((cNodeUsed+tNodeUsed)==numNodes||(cEdgeUsed+tEdgeUsed)==numEdges))
			return true;
		if(tStatus==noT||tEdgeUsed==prevTEdgeUsed&&tNodeUsed==prevTNodeUsed){
			int ec=-1;
			int nc=-1;
			for(i=0;i<numEdges;i++){
				if(edgesType[i]==unsetType&&emask[i]==0){
					for(j=0;j<NODES_PER_EDGE;j++){
						if(nodesType[line_buf[i][j]]==constType){
							break;
						}
					}
					if(j<NODES_PER_EDGE){///find edge nodesToEdges[i][j]
						ec=i;
						nc=line_buf[i][j];
						break;
					}
				}
			}
			if(ec==-1)
				return true;
			constCoord& c=cnodes[nodesIndex[nc]];
			resetT();
			if(c.getX()!=0){
				constCoord c1(-c.getZ(),0,c.getX());
				constCoord k1(-c.getY(),c.getX(),0);
#ifdef TRACE
				fprintf(stderr, "Init TEdge %d ",ec); c1.trace();k1.trace();fprintf(stderr,"\n");
#endif
				setTEdge(ec, tCoord::tCoord(c1,k1));
			}else{
				constCoord c1(0,c.getZ(),-c.getY());
				constCoord k1(1,0,0);
#ifdef TRACE
                                fprintf(stderr, "Init TEdge %d ",ec); c1.trace();k1.trace();fprintf(stderr,"\n");
#endif
				setTEdge(ec, tCoord::tCoord(c1,k1));
			}

		}
		if(tEdgeUsed>prevTEdgeUsed){
			for(i=prevTEdgeUsed;i<tEdgeUsed;i++){
				for(j=0;j<cEdgeUsed;j++){
					int edge1=tedgeId[i];
					int edge2=cedgeId[j];
					int n=edgesToNodes[edge1][edge2];
					if(n<0)continue;
					tCoord ce=tedges[i].linearEqu(cedges[j]);
					if(ce.isConst()){
						if(!setConstNode(n,ce.getConst())){
#ifdef TRACE
							fprintf(stderr,"Fail to set node %c to const by edge %d,%d at line %d\n\t",n+'A',edge1, edge2, __LINE__);ce.trace();fprintf(stderr,"\n");
#endif
							return false;
						}
					}else{
						if(!setTNode(n,ce)){
#ifdef TRACE
							fprintf(stderr, "Fail to set node %c to Tconst by edge %d,%d at line %d\n\t",n+'A',edge1,edge2, __LINE__);ce.trace();fprintf(stderr,"\n");
#endif
							return false;
						}
					}
				}
				prevTEdgeUsed++;
			}
		}
		updateTValue();
		if(cNodeUsed>prevCNodeUsed||cEdgeUsed>prevCEdgeUsed)continue;
		if(tNodeUsed>prevTNodeUsed){
			for(i=prevTNodeUsed;i<tNodeUsed;i++){
				for(j=0;j<cNodeUsed;j++){
					int node1=tnodeId[i];
					int node2=cnodeId[j];
					int e=nodesToEdges[node1][node2];
					if(e<0)continue;
					tCoord ce=tnodes[i].linearEqu(cnodes[j]);
					if(ce.isConst()){
						if(!setConstEdge(e,ce.getConst())){
#ifdef TRACE
							fprintf(stderr,"Fail to set edge %d to const by node %c%c at line %d\n\t", e, 'A'+node1,'A'+node2, __LINE__);ce.trace();fprintf(stderr,"\n");
#endif
							return false;
						}
					}else{
						if(!setTEdge(e,ce)){
#ifdef TRACE
                                                        fprintf(stderr,"Fail to set edge %d to tconst by node %c%c at line %d\n\t", e, 'A'+node1,'A'+node2, __LINE__);ce.trace();fprintf(stderr,"\n");
#endif
							return false;
						}
					}
				}
				prevTNodeUsed++;
			}
		}
		updateTValue();
		if(cNodeUsed>prevCNodeUsed||cEdgeUsed>prevCEdgeUsed||tEdgeUsed>prevTEdgeUsed||tStatus!=knownT)continue;
		for(i=prevTTNodeUsed;i<tNodeUsed;i++){
			for(j=0;j<i;j++){
				int node1=tnodeId[i];
				int node2=tnodeId[j];
				int e=nodesToEdges[node1][node2];
				if(e<0)continue;
				const tCoord& ce1 = tnodes[i];
				const tCoord& ce2 = tnodes[j];
				tCoord ce = crossProd(ce1,ce2);
				if(ce.isConst()){
					if(!setConstEdge(e,ce.getConst())){
#ifdef TRACE
						fprintf(stderr,"Fail to set edge %d to const by node %c%c at line %d\n\t",
								e,node1+'A',node2+'A',__LINE__);ce.trace();fprintf(stderr,"\n");
#endif
						return false;
					}
				}else{
					if(!setTEdge(e,ce)){
#ifdef TRACE
                                                fprintf(stderr,"Fail to set edge %d to tconst by node %c%c at line %d\n\t",
                                                                e,node1+'A',node2+'A',__LINE__);ce.trace();fprintf(stderr,"\n");
#endif
						return false;
					}
				}
				if(tStatus==updateT)
					goto update_loop;
			}
			prevTTNodeUsed++;
		}
		for(i=prevTTEdgeUsed;i<tEdgeUsed;i++){
			for(j=0;j<i;j++){
				int edge1=tedgeId[i];
				int edge2=tedgeId[j];
				int n=edgesToNodes[edge1][edge2];
				if(n<0)continue;
				const tCoord& ce1 = tedges[i];
				const tCoord& ce2 = tedges[j];
				tCoord ce = crossProd(ce1,ce2);
				if(ce.isConst()){
					if(!setConstNode(n,ce.getConst())){
#ifdef TRACE
						fprintf(stderr,"fail to set node %c to const by edge %d,%d at line %d\n\t",n+'A',edge1,edge2,__LINE__);ce.trace();fprintf(stderr,"\n");
#endif
						return false;
					}
				}else{
					if(!setTNode(n,ce)){
#ifdef TRACE
                                                fprintf(stderr,"fail to set node %c to tconst by edge %d,%d at line %d\n\t",n+'A',edge1,edge2,__LINE__);ce.trace();fprintf(stderr,"\n");
#endif
						return false;
					}
				}
				if(tStatus==updateT)
					goto update_loop;
			}
			prevTTEdgeUsed++;
		}
update_loop:
		updateTValue();
	}while(1);
}

char chessboard[NUM_NODES][NUM_NODES];
char used[NUM_NODES];
char line_buf[MAX_TOTAL_EDGES][NODES_PER_EDGE];
int line_count;
int cur_free_edge;
int cur_max_degree;
int curr_comb_id;
int curr_line_num;
int last_comb_id;
int last_line_num;


void output_cur()
{
    int i,j;
    for(i=0;i<line_count;i++)for(j=0;j<NODES_PER_EDGE;j++){
        printf("%c",'A'+line_buf[i][j]);
    }
    printf("\n");
}

void init()
{
    int i,j;
    line_count=0;
    for(i=0;i<NUM_NODES;i++)for(j=0;j<NUM_NODES;j++){
        chessboard[i][j]=-1;
    }
    for(i=0;i<NUM_NODES;i++)used[i]=0;
    curr_comb_id=0;
    curr_line_num=0;
}
void init_mem(){
}

char test(int x,int y,int z, int w)
{
    if(chessboard[x][y]>=0||chessboard[x][z]>=0||
            chessboard[x][w]>=0||chessboard[y][z]>=0||
            chessboard[y][w]>=0||chessboard[z][w]>=0)
        return 0;
    return 1;
}

void set_to(int x,int y, int z,int w, int value)
{
    chessboard[x][y]=chessboard[x][z]=chessboard[x][w]=
        chessboard[y][x]=chessboard[y][z]=chessboard[y][w]=
        chessboard[z][x]=chessboard[z][y]=chessboard[z][w]=
        chessboard[w][x]=chessboard[w][y]=chessboard[w][z]=value;
}

void setv(int x,int y,int z,int w)
{
    set_to(x,y,z,w,line_count);
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
    set_to(x,y,z,w,-1);
    used[x]--;
    used[y]--;
    used[z]--;
    used[w]--;
    line_count--;
}

void process_one_line(char *input)
{
    int i,j,k,t;
    init();
    for(i=0;;i++){
        char c=input[i*4];
        if(c>='A'&&c<='Z'){
            setv(input[i*4]-'A',input[i*4+1]-'A',input[i*4+2]-'A',input[i*4+3]-'A');
        }else{
            break;
        }
    }

    int max_degree=0;
    char e_to_n[MAX_TOTAL_EDGES][MAX_TOTAL_EDGES];
    char n_to_e[NUM_NODES][NUM_NODES];
    for(i=0;i<MAX_TOTAL_EDGES;i++)for(j=0;j<MAX_TOTAL_EDGES;j++){
        e_to_n[i][j]=-1;
    }
    for(i=0;i<NUM_NODES;i++)for(j=0;j<NUM_NODES;j++){
        n_to_e[i][j]=-1;
    }
    for(i=0;i<line_count;i++){
        for(j=0;j<NODES_PER_EDGE;j++){
            for(k=j+1;k<NODES_PER_EDGE;k++){
                n_to_e[line_buf[i][j]][line_buf[i][k]]=
                    n_to_e[line_buf[i][k]][line_buf[i][j]]=i;
            }
        }
    }
    for(i=0;i<NUM_NODES;i++){
        for(j=0;j<NUM_NODES;j++){
            if(n_to_e[i][j]==-1)continue;
            for(k=j+1;k<NUM_NODES;k++){
                if(n_to_e[i][k]==-1)continue;
                if(n_to_e[i][j]==n_to_e[i][k])continue;
                e_to_n[n_to_e[i][j]][n_to_e[i][k]]=
                    e_to_n[n_to_e[i][k]][n_to_e[i][j]]=i;
            }
        }
    }

	int n1,n2,n3,n4;

	int ee[6];
	for(i=0;i<NUM_NODES;i++){
		for(j=i+1;j<NUM_NODES;j++){
			ee[0]=n_to_e[i][j];
			for(k=j+1;k<NUM_NODES;k++){
				ee[1]=n_to_e[i][k],ee[2]=n_to_e[j][k];
				for(t=k+1;t<NUM_NODES;t++){
					ee[3]=n_to_e[i][t],ee[4]=n_to_e[j][t],ee[5]=n_to_e[k][t];
					int u,v;
					bool no_same=true;
					for(u=0;u<6;u++)for(v=u+1;v<6;v++){
						if(ee[u]==ee[v]){
							no_same=false;
							goto end_find;
						}
					}
end_find:
					if(no_same){
						nvmask[i][j][k][t]=1;
					}else{
						nvmask[i][j][k][t]=0;
					}
				}
			}
		}
	}
#define MAX_TC 4
	int tc=0;
	for(n1=0;n1<NUM_NODES;n1++){
		for(n2=n1+1;n2<NUM_NODES;n2++){
			for(n3=n2+1;n3<NUM_NODES;n3++){
				for(n4=n3+1;n4<NUM_NODES;n4++){
					if(nvmask[n1][n2][n3][n4]){
						tc++;
						if(!testEq(line_buf,n_to_e,e_to_n,used,NUM_NODES,line_count,n1,n2,n3,n4)){
//							fprintf(stderr,"Total %d tested\n",tc);
							return;
						}
						if(cNodeUsed==NUM_NODES||cEdgeUsed==line_count)
							goto success;
						if(tStatus==knownT&&((cNodeUsed+tNodeUsed)==NUM_NODES||(cEdgeUsed+tEdgeUsed)==line_count))
							goto success;
						int mask=0;
						char nodeList[NUM_NODES];
						int u,v;
						for(u=0;u<cNodeUsed;u++){
							mask|=1<<cnodeId[u];
						}
						if(tStatus==knownT){
							for(u=0;u<tNodeUsed;u++){
								mask|=1<<tnodeId[u];
							}
						}
						for(u=0,v=0;u<NUM_NODES;u++){
							if(mask&(1<<u)){
								nodeList[v++]=u;
							}
						}
						for(i=0;i<v;i++)for(j=i+1;j<v;j++)for(k=j+1;k<v;k++)for(t=k+1;t<v;t++){
							nvmask[nodeList[i]][nodeList[j]][nodeList[k]][nodeList[t]]=0;
						}
						if(tc>=MAX_TC)break;
					}
				}
			}
		}
	}
success:
//	fprintf(stderr,"total %d tested\n",tc);
	printf("%s\n",input);
	fflush(stdout);
}

#define LINE_BUF_SIZE (MAX_TOTAL_EDGES*(NODES_PER_EDGE+1)+20)
int  main()
{
    char line[LINE_BUF_SIZE];
    int lc=0;
    while(fgets(line,LINE_BUF_SIZE,stdin)){
        int len=strlen(line);
        while(isspace(line[len-1]))len--;
        line[len]='\0';
        process_one_line(line);
    }
    return 0;
}
