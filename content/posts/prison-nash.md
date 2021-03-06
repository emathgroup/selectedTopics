---
title: 三方博弈之囚徒困境
categories: 博弈
keywords: 'game'
thumbnail: "../thumbnails/prison.jpg"

tags:
  - game
date: 2020-01-04
---

# 摘要
2017年10月KeyTo9\_Fans先后在[知乎](https://www.zhihu.com/question/67018671) 和[数学研发论坛](https://bbs.emath.ac.cn/thread-9685-1-1.html) 提问:  
3人聚餐后，想玩一个游戏来决定谁买单，这个这个游戏的规则公平吗？  
游戏规则描述：  
A、B、C 3人约定同时伸出手，每个人都只有手心朝上或者手背朝上2种选择，然后统计手心朝上的人数：  
i)   0人或3人则A买单  
ii)  1人则B买单  
iii) 2人则C买单。  
在任何2人都没有互相串通的前提下，这个游戏公平吗？如果不公平，哪方占优哪方劣势？  
如果有部分人串通，又如何呢？  

# 问题初探
[kastin认为](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=9685&pid=67503&fromuid=20) :  
A买单的概率为 $\frac 28$，B买单的概率为 $\frac 38$，C买单的概率为 $\frac 38$，所以A比较占优势。  
Fans指出，这只是三方都随机乱出时的结果，但是他们的最优策略不一定是随机乱出。

# 纳什均衡
几乎在同一时间，知乎上的司马懿等人和数学研发论坛的[mathe](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=9685&pid=67506&fromuid=20) 找出了这个问题的纳什均衡解:  
wayne首先列出了三人的胜率公式:  
假设A，B，C三人各自打算出手心向上的概率分别是x,y,z. 则各自买单的概率是x,y,z的函数,  
$P_A(x,y,z) = (1-x)(1-y)(1-z) +xyz$  
$P_B(x,y,z) = (1-y)(1-z)x + (1-x)(1-z)y + (1-x)(1-y)z$  
$P_C(x,y,z) = (1-z)xy + (1-y)xz + (1-x)yz$  
利用这个公式，mathe做了如下分析:  
对于A来说，由于手心手背是对称的，所以在没有和其他人预谋的情况下，手心手背都一样，正常情况他就会以各50%的概率随机出。  
在此前提下，概率公式转变为  
$P_A(y,z) = \frac12(1-y)(1-z) +\frac12yz$  
$P_B(y,z) = \frac12(1-z) + \frac12(1-y)z$  
$P_C(y,z) = \frac12y + \frac12(1-y)z $  
于是很有意思，对于B来说，对于C的任何策略，都应该y最大的时候$P_B$最小，于是他会选择y=1,得到$P_B=\frac12(1-z)$；同样对于C来说，无论B怎么选择方案，他都是选择z=0时有最小买单概率$P_C=\frac12y$,于是在双方互相不信任的前提下，B都选择手心向上(y=1),而C都选择手背向上(z=0),于是他们两败俱伤，各自需要有50%的概率买单，而A坐收渔翁之利。

如果在游戏需要重复进行很多次或者B,C相互之间充分信任（但是又无法进行预谋）的情况下，比较明智的B和C要选择一种双方买单概率相同但是让A买单概率最大的情况，也就是B和C都以50%的概率选择手心和手背，那么这时A虽然还是占有优势(只有$\frac14$概率需要买单),但是B和C合作以后可以将买单概率从$\frac12$降低到$\frac38$。

但是聪明的A可能不会满足$\frac14$的买单概率，在游戏需要重复很多次时，他为了挑拨B,C的关系，可以连续出手心状态，也就是改变策略为x=1。
而这种改变下A自己的买单概率还是保留为$\frac14$,而B惊喜的发现自己买单的概率也降低为$\frac14$。
可怜的C发现自己买单的概率上升为$\frac12$后很可能会火冒三丈从而恢复到只出手背的策略（最主要的是怀疑A,B串通）。
此后只要A调整回随机策略，那么B必然发现自己买单概率急升而恢复到只出手心的策略来对抗。  
出现这种A可以掌控全局的情况的主要原因在于B,C以50%概率选择手心或手背的策略是不稳定的。  

B,C为了能够获得最大利益，那么只要A不使用均匀概率，那么目标就应该让A最大概率买单，所以这时策略就是在A使用不均匀策略时，B和C必须一致的对抗A,但是这会导致B,C两者之间费用严重失衡，正常情况很难维护下去。  

# 囚徒困境
KeyTo9\_Fans[总结知乎上的回复](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=9685&pid=67524&fromuid=20) ，得出:  

A必需严格随机出，才可以保证败率不超过$\frac14$。否则A只要有一丁点不随机，就会被BC利用，导致败率大于$\frac12$。

当A严格随机出的时候，BC就遇到了[囚徒困境](https://baike.baidu.com/item/囚徒困境) 。由于不能串通，BC的败率之和必定大于等于$\frac34$。

当BC都采取各自的[占优策略](https://baike.baidu.com/item/占优策略) （即互相背叛）时，那么三方的策略就达到[纳什均衡](https://baike.baidu.com/item/纳什均衡) 了，这是此三人博弈里唯一的一个纳什均衡点。

但对BC来说，这一结果反而是最糟糕的：BC两败俱伤，败率各为$\frac12$，A坐收渔翁之利，败率为$0$。

所以BC会想方设法逃出这个囚徒困境。所幸的是，这个囚徒困境并不难逃脱。

这个囚徒困境与一般的囚徒困境最大的区别是：当一方坚持背叛的时候，另一方选择合作不会有任何损失。

因此当一方坚持背叛的时候，另一方为了逃出囚徒困境，会随机乱出或者使用完全相反的固定策略去搅局，反正怎么搅局都没有任何损失嘛。

于是很神奇的事情发生了：唯一的纳什均衡点竟然会被人搅局，变得不稳定了！

之所以会出现这样的矛盾，是因为纳什均衡理论是基于所有的参与者都是极端理性，完全没有感情用事，并且【所有的参与者都是极端理性】是公共知识（即：1:所有的参与者都是极端理性的。2:每个人都知道1。3:每个人都知道2。4:每个人都知道3，……）的前提下得出的。

然而，人是有感恩之心（如果你不对我使坏，那么我也不会对你使坏）和报复之心（如果你坚持对我使坏，那么我也会坚持对你使坏）的，

因此当【每个人都有感恩之心和报复之心】成为公共知识（即：1:每个人都有感恩之心和报复之心，2:每个人都知道1，3:每个人都知道2，4:每个人都知道3，……）的时候，BC的最佳策略就变成互相合作了。

（说白了就是BC的策略都是【如果你合作，那么我也合作；如果你背叛，那么我也背叛】并且互相知道，于是在这个前提下，【合作】就变成BC的最佳策略了）

结果BC的败率皆为$\frac38$，A的败率为$\frac14$。

这对BC来说，就是该三人博弈的最好结果了，而且A无法破坏BC的合作（前面说过，A只要有一丁点不随机，就会被BC利用，导致败率大于$\frac12$）。

# 串通情况分析
Fans[继续给出](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=9685&pid=67525&fromuid=20) ：  
前面的讨论都是基于【任何$2$人都没有互相串通】的前提下进行的。

如果把这个假设去掉，结果会怎样呢？

具体地说，是任何$2$个人都可以串通，且串通的规律无法被第$3$人识破（这也是一种平等嘛）。

例如，当A会背$e$和$\phi$，B会背$e$和$\pi$，C会背$\phi$和$\pi$，并且这是公共知识的时候，任何$2$个人都可以串通，且串通的规律无法被第$3$人识破。

一种可能的结果是，BC把$\pi$的二进制小数转化成手心手背，两个人就可以把把出相同的手，且A无法预测，结果A的败率被锁死在$\frac12$，然后由A来决定剩下的$\frac12$败率如何分配给B和C。

但这个结果似乎不稳定。

由于A是知道B和C是拿他不会背的$\pi$来欺负他的，A为了减小自己的败率，很可能会威胁BC的其中一方。

例如，A会对B说：你跟我合作，各自独立地以$\frac23$的概率出手心如何？这样就能把C的败率锁死在$\frac49$，由C来决定剩下的$\frac59$败率如何分配给我们。但不论C如何分配，我们的败率都必定介于$\frac19$到$\frac49$之间，不可能大于$\frac49$。但你要是坚持与C合作，我就会拉你下水，把剩余的$\frac12$败率全放你身上，让你死得比$\frac49$的败率更惨。

于是B由于受到了A的威胁，很可能就会与A合作，从而获得介于$\frac19$到$\frac49$之间的败率，而不是$\frac12$的败率。

但这个结果似乎仍然不稳定，C可能会把B拉回来说，要是A敢威胁你，让你的败率大于$\frac13$，我们就采取固定的策略对付A，让A的败率大于$\frac23$，我们的败率之和小于$\frac13$，看A还敢不敢拉你下水。如果你还是坚持与A合作，我就把你的败率锁死在$\frac49$，让你死得比$\frac13$的败率更惨。

于是B由于受到了C的威胁，很可能又跑回去与C合作，从而获得不超过$\frac13$的败率，而不是$\frac49$的败率。

但这个结果就稳定了吗？A还有没有别的招呢？

有什么串通策略即使公开叫板，第$3$个人也无可奈何呢？


最近似乎又想到了一个策略：签协议。

例如：$B$和$C$签订以下协议：


第1条、盯紧$A$：时刻关注$A$出手的熵，如果出手的熵小于$1$比特/次，则根据$A$所丢失的熵，把$A$的败率整成大于$\frac12$，否则遵守第$2$条。

第2条、自律：保证自己出手的熵为$1$比特/次。

第3条、毁约惩罚：如果发现对方不遵守第2条，则执行毁约预案, 回归纳什均衡策略。


这协议非常牛B，以至于$B$和$C$当着$A$的面，把协议内容给$A$看，然后双方签约，最后遵照执行，$A$也拿$B$和$C$没有办法。

协议生效后，$3$人的策略最终将趋于满熵出手，也就是完全随机。

结果$A$的败率为$\frac14$，$B$和$C$的败率为$\frac38$。

而且这一策略将变得非常稳定，$A$无可奈何只能坚持满熵出手，$B$和$C$害怕毁约惩罚，也不敢不坚持满熵出手。

（要注意，毁约惩罚的纳什均衡结果是，$A$的败率为$0$，$B$和$C$的败率为$\frac12$，$B$和$C$任何一方都不希望这样的结果。）

#####

那么，问题又来了：

$A$能否拟定一份新的协议，然后诱惑$B$和$C$的其中一方和他签约（比如：保证签约方的败率小于$\frac38$），以获得低于$\frac14$的败率呢？
