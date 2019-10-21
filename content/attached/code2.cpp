#include <vector>
#include <set>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned int dtype;
typedef std::vector<dtype> DataState;
typedef std::set<dtype> UnionState;
#ifndef K
#define K 10
#endif
#define COUNT 100000
#if K==10
#define HK 5
#define UPBOUND 30
#define REPLACE_NUMS 5
#define LARGE_REPLACE_NUMS 8
#endif
#if K==11
#define HK 5
#define UPBOUND 60
#define REPLACE_NUMS 5
#define LARGE_REPLACE_NUMS 10
#endif
#if K==12
#define HK 6
#define UPBOUND 70
#define REPLACE_NUMS 5
#define LARGE_REPLACE_NUMS 11
#endif
#if K==13
#define HK 6
#define UPBOUND 100
#define REPLACE_NUMS 4
#define LARGE_REPLACE_NUMS 12
#endif
#if K==14
#define HK 7
#define UPBOUND 200
#define REPLACE_NUMS 6
#define LARGE_REPLACE_NUMS 13
#endif
#if K==15
#define HK 7
#define UPBOUND 350
#define REPLACE_NUMS 7
#define LARGE_REPLACE_NUMS 15
#endif
#if K==16
#define HK 7
#define UPBOUND 400
#define REPLACE_NUMS 7
#define LARGE_REPLACE_NUMS 15
#endif
#if K==17
#define HK 8
#define UPBOUND 400
#define REPLACE_NUMS 7
#define LARGE_REPLACE_NUMS 15
#endif
#if K==26
#define HK 12
#define UPBOUND 1500
#define REPLACE_NUMS 6
#define LARGE_REPLACE_NUMS 8
#endif
#if K==27
#define HK 13
#define UPBOUND 1800
#define REPLACE_NUMS 6
#define LARGE_REPLACE_NUMS 8
#endif
#if K==28
#define HK 13
#define UPBOUND 2000
#define REPLACE_NUMS 7
#define LARGE_REPLACE_NUMS 9
#endif
#if K==29
#define HK 13
#define UPBOUND 2000
#define REPLACE_NUMS 7
#define LARGE_REPLACE_NUMS 9
#endif
#if K==30
#define HK 13
#define UPBOUND 2000
#define REPLACE_NUMS 8
#define LARGE_REPLACE_NUMS 16
#endif
#if K==31
#define HK 13
#define UPBOUND 2000
#define REPLACE_NUMS 8
#define LARGE_REPLACE_NUMS 16
#endif
#if K==32
#define HK 14
#define UPBOUND 2000
#define REPLACE_NUMS 8
#define LARGE_REPLACE_NUMS 16
#endif
#define UB UPBOUND
#define MIN_EDGES (REPLACE_NUMS+LARGE_REPLACE_NUMS)

unsigned gen_rand_edge()
{
     unsigned d = 0;
     unsigned rd[K];
     int i;
     for(i=0;i<K;i++){
        unsigned x=rand();
        if(x<RAND_MAX/3){
            d|=1<<i;
        }
     }
     return d;
}

long update_bits(long v, long bits)
{
    int i;
    unsigned rd[K];
    for(i=bits+1;i<K;i++){
       unsigned x=rand();
       if(x<RAND_MAX/3){
           v|=1<<i;
       }
    }
    return v;
}

int should_we_change()
{
    unsigned d=rand();
    d%=10;
    if(d==0)return 1;
    return 0;
}

UnionState tus;
DataState ds;
DataState best_result;
#define WORD_OF(x)        ((x)>>5)
#define INDEX_IN_WORD(x)  ((x)&31)
#define IS_SET(mask,x)     (mask[WORD_OF(x)]&(1<<INDEX_IN_WORD(x)))
#define SET(mask,x)        (mask[WORD_OF(x)]|=1<<INDEX_IN_WORD(x))
#define CLEAR(mask,x)      (mask[WORD_OF(x)]&=~(1<<INDEX_IN_WORD(x)))

void init()
{
    ds.clear();
    tus.clear();
}
int bitcount(dtype d);
int test_add(dtype d)
{
    UnionState ls;
    if(bitcount(d)>HK)return -1;
#ifdef STRICT
    if(tus.find(d)!=tus.end()){
         return -1;
    }
    ls.insert(d);
#endif
    int i;
    for(i=0;i<ds.size();++i){
        if(ds[i]==d)return -1;
        dtype u=d|ds[i];
        if(tus.find(u)!=tus.end()){
            return -1;
        }
        if(ls.find(u)!=ls.end()){
           return -1;
        }
        ls.insert(u);
    }
    return 0;
}

int bitcount(dtype d){
    int i,b=0;
    for(i=0;i<K;i++){
       if(d&(1<<i))b++;
    }
    return b;
}

void do_add(dtype d){
    int i;
#ifdef STRICT
    tus.insert(d);
#endif
    for(i=0;i<ds.size();++i){
        dtype u=d|ds[i];
        tus.insert(u);
    }
    ds.push_back(d);
}

void pop()
{
   int i;
   dtype d=ds.back();
   ds.pop_back();
#ifdef STRICT
   tus.erase(d);
#endif
   for(i=0;i<ds.size();++i){
       dtype u=d|ds[i];
       tus.erase(u);
   }
}

int cds;
unsigned long long cdc;
unsigned long long cdi;
void dumpg(const DataState& data)
{
    int i;
    printf("[%d]",(int)data.size());
        for(i=0;i<data.size();++i){
             unsigned int d;
             d=data[i];
             printf(" %x",d);
        }
    printf("\n");
}

unsigned getrande(int e)
{
    unsigned d=rand();
    d%=e;
    return d;
}

void rsearch(int s)
{
    int i;
    while(1){
       for(i=0;i<100;i++){
           unsigned d = gen_rand_edge();
           if(test_add(d)==0){
               do_add(d);
               break;
           }
       }
       if(i==100){
          return;
       }
    }
}
DataState candset;
int inc=0;
bool found_larger = false;
int addinorder(int c)
{
     int i;
     for(i=c;i<candset.size();i++){
            if(test_add(candset[i])==0){
                 do_add(candset[i]);
                 int r = addinorder(i+1);
                 pop();
                 return r;
            }
     }
     if(ds.size()>best_result.size()){
           best_result=ds;
           dumpg(best_result);
     }
     return ds.size();
}

int search(int c, clock_t limit)
{
    int i,j;
    if(ds.size()>best_result.size()){
        found_larger=true;
        inc=(ds.size()-best_result.size())*20;
        best_result=ds;
        dumpg(best_result);
    }else if(ds.size()==best_result.size()){
        int d=rand();
        if(d%10==0){
            best_result = ds;
        }
        if(inc<0&&found_larger)return -1;
        if(found_larger)inc-=3;
    }
    if(clock()>limit)return -1;
    
    for(i=0;i<c;++i){
       unsigned d = getrande(candset.size());
       if(test_add(candset[d])==0){
           do_add(candset[d]);
           int r = search(c, limit);
           if(r<0){pop();return r;}
           pop();
       }
    }
    return addinorder(0);
}

int largest_bit(long v)
{
    int i;
    for(i=31;i>=0;i--){
       if(v&(1<<i))return i;
    }
    return -1;
}

int count = 0;
#if K==28
#define MIN_TIME (5*CLOCKS_PER_SEC)
#else
#define MIN_TIME (600*CLOCKS_PER_SEC)
#endif
struct EC{
    int eid;
    int ec;
};

int cmp_ec(const void *p, const void *q)
{
    const struct EC *ep =(const struct EC *)p;
    const struct EC *eq = (const struct EC *)q;
    if(ep->ec<eq->ec)return -1;
    if(ep->ec>eq->ec)return 1;
    return (ep->eid-eq->eid);
}

int intersect(const UnionState &u1, const UnionState& u2)
{
    UnionState::const_iterator cit1, cit2;
    cit1 = u1.begin(); cit2=u2.begin();
    while(cit1!=u1.end()&&cit2!=u2.end()){
          if(*cit1 == *cit2)return 1;
          if(*cit1<*cit2)++cit1;
          ++cit2;
    }
    return 0;
}

int cur_time_interval=MIN_TIME;
int main(int argc, const char *argv[])
{
    int i;
    int orbits[UPBOUND];
    long long mul;
    srand(time(NULL));
    if(argc>1){
        init();
        mul = atoi(argv[1]);
        for(i=2;i<argc;i++){
            char *endp=NULL;
            long v=strtol(argv[i],&endp, 16);
            if(test_add(v)==0){
                do_add(v);
            }else{
                printf("invalid input %s\n",argv[i]);
                return -1;
            }
        }
    }else{
        return 0;
    }
    dumpg(ds);
    best_result=ds;
    printf("\n\n");
    do{
        int replace_num = 45;
        for(i=0;i<best_result.size();i++)orbits[i]=0;
        for(i=0;i<replace_num;i++)orbits[i]=1;
        for(i=0;i<replace_num;i++){
              int j=getrande(best_result.size());
              int t=orbits[i];
              orbits[i]=orbits[j];
              orbits[j]=t;
        }
        init();
        for(i=0;i<best_result.size();i++){
              if(orbits[i]==0){
                   do_add(best_result[i]);
              }
        }
        candset.clear();
       for(i=0;i<1<<K;i++){
          int j=(int)((i*mul)&((1ll<<K)-1));
          if(test_add(j)==0){
            candset.push_back(j);
          }
          if((i+1)%1000000==0)fprintf(stderr, "Total %d scanned\n",i+1);
       }
       fprintf(stderr, "total %d cands found\n", (int)candset.size());
       UnionState *uss = new UnionState[candset.size()];
       EC *ec = new EC[candset.size()];
       for(i=0;i<candset.size();i++){
             int j;
             ec[i].eid = i;ec[i].ec=0;
             for(j=0;j<ds.size();j++){
                 dtype r = candset[i]|ds[j];
                 uss[i].insert(r);
             }
       }
       for(i=0;i<candset.size();i++){
             int j;
             for(j=i+1;j<candset.size();j++){
                  if(intersect(uss[i],uss[j])){
                      ec[i].ec++;ec[j].ec++;
                  }
             }
       }
       delete []uss;
       qsort(ec, candset.size(), sizeof(EC), cmp_ec);
       for(i=0;i<candset.size();i++){
           if(test_add(candset[ec[i].eid])==0){
               do_add(candset[ec[i].eid]);
           }
       }
       if(ds.size()>best_result.size()){
           best_result = ds;
       }
       dumpg(best_result);
       delete []ec;
    }while(1);
    return 0;
}
