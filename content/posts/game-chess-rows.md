---

title: "棋子游戏r"
date: "2019-10-24 04:06:57"
thumbnail: "../thumbnails/piece.jpg"
author: "mathe"
tags: 
    - "game"
    - "chess" 
---

medie2005提出了一个问题:

在一条线上[连续地放有n个棋子], 甲乙两人轮流每次拿走1个或者相邻的2个棋子(拿走后两边的棋子就不相邻了) 
不允许把棋子拿光, 不允许不拿. 谁不能行动就输了. 

问n为多少时先拿的输?

这个问题咋一看就是一个典型的[Nim游戏]。事实上，如果我们把"不允许把棋子拿光"这个限制去掉，问题马上转化为
一个非常简单的Nim游戏。先手只要拿走最中间的一个或两个棋子，接下去只要选择和对手上一轮选择的对称方案即可
保证胜利（拿走最后一颗棋子）。

而[最终结果]也非常奇怪，当且仅当n为1,4,9,12,20之一时先手输。



[连续地放有n个棋子]: https://bbs.emath.ac.cn/forum.php?mod=viewthread&tid=763&fromuid=20
[Nim游戏]: https://baike.baidu.com/item/Nim%E6%B8%B8%E6%88%8F/6737105
[最终结果]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=763&pid=9921&fromuid=20
