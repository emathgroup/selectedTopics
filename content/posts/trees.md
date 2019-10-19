

## 二十棵树最优解求解问题

#**简介**

20棵树植树问题要求将20棵树种在一个平面果园中，要求正好每行四棵树的行数尽量多。
这个问题来源于[Orchard Planting Problem], 或者英文维基百科[果树问题]上也有介绍。原始问题是要求将n棵树种在一个平面果园中，要求每行k棵树的行数尽量多。而其中20棵树每行四棵的问题不知道为何，在国内互联网流传比较广泛。
这个问题中如果[每行3棵树]的情况已经有非常好的结果，B. Grunbaum和Sloane在1974年通过复杂的数学理论（椭圆曲线）给出了很好的下界，不少于⌊n^2/6-n/2⌋行，而G. Ben和陶哲轩在2013年证明了对于充分大的n,上面方法给出的已经是最优解。
但是对于每行4棵或更多树的情况，现在我们还没有发现存在很好的解决方案。

而对于20棵树每行四颗的问题，网络上流传着一个比较漂亮的多层正五角星嵌套的20行结果（计算机搜索结果），不过这个远远不是最优解
![20rows](../images/trees/r20.jpg)

2005年，[Xianzu Lin] (可以能是福建师范大学的林贤祖教授)最早给出了一种23行的方案，只是给出的图中部分点在无穷远直线，非专业人员难以看懂（但是后面我们会介绍如何转化为更加容易看懂的结果），可能因为这个原因，这个结果并没有怎么流传开来。
![xianzu](../images/trees/xianzu.jpg)

2006年，辽宁锦州开发区笔架山小学的[王兴君]成功地绘制出了欧氏平面中23行图，这个结果在国内网络上流传比较广泛
![xwang](../images/trees/xwang.jpg)

2009年，河北省邢台学院学生[黄阳阳]（昵称eyond）成功地绘制出了另一个漂亮的23行植树图，结果如下：
![xwang](../images/trees/yhuang.jpg)

2010年2月，我们在数学研发论坛上通过使用计算机合作验证了20棵树，每行四棵树，[最多只能种植23行]。但是由于计算能力的限制，此程序还无法找出所有的23行解。程序只找到了两种不等价的解，其中一个解和Xianzu Lin的结果等价，而另外一组解和王兴君和黄阳阳的都等价。其中王兴君和黄阳阳的结果虽然看似不同，但是从数学中的射影变换的角度来看，是可以相互转化的。
2019年9月，我们利用计算机又找出一种和上面两种方案都不等价的[新的23行解]。
![n23](../images/trees/n23.jpg)

从数学角度，我们可以通过射影变换将上面这些图片进行变换得出一些等价的但是看似不同的结果，比如2012年1月wayne就将前两种结果经过射影变换变化出如下[一些漂亮的图片]， 链接中含有更多的等价图片。
![b1](../images/trees/b1.jpg)
![b2](../images/trees/b2.jpg)
![b3](../images/trees/b3.jpg)
![b4](../images/trees/b4.jpg)
 

#**射影变换介绍**

如果我们观察圆盘在地面上的影子，我们会发现圆形的圆盘会被投影成椭圆形的，也就是在投影变换下，图片的形状会发生变换，而且随着点光源，圆盘和投影平面位置的变化，我们甚至还可以将圆盘投影为抛物线和双曲线。但是我们发现，在投影下，直线的投影还是直线，并且投影并不会改变直线的相交关系。
![proj](../images/trees/proj.jpg)
所以如果我们找到一个二十棵树问题的解，那么通过投影变换就可以转化出无数幅看似不同的结果，比如[王兴君]和[黄阳阳]的答案看似不同，本质相同。

在https://bbs.emath.ac.cn//thread-2117-1-1.html 中给出了射影变换的介绍。而为了能够自己通过数学计算的方案对上面这些结果进行射影变换转化出更多的结果，那么我们首先需要理解齐次坐标。

在平面坐标系中，任何一个点可以通过其横坐标和纵坐标值(x,y)来确定。对于这样的一个点，在其齐次坐标系中，我们需要使用[rx:ry,r]来表示，其中参数r可以是任意非零的实数。也就是说，任何一个点在齐次坐标系下其表示形式是不唯一的，可以有无穷种形式。比如平面坐标系上点(-1,1)在齐次坐标系下可以写成[-1:1:1],也可以表示为[-2:2:2]等等。
齐次坐标系的最大好处是可以方便的描述了无穷远点。在平面几何中，两条直线可以平行，也可以相交于唯一的一点。但是在射影几何中，任何两条直线相交于唯一的一点，其中，平面几何中平行的两条直线在射影几何中相交于一个无穷远点。而所有无穷远点的齐次坐标形式中第三个分量总是0。比如所有和横坐标平行的直线相交于无穷远点[1:0:0],而所有和纵坐标平行的直线相交于无穷远点[0:1:0]。而所有的无穷远点构成一条无穷远直线Z=0。

而对于一个平面图案，如果我们选择一个3×3可逆矩阵$A=[(a_11&a_12&a_13@a_21&a_22&a_23@a_31&a_32&a_33 )]$,那么对于射影平面上任意一个点[x:y:z], 变换[x’:y’:z’]= [x:y:z]A就对应前面的投影变换,
也就是${(x^'=a_11 x+a_21 y+a_31 z@y^'=a_12 x+a_22 y+a_32 z@z^'=a_13 x+a_23 y+a_33 z)}$, 或者对应平面坐标系下的变换（如果没有无穷远点参与）{(x^'=(a_11 x+a_21 y+a_31)/(a_13 x+a_23 y+a_33 )@y^'=(a_12 x+a_22 y+a_32)/(a_13 x+a_23 y+a_33 ))}$
wanye通过一个简单的mathematica代码给出了将一个二十棵树问题的解[随机投影]成另外一些形式, 其中通过将无穷远直线投影成普通直线可以使得得到的图看上去和原先的图完全不同。

#**计算机求解算法简介**
由于20棵树问题的搜索计算量非常大，我们使用了很多技巧来解决这个问题。这个算法首先构造了一些使用较少树的方案，然后通过逐步添加额外的树得到包含更多树的方案。计算过程中采用了齐次坐标，并且可以事先任意设定其中四个点的坐标（射影变换可以将任意三个不共线的四个点坐标任意投影）。

链接给出了[算法总体介绍]，其中第三步中给出了一种对于给定两个看似不同的点线关系图，判断它们是否等价（通过交换点或线的顺序）的算法。在图的搜索扩张过程中，如果我们发现两个图（包含了若干树）是等价，就可以淘汰掉其中一个图，不需要继续搜索从它派生的图，从而可以节省了大量的计算时间。

另外对于给定的一种点线关系图（特别是包含的线很多时），[无法保证一定存在对应的果树问题解]。在链接中给定了一种算法，通过消元来简化方程组，最后可以部分确认/简化或淘汰某些关系是否存在对应的果树问题解。这个算法不仅仅在我们得到一种合法的果树问题解的点线关系后，可以用于求出最终的坐标，也可以在上述搜索过程中，可以提前淘汰一些没有对应果树问题解的点线关系，从而也缩小了搜索空间，节省大量的计算时间。
通过使用上述的算法，使用计算机已经可以解决[13棵树]和[14棵树]问题，但是对于20棵树问题还远远不够。

![t13](../images/trees/t13.jpg) 
13棵树问题的解（9行，包含无穷远直线）
ABEFAGHMBIJMCEKMCGILDFLMDGJKEHJLFHIK
A(0,1) B(1,0) C(1/4,-1/4) D(-1/2,-1/2) E[1:-1:0] F(1/2,1/2) G(0,-1/2) H[0:1:0] I(1/2,0) J[1:0:0] K(1/2,-1/2) L[1:1:0] M(0,0)

![t14](../images/trees/t14.jpg) 
14棵树问题的解（10行，包含无穷远直线）
ABDEAFGHBIJNCDIKCFLNDGMNEGJLEHKNFJKMHILM
solve([-4/3+1*K_X,-8/3+1*L_X,+3+1*L_Y,+9/4+1*J_Y,+2+1*K_Y,+3/2+1*I_Y,+1+1*H_Y,-3+1*G_Y,+4+1*C_Y,+5+1*F_Y,-4/3+1*H_X,-8/3+1*C_X,-8/3+1*F_X,-4/3+1*E_X],[K_X,L_X,L_Y,J_Y,K_Y,I_Y,H_Y,G_Y,C_Y,F_Y,H_X,C_X,F_X,E_X]);
print("A_x=1 A_y=0 B=[1:0:0] D_x=0 D_y=0 E_y=0 G_x=0 I=[1:I_y:0] J=[1:J_y:0] M_x=0 M_y=1 N=[0:1:0] ");

后来，我们进一步提供了一种[只搜索包含线的数目比较多的方案]，加速了搜索过程，使得我们能够解决到[17棵树]的问题。

![t15](../images/trees/t15.jpg)  
15棵树问题的最优解

![t16](../images/trees/t16.jpg)  
16棵树问题的最优解

![t17](../images/trees/t17.jpg) 
17棵树问题的最优解
我们进一步找到了[一种更快速的部分判断识别一个点线关系是否存在合法的果树问题解的算法]，从而解决了[18棵树]和[19棵树]问题。

![t18](../images/trees/t18.jpg)  
18棵树问题的最优解

![t19](../images/trees/t19.jpg)  
19棵树问题最优解
然后[数学研发论坛上的网友一起合作]，利用很多计算机一起证明了20棵树不存在24行解，从而证明23行已经是最优结果。但是在这个过程中，由于没有足够计算资源，我们还无法找出所有的23行解。
2019年9月，我们重新运行了以前的代码，利用计算机又找出一种和上面两种方案都不等价的[新的23行解]。


而已知的20棵树23行解坐标如下:
[解1]
print(ADGJBEIJCDHKAFIKCEGLBFHLCJMODINODLMPAHNPGKOPBGMQFJNQAEOQEHMRBKNRCFPRILQRABCSDEFSGHITJKLTMNST);
solve([+1+3*T_Y+1*T_Y*T_Y,-3/5+1*R_Y-2/5*T_Y,-4/5+1*R_X-1/5*T_Y,-3+1*H_X-1*T_Y,-4+1*P_X-1*T_Y,+1+1*S_X,-2+1*Q_X-1*T_Y,+2+1*O_X+1*T_Y,-1+1*L_Y-1*T_Y,+1+1*P_Y,-3+1*M_Y-2*T_Y,+1+1*D_Y+1*T_Y,-2+1*S_Y-1*T_Y,+1*G_Y-1*T_Y,-1+1*Q_Y,+1+1*N_Y,-2+1*C_Y-1*T_Y,-2+1*B_Y-1*T_Y,-1+1*O_Y,+1+1*H_Y,+2+1*C_X+1*T_Y,-2+1*N_X-1*T_Y,-1+1*L_X,-1+1*T_X,+2+1*M_X+1*T_Y,-2+1*F_X-1*T_Y],[T_Y,R_Y,R_X,H_X,P_X,S_X,Q_X,O_X,L_Y,P_Y,M_Y,D_Y,S_Y,G_Y,Q_Y,N_Y,C_Y,B_Y,O_Y,H_Y,C_X,N_X,L_X,T_X,M_X,F_X]);
print("A=[1:0:0] B_x=0 D=[1:D_y:0] E_x=0 E_y=1 F_y=0 G=[1:G_y:0] I_x=0 I_y=0 J=[0:1:0] K_x=1 K_y=0 ");

[解2]
ACEFDFHJBCIJBEHKADIKBFGMEJLMBDLNCGNODEGPFKNPAJOPAGLQCKMQCHLRDMOREIQRAHNSFIOSBPQSGHITABRTCDST
+4/3+1*Ey,-1/2+1*Tx,+3+1*Py,+2+1*Qy,-5/2+1*Qx,+1/2+1*Rx,-3+1*Px,+3/2+1*Lx,-3/2+1*Mx,-2+1*Ry,+1+1*Oy,-2+1*Ly,+2+1*My,-3+1*Nx,+1+1*Ny,-2+1*Hy,-1+1*Ox,-1+1*Sx,-3+1*Kx,+1+1*Ay,+2+1*Ky,-3/2+1*Gx,-3/2+1*Bx,+1+1*Gy,-1+1*Ty,-1+1*Sy
Ey,Tx,Py,Qy,Qx,Rx,Px,Lx,Mx,Ry,Oy,Ly,My,Nx,Ny,Hy,Ox,Sx,Kx,Ay,Ky,Gx,Bx,Gy,Ty,Sy
Ax=1,Az=0,By=0,Bz=1,Cx=1,Cy=0,Cz=0,Dx=0,Dz=1,Dy=1,Dz=1,Ex=1,Ez=0,Fx=0,Fy=1,Fz=0,Hx=0,Hz=1,Ix=1,Iz=1,Iy=0,Iz=1,Jx=0,Jz=1,Jy=0,Jz=1,

[解3]
print(BFHICDJKAEGLHJLMDGINAKMNFGKOCIMOEIKPBGMPBCLQFJNQADOQACHRBENRDLPRABJSEHOSCFPSDEFTGHQTIJRTKLST);
solve([+1-2*T_X+1*P_Y*T_X-2*T_X*T_X,+1*P_Y-1*T_X-1*T_X*T_X,+1*P_Y*P_Y+1*T_X-3*P_Y*T_X,+1*N_X-1*P_Y+1*T_X,-1+1*P_X+1*P_Y-1*T_X,-1+1*S_X+1*T_X,-1+1*S_Y-1*T_X,+1*Q_Y+1*T_X,-2+1*A_Y+1*P_Y-1*T_X,+1+1*L_Y-1*P_Y+1*T_X,+3+1*F_Y-1*P_Y+2*T_X,-1+1*N_Y+1*P_Y,-1+1*K_X+1*P_Y,+1*D_X-1*P_Y,-1+1*G_Y+1*P_Y,-1+1*E_X+1*T_X,-1+1*O_X+1*T_X,-1+1*B_Y+1*P_Y-1*T_X,-1+1*R_X,-1+1*A_X,-1+1*D_Y+1*P_Y,+1*G_X-1*T_X,+1*Q_X-1*T_X,-1+1*R_Y,-1+1*T_Y,+1*K_Y-1*P_Y,+1*E_Y-1*P_Y],[P_Y,T_X,N_X,P_X,S_X,S_Y,Q_Y,A_Y,L_Y,F_Y,N_Y,K_X,D_X,G_Y,E_X,O_X,B_Y,R_X,A_X,D_Y,G_X,Q_X,R_Y,T_Y,K_Y,E_Y]);
print("B=[1:B_y:0] C_x=1 C_y=0 F=[1:F_y:0] H=[0:1:0] I=[1:0:0] J_x=0 J_y=1 L_x=0 M_x=0 M_y=0 O_y=0 ");

另外链接中给出了部分[关键C++代码]。


[Orchard Planting Problem]: http://mathworld.wolfram.com/Orchard-PlantingProblem.html
[果树问题]: https://en.wikipedia.org/wiki/Orchard-planting_problem
[每行3棵树]: https://oeis.org/A003035
[Xianzu Lin]: http://oeis.org/A006065/a006065.gif
[王兴君]: http://www.eol.cn/zheng_ming_1877/20060309/t20060309_166193.shtml
[黄阳阳]: https://bbs.emath.ac.cn/thread-1418-1-1.html
[最多只能种植23行]: https://bbs.emath.ac.cn/thread-2007-1-1.html
[新的23行解]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=3953&pid=81177&fromuid=20
[一些漂亮的图片]: https://bbs.emath.ac.cn/thread-3953-1-1.html
[随机投影]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=3953&pid=41123&fromuid=20
[算法总体介绍]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=703&pid=8670&fromuid=20
[无法保证一定存在对应的果树问题解]: https://bbs.emath.ac.cn/thread-703-1-1.html
[13棵树]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=703&pid=8917&fromuid=20
[14棵树]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=703&pid=9023&fromuid=20
[只搜索包含线的数目比较多的方案]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=703&pid=11859&fromuid=20
[17棵树]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=703&pid=11837&fromuid=20
[一种更快速的部分判断识别一个点线关系是否存在合法的果树问题解的算法]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=703&pid=23066&fromuid=20
[18棵树]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=703&pid=16447&fromuid=20
[19棵树]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=703&pid=17544&fromuid=20
[数学研发论坛上的网友一起合作]: https://bbs.emath.ac.cn/thread-2007-1-1.html
[解1]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=3953&pid=41115&fromuid=20
[解2]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=3953&pid=41141&fromuid=20
[解3]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=3953&pid=81167&fromuid=20
[关键C++代码]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=703&pid=9077&fromuid=20
