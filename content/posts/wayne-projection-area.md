---
title: wayne的投影面积
categories: 数学
thumbnail: "../thumbnails/blockproject.jpg"
date: 2020-01-28
tags:
  - "几何"
  - "极值"

---
2013年7月[wayne提问](https://bbs.emath.ac.cn/thread-5104-1-1.html):  
问题一)  一个正方形 投影到 其所在平面内的 一条直线 上的线段长度的期望值 是多少？  
问题二)  一个立方体 投影到 某一平面内上的 面积的期望值 是多少？  
最后大家得出了一个适用于所有凸体的通用性结论。  
<!--more-->

# 具体内容
[gxqcn认为](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=5104&pid=50101&fromuid=20)对于第一个问题：
可以考虑从正方形中心向某个顶点引一条射线，
然后将该顶点绕中心逆时针从0°旋转到45°，
在旋转过程中，该顶点在射线上的投影与射线顶点间的线段长度即是正方形在该射线所在直线上投影长度的一半。

[KeyTo9\_Fans得出](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=5104&pid=50102&fromuid=20):  假设正方形的边长是$1$，
那么问题$1$的期望值是：
$\frac{\sqrt{2}\int_0^{\frac{\pi}4}\cos(x)dx}{\frac{\pi}4}$
$=\frac{\sqrt{2}\sin(\frac{\pi}4)}{\frac{\pi}4}$
$=\frac{4}{\pi}$.  

对于问题2，mathe首先给出了一种[使用多重积分分析计算的方案](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=5104&pid=50174&fromuid=20):  
关于正方体，我们知道任意一个平面图形在另外一个平面上投影面积只同其面积以及两个平面夹角有关系，也就是如果平面图形面积是S,而两个平面夹角为$\theta$那么投影面积为$S\cos(\theta)$
另外，正方体有六个面，投影到一个平面上正好每个点都重复投影一次，所以投影面积是六个面投影面积和的一半。而由于六个面相互之间都是对称关系。实际上正方体投影平均投影面积就是一个面平均投影面积的三倍。
现在我们查看空间中一个动单位正方形投影到一个固定平面上面积的期望值，正方形单位法向量必然在单位球面上均匀分布。而投影面积为正方形和固定平面夹角的余弦值的绝对值，也就是单位向量和固定平面法向量的余弦值绝对值。我们不妨设固定平面法向量为z轴，动单位正方行法向量为$(x,y,z)$其中$||(x,y,z)||=1$
于是结果就变成曲面$\int\int_{x^2+y^2+z^2=1}|z|dS$除以单位球的面积，这个转化为球坐标比较容易计算，相当于半球面重心的坐标。  
然后[他发现](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=5104&pid=50175&fromuid=20): 发现上面积分直接通过几何意义计算也非常简单。实际上好像有一个定理是说球面上任意用两个距离相等（比如h)的平行平面割下的“环”面积是相等的：
实际上$z$到$z+dz$之间周长为$2\pi\sqrt{1-z^2}$,而由于任意方向法向同z轴夹角余弦值也为$\sqrt{1-z^2}$,于是得出$z$到$z+dz$之间面积总是$2\pi dz$
由此得出上面期望值非常简单，就是$|z|$的平均值，为$\frac12$.于是整个单位正方体投影的平均面积为$\frac32$.  
[倪举鹏](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=5104&pid=60777&fromuid=20)和[hujunhua](https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=5104&pid=60782&fromuid=20)做了总结:  
原来任取凸体的一个表面面积元，其法向在空间是概率均布的，因此它的平均投影面积是本身面积的二分之一。而凸体的平均投影面积就是其表面积的四分之一。  
hujunhua指出这一点可以从球体直接得证，不需要进行积分计算。在这个面积元的中心法线上任取一点作为定点，这个面积元在绕此定点转遍所有的空间方向后刚好构成一个球面。  

