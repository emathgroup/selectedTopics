#include <stdio.h>
#include <time.h>
#include <set>

#ifdef DEBUG
#define TRACE(fmt,...) printf(fmt, __VA_ARGS__)
#else
#define TRACE(fmt, ...)
#endif

#define ROW 6
#define HROW (ROW/2)
#define TNUM (ROW*ROW)
#define NUMP 4
int b[ROW][ROW];
int order[NUMP]={2,2,3,3};
int list[TNUM][NUMP];
time_t starttime;
void initlist()
{
    int i,j,v;
    for(i=0;i<TNUM;i++){
	    v=i;
	    for(j=0;j<NUMP;j++){
		    list[i][j]=v%order[j];
		    v/=order[j];
	    }
    }
}

int step;
char used[TNUM];
char v[TNUM];

void search()
{
	int i;
	for(i=0;i<TNUM;i++){
		if(!used[i]){//try use used[i];
			v[step]=i;
			step++;
			search();
			step--;
		}
	}
}
long tv;
std::set<long> tvs;
void output()
{
	int i,j;
	int sum[NUMP];
	for(i=0;i<NUMP;i++)sum[i]=0;
	for(i=0;i<ROW;i++){
		int x=v[i];
		for(j=0;j<NUMP;j++){
			sum[j]+=list[x][j];
		}
	}
	for(j=0;j<NUMP;j++){
		if(sum[j]!=(order[j]-1)*HROW)break;
	}
	if(j==NUMP){
		tvs.insert(tv);
	}
}

void searchrow(int start)
{
	int i;
	if(step==ROW){
		output();
		return;
	}
	for(i=start;i<TNUM;i++){
			tv|=1l<<i;
			v[step]=i;
			step++;
			searchrow(i+1);
			tv&=~(1l<<i);
			step--;
	}
}

long rowid[ROW];
long columnid[ROW];
long rowmask, colmask;
long cmbcount;
void outputrows()
{
	if(cmbcount%10==0){
		printf("%ld\n",cmbcount);
	}
}

int is_single_bit(long x)
{
	if(x==0)return 0;
	return (x&(x-1))==0;
}

void searchrows(std::set<long>::iterator& rit, std::set<long>::iterator& cit)
{
	int i;
	int rc=0;
	std::set<long>::iterator hit,lit;
	if(step==ROW){
		cmbcount++;
		outputrows();
		return;
	}
	for(hit = rit; hit!= tvs.end();++hit){
		rc++;
		if(rowmask&*hit)continue;
		if(step==1)printf("\t");
		if(step<=1){
			int tdiff=time(NULL)-starttime;
			printf("Search row %d for step %d(%d seconds)\n",rc,step, tdiff);
		}
		for(i=0;i<step;i++){
			if(!is_single_bit(*hit&columnid[i]))
				break;
		}
		if(i<step)continue;
		if(step>=4){
			for(i=0;i<step;i++){
				TRACE("row %d v %09lx ", i, rowid[i]);
			}
			TRACE("row %d v %09lx\n",step, *hit);
		}
		rowid[step]=*hit;
		rowmask|=*hit;
		for(lit = cit; lit!=tvs.end(); ++lit){
		    if(colmask&*lit)continue;
		    for(i=0;i<=step;i++){
			    if(!is_single_bit(*lit & rowid[i]))
				    break;
		    }
		    if(i<=step)continue;
		    if(step>=4){
			    for(i=0;i<step;i++){
				    TRACE("col %d v %09lx",i,columnid[i]);
			    }
		    	TRACE("col %d v %09lx\n",step, *lit);
		    }
		    columnid[step]=*lit;
		    colmask|=*lit;
		    step++;
		    std::set<long>::iterator n1it=hit;
		    std::set<long>::iterator n2it=lit;
		    searchrows(++n1it,++n2it);
		    colmask&=~*lit;
		    step--;
		}
		rowmask &= ~*hit;
	}
}

int main()
{
	starttime = time(NULL);
	initlist();
	searchrow(0);
	printf("Total %ld rows found\n",tvs.size());
	std::set<long>::iterator sit = tvs.begin();
	searchrows(sit,sit);
	printf("%ld\n",cmbcount);
	return 0;
}
