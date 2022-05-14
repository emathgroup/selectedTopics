#include <stdio.h>
#include <time.h>
#include <vector>
#include <algorithm>

#ifdef DEBUG
#define TRACE(fmt,...) printf(fmt, __VA_ARGS__);fflush(stdout)
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
	    printf("%02d:{",i);
	    for(j=0;j<NUMP;j++){
		    list[i][j]=v%order[j];
		    printf(" %d",list[i][j]);
		    v/=order[j];
	    }
	    printf(" }\n");
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
std::vector<long> tvs;
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
		tvs.push_back(tv);
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
int firstbit(long x){
	int i;
	for(i=0;i<36;i++){
		if(x&(1l<<i))return i;
	}
	return -1;
}
void outputrows()
{
	char x[6]={0,1,2,3,4,5};
	char y[6]={0,1,2,3,4,5};
	int sum1[NUMP],sum2[NUMP];
	do{
		do{
			int i,j;
			long v1=0,v2=0;
			int failed = 0;
			for(i=0;i<6;i++){
				if(v1&rowid[x[i]]&columnid[y[i]]){
					failed=1;break;
				}
				v1|=rowid[x[i]]&columnid[y[i]];
				if(v2&rowid[x[i]]&columnid[y[ROW-1-i]]){
					failed=1;break;
				}
				v2|=rowid[x[i]]&columnid[y[ROW-1-i]];
			}
			for(i=0;i<tvs.size();i++){
				if(v1==tvs[i])break;
			}
			if(i==tvs.size())continue;
			for(i=0;i<tvs.size();i++){
				if(v2==tvs[i])break;
			}
			if(i==tvs.size())continue;
			printf("Sol:\n");
			for(i=0;i<6;i++){
				for(j=0;j<6;j++){
					long h=rowid[x[i]]&columnid[y[j]];
					printf("%02d,",firstbit(h));
				}
				printf("\n");
			}
			printf("\n");
			fflush(stdout);
			return;

		}while(std::next_permutation(x,x+6));
	}while(std::next_permutation(y,y+6));

}

int is_single_bit(long x)
{
	if(x==0)return 0;
	return (x&(x-1))==0;
}

void searchrows(int rit, int cit)
{
	int i;
	int rc=0;
	int hit,lit;
	if(step==ROW){
		cmbcount++;
		outputrows();
		return;
	}
	for(hit = rit; hit!= tvs.size();++hit){
		rc++;
		if(rowmask&tvs[hit])continue;
		if(step==1)printf("\t");
		if(step<=1){
			int tdiff=time(NULL)-starttime;
			fprintf(stderr,"Search row %d for step %d(%d seconds)\n",rc,step, tdiff);
		}
		for(i=0;i<step;i++){
			if(!is_single_bit(tvs[hit]&columnid[i]))
				break;
		}
		if(i<step)continue;
		if(step>=5){
			for(i=0;i<step;i++){
				TRACE("row %d v %09lx ", i, rowid[i]);
			}
			TRACE("row %d v %09lx\n",step, tvs[hit]);
		}
		rowid[step]=tvs[hit];
		rowmask|=tvs[hit];
		for(lit = cit; lit!=tvs.size(); ++lit){
		    if(colmask&tvs[lit])continue;
		    for(i=0;i<=step;i++){
			    if(!is_single_bit(tvs[lit] & rowid[i]))
				    break;
		    }
		    if(i<=step)continue;
		    if(step>=5){
			    for(i=0;i<step;i++){
				    TRACE("col %d v %09lx",i,columnid[i]);
			    }
		    	TRACE("col %d v %09lx\n",step, tvs[lit]);
		    }
		    columnid[step]=tvs[lit];
		    colmask|=tvs[lit];
		    step++;
		    searchrows(hit+1,lit+1);
		    colmask&=~tvs[lit];
		    step--;
		}
		rowmask &= ~tvs[hit];
	}
}

int main()
{
	starttime = time(NULL);
	initlist();
	searchrow(0);
	printf("Total %ld rows found\n",tvs.size());fflush(stdout);
	searchrows(0,0);
	printf("%ld\n",cmbcount);
	return 0;
}
