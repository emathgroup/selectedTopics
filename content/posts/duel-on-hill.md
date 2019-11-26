---
title: 山顶的决斗
categories: 数学
keywords: '博弈, 概率, probability, game'
thumbnail: "../thumbnails/duel.jpg"

tags:
  - game
  - probability
  - 博弈
  - 概率
date: 2019-11-26
---

# 问题描述
KeyTo9\_Fans于[2013年愚人节提出一个问题](https://bbs.emath.ac.cn/thread-4968-1-1.html) :   
wayne在登一座山，在离山顶还有$1$步之遥的时候，  
发现KeyTo9\_Fans已经在山顶上恭候多时了。  
KeyTo9\_Fans决定给wayne出道难题，  
于是拦住wayne，不让wayne登到山顶，如下图所示：  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;山顶  
———————  
Fans（满血:lol）  　
wayne（$1/3$血）　1步之遥  
———————  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2步之遥  
———————  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3步之遥  
———————  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4步之遥  
———————  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5步之遥  
———————  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;…………  
登山之路只有$1$条，  
Fans会一直在路上阻止wayne往上走，  
wayne想要登到山顶，只能将Fans顶到山顶上去。  
Fans目前拥有$1$体力值，  
而wayne只有$1/3$体力值。  
Fans和wayne将进行$N$个回合的较量。  
在每一回合里面：  
————————————  
Fans和wayne同时发力，  
假设本回合Fans使出了$a$的体力值，wayne使出了$b$的体力值，  
如果$a>b$，那么wayne就会被Fans往下推$1$步，于是离山顶又多了$1$步之遥，但Fans会消耗掉$a$的体力值，wayne的体力值则保持不变；  
如果$a<b$，那么wayne就会把Fans往上顶$1$步，于是离山顶就少了$1$步之遥，但wayne会消耗掉$b$的体力值，Fans的体力值则保持不变；  
如果$a=b$，那么上帝就会掷骰子决定$1$种结果（此时，上述$2$种结果发生的概率均为$50%$）。  
————————————  
注意：体力值只会减少或不变，无法增加。  
体力值可以用光，但不能透支。  
wayne会采取最佳策略登到山顶。  
Fans会采取最佳策略阻止wayne登到山顶，如果阻止不了，则会让wayne登到山顶所用的回合数的期望值尽可能大。  
问题$1$：当$N\to\infty$时，wayne能登到山顶的概率是多少？  
问题$2$：在wayne能登到山顶的前提下，wayne所用回合数的期望值是多少？  

#Fans的砖头
KeyTo9\_Fans首先表示，  
【抛砖】满血的Fans看起来坚不可摧，实际上……  
可以预见地，wayne在第$1$回合就会不惜一切代价，使出所有的体力顶Fans。  
Fans只能使出$1/3$以上的体力阻止wayne（不然Fans就输啦！），  
结果wayne一上来就耗掉了Fans$1/3$以上的血，如下图所示……  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;山顶  
———————  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1步之遥  
———————  
Fans（$\leq 2/3$血）　
wayne（$1/3$血）　　2步之遥  
———————  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;3步之遥  
———————  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4步之遥  
———————  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;5步之遥  
———————  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;…………  
wayne接下来还有什么高招呢？  

#zeroieme表示wayne必然赢
假设wayne离山顶还有n步，此时他计划把体力非平均分配为$(W_1,W_2,\dots,W_n)$，各步体力总和$\sum W_i=W_a$，假设是第a次规划  
前面Fans的砖头就是n=1的特例。  

如果wayne前n-1步,都成功。wayne剩余体力$W_a$。如Fans砖头的分析，Fans为了最后一步阻止，必然使用$W_a+ε$,其中$ε$是无限逼近0的正数。  
往前想，wayne为了$W_a$最大，就是消耗Fans最大体力。前n-1步只要用一点点力ε。Fans怎么办？保留体力用更小力。于是双方无限逼近0?干脆大家都是0算了，让上帝发声。  
这样的话wayne回到n=1的概率长远说是100%，Fans再次消耗$\frac13+ε$。

最后wayne赢。

可是KeyTo9\_Fans问，如果Fans依次使用$\frac18,\frac1{16},\frac1{32},\frac1{64},\dots$的体力，wayne该如何应对呢？  
然后zeroieme问了一个很好的问题，就是游戏中双方是否知道对方还剩余多少体力， Fans表示不知道对方每轮使用了多少体力，所以也不知道对方剩余多少体力。不过这个信息好像在后面倍大家忽略了。

#mathe给出了一个递推数列
mathe[也表示wayne必赢](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=4968&pid=48483&fromuid=20) ，他说可以用动态规划来做，于是他抛出了一个递推数列:  
$a_{0,n}=0,a_{n,n}=n,a_{n,k+1}=\frac{1+a_{n-1,k}}{1+a_{n+1,k}}a_{n+1,k}$  
找出最小的n使得$a_{1 ,n}\le \frac13$即可.  
这个数列实在有些莫名其妙, KeyTo9\_Fans也表示看了很晕，但是他评论这个数列是正确的。  
mathe提示上面表达式中k和n的差必须是偶数。  

#动态规划初步分析
mathe表示  
其实就是要计算出所有k步可以到达的状态集合。体力比表示wayne的体力和Fans体力之比。  
显然其中一步达到的只有$n=1$,体力比$p\ge 1$,, 得到集合$S_1={(n,p)|n=1,p\gt 1}$    
如果两步达到，wayne必然有办法使得一步后必然到达$S_1$,对于集合$S_2={(n,p)|n=2,p\gt 2}$可以达到。  
同样，对于$S_3$,只能初始状态为$n=3$或$n=1$.其中$n=1$的，wayne同样要想法达到$S_2$,  
假设wayne使用策略体力$x=p$,而Fans使用一个略大于p但是非常接近p的策略，于是到达$(2,\frac{p}{1-p}) \in S_2$。  

首先我们知道离山顶$n$步时，如果wayne体力是Fans的$n$倍以上，那么必胜。假设wayne体力是Fans的$a(n)$倍以上时，可以必胜，其中$a(n)$是上确界。那么当体力比$a$充分接近$a(n)$时，wayne必然可以有策略使用体力$x$,使得当Fans使用体力小于$x$时，得出$a(n-1)\lt a-x$,而当Fans使用体力大于$x$一点点时，有$a(n+1)\lt \frac{a}{1-x}$,
于是$x\lt a-a(n-1), a(n+1)\lt \frac a{1-x} \lt \frac a{1-a+a(n-1)}$,让$a$趋向$a(n)$得出关系式$a(n+1)\le\frac{a(n)}{1-a(n)+a(n-1)}$  
反之，如果这个不等式对于某个$n$严格不成立，我们可以找出$x$改善$a(n)$,同$a(n)$是确界矛盾。  
于是得出$a(n+1)=\frac{a(n)}{1-a(n)+a(n-1)}, a(n+1)-a(n)=a(n)\times\frac{a(n)-a(n-1)}{1-a(n)+a(n-1)}\gt a(n)(a(n)-a(n-1))$  
容易看出a(n)单调增，得出$a(n+1)-a(n)$越来越大，很快就可以大于$1$，但是这个是不可能的。由此得出只能$a(n)=a(n+1)$,然后可以得出它们都是0，也就是wayne必胜。  
于是在必胜的条件下用类似方法可以推导出前面的递推数列。  

#存在临界点
mathe后来发现前面推理中有错误，也就是wayne不是必胜的。根据初步分析中的递推式，给定$a_0=0,a_1$为任意正数，然后
$a_{n+1}=\frac{a_n}{1-a_n+a_{n-1}}$,如果递推到某一项出现负数，那么wayne是可以100%取胜。但是如果可以一直保持正数，那么Fans可以一直将wayne阻挡在山顶以外，
唯一的例外是存在一个体力值比值的临界点值，这时，wayne可以以100%的概率登顶，但是平均需要的步长是无穷步。  
后来mathe经过计算发现$\frac13$就正好是这个临界点，所以在初始体力比大于$\frac13$时, wayne必然赢，但是小于$\frac13$时，Fans可以已知将wayne阻挡在山顶之外。  
但是Fans给出的条件很有意思，正好在临界点，导致wayne必然会赢，但是必然会精疲力尽，Fans可以让wayne花费任意多步数后才能够赢。  

#数学推导
mathe定义$f_0(x)=0,f_1(x)=x$,$f_{n+1}(x)=\frac{f_n(x)}{1-f_n(x)+f_{n-1}(x)}$  
记$g_n(x)=f_n(x)-f_{n-1}(x)$,于是$g_{n+1}(x)=\frac{f_n(x)g_n(x)}{1-g_n(x)}, f_{n+1}(x)=f_n(x)+g_{n+1}(x)$  
于是可以看出,在f,g连续的部分，必然有g和f都严格单调增(注意$\frac{x}{1-x}$是增函数)  
然后我们计算可以知道$f_n(\frac13)=\frac{n}{n+2}$,由此可以知道$\lim_{n\to+\infty}f_n(\frac 13)=1$  
反之，对于任意$e\gt\frac13$,由于$f_n(e)=g_1(e)+g_2(e)+...+g_n(e)\ge(e-\frac13)+g_1(\frac13)+g_2(\frac13)+\dots+g_n(\frac13)$  
所以我们得出$\lim_{n\to+\infty}f_n(e)\ge(e-\frac13)+\lim_{n\to+\infty}f_n(\frac13)=1+(e-\frac13)\gt 1$  
也就是$\frac13$是题目中提到的界。  

#结论分析
KeyTo9\_Fans最后做出总结  
我们得到以下结论：  
设wayne的初始体力是$x$，  
当$x\in(1,+\infty)$时，只需要$1$步登顶；  
当$x\in(\frac23,1)$时，需要$3$步登顶；  
当$x\in(\frac59,\frac2/3)$时，需要$5$步登顶；  
当$x\in(\frac12,\frac59)$时，需要$7$步登顶；  
当$x\in(\frac7{15},\frac12)$时，需要$9$步登顶；  
当$x\in(\frac49,\frac7{15})$时，需要$11$步登顶；  
当$x\in(\frac37,\frac49)$时，需要$13$步登顶；  
当$x\in(\frac5{12},\frac37)$时，需要$15$步登顶；  
当$x\in(\frac{11}{27},\frac5{12})$时，需要$17$步登顶；  
当$x\in(\frac25,\frac{11}{27})$时，需要$19$步登顶；  
当$x\in(\frac{13}{33},\frac{2}{5})$时，需要$21$步登顶；  
当$x\in(\frac{7}{18},\frac{13}{33})$时，需要$23$步登顶；  
……  
总之，$N$步之内登顶所需的最小初始体力可以无限接近$\frac13$，但达不到$\frac13$。  
当初始体力为$\frac13$时，能登顶的概率是$1$，但所需步数的期望值是$+\infty$。  
当初始体力小于$\frac13$时，能登顶的概率是$0$。  
所以，在这个游戏中，wayne能登顶的概率要么是$0$，要么是$1$，  
不存在某个初始体力使得wayne能登顶的概率介于$0$到$1$之间，除非上帝不公正。  




