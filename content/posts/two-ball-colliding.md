---
title: 弹性碰撞与圆周率的计算
categories: 数学
keywords: 'bubbles'
thumbnail: "../thumbnails/c44.png"

tags:
  - bubbles
date: 2019-10-23 10:51:34
---

在理想模型下(没有任何阻力，弹性碰撞)，将一个静止小方块放在在墙壁和大方块之间，
让大方块以一定的初始速度撞向小方块，那么小方块总共会碰撞几次呢？
简单计算可以知道，当

大方块质量为小方块一倍时，经过第一次碰撞加速后，小方块会在和墙壁来一次亲密接触后
反弹并再次和大方块发生一次碰撞，所以小方块一共碰撞了3次。

那么当大方块质量为小方块的100时呢？这时由于质量的巨大差别，小方块需要在墙壁和大方块之间
往返多次才能大方块掉头远离。经过逐步计算可以知道这次小方块需要碰撞31次。

那如果质量比变为10000倍呢？计算得知，倍感压力的小方块现在需要碰撞314次了。

...

发现规律了？

没错，神奇的$\pi$强势出现在次碰撞试验中了。

查看视频可以有详细解答。
-----------------------------  

<iframe height=500 width=100% src="//player.bilibili.com/player.html?aid=61421110&cid=106845214&page=1" scrolling="no" border="0" frameborder="no" framespacing="0" allowfullscreen="true"> </iframe>

-----------------------------  
这里提供一个计算方法:
根据动量守恒,能量守恒,不难得到:  
$$\frac{1}{2}m_1v_1^2+\frac{1}{2}m_2v_2^2=\frac{1}{2}m_1v_1'^2+\frac{1}{2}m_2v_2'^2, m_1v_1 + m_2v_2 = m_1v_1' + m_2v_2'$$

然后相减,相除相消可以得到$v_1+v_1'=v_2+v_2'$,代入继而得到  
$$v_1'=\frac{v_1(m_1-m_2)+2m_2v_2}{m_1+m_2},v_2'=\frac{v_2(m_2-m_1)+2m_1v_1}{m_1+m_2}$$  
观察发现这是一次线性表达, 为了方便计算的迭代,我们总要转换成矩阵表达  
$A=(
\begin{array}{cc}
 \frac{m_1-m_2}{m_1+m_2} & \frac{2 m_2}{m_1+m_2} \\
 \frac{2 m_1}{m_1+m_2} & \frac{m_2-m_1}{m_1+m_2} \\
\end{array}
), 
(
\begin{array}{c}
 v_1' \\
 v_2' \\
\end{array}
)=A (
\begin{array}{c}
 v_1 \\
 v_2 \\
\end{array}
)
$  
小方块碰撞墙壁后速度大小不变,方向取反,也就是上面的矩阵第一行整体取相反数.同时为了尽可能简化这个矩阵,我们设$m_2=\cot ^2(\theta )m_1$,代入化简得到:
$$
B = \left(
\begin{array}{cc}
 -1 & 0 \\
 0 & 1 \\
\end{array}
\right)\cdot A= \left(
\begin{array}{cc}
 \cos (2 \theta ) & -2 \cos ^2(\theta ) \\
 2 \sin ^2(\theta ) & \cos (2 \theta ) \\
\end{array}
\right)
$$
n轮碰撞后,
$$
B^n = \left(
\begin{array}{cc}
 \cos (2n\theta) & -\cot (\theta)\sin(2n\theta) \\
 \tan(\theta) \sin(2n\theta) & \cos(2n\theta) \\
\end{array}
\right)
$$
经过n次方块之间的碰撞+墙壁反弹之后,
$$
(
\begin{array}{c}
 v_1^{Final} \\
 v_2^{Final} \\
\end{array}
)=B^n (
\begin{array}{c}
 v_1^{initial} \\
 v_2^{initial} \\
\end{array}
)=B^n (
\begin{array}{c}
 0 \\
 -1 \\
\end{array}
)
$$
化简得到$v_2^{Final}-v_1^{Final} = -\csc(\theta)\sin(\theta +2n\theta) >0$,
继而得到碰撞次数$N = 2n+1,  \frac{\pi}{\theta}<N<\frac{2\pi}{\theta}$[取首次速度反转的结果], 又由于$\theta\approx \tan(\theta)$ ,所以 $N \theta \approx \pi$ , 得证.