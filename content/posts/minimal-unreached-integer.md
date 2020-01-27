---
title: "最小无法表达的正整数"
date: 2020-01-27
thumbnail: "../thumbnails/exp.png"
tags:
    - "加减乘除"
---

# 问题提出
2008年8月[mathe提问](https://bbs.emath.ac.cn/thread-705-1-1.html):  
求最小无法用1~n这n个数通过四则混合运算表达出来的正整数.  
如  
n=1,那么显然只能表示1,所以结果是2.  
n=2,由于$1=2-1,2=2\times 1,3=2+1$,4无法表示所以结果是4  
n=3,由于$1=\frac{2+1}3,2=3+1-2,3=3\times (2-1),4=3+2-1,5=3\times 2-1,6=3\times 2\times1,7=3\times 2+1,8=(3+1)\times 2,9=3\times (2+1)$,10无法表示,所以结果为10.  
对不同的n分别求值,看谁能够达到最大的n.  
<!--more-->

# 问题进展
无心人首先手工计算，[得出n=4应该是29](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=705&pid=8715&fromuid=20):  
```bash
1=(3+2-1)/4
2 = (2-1) + (4-3)
3 = 4 - (1 + 2) / 3
4 = (3 - 1) + (4 - 2)
6 = 1 + 3 + 4 - 2
8 = 2 + 3 + 4 - 1
10 = 1 + 2 + 3 + 4
7 = (1 + 2 + 4) * 1
5 = (2 - 3 + 4) * 1
9 = (2 + 3 + 4) * 1
11 = 2 * 4 + 1 * 3
12 = 2 * 4 + 1 + 3
13 = 3 * 4 + 2 - 1
15 = 3 * 4 + 2 + 1
14 = 1 * 3 * 4 + 2
16 = (1 + 3) * 4 + 2
17 = (1 + 4) * 3 + 2
18 = (4 - 1) * 3 * 2
19 = (4 + 2) * 3 + 1
20 = (3 * 2 - 1) * 4
21 = ( 4 + 2 + 1) * 3
22 = (4 * 3 - 1) * 2
23 = 4 * 3 * 2 - 1
24 = (4 + 2) * (1 + 3)
25 = (4 + 1) * (2 + 3)
26 = (4 * 3 + 1) * 2
27 = (4 * 2 + 1) * 3
28 = (2 * 3 + 1)  * 4
```
kenmark[认为应该](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=705&pid=8725&fromuid=20)有$a[n]\gt \frac{n(n+1)}2$.  
无心人[认为应该](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=705&pid=8733&fromuid=20)有$a[n]\le n\times a[n-1]$.  
无心人开始尝试用[代码](../attached/minimalinteger/无心人1.txt)解决. 他估算了一下复杂度，发现n=10时需要穷举$2^8\times 4^9\times 10!=243,524,645,683,200$,  
mathe提议应该使用[加减乘除产生的表达式计数](https://emathgroup.github.io/blog/count-expressions)中的方法，可将穷举数目降低到$2,894,532,443,154$。  
无心人在继续更新[他的代码](../attached/minimalinteger/无心人2.txt).  
最后提供了他的[第三个版本Delphi的代码](../attached/minimalinteger/无心人3.txt).  

mathe发现[A060315](http://oeis.org/A060315)已经计算到n=10了。 mathe也提供了他的可以计算到n=10的[代码](../attached/minimalinteger/mathe1.txt)。  
[mathe介绍](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=705&pid=9097&fromuid=20)他的实现主要是要避免产生等价的表达式，但是每个表达式都要从头开始计算。当然应该还可以做一些优化，但是从复杂度看，不会有本质的改善。而如果将宏CACHE_LIMIT定义成非零的值。那么那些操作数数目不超过CACHE_LIMIT的子表达式的结果会被缓存起来（结果重复的会被合并），从而表达式其他部分发生更新时这个部分不需要重复计算。这个方法可以用来提高一些计算速度。但是CACHE_LIMIT不能太大，不然内存消耗受不了。当然程序采用分数运算也降低了速度，但是保证了正确性.  
后来[他发现](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=705&pid=10206&fromuid=20):其实非常可能只使用+,-,$\times$三种操作符就可以得到我们需要的大部分整数结果(甚至于全部).
而对于这个情况,由于肯定只产生整数结果,可以使用动态规划，并提供了[对应的代码](../attached/minimalinteger/mathe2.txt), 并最后得到[如下结果](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=705&pid=10404&fromuid=20):  
r[3]=10  
r[4]=29  
r[5]=76  
r[6]=284  
r[7]=1413  
r[8]=7187  
r[9]=38103  
r[10]=231051  
r[11]=1765186  
r[12]=10539427  
为了检验使用除法后结果还是先相同，mathe有[提供了两个C代码](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=705&pid=10569&fromuid=20):  
代码挺慢,分两部分.  
[第一部分](../attached/minimalinteger/mathe3.txt)将所有1~n之间不超过8个数字通过+,-,\*,/所能得到的结果全部计算出来并且保存在文件中,其中1~11所有结果产生需要10个多小时,总共数据15.6G.  
而计算1~12所产生的结果运行了37个多小时,总共数据量54G  
[第二部分代码](../attached/minimalinteger/mathe4.txt)是有了前面的结果,那么我们就可以将表达式拆分成两部分来求解,不过现在代码写的很简单,没有什么优化,即使N=11也运行了近一个晚上,这个代码应该可以改写的好看写,当然效率也可以改善一些.  
