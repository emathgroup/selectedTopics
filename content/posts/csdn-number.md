---

title: "寻觅CSDN Number"
date: "2019-10-22 04:06:57"
thumbnail: "../thumbnails/p20.png"
author: "mathe"
tags: 
    - "bash"
    - "CSDN Number" 
---

#简介
medie2005[在csdn论坛提出一个问题]:

设一个合数n的素因子分解式为$S(n)=p_1^{c_1}p_2^{c_2}\dots p_i^{c_i}$. ( $p_1\lt p_2\lt\dots\lt p_i$ ) 
将S(n)全部展开,形成如下形式: 

$(p_1\times\dots\times p_1) \times (p_2\times\dots\times p_2) \times (p_3\times\dots\times p_3) \times\dots\times (p_i\times\dots\times p_i)$. 

再提取上面形式的各个素因子,得到如下形式: 

$p_1\dots p_1p_2\dots p_2p_3\dots p_3\dots p_i\dots p_i$ 

($c_1$个$p_1$)($c_2$个$p_2$)($c_3$个$p_3$)$\dots$($c_i$个$p_i$) 

顺次连接上面的素因子,得到了一个10进制数: 

$\overline{p_1\dots p_1p_2\dots p_2p_3\dots p_3\dots p_i\dots p_i}$ 

记为$Factor(n)=\overline{p_1\dots p_1p_2\dots p_2p_3\dots p_3\dots p_i\dots p_i}$. 

比如:$n=20=2\times 2\times 5, S(20)=2\times 2\times 5$. 
于是Factor(20)=225. 

如果对某个n,有Factor(n)%n==0成立,我们称n为一个CSDN number.(^_^,恶搞一下). 

比如:$n=28749=3\times 7\times 37\times 37$. 
于是Factor(28749)=373737. 
而$\frac{373737}{28749}=13$. 
于是28749是一个CSDN Number. 

你能求出多大范围内的CSDN Number?

有人通过穷举发现$10^8$以内28749是唯一的一个CSDN Number,那么是否就不存在其它的CSDN Number了呢？

后来无心人在2008年4月19日寻觅到了第二个CSDN Number

n=211241216126899091459691909091414757129970369241166137826081416126521279324961764118276168846821336356291

其中$\frac{factor(n)}n = 7\times 7\times 11\times 23\times 127$。 
让我们一起看看这么神奇的数据究竟是如何被寻觅到的。

#详细内容
在medie2005将信息发布到国外网站 primepuzzles.net后，J.K. Andersen首先在2008年12月8日找到了第二小的CSDN Number是
$21757820799 = 3\times 11\times 13\times 683\times 74257$其中$Factor(n)=3111368374257,\frac{Factor(n)}n=143$。

关于CSDN Number是不是只有有限个， mathe给出的估计恰恰相反，[应该有无穷个]

假设n是一个候选数，那么通过n的所有素因子排列，我们可以得到另外一个大于n的数m,如果我们看成m是否n的倍数完全随机，那么m是n的倍数的概率应该是$\frac 1 n$.通过这个想法，
我们可以估计$N$以内csdn数目的期望值应该为
$\sum_{n\le N, n=p_1^{a_1} p_2^{a_2} ...p_t^{a_t}, t>=3} \frac 1n$, 其中求和式种n至少有三个素因子，而且n不是2和5的倍数。
可以看出，这个数据在$N$比较小时不会很大(显然远远小于$\sum_{n=1}^N \frac 1n ~= \ln(N)$)
但是可以证明上面的和在N趋向无穷时也趋向无穷。所以有理由相信csdn数应该是很多的，只是通常会比较大。

而[一个突破口]是他认为

不过另外一方面，我觉得我们可以通过构造法得出一些特殊的解。
我们首先可以寻找一些形式如$a*10^b+1$的合数，其中a很小，而合数的因子数目尽量多。
找到这样的合数以后，我们将这个合数的部分素因子按顺序排序，如果得到结果能够形成一个$a\times p$形式的数，其中p为b位素数，我们就可以得到一个解。
特别的对于a=1，我们选择b为适当的合数应该会比较好，比如a=1,b=15就很可能有解.

gxqcn对上面苦涩难懂的内容[做了进一步解读]

将 $10^b + 1$ 分解因数，假设素数 $p_1 \le p_2 \le \dots \le p_r$ 均在其中，
如果将这$r$个素数依次排列成的十进制数恰好为一个b位素数 $P = \overline{p_1p_2\dots p_r}$ ，
那么$n = p_1\times p_2\times\dots\times p_r\times P$ 即为解之一。

因为：$Factor(n) = \overline{p_1p_2\dots p_rP} = \overline{PP} = P\times(10^b+1)$，
所以：$\frac{Factor(n)}/{n} = \frac{P\times(10^b+1)}{p_1\timesp_2\times\dots\times p_r\times P} = \frac{10^b+1}{p_1\times p_2\times\dots\times p_r}$，

gxqcn从网络上找到了$10^n\pm 1$[因子分解的现成结果]

无心人在2008年4月19日首先利用$10^{105}+1$的因子分解摘选了部分因子
```bash
	211
	241
	2161
	2689
	9091
	459691
	909091
	4147571
	29970369241
	1661378260814161
	265212793249617641
	18276168846821336356291
```
给出了[第一个巨大的CSDN Number] 211241216126899091459691909091414757129970369241166137826081416126521279324961764118276168846821336356291

再后来[shshsh_0510]和[无心人]在2008年4月有陆续利用上面方法找到了一些CSDN Number。

[J.K. Andersen]再次出手，引用了同样的方法在2008年12月构造了[更多的结果]。

medie2005另外非常期望能够找到$\frac{Factor(n)}n$尽量小的结果。到现在我们还只能找到$\frac{Factor(n)}n=11$的结果

```bash
n=859 × 1058313049 × 8591058313049 
Factor(n)=85910583130498591058313049 
         =8591058313049 × (10^13+1)
Factor(n)/n=11 
```
而对于是否存在n使得$\frac{Factor(n)}n=3$或$\frac{Factor(n)}n=7$还是未知的问题。

[在csdn论坛提出一个问题]: https://bbs.csdn.net/topics/220021599
[应该有无穷个]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=361&pid=3400&fromuid=20
[一个突破口]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=361&pid=3393&fromuid=20
[做了进一步解读]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=361&pid=3416&fromuid=20
[因子分解的现成结果]: http://swox.com/~tege/repunit.html
[第一个巨大的CSDN Number]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=361&pid=3423&fromuid=20
[shshsh_0510]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=361&pid=3455&fromuid=20
[无心人]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=361&pid=3470&fromuid=20
[J.K. Andersen]: https://www.primepuzzles.net/puzzles/puzz_472.htm
[更多的结果]: ../attached/anderson.txt
