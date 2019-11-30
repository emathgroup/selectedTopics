---
title: 连续Smith数
categories: 数学
keywords: '素数 数字和'
thumbnail: "../thumbnails/primes.jpg"

tags:
  - 素数
  - 数字和
date: 2019-11-30
---

medie2005[于2008年11月21日提问](https://bbs.emath.ac.cn/thread-926-1-1.html) ，  
Smith number 是自身数字和等于其所有素因子（包含重复）的数字和的合数。  
比如：$666=2\times 3\times 3\times 37$.  
$666$的数字和为：$6+6+6=18$；而$666$的所有素因子（包含重复）的数字和为：$2+3+3+3+7=18$，于是，$666$是一个Smith number。

更奇特的，三个相邻合数73615 ，73616，73617都是Smith number（$73615=5\times 14723$ ，$73616=2^4\times 43\times 107$，$73617=3\times 53\times 463$）。我们把K个相邻合数都是Smith number的数称为K-Smith numbers ，为了方便，就以第1个合数来表示这K个数。比如，73615 ，73616，73617，表示为3-Smith numbers : 73615。


问题：
1）：请求出$10^{12}$内的所有3-Smith numbers。  
2）：对$1\le i\le 10$中的每个$i$，请求出i-Smith numbers各一个。  


# 详细内容
medie2005一开始把数据搜索范围设定在$10^{19}$以内，不过后来应该是在大家的建议之下，把范围缩小到$10^{12}$.  

## 初步分析
liangbch首先给出一种复杂度接近$O(n)$的穷举方法:  
我的想法是穷举，但没想出什么好的剪枝算法。  
&nbsp;算法思路：  
&nbsp;&nbsp;1. 采用筛法，批量分解质因数。应该比逐个分解质因数快不少。需要用到链表（可用内存池优化性能），也需要不少的内存。  
&nbsp;&nbsp;2. .对于每个合数，计算其各位数字之和，和各个素因数各个数字之和。  

liangbch、无心人、mathe首先讨论给定一个数字，其素因子数目的估计值，mathe认为这个值应该近似为$\sum_{p\le n}\frac1p=O(\log(\log(n)))$。  
liangbch认为：  
n以内整数中，平均每个数的素因子的个数计算方法（重复的值计算一次），应该如mathe所言。

更明确的描述应该是这样的。 如n以内素数的个数为m,将第i个素数记为$P_i$ 则n以内的数总的素因子的个数为：  
&nbsp;$\frac{n}{P_1} + \frac{n}{P_2}+\frac{n}{P_3} +\dots +\frac{n}{P_m}$。    
&nbsp;故平均每个数素因子的个数为 $\frac1{P_1}+\frac1{P_2}+\dots +\frac1{P_m}$。  
&nbsp;我们知道，当n很大是，n以内的自然数的倒数有极限，$\sum_{i=1}^{n}\frac 1i=\log(n)+C,c=0.5722$，但不知道mathe如何推出n以内的素数的倒数和公式。  
那么筛法求素数的复杂度 就是 $n\log(\log(n))$了?  

mathe答复:  
我们知道对于一个整数x,它为素数的概率大概为$\frac1{\log(x)}$,所以我们可以将
$sum_{p\le n}\frac1p$近似写成$\int_a^n\frac1{x\log(x)}dx=\int_a^n\frac{d\log(x)}{\log(x)}=\int_{\log(a)}^{\log(n)}\frac{dt}t=O(\log(\log(n))$.  

## 求解
无心人在2008年11月25日给出如下的Huskell代码
```bash
  import Factoring
  import Primes

  readInt :: IO Integer
  readInt = readLn
  
  digitsSum n = if (n == 0) then 0 else ((n  \`mod\` 10) + digitsSum (n \`div\` 10))
  addFactors (a, b) = toInteger (b) * (digitsSum a)
  f (a, b, c) = (b == a+1) && (c == b+1)
  
  smithNumber n = [ n' | n' <- [6..n], not (isPrime n'), let nFactors = primePowerFactorsL n', (digitsSum n') == sum (map (addFactors) nFactors)]
  filter3 ls = filter f (zip3 ls (tail ls) (drop 2 ls)) 
  smithNumber3 n = filter3 (smithNumber n)
  
  main = do
           putStrLn "Input Max n:"
           n <- readInt
           let filename = "ksmith" ++ show(n) ++ ".TXT"
           writeFile filename (show $ smithNumber3 n)
```
1000000内找出所有3-Smith Numbers.  
\[73615,209065,225951,283745,305455,342879,656743,683670,729066,747948,774858,879221,954590\]  
可以看出，$10^6$以内不存在4-Smith Numbers.  
而后他使用1小时多的时间计算出$10^8$以内的结果。  

次日liangbch表示他的C版本代码可以在10秒内解决同样的问题。
```bash
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "memory.h"

typedef unsigned __int64 UINT64;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;


//文件描述：使用最简单的方法筛素数

// 一定范围内质数的个数
//                           10                            4
//                          100                           25
//                        1,000                          168
//                       10,000                        1,229
//                      100,000                        9,592
//                    1,000,000                       78,498
//                   10,000,000                      664,579
//                  100,000,000                    5,761,455
//                1,000,000,000                   50,847,534
//               10,000,000,000                  455,052,511
//              100,000,000,000                4,118,054,813
//            1,000,000,000,000               37,607,912,018
//           10,000,000,000,000              346,065,536,839
//          100,000,000,000,000            3,204,941,750,802
//        1,000,000,000,000,000           29,844,570,422,669
//       10,000,000,000,000,000          279,238,341,033,925
//      100,000,000,000,000,000        2,623,557,157,654,233
//    1,000,000,000,000,000,000       24,739,954,287,740,860
//  估计x 范围内的质数的个数， 欧拉公式 pi(x)=x /( ln(x));

// 经过测试，
// 在1-1百万之间，相邻2个质数的最大差为114,这两个质数分别为492113,492227
// 在1-1千万之间，相邻2个质数的最大差为154,这两个质数分别为4652353,4652507
// 在1-3千万之间，相邻2个质数的最大差为210,这两个质数分别为20831323,20831533
// 在1-1亿之间，  相邻2个质数的最大差为220,这两个质数分别为47326693,47326913


//计算n以内素数个数的上限
UINT64 maxPrimeCount(UINT64 n)
{
        double f= (double)n / log((double)n) * 10.0/9.0 +32;
        return (UINT64)f;
}

DWORD Sift32(DWORD n,DWORD *primeTab)
// 对0到n 之间(不包括n)的数进行筛选，返回质数的个数
// n 不能大于L2 cache，否则效能很低
//**********************************************
{
        BYTE *pBuff= new BYTE[n];
        if (pBuff==NULL)
                return 0;

        DWORD i,sqrt_n,p;
        DWORD pn;
        //----------------------------
        sqrt_n=(DWORD)(sqrt(n)+1);
        
        while (sqrt_n * sqrt_n >=n)
                sqrt_n--;
        
        memset(pBuff,1,n*sizeof(BYTE));
        *pBuff=0;
        *(pBuff+1)=0; //1不是质数
        *(pBuff+2)=1; //2是质数

        for ( i=4;i<n;i+=2) //筛去2的倍数，不包括2
                *(pBuff+i)=0;
        
        for (i=3;i<=sqrt_n;) //i 是质数
        {
                for (p=i*i;p<n; p+=2*i)
                        *(pBuff+p)=0; //筛去i 的奇数倍
                i+=2;
                while ( *(pBuff+i)==0 ) 
                        i++; //前进到下一个质数
        }
        pn=0; //素数个数
        for (i=2;i<n;i++)
        {
                if (pBuff[i])
                {
                        primeTab[pn++]=i;
                }
        }
        delete[] pBuff;
        return pn;
}

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <windows.h>
#include <assert.h>

typedef unsigned __int64 UINT64;

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;


#define SIFT_BLOCK_LEN        (64*1024)

typedef struct fac_struct
{
        DWORD n;
        WORD digSum;
}FAC_ST;

BYTE        g_digSumTab[10000];
FAC_ST        g_facArray[SIFT_BLOCK_LEN+1];

extern UINT64 maxPrimeCount(UINT64 n);
extern DWORD Sift32(DWORD n,DWORD *primeTab);

WORD getDigSum(DWORD n)
{
        DWORD s=0;
        while (n>0)
        {
                s+= (n % 10);
                n/=10;
        }
        return (WORD)s;
}

WORD getDigSum2(DWORD n)
{
        assert(g_digSumTab[1]==1);

        DWORD s=0;
        if (n<10000)
                return g_digSumTab[n];
        else if (n<100000000)
        {
                DWORD t=n % 10000;
                n/=10000;
                return g_digSumTab[n]+g_digSumTab[t];
        }
        else
        {
                DWORD t0,t1;
                t0=n % 10000;
                n/=10000;
                t1=n % 10000;
                n/=10000;
                return g_digSumTab[n]+g_digSumTab[t0]+g_digSumTab[t1];
        }
}

void initTable(BYTE *tab,DWORD n)
{
        DWORD i;
        assert(n==10 || n==100 || n==1000 || n==10000);
        for (i=0;i<n;i++)
        {
                tab[i]=(BYTE)getDigSum(i);
        }
}


void searchSmithNumber(DWORD n,const char *fileName)
{
        DWORD *primeTab=NULL;
        DWORD base;
        DWORD s_n;
        DWORD primeCount;
        DWORD i,count;
        DWORD time1,time2;
        DWORD c;
        DWORD numBuff[1024];
        int   countInBuff;
        FILE *fp=NULL;

        if (fileName!=NULL)
        {
                fp=fopen(fileName,"wb");
        }

        time1=GetTickCount();
        initTable(g_digSumTab,10000 );

        s_n =(DWORD)sqrt((double)n)+2;
        while (s_n * s_n >n)
                s_n--;

        //估算s_n以内素数个数的上限;
        c=(DWORD)maxPrimeCount(s_n);
        
        primeTab=new DWORD[c];
        if (primeTab==NULL )
        {
                goto thisExit;
        }
        
        primeCount=Sift32(s_n+1,primeTab);
        
        count=0;        base=0;
        countInBuff=0;
        while (base<=n)
        {
                DWORD end=base+SIFT_BLOCK_LEN-1;
                
                for (i=0;i<SIFT_BLOCK_LEN;i++)
                {
                        g_facArray[i].n=base+i;
                        g_facArray[i].digSum=0;
                }
                
                i=0;
                while (primeTab[i]*primeTab[i]<=end && i<primeCount)
                {
                        DWORD k,m;
                        
                        k= base/primeTab[i];
                        if (k<primeTab[i])
                                k=primeTab[i];
                        m= k*primeTab[i];
                        if (m<base)
                                m+= primeTab[i];
                        
                        while (m <=end)
                        {
                                while ( g_facArray[m-base].n  % primeTab[i] ==0 )
                                {
                                        g_facArray[m-base].n /= primeTab[i]; 
                                        g_facArray[m-base].digSum += getDigSum2(primeTab[i]);
                                }
                                m+= primeTab[i];
                        }
                        i++;
                }
                
                for (i=0;i<SIFT_BLOCK_LEN && i+base<=n;i++)
                {
                        DWORD x=i+base;
                        WORD s1= getDigSum2(x);
                        WORD s2= g_facArray[i].digSum;
                         
                        if ( g_facArray[i].n !=1 && g_facArray[i].n != x)
                                s2+=getDigSum2(g_facArray[i].n);
                        if (s1==s2 && s1>0)
                        {
                                if (fp!=NULL)
                                {
                                        numBuff[countInBuff++]=x;
                                        if ( countInBuff*sizeof(DWORD)>=sizeof(numBuff) )
                                        {
                                                fwrite(numBuff,sizeof(DWORD),countInBuff,fp);
                                                countInBuff=0;
                                        }
                                }
                                count++;
#ifdef _DEBUG
                                printf("%u\n",x);
#endif
                        }
                }

                base+= SIFT_BLOCK_LEN;
        }
        
        if (fp!=NULL && countInBuff!=0 )
        {
                fwrite(numBuff,sizeof(DWORD),countInBuff,fp);
        }
        
        time2=GetTickCount()-time1;
        printf("It take %d ms,total %d smith num within %d\n",time2,count,n);

thisExit:
        if (primeTab!=NULL)
        {
                delete[] primeTab;        primeTab=NULL;
        }
        if (fp!=NULL)
        {
                fclose(fp);        fp=NULL;
        }
}

#include "stdlib.h"
#include "searchSmithNum.h"

void searchSmithNumber(DWORD n,const char *fileName);
int main(int argc, char* argv[])
{
        DWORD n=100000000;        
        searchSmithNumber(n,NULL); //不输出
        //searchSmithNumber(n,"smith.dat"); //输出到2进制文件 
        
        //计算10^8以内的smith数,不输出，在 PM1.7 需 23.766秒,若输出到话，时间将轻微增加，为24.109秒
        //计算10^9以内的smith数,不输出，在 PM1.7 需 252.578秒
        return 0;
}
```

然后经过分析，liangbch得出,在$10^8$以内， 4阶smith数找到38组，见下：  
```bash
4463535,4463536,4463537,4463538
6356910,6356911,6356912,6356913
8188933,8188934,8188935,8188936
9425550,9425551,9425552,9425553
11148564,11148565,11148566,11148567
15966114,15966115,15966116,15966117
15966115,15966116,15966117,15966118
18542654,18542655,18542656,18542657
21673542,21673543,21673544,21673545
22821992,22821993,22821994,22821995
23767287,23767288,23767289,23767290
28605144,28605145,28605146,28605147
36615667,36615668,36615669,36615670
39227466,39227467,39227468,39227469
47096634,47096635,47096636,47096637
47395362,47395363,47395364,47395365
48072396,48072397,48072398,48072399
54054264,54054265,54054266,54054267
55464835,55464836,55464837,55464838
57484614,57484615,57484616,57484617
57756450,57756451,57756452,57756453
57761165,57761166,57761167,57761168
58418508,58418509,58418510,58418511
61843387,61843388,61843389,61843390
62577157,62577158,62577159,62577160
64572186,64572187,64572188,64572189
65484066,65484067,65484068,65484069
66878432,66878433,66878434,66878435
67118680,67118681,67118682,67118683
71845857,71845858,71845859,71845860
75457380,75457381,75457382,75457383
75457381,75457382,75457383,75457384
77247606,77247607,77247608,77247609
78432168,78432169,78432170,78432171
88099213,88099214,88099215,88099216
89653781,89653782,89653783,89653784
90166567,90166568,90166569,90166570
92656434,92656435,92656436,92656437
```
在$10^8$以内，5阶smith数只找到 2组，见下。而5阶以上的smith数，则没有发现。  
```bash
15966114,15966115,15966116,15966117,15966118
75457380,75457381,75457382,75457383,75457384
```

然后他重新计算到$10^9$, 4阶smith数达到384个，而5阶smith数也达到14个，见下。而5阶以上smith数则依然没有发现。  
```bash
15966114,15966115,15966116,15966117,15966118
75457380,75457381,75457382,75457383,75457384
162449165,162449166,162449167,162449168,162449169
296049306,296049307,296049308,296049309,296049310
296861735,296861736,296861737,296861738,296861739
334792990,334792991,334792992,334792993,334792994
429619207,429619208,429619209,429619210,429619211
581097690,581097691,581097692,581097693,581097694
581519244,581519245,581519246,581519247,581519248
582548088,582548089,582548090,582548091,582548092
683474015,683474016,683474017,683474018,683474019
809079150,809079151,809079152,809079153,809079154
971285861,971285862,971285863,971285864,971285865
977218716,977218717,977218718,977218719,977218720
```

2018年11月30日，suan1024[建议了一种改进算法](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=926&pid=12483&fromuid=20), 虽然有一点小问题，但是liangbch利用其思路将代码速度有提高了一倍。
对应改进代码片段为
```bash
i=0;
while (primeTab[i]*primeTab[i]<=end && i<primeCount)
{
        DWORD k,m,np;
        DWORD npx;
        DWORD npx_end;
        BYTE dsum;

        np=primeTab[i];
        dsum=getDigSum2(np);
        
        k= (base + np-1)/np;
        if (  k<np)
                k=np;
        m= k*np;
        while (m <=end)
        {
                g_valueArray[m-base ] /= np; 
                g_digSumArray[m-base] += dsum;
                m+= np;
        }
        
        npx=np;
        npx_end=end/npx;
        while (npx<=npx_end)
        {
                npx *= np;
                m= (base + npx-1)/npx*npx;
                while (m <=end)
                {
                        g_valueArray[m-base ] /= np; 
                        g_digSumArray[m-base] += dsum;
                        m+= npx;
                }
        }
        i++;
}
```

## 已知最优结果
最后liangbch在网络上找到[国外同行的最优进展](http://www.shyamsundergupta.com/smith.htm)  
其中6阶smith数最小结果为
```bash
2050918644, 2050918645, 2050918646, 2050918647, 2050918648, 2050918649
```
medie2005[在网络上发现了7阶结果](https://www.primepuzzles.net/puzzles/puzz_247.htm) 
```bash
7-Smith numbers
164736913905=3*3*5*257*14244437   (dig_sum=54)
164736913906=2*61*109*139*89123   (dig_sum=55)
164736913907=31*31*2293*74759      (dig_sum=56)
164736913908=2*2*3*7*1961153737  (dig_sum=57)
164736913909=3947*41737247            (dig_sum=58)
164736913910=2*5*19*41*21147229  (dig_sum=50)
164736913911=3*293*187414009         (dig_sum=51)
```
