---
title: 关灯游戏和扫雷问题
categories: 算法 数学
keywords: 'mine lights'
thumbnail: "../images/lights.jpg"

tags:
  - mine
  - 扫雷
  - 关灯
date: 2019-11-27
---

# 关灯问题
2005年1月[lkrich7在csdn询问](https://bbs.csdn.net/topics/70048438)  
有一个$5\times 6$的灯泡构成的矩阵，灯的开关规则是这样：当改变某盏灯的，状态时，这盏灯的上下左右相邻的灯的状态也随之改变。例如：  
0 1 1 0 1 0  
1 0 0 1 1 1  
0 0 1 0 0 1  
1 0 0 1 0 1  
0 1 1 1 0 0  

当按下2行3列的开关时，状态变为：  
0 1 0 0 1 0  
1 1 1 0 1 1  
0 0 0 0 0 1  
1 0 0 1 0 1  
0 1 1 1 0 0  

游戏的目的是对于任意给定的亮灭初始态，通过一系列动作关闭所有的灯。  
可以注意到的是：  
1.矩阵的状态与按开关的顺序无关  
2.如果某个开关按下了两次，那么就相当于取消了第一次的操作，也就是说没有开关需要按超过1次  

现在问题是：对于给定的初始状态，求出需要按哪些开关来完成游戏.  

# 扫雷算法问题
2007年10月[pavelalex在csdn提问](https://bbs.csdn.net/topics/190068993)  
已知把雷区的雷布好了，每个格子数字都填好（可见），怎么通过这些数字把雷找出来?  
zgg\_\_提问: 那么地雷上填什么数字呢？  
pavelalex要求：数字是它周围雷的数目。  

# 二维递推数列求解
2007年11月[goodenfeet在csdn提问](https://bbs.csdn.net/topics/190086722)  
$f(x,y)=f(x-1,y)+f(x+1,y)+f(x,y-1)+f(x,y+1)$，求$f(x,y)$  
其中，
$f(x,0)=0,f(500,y)=50,f(x,500)=75,f(0,y)=100$.
然后，
$0\le x,y\le 500,x,y$是正整数。
要求是，
输入$x,y$，返回$f(x,y)$。


# 算法分析
上面三个看似完全独立的问题，在数学上却密切相关。  
## 关灯问题最初分析
mathe通过把[关灯问题看成有限域$F_2$上的线性变换](https://blog.csdn.net/mathe/article/details/1143634) 来提出初步有效的解决方案：  
把上面的矩阵看成有限域$F_2$上一个$m\times n$的向量$X=(x_1,x_2,\dots,x_{m\times n})$  
对于位置k上的开关，它将变化最多5个位置的开关，对应一个向量
$C_k=(0,0,\dots,1,0,\dots,1,\dots,0)$, 其中开关状态改变的位置为1，开关状态不改变的位置为0。  
对于初始向量$X=(x_1,x_2,\dots,x_{m\times n})$,使用了开关$C_k$后，状态会变成 $X+C_k (\mod 2)$, 也就是在$F_2$上做一次加法运算。  
所以对初始向量X,我们需要选择一系列的$k_1,k_2,\dots,k_s$使得
$X+C_{k_1}+C_{k_2}+\dots+C_{k_s} (\mod 2)=O=(0,0,0,\dots,0)$。  
我们可以同样构造一个0,1向量Y,使得，如果位置k出现在$k_1,k_2,\dots,k_s$中，那么Y
在位置k的值是1，不然是0，这样，我们就可以将上面公式写成$F_2$上的矩阵形式$X+YC (\mod 2)=O$，
其中$C=(C_1^{\prime} C_2^{\prime} \dots C_{m\times n}^{\prime})^{\prime}$。  
也就是C是由这$m\times n$个行向量构成的矩阵，第k行就是向量$C_k$。  
在二阶域上，加和减是相同的，也就是上面的方程等价于$YC (\mod 2)=X$，其中C,X已知，求Y。  
我们可以直接在二阶域上用高斯消元法求解（注意加减是$\mod 2$的，对应计算机上的异或运算）,  
其中，如果C可逆，解是唯一的，如果C不可逆，解可能不存在，也可能不唯一。
如果解不唯一时，这时解的数目同C的秩有关系，比较有意思的是，如果我们记$f(n,2x)$是n次第二类切皮雪夫多项式，
那么$gcd(f(m,x)(\mod 2),f(n,1-x)(\mod 2))$的次数就是C的自由度（也就是矩阵的阶数减去秩）。

而这里C是一个分块三对角阵，我们可以使用追逐法求解,时间复杂度可以只有$O(n^3)$。

mathe提供了[一个C程序](../attached/minelights/lights.txt) , 时间复杂度为$O(n^4)$,这是因为其中稀疏矩阵的乘法没有优化。

## 必然有方案关闭所有灯
在网络上,mathe还找到一个俄国人的一个证明:如果开始所有的灯都是开的,那么必然存在一种方案将所有的灯关闭.这个证明用到线性代数知识:

Puzzle 2: for this problem, we are giving up the detailed information about how the lights are connected. We are left knowing only that the connections are reflexive and symmetric, and that our task is to go from all lights ON to all lights OFF. Lossers gives a short explanation of how to see that this is so. First, we define the obvious matrix A with 1's indicating which switches control which lights. The matrix is symmetric and has 1's on its diagonal.

Now it's equivalent to show that there is some strategy x of button pushing that can go from all lights OFF to all lights ON. But that's just asking if there is a vector x such that

$A x = d$,  
where d is the vector of all 1's. That is equivalent to asking if d is in the column space of A. And that is equivalent to asking if the "perpendicular space" of A' is contained in the "perpendicular space" of d. In other words, it is enough to show that $A'x=0$ implies $d'x=0$.

So let x be any vector in the perpendicular space of A'. Then

$\sum_{i=1}^N x_i  A_{i,j} = 0 $ for all j. This implies $\sum_{j=1}^N \sum_{i=1}^N x_i A_{i,j} x_j = 0$,

Now we know that A is symmetric (assumption 2). Therefore, in the preceding sum, let us consider, for distinct i and j every pair of terms

$x_i A_{i,j} x_j + x_j A_{j,i} x_i = ( A_{i,j} + A_{j,i} ) x_i x_j = 2 A_{i,j} x_i x_j = 0$  
because we are working in arithmetic module 2! Therefore, all the offdiagonal terms pair up and drop out.

Our sum now only involves the diagonal terms:

$\sum_{i=1}^N x_i^2 A_{i,i} = 0$  
Now $A_{i,i} = 1$, because the light-switch relationship is reflexive (assumption 1), and $d_i = 1$ too, so in the sum, we can replace $A_{i,i} by $d_i$. Moreover, we know that $x_i^2=x_i$, because $x_i$ is either 0 or 1. Therefore, we now have
$\sum_{i=1}^N x_i d_i = 0$.

Hence x is in the perpendicular space of d. This means d is in the column space of A, so a button strategy exists that turns a set of "off" lights all "on", and the same strategy will turn a set of "on" lights all "off".

## 扫雷的进一步分析
对于$m\times n$阶雷区  
我们记I为n阶单位矩阵  
B为一个n阶矩阵,所有同主对角线相邻的位置是1(不包含主对角线),其余位置都是0,比如对于n=4,B如下  
0100  
1010  
0101  
0010  
那么,原题目写成矩阵形式,所有方程系数构成如下$m\times n$阶矩阵  
B   I+B 0  0 ... 0  
I+B  B I+B 0 ... 0  
0   I+B B I+B ...0     
..................  
...........0 I+B B  
当然如果这个方阵可逆,那么问题的解唯一.  
记$f(n,x)$为$n$阶[第二类切皮雪夫多项式](http://mathworld.wolfram.com/ChebyshevPolynomialoftheSecondKind.html)  
$f(0,x)=1$,    
$f(1,x)=2x$  
$f(2,x)=4x^2 - 1$  
另多项式:  
$g(a,b)=f(m,\frac a{2b})b^m$  
那么上面矩阵同矩阵$g(B,I+B)$可逆性相同.  
也就是我们只要判断$g(B,I+B)$是否可逆就可以判断问题的解是否唯一了.  
如果修改题目将周围理解成只有上下左右,那么上面分析中的$I+B$替换成I就可以了.  
对于$m=1, g(a,b)=a$, 所以$g(B,I+B)=B=0$ 不可逆.实际上$1\times 1$的情况解就是不唯一的.  
对于$m=2, g(a,b)=a^2-b^2, g(B,I+B)=-I-2B$,通过切皮雪夫多项式的行列式形式可以知道  
&nbsp;&nbsp;&nbsp;&nbsp;$\det(g(B,I+B))=(-2)^n f(n,\frac 14)$,  
通过切皮雪夫多项式的三角函数形式可以知道上面值非0.  
所以对于两行或两列的情况解都唯一.  
对于$m=3, g(a,b)=a^3-2ab^2, g(B,I+B)=B^3-2B(I+B)^2=-2B-4B^2-B^3$  
  这个就比较难分析了.但是我们知道 $B|g(B,I+B)$  
但是$det(B)=f(n,0)$在n为奇数的时候是0.  
实际上,对于m和n都是奇数的情况,总是有$B|g(B,I+B)$,而且$det(B)=f(n,0)=0$,所以$\det(g(B,I+B))=0$.  
也就是说,我们证明了在$m,n$都是奇数的情况,上面方程不可逆,解可能不唯一(但是没有证明解必然不唯一,毕竟挖雷中要求所有数字都是0,1,这个解方程是无法限制的).
但是对于m,n至少有一个数是偶数时,我们还没有很好的结果.  

我们再考虑n+1不被3整除的情况:  
由于$\det(I+B)=f(n,\frac12)$,根据切皮雪夫多项式的三角函数形式可以知道这个值只有再$3|n+1$时为$0$,
所以$n+1$不被3整除时,$I+B$可逆,这时$\det(g(B,I+B))=\det((I+B)^m)\det(f(m, \frac12B(I+B)^{-1}))$.  
上面第一项非0,而对于第二项,如果对于$B$的某个特征值$u$,  
&nbsp;&nbsp;&nbsp;&nbsp;$f(m,\frac12\frac u{1+u})=0$,那么它为$0$,不然必然非$0$.  
根据切皮雪夫多项式的三角函数:设$x=\cos(t)$,那么$f(n,x)=\frac{\sin((n+1)t)}{\sin(t)}$, 
得到使$f(n,x)=0$的$x$为$\cos(\frac{k\pi}{n+1})$, 
所以$B$的所有特征值是$2\cos(\frac{k\pi}{n+1})$。  
所以只有对于存在$k_1,k_2$使得$\frac{2\cos(\frac{k_1\pi}{n+1})}{1+2\cos(\frac{k_1\pi}{n+1})} = 2\cos(\frac{k_2\pi}{m+1})$的情况下,上面第二项才为$0$.  
分析对于什么样的$m,n$上面式子成立也是挺难的,不过计算机验算很简单.  

上面的分析过程给出了一个这道题目在解唯一时比较有效的解法, 
就是解方程,方程对应矩阵是  
B       I+B   0     0   ...   0  
I+B     B   I+B   0   ...   0  
0       I+B   B   I+B   ...0  
.....................  
.................0   I+B   B  
这样的方程可以通过追赶法计算,时间复杂度为$O((n+m)^4)$.

另外还有$n+1$被3整除的情况,我们判断$\det(g(B,I+B))$是否等于0,
只要判断对于$B$的所有特征值$u,g(u,1+u)$是否都不是$0$.  
对于$1+u=0$(也就是特征值$-1$),结果就是$f(m,-1)$展开的第一项,当然不是0.  
对于$1+u\ne 0,g(u,1+u)=f(m,\frac u{2(1+u)})$,所以我们实际上还是可以用判断式$\frac{2\cos(\frac{k_1\pi}{n+1})}{1+2\cos(\frac{k_1\pi}{n+1})}=2\cos(\frac{k_2\pi}{m+1})$来判断.

所以判断方程中矩阵是否可逆,我们可以用如下方法.  
i)如果m,n都是奇数,不可逆.  
ii)不然,对于$x=\cos(\frac{k\pi}{n+1}), (k=1,2,\dots,n)$  
&nbsp;&nbsp;&nbsp;&nbsp;计算$\sin((m+1)\cos^{-1}(\frac x{1+2x}))$,如果这个值为$0$,矩阵不可逆; 不然,矩阵可逆.  
当然第二步只能近似计算.

还有一个有意思的结论:  
如果$\det(g(B,I+B))$是奇数, 那么我们只要知道每个格子周围的雷的总数的奇偶性就可以唯一确定结果了.  
比如对于上面列出的$4\times 5$的例子,可以算出$\det(g(B,I+B))$是奇数 (模2计算就可以了), 
所以对于这个例子,我们只要知道雷数的奇偶性就可以唯一确定结果了.  
只知道雷的总数的奇偶性的问题关灯游戏问题就几乎一模一样了,除了每次关灯影响的灯的数目不同.  
而上面解法的思想也来源于关灯游戏的解法。 只是对于关灯游戏,所有计算都是模2运算,计算更加方便.  

## 二维递推数列的求解
关于这个递推数列，mathe首先想到，记$a=f(x,0)=0, b= f(0,y)=100, c=f(x,500)=75, d= f(500,y)=50$, 
定义,B和I都是$499\times 499$阶方阵，其中空格位置为0, G是一个$249001\times 249001$阶方阵($249001=499\times 499$)，写成分块矩阵形式.空格位置也都是0
那么这道题目写成方程形式就是解方程$Gx=r$, 这道题目可以用追赶法求解，复杂度为$O(n^4)$,这里$n=499$,基本上是可行的。

随后mathe发现:  
对于$n\times n$的问题（在这个题目里面$n=499$)  
上面问题存在唯一解的充分必要条件是  
对于任意的整数$k_1,k_2 0\lt k_1\lt k_2\lt n+1$, 有  
$\cos(\frac{k_1\pi}{n+1})-\cos(\frac{k_2\pi}{n+1})\ne \frac12$.  
而对于这个题目，由于$k_1=300,k_2=400时会不满足上面条件，也就是
$\cos(\frac35\pi)-\cos(\frac45\pi)=\frac12$, 所以方程不一定有解，而且即使有解的时候解也不会唯一。  
随后，mathe给出了[改自关灯问题的代码](../attached/minelights/sequence.txt) ,连里面的注释都还是关灯问题的。  

此后mathe发现，这个题目可以用$O(n^2\log(n))$的算法达到，  
而其中离散正弦变换竟然可以得到应用。也许，这个题目一开始就应该用傅立叶变换来分析。  
还是采用前面的记号，分别用$a,b,c,d$代替四条边界上的取值（其实每条边界上不需要要求所有点的初始值相同）。  
首先我们可以发现，这个题目满足可加性。  
我们可以先计算出$(a,b,c,d)=(a,0,0,0)$的解，然后计算$(a,b,c,d)=(0,b,0,0)$的解，然后再计算另外两个只有一条边界为0的解。算出来后，将4个解对应位置相加，就是所求的解了。所以我们可以先只分析$(0,0,0,d)$这组情况。  
记函数$f(1,x)=1,f(2,x)=x, f(n+1,x)=x f(n,x)-f(n-1,x)$,  
那么$f(n,2x)$就是第二类切皮雪夫多项式.  
记矩阵$T$为一个$n$阶对称阵，其中,  
$T_{i,j}=\sqrt(\frac2n)\sin(\frac{ij\pi}{n+1}),  (1\le i\le n,1\le j\le n)$.  
可以知道$T$是正交阵, 而且对于任意一个长度为$n$的向量$X$, $TX$其实就是$X$的离散正弦变换，而计算离散正弦变换可以有$O(n\log(n))$的快速算法。  

现在回到题目,$n=499$.  
由于$a=b=c=0$,所以题目中$r_1,r_2,\dots,r_{n-1}$都是$0$，只有$r_n=(-d,\dots,-d)$不等于$0$. (其实对于这个边界条件，可以直接求解不同的位置不同的情况，只要修改$r_n$的值就可以了）.  
所以得到上面递推式中$u_1=u_2=\dots=u_n=0, u_{n+1}=r_n$,而$V_{n+1}=f(n,-B)$,所以我们得到  
$f(n,-B)x_1=-r_n$  
而  
$TBT=\diag{2\cos(1\times\frac{\pi}{n+1})-1,2\cos(2\times\frac{\pi}{n+1})-1,\dots,2\cos(n\times\frac{\pi}{n+1})-1$  
所以, 如果假设$Tx_k=y_k, Tr_n=-R_n$,  
那么我们得到  
$f(n,(-2\cos(\frac{h\pi}{n+1})+1)y_1(h)=-R_n(h)$,  
其中$y_1(h)$和$R_n(h)$分别表示这两个向量的第h项。  
所以我们可以  
i)首先计算$r_n$的离散正弦变换$R_n$. (如果所有d相同，我们可以发现，$R_n$的所有奇数项都是0）  
ii)计算$f(n, -2\cos(\frac{h\pi}{n+1})+1), (h=1,2,\dots,n)$  
&nbsp;&nbsp;&nbsp;&nbsp;这个可以采用$f(n,.)$的迭代式计算，计算每个时间复杂度为$O(n)$，总共$O(n^2)$  
iii)解出$y_1$,如果对于某个$h$,对应$f(n,-2\cos(\frac{h\pi}{n+1})+1)=0$, 而$R_h\ne 0$,那么方程无解。  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;而如果对于$f(n,-2\cos(\frac{h\pi}{n+1})+1)=0$的$h$都有，$R_h=0$,那么方程有无穷个解，这时$y_1(h)$可以任意取.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;而对于$f(n,-2\cos(\frac{h\pi}{n+1})+1)=0$的情况，不论那种情况，取$y_1(h)=0$都是$x_1$的最小二乘解。  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;而在我们的题目中，$d$这条边界对应所有数据相同，所以对于$h$是偶数的情况$R_n$才为$0$.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;而对于$n=499$,我们发现$h=100,200,300,400$的情况$f(n,-2\cos(\frac{h\pi}{n+1})+1)=0$. 所以我们知道，对于这个题目，有无穷的解，但是我们可以取$y_1(100)=y_1(200)=y_1(300)=y_1(400)=0$,求出最小二乘解。  
iv)通过再次离散正弦变换（逆变换)计算出$x_1$  
上面部分计算$x_1$总共花费了$O(n^2)$的时间。  
此后，我们就可以通过方程  
&nbsp;&nbsp;$x_k=f(k-1,-B)x_1$来计算$x_k$  
同样做离散正弦变换，上面等式变成  
&nbsp;&nbsp;$y_k(h)=f(k-1,-2\cos(\frac{h\pi}{n+1})+1)y_1(h)$  
而其中$f(k-1,-2\cos(\frac{h\pi}{n+1})+1$在ii)中已经产生，直接使用就可以解出$y_k$.  
所以最后我们只要在通过$n-1$次离散正弦变换(逆变换）可以计算出$x_2,x_3,\dots,x_n$.  
这一步需要花费$O(n\times n\log(n))$的时间,
所以总共时间复杂度$O(n^2\log(n))$.  
点击可以[下载对应的C代码](../attached/minelights/sequence2.txt) .

## 扫雷问题深入分析
通过上面二维递推数列的分析，可以知道，扫雷问题也可以用$O(nm\log(n+m))$的算法达到。  
对于雷区最终和数据矩阵假设为a(i,j)，  
而每个格子用x(i,j)表示，0表示无雷，1表示有雷，  
分别用A和X表示a和x的二维离散正弦变换(也就是先各行做离散正弦变换，再各列做变换）。  
那么可以有：  
$((2\cos(\frac{i\pi}{n+1})+1)(2\cos(\frac{j\pi}{m+1})+1)-1)X(i,j)=A(i,j)$.  
如果左边系数总不是0，马上就可以解出$X(i,j)$,
不然，必然要求对应的$A(i,j)$也是0，这时，对应$X(i,j)$可以取任意值，这是一个可变参数。  
根据前面的一个分析，我们知道在$m,n$不超过500的范围内，最多只有$m=n$都是奇数的情况有可能有一项系数是0,  
也就是说，这个可变参数最多只有一个。  
所以我们得到了x(i,j)的二维离散正弦变换，其中最多一个可变参数。  
然后我们对X做逆变换，变换结果可以写成:  
$u(i,j)+t\times v(i,j)$,  
其中t就是那个可变参数.  
而且矩阵v很特殊，它每个元素都不是0。  
由于$u(i,j)+t\times v(i,j)$总是0或1,  
我们只要任意选一个位置,分别让这个位置取0或1就可以得到一个t的值，  
然后带入求出其他位置的值，如果对应值不是0和1，说明是非法解。  
这个也正好检验了前面zgg\_\_提到的只需要蒙一次的过程，上面分析说明对于任何局面，我们最多需要蒙一次。  
而最终的解也可以证明是唯一的。如果不唯一，那么说明存在两个不同的$t: t_1,t_2$使得  
$u(i,j)+t_1\times v(i,j)$和$u(i,j)+t_2\times v(i,j)$都是0或1,  
也就是$(t_1-t_2)v(i,j)$总是$0,-1,1$.  
由于$v(i,j)$总是非$0$，$t_1\ne t_2$,所以$(t_1-t_2)v(i,j)$都只能是$-1,1$, 
也就是说，$|v(i,j)|$必须是常数.  
而矩阵$v$的值是有解析形式的，只有$m$和$n$都不超过$2$的时候，才会有$|v(i,j)|$是常数,
而我们前面说过只有$m=n$而且是奇数时才出现待定系数。所以总共我们就得出$m=n=1$的这种特殊情况解不唯一（实际上这种情况的确解不唯一).  

发现前面我写的判断等式  
$\frac{2\cos(\frac{k_1\pi}{n+1})}{1+2\cos(\frac{k_1\pi}{n+1})}=2\cos(\frac{k_2\pi}{m+1})$  
无解的代码写错了，实际上，对于很多$m,n$都是有解的。  
比如4×4，4×9，4×19等都有解。
但是500以内，最多都只有3组解。所以在500以内，最差情况要穷举8种情况。（试验3次）  
那个额外的解就是  
$(2\cos(\frac{\pi}5)+1)(2\cos(\frac{3\pi}5)+1)=1$.  

有一道关于正三十边形的对角线可将其为多少部分的题目，  
关于这个题目，berkeley大学的Bjorn Poonen给出了一个公式，在其这篇文章中，他分析了所有形如  
$u_1+u_2+\dots+u_k=0$（其中$u_1,u_2,\dots,u_k$都是$1$的单位根，也就是有某个整数$h_i$使得$u_i^{h_i}=1$)的解。  
通过他的文章的结论，得知所有最多8项的这样的表达式中单位根的幂总是7,5,3,2或它们的乘积,  
而我上面的等式可以转化为长度为8的这样的和。  
由于我已经搜索了500以内的范围，所以不会有其他解了。  
也就是所有可能为0的系数已经全部得出。  

所以最后的结论是,  
一般情况只有唯一解,  
如果m和n都是奇数，那么,  
$((2\cos(\frac{i\pi}{n+1})+1)(2\cos(\frac{j\pi}{m+1})+1)-1)$中会有一项为$0$, 其中$(i=\frac{n+1}2, j=\frac{m+1}2)$.  
如果$m+1$和$n+1$都是$5$的倍数，那么还有另外两项是$0$,  
  $(i=\frac{m+1}5, j=\frac{3(n+1)}5$ 或 $i=\frac{3(m+1)}5, j=\frac{n+1}5)$,  
最多有3个自由参数，也就是最多枚举8种情况就可以了。

点击[下载对应C代码](../attached/minelights/mine.txt) 。

