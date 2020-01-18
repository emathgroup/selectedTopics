---
title: 圆内接四边形的三条对角线
categories: 数学
keywords: 'circle, diagonal'
thumbnail: "../images/threediagonals.png"

tags:
  - diagonal
date: 2020-01-17
---

# 问题溯源
wayne的微信群提出了[一个恒等式问题](https://bbs.emath.ac.cn/thread-16974-1-1.html) :  
设$a,b,c,d,x,y$为正实数，且满足$xy=ac+bd, \frac xy=\frac{ad+bc}{ab+cd}$  
求证：$\frac{abx}{a+b+x}+\frac{cdx}{c+d+x}=\frac{ady}{a+d+y}+\frac{bcy}{b+c+y}$。  
比较有意思的是这个恒等式可以转为为一个和圆内接四边形的对角线相关联的几何问题。

# 几何意义

mathe提议引入参数z并且设 $xz=ad+bc, yz=ab+cd$, “于是考虑边长为$a,b,c,d$的圆内接四边形，边顺序的不同安排得出对角线长度正好分别为$x,y;y,z;z,x$. 这时右边等式两边是不是有什么特殊的几何意义呢？”
通过利用托勒密定理：圆内接四边形对边乘积之和等于对角线乘积之和， 我们可以将[恒等式中变量几何化](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=16974&pid=81547&fromuid=20) 。  
![threediagonals](../images/threediagonals.png)  
另外为了分析表达式$\frac{abx}{a+b+x}$的几何意义，可以查看图中a,b,x三边组成三角形。根据正弦定理我们可以知道$abx$代表这个三角形面积和4R的乘积，所以$\frac{abx}{a+b+x}=\frac{4SR}{a+b+x}=2Rr$,其中R和r分别是三角形外接圆和内切圆半径长度。由此我们得出这个等式的几何意义代表[圆的内接四边形两条对角线各自将四边形分成的两个三角形的内接圆半径之和相等](https://zhidao.baidu.com/question/179520048.html)。 
利用双心三角形的关系式$R^2-2Rr=d^2$还可以得出两内心到外心距离平方和也和对角线的选择无关。


# 利用对称性证明等式
hujunhua按照前面的思路，进一步把原题[改造为对称形式](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=16974&pid=81544&fromuid=20)  
已知：正实数$a,b,c,d,x,y,z$  满足 $xy=ac+bd,yz=ab+cd, xz=ad+bc$.  
求证：$\frac{abx}{a+b+x}+\frac{cdx}{c+d+x}=\frac{ady}{a+d+y}+\frac{bcy}{b+c+y}=\frac{acz}{a+c+z}+\frac{bdz}{b+d+z}$ 
然后给出了简洁的[代数证明](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=16974&pid=81545&fromuid=20) :  
由改造后的题设三等式两两相加可得$\begin{cases}x(y+z)&&=(a+b)(c+d),\\y(z+x)&&=(a+d)(b+c),\\z(x+y)&&=(a+c)(b+d).\end{cases}$  
由此立即可以看出直接通分的美好前景, 比如连等式的左式通分后易化简为  
$\frac{abc+bcd+cda+dab+xyz}{a+b+c+d+x+y+z}$  
结果是$a,b,c,d;x,y,z$的全对称式，连等式当然成立!

# 派生的几何结论
这个方法还可以非常巧妙地顺便给出了圆的内接四边形的[对角线长度公式](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=16974&pid=81549&fromuid=20):  
需要注意对于任意给定的正数a,b,c,d,我们可以求得唯一的一组正数x,y,z满足条件。
因为三式相乘开平方得到$xyz=\sqrt{(ac+bd)(ad+bc)(ab+cd)}$，然后分别除以三式可以唯一确定x,y,z。  
[dingjifen发现](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=16974&pid=82881&fromuid=20)，如果圆内接四边形面积为S,那么$xyz=4RS$.

# 问题推广
[wayne另外发现](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=16974&pid=81568&fromuid=20) a,b,c,d可以拓展到任意实数范围，只要分母非零，等式还是依旧成立，这时就不能用几何模型解释了。我们可以从解析函数的解析延拓角度去理解, 所以只要分母非零，等式对复数也会成立。


