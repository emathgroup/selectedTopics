#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <rdrand.h>

typedef unsigned int dtype;
typedef std::vector<dtype> DataState;
#ifndef K
#define K 10
#endif
#if K==10
#define HK 5
#define UPBOUND 30
#define REPLACE_NUMS 5
#endif
#if K==11
#define HK 5
#define UPBOUND 60
#define REPLACE_NUMS 5
#endif
#if K==12
#define HK 6
#define UPBOUND 70
#define REPLACE_NUMS 5
#endif
#if K==13
#define HK 6
#define UPBOUND 100
#define REPLACE_NUMS 6
#endif
#if K==14
#define HK 7
#define UPBOUND 200
#define REPLACE_NUMS 6
#endif
#if K==15
#define HK 7
#define UPBOUND 350
#define REPLACE_NUMS 6
#endif
#define MASK_SIZE (1<<(K-5))
#define MIN_EDGES (2*REPLACE_NUMS)

unsigned gen_rand_edge()
{
     unsigned d = 0;
     unsigned rd[K];
     int i;
     rdrand_get_n_32(K,rd);
     for(i=0;i<K;i++){
        if(rd[i]<1288490189){
            d|=1<<i;
        }
     }
     return d;
}

int should_we_change()
{
    unsigned d=0;
    rdrand_get_n_32(1,&d);
    d%=10;
    if(d==0)return 1;
    return 0;
}

unsigned int gmask[MASK_SIZE];
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
    memset(gmask,0,sizeof(gmask));
}
int bitcount(dtype d);
int test_add(dtype d)
{
    unsigned int lmask[MASK_SIZE];
    memset(lmask,0,sizeof(lmask));
    if(bitcount(d)>HK)return -1;
#ifdef STRICT
    IS_SET(d){
         return -1;
    }
    SET(lmask, d);
#endif
    int i;
    for(i=0;i<ds.size();++i){
        if(ds[i]==d)return -1;
        dtype u=d|ds[i];
        if(IS_SET(gmask, u)){
            return -1;
        }
        if(IS_SET(lmask, u)){
           return -1;
        }
        SET(lmask, u);
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
    SET(gmask, d);
#endif
    for(i=0;i<ds.size();++i){
        dtype u=d|ds[i];
        SET(gmask, u);
    }
    ds.push_back(d);
}

void pop()
{
   int i;
   dtype d=ds.back();
#ifdef STRICT
   CLEAR(gmask, d);
#endif
   ds.pop_back();
   for(i=0;i<ds.size();++i){
       dtype u=d|ds[i];
       CLEAR(gmask, u);
   }
}

int cds;
unsigned long long cdc;
unsigned long long cdi;
void dumpg(const DataState& data)
{
    int i;
    if(data.size()<=cds){
        cdc++;
    }else{
        cdi=10;
        cdc=10;
        cds=data.size();
    }
    if(cdi<=10){
        cdi=10;
    }
    if(cdc<cdi){
         return;
    }
    if(cdi<10000000000000LL){
       cdi*=2;
    }
    cdc=0;
    printf("[%d]",(int)data.size());
    for(i=0;i<data.size();i++){
       printf("%x ",data[i]);
    }
    printf("\n");
    fflush(stdout);
}

unsigned getrand(int s)
{
    unsigned d=0;
    rdrand_get_n_32(1,&d);
    d%=(1<<K)-s;
    d+=s;
    return d;
}

unsigned getrande(int e)
{
    unsigned d=0;
    rdrand_get_n_32(1,&d);
    d%=e;
    return d;
}

void rsearch(int s)
{
    int i;
    while(1){
       for(i=0;i<10;i++){
           unsigned d = gen_rand_edge();
           if(test_add(d)==0){
               do_add(d);
               break;
           }
       }
       if(i==10){
          unsigned d = getrand(s);
       for(i=d;i<(1<<K);++i){
           if(test_add(i)==0){
                do_add(i);
                break;
           }
       }
       if(i==(1<<K)){
          for(i=s;i<d;++i){
             if(test_add(i)==0){
                 do_add(i);
                 break;
             }
          }
          if(i==d){
             return;
          }
       }
       }
    }
}

void search()
{
    int i,j;
    if(ds.size()>best_result.size()){
        best_result = ds;
        dumpg(best_result);
    }else if(ds.size()==best_result.size()){
        if(should_we_change()){
            best_result = ds;
            dumpg(best_result);
        }
    }
    for(i=1;i<1<<K;++i){
       if(test_add(i)==0){
           do_add(i);
           search();
           pop();
       }
    }
}

int main(int argc, const char *argv[])
{
    int i;
    int orbits[UPBOUND];
    DataState ld; 
    do{
        init();
        rsearch(1);
    }while(ds.size()<MIN_EDGES);
    fprintf(stderr,"Init state:\n");
    dumpg(ds);
    for(i=0;i<ds.size();i++){
        best_result.push_back(ds[i]);
    }
    do{
        for(i=0;i<best_result.size();i++)orbits[i]=0;
        for(i=0;i<REPLACE_NUMS;i++)orbits[i]=1;
        for(i=0;i<REPLACE_NUMS;i++){
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
        search();
    }while(1);
    return 0;
}
