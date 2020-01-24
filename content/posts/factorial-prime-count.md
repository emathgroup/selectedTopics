---

title: "阶乘和素数函数"
date: "2020-01-24"
thumbnail: "../images/primecount.jpg"
author: "mathe"
tags: 
    - "阶乘"
    - "素数函数" 
---

# 问题提出
[2008年11月medie2005](https://bbs.emath.ac.cn/thread-918-1-2.html)在数学研发论坛提出:  
用$\pi(n)$表示不大于n的素数个数.
比如,$\pi(13)=6$.     (不大于13的素数有:2,3,5,7,11,13)

 13有一个很有意思的性质:$pi(13)=6=1! \times 3!$,即:$\pi(13)$等于13的数字组成(1,3)的阶乘的乘积.  
若自然数n满足:$\pi(n)$等于n的数字组成的阶乘的乘积,我们就称n为PF数.  
有同样性质的数还有1512,1520,1521等等.

更难得的,$pi(226130351)=2!\times 2!\times 6!\times 1!\times 3!\times 0!\times 3!\times 5!\times 1!$,并且,226130351是素数.  
如果n既是PF数,又是素数,则称n为PFP数.

问题:  
1):求出$10^{18}$内的所有PF数.  
2): 13,226130351是前2个PFP数,请求出接下来的两个PFP数.  

# 进展
medie2005说问题来源自[A066457](http://oeis.org/A066457),  
[他指出](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=918&pid=12056&fromuid=20)：  
注意到，数字组成的阶乘的乘积是与顺序无关的。  
因此，$10^{18}$内的的数的数字组成的阶乘的乘积的形式一共只有：$C_27^9=4686825$个。  
现在关键是计算$\pi(n)$。  
他还给出了一个只包含2829个候选素数的列表，只是可惜这个附件已经无法下载了。  

### 计算$\pi(n)$
medie2005找到一份源自张一飞$O(n^{\frac23+\epsilon})$复杂度的[计算$\pi(n)$的算法](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=918&pid=12125&fromuid=20)。
```bash
#include   <iostream.h>   
#include   <math.h>   
#include   <windows.h>   

const   unsigned   default_n=4000000000;   
const   int   maxlen=1000;   

int   primes[maxlen];   
int   len;   
int   sum;   
unsigned   int   n;   
int   m=7;   
int   Q=1;   
int   phiQ=1;   
int\times    v;   

bool   string2int(unsigned   int&   n,char\times    s);   
void   init(void);   
unsigned   int   phi(unsigned   int   x,int   a);   

void   main(void){   
        char   number_string[80];   
        unsigned   int   time;   
    
        cout<<"calc   pi(n)\n";   
        cout<<"n(default   =   "<<default_n<<")   =   ";   
        cin.get(number_string,80);   
        if(!string2int(n,number_string))n=default_n;   
    
        time=GetTickCount();   
        init();   
        int   num=(int)phi(n,len)-sum+len-1;   
        time=GetTickCount()-time;   
        GlobalFree(v);   
        cout<<"pi(n)="<<num<<"("<<time<<"ms)\n";   
        cout<<"press   any   key   to   continue...";   
        cin.get();   
        cin.get();   
}   

void   init(void){   
        int   max;   
        int   sqrt_max;   
        bool\times    mark;   
        int   i,j;   
        int   len2,len3;   
        int   s;   
    
        max=(int)pow(1.0\times n,2.0/3.0);   
        sqrt_max=(int)sqrt(max);   
        mark=(bool\times )GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT,max);   
    
        for(i=2;i<sqrt_max;++i){   
                if(!mark[i]){   
                        j=i\times i;   
                        while(j<max){   
                                mark[j]=true;   
                                j+=i;   
                        }   
                }   
        }   
    
        for(len=0,i=2;(unsigned)i\times i\times i<=n;++i){   
                if(!mark[i])primes[++len]=i;   
        }   
    
        for(j=max-1,sum=0,s=0,len2=len;(unsigned)i\times i<=n;++i){   
                if(!mark[i]){   
                        ++len2;   
                        while((unsigned)i\times j>n){   
                                if(!mark[j])++s;   
                                --j;   
                        }   
                        sum+=s;   
                }   
        }   
    
        for(len3=len2;i<max;++i){   
                if(!mark[i])++len3;   
        }   
    
        sum=(len2-len)\times len3-sum;   
        sum+=len\times (len-1)/2-len2\times (len2-1)/2;   
    
        if(m>len)m=len;   
        for(i=1;i<=m;++i){   
                Q\times =primes[i];   
                phiQ\times =primes[i]-1;   
        }   
    
        v=(int\times )GlobalAlloc(GMEM_FIXED,Q\times sizeof(int));   
        for(i=0;i<Q;++i)v[i]=i;   
        for(i=1;i<=m;++i){   
                for(j=Q-1;j>=0;--j){   
                        v[j]-=v[j/primes[i]];   
                }   
        }   
    
        GlobalFree(mark);   
}   

bool   string2int(unsigned   int&   n,char\times    s){   
        unsigned   int   val=0;   
        for(int   i=0;s[i];++i){   
                if(s[i]>'9'||s[i]<'0')return   false;   
                val\times =10;   
                val+=s[i]-'0';   
                if(val>default_n)return   false;   
        }   
        n=val;   
        return   true;   
}   

unsigned   int   phi(unsigned   int   x,int   a){   
        if(a==m){   
                return   (x/Q)\times phiQ+v[x%Q];   
        }   
        if(x<(unsigned)primes[a]){   
                return   1;   
        }   
        return   phi(x,a-1)-phi(x/primes[a],a-1);   
  }  
```

# 成果展示
medie2005于2008年11月20日一下子找到了一组[四胞胎数据](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=918&pid=12151&fromuid=20)：  
```bash
PF: 8360755200   209210612202
PF: 8360755200   209210612212
PF: 8360755200   209210612220
PF: 8360755200   209210612221
```
格式说明：  
PF  X Y  
X表示$\pi(Y)$  
并且他把结果提交到了[A066457](http://oeis.org/A066457)。  
2017年3月22日[alt找到了](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=918&pid=65705&fromuid=20)：  
$\pi(13030323000581525)=361184624640000=1!\times 3!\times 0!\times 3!\times 0!\times 3!\times 2!\times 3!\times 0!\times 0!\times 0!\times 5!\times 8!\times 1!\times 5!\times 2!\times 5!$  
而这个结果后来也被收录到[A066457](http://oeis.org/A066457)。
