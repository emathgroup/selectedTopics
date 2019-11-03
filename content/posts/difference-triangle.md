---
title: 完美和最小差三角形
categories: 数学
keywords: 'combination,difference,triangle'
thumbnail: "../thumbnails/diftriangle.png"

tags:
  - combination
  - difference
  - triangle
date: 2019-11-3 1:28:30
---

# 摘要
chyanog提出一个[完美差三角问题],  

仔细观察下面的数字组成的三角形：
```bash
    3
  1   4
5   6   2
```
看出什么特征吗？首先，它包含了1到6的连续整数。  
重要的是：每个数字都是其下方相邻的两个数字的差（当然是大数减去小数）  
满足这样特征的三角形，称为：#完美差三角#。

我们的任务是找出1到15的整数组成的一个更大的完美差三角，即五阶完美差三角。  
hujunhua很快找出了两种不同的三阶完美差三角和一个四阶完美差三角，后来他和chyanog还发现了五阶的完美差三角，但是他们发现更大的完美差三角好像不存在了。 ```bash
         5
       9   4
     2  11  7 
   10 12  1  8
 13  3  15 14  6
``` 

KeyTo9_Fans为了使得问题更有趣，放松了条件，我们可以称之为#[最小差三角]#:  
给定n，求最小的m，使得存在一个n阶的、数字范围是1到m的、每个数字最多出现1次的差三角。  
最终他得出[12阶最小差三角的范围到125]  
```bash
12: 125
   20
  101  81
  118  17  64
    5 113  96  32
  125 120   7  89  57
  122   3 117 110  21  36
    8 114 111   6 104  83  47
  124 116   2 109 103   1  82  35
  115   9 107 105   4  99  98  16  19
   13 102  93  14  91  87  12  86  70  51
   90  77  25  68  54  37  50  38  48  22  29
   62  28  49  24  44  10  27  23  15  33  11  18
```

# 具体内容
chyanog于2013年4月提出了[完美差三角问题]，我们可以搜索到[一个英文网站]也讨论了相同的问题，只是其三角形的方向和我们是颠倒的
他们给出了
![3d](../thumbnails/diftriangle.png)  
hujunhua很快给出了[两个三阶的完美差三角]和[一个四阶完美差三角]
```bash
    2
  5   3
6   1   4

    3
  5   2
1   6   4
      4
    5   1
  2   7   6
8  10   3   9
```
然后他有发现[还有一个三阶完美差三角]
```bash
    1
  4   3
6   2   5
```
而[四阶的总共四个]
```bash
      3
    5   2
  4   9   7
6  10   1   8

      3
    7   4
  2   9   5
8  10   1   6

      4
    5   1
  2   7   6
8  10   3   9

      4
    6   2
  1   7   5
9  10   3   8
```
chyanog找出了[两个五阶完美差三角]并且给出了[对应的C代码](../attached/diftria/chyanog.txt)，但是hujunhua指出这两个是对称的，所以本质只有如下一个
```bash
         5
       9   4
     2  11  7 
   10 12  1  8
 13  3  15 14  6
```
hujunhua用如下的mathematica代码验证
```bash
f[x_List]:=Abs@Differences@x; (*定义一个函数，计算表x的差分绝对值*)
Do[y=Reverse@ FixedPointList[f, x];If[Union @@y==Range[15], Print/@y], {x, Permutations[Range[15],{5}]}]
```
得出[上面的五阶完美差三角是唯一]的。 但是紧接着他们俩很失望地发现6阶和7阶都无解。

他们俩继续对上面mathematica代码做了多次性能优化工作
```bash
(n = 5;
  k = n (n + 1)/2;
  table = Quiet@With[{
      expr=Evaluate[Join@@NestList[Abs@Differences@# &, Slot~Array~n, n-1] /.  Slot[x_] :> i[[x]]]
      }, 
  Table[expr, {i, Permutations[Range@k, {n}]}]];
  table = Pick[table, Total[table, {2}], Tr@Range@k];
  fmt = MatrixForm@Internal`PartitionRagged[Reverse@#, Range[n]] &;
  Do[If[Unequal @@ i, Print@fmt@i], {i, table}]
  ) // AbsoluteTiming
Clear["`*"]
```

G-Spider也贴出了他的搜索[五阶](../attached/diftria/G-Spider1.txt) 和[四阶](../attached/diftria/G-Spider1.txt) 的代码, 然后zgg___和KeyTo9_Fans表示[8阶也无解]。  

2013年5月17日KeyTo9_Fans宣布9阶、10阶、11阶都无解，并且赞同hujunhua的更高阶完美差三角都无解的猜想。

为了保持问题的趣味性，KeyTo9_Fans决定将问题要求放松为[最小差三角]问题  
给定n，求最小的m，使得存在一个n阶的、数字范围是1到m的、每个数字最多出现1次的差三角。  
并且给出了$n\le 8$的结果（下面的三角形是横着摆放的，每个数是其左边两个数之差)
```bash
1: 1
   1
2: 3
   3
   2   1
3: 6
   6
   2   4
   5   3   1
4: 10
   9
  10   1
   3   7   6
   8   5   2   4
5: 15
  13
   3  10
  15  12   2
  14   1  11   9
   6   8   7   4   5
6: 22
  13
  21   8
   3  18  10
  22  19   1   9
  20   2  17  16   7
   6  14  12   5  11   4
7: 33
  19
  32  13
   3  29  16
  33  30   1  15
  31   2  28  27  12
   6  25  23   5  22  10
  17  11  14   9   4  18   8
8: 44
  29
   6  23
  43  37  14
  44   1  36  22
   3  41  40   4  18
  42  39   2  38  34  16
  33   9  30  28  10  24   8
   7  26  17  13  15   5  19  11
```
在三天后，KeyTo9_Fans又给出了[9阶和10阶的结果]  
```bash
9: 59
  43
  53  10
  13  40  30
  59  46   6  24
  55   4  42  36  12
   1  54  50   8  28  16
  58  57   3  47  39  11   5
  49   9  48  45   2  37  26  21
  17  32  23  25  20  18  19   7  14
10: 76
  52
  16  36
  66  50  14
  75   9  41  27
  15  60  51  10  17
  73  58   2  49  39  22
  76   3  55  53   4  35  13
  12  64  61   6  47  43   8   5
  69  57   7  54  48   1  42  34  29
  45  24  33  26  28  20  19  23  11  18
```
又经过两周时间他[验证了11阶的结果]并把结果提交到[A226239](https://oeis.org/A226239) 
```bash
11: 101
   21
   79  58
   92  13  45
   19  73  60  15
   99  80   7  53  38
   93   6  74  67  14  24
   11  82  76   2  65  51  27
  101  90   8  68  66   1  50  23
   97   4  86  78  10  56  55   5  18
   16  81  77   9  69  59   3  52  47  29
   64  48  33  44  35  34  25  22  30  17  12
```
最后经过6个月的漫长烧机历程，他[验证了12阶的结果]
```bash
12: 125
   20
  101  81
  118  17  64
    5 113  96  32
  125 120   7  89  57
  122   3 117 110  21  36
    8 114 111   6 104  83  47
  124 116   2 109 103   1  82  35
  115   9 107 105   4  99  98  16  19
   13 102  93  14  91  87  12  86  70  51
   90  77  25  68  54  37  50  38  48  22  29
   62  28  49  24  44  10  27  23  15  33  11  18
```

[完美差三角问题]: https://bbs.emath.ac.cn/thread-4977-1-1.html
[最小差三角]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=4977&pid=48932&fromuid=20
[12阶最小差三角的范围到125]: https://oeis.org/A226239
[一个英文网站]: https://puzzling.stackexchange.com/questions/49877/difference-triangle
[两个三阶的完美差三角]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=4977&pid=48526&fromuid=20
[一个四阶完美差三角]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=4977&pid=48527&fromuid=20
[还有一个三阶完美差三角]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=4977&pid=48543&fromuid=20
[四阶的总共四个]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=4977&pid=48544&fromuid=20
[两个五阶完美差三角]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=4977&pid=48548&fromuid=20
[上面的五阶完美差三角是唯一]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=4977&pid=48558&fromuid=20
[8阶也无解]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=4977&pid=48908&fromuid=20
[9阶和10阶的结果]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=4977&pid=48962&fromuid=20
[验证了11阶的结果]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=4977&pid=49216&fromuid=20
[验证了12阶的结果]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=4977&pid=56619&fromuid=20
