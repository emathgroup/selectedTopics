---
title: 圆周率和阶乘的数字握手
categories: 数学
keywords: '阶乘'
thumbnail: "../thumbnails/digitalhand.jpg"

tags:
  - 阶乘
  - 圆周率
date: 2019-11-1 03:51:34
---

medie2005[于2008年12月提问]，请找出第一个满足n!=31415926xxxx...xxx的整数n。 (其中xxxx表示任意数字）  
表示这个问题很困难的无心人勇攀高峰，最后找出了1544607046599!=31415926535899498xxx...xxx。

# 详细内容
gxqcn[首先提议用斯特林公式去搜索]， medie2005说[减少一位要求还是很容易]的，他自己很快给出3012584!=3141592x...x，但是他表示再增加一位就很困难了
然后他在一个半小时后给出了5385973!=31415926x...x。但是gxqcn用他的[HugeCalc]计算了一下，发现[medie2005的计算精度不够]。
mathe强调应该像qxqcn所说的用Stirling公式独立计算各个阶乘而不应该通过连乘来计算，不然会很容易丢失精度。并且mathe表示用Stirling公式[只需要double类型数据]
来表示就可以了，但是后面可以看出，问题远远没有这么简单。  

对于比较大的n,使用stirling级数精度可以达到非常之高:
$\ln\Gamma(z)=\frac12\ln(2\pi)+(z-\frac12)\ln(z)-z+\sum_{n=1}^{\infty}\frac{B_{2n}}{2n(2n-1)z^{2n-1}}$
$=\frac12\ln(2\pi)+(z-\frac12)\ln(z)-z+\frac1{12z}-\frac1{360z^3}+\frac1{1260z^5}-...$  
其中$B_{2n}$为贝努利数。  
[liangbch指出double类型精度是不够]的:  
因为double型数可精确到15-16位有小数字，如果斯特林公式是精确的，使用斯特林公式来计算log10(n!) 可精确到15-16位有效数字，而这15-16位有效数字又分成整数部分和小数部分，现在的要求尾数（小数部分必须达到8位），那么整数部分至多就不能超过7位了，所有当n!大于$10^7$时，就无法使用double型来估算了，必须采用更高的精度。  
无心人开始开启了刷机模式，他的[第一个版本代码]将各个数字取对数然后进行连加,然后发现精度很快就不够了，于是他决定用mpfr进行高精度运算。

下面是他[使用了mpfr后的代码]
```bash
#include <gmp.h>
#include <mpfr.h>
#include <stdio.h>

#define DefaultBitLength 128

int main(void)
{
  mpfr_t a, b, low, hi, lf, t;
  double dLow, dHi, dLf, dA;
  int find = 0;
  long i = 0;
  
  mpfr_init2(a, DefaultBitLength);
  mpfr_set_d(a, 2.0, GMP_RNDD);
  mpfr_init2(b, DefaultBitLength);
  mpfr_set_d(b, 1.0, GMP_RNDD);
  mpfr_init2(low, DefaultBitLength);
  mpfr_init2(hi, DefaultBitLength);
  mpfr_init2(lf, DefaultBitLength);
  mpfr_set_d(lf, 0.0, GMP_RNDD);
  mpfr_init2(t, DefaultBitLength);

  printf("请输入上界: ");
  scanf("%lf", &dHi);
  printf("请输入下界: ");
  scanf("%lf", &dLow);

//  printf("当前界限[%f, %f]\n", dLow, dHi);

  mpfr_set_d(low, dLow, GMP_RNDD);
  mpfr_set_d(hi, dHi, GMP_RNDD);
  
  mpfr_log10(low, low, GMP_RNDD);
  mpfr_log10(hi, hi, GMP_RNDD);

  while (1)
  {
    mpfr_log10(t, a, GMP_RNDD);
    mpfr_add(lf, lf, t, GMP_RNDD);

    if (mpfr_cmp_d(lf, 1.0) >= 0)
      mpfr_sub_ui(lf, lf, 1, GMP_RNDD);

    if ((mpfr_cmp(lf, low) >= 0)  && (mpfr_cmp(lf, hi) < 0))
     {
       find = 1;
       break;
     }

    mpfr_add(a, a, b, GMP_RNDD);

    if (mpfr_cmp_d(a, 10.0) >= 0)
    {
      i++;
      mpfr_div_ui(b, b, 10, GMP_RNDD);
      mpfr_add_ui(a, b, 1, GMP_RNDD);
    }
    
  } 
  
  if (find)
  {
    dLf = mpfr_get_d1(lf);
    while (i > 0)
    {
        mpfr_mul_ui(a, a, 10, GMP_RNDD);
        i--;
    }
    dA = mpfr_get_d1(a);
    
    printf("当前界限下的阶乘%.0f!, 对应的对数%.15f", dA, dLf);
  }
  
  mpfr_clear(a);
  mpfr_clear(b);
  mpfr_clear(low);
  mpfr_clear(hi);  
  mpfr_clear(lf);
  mpfr_clear(t);
  return 1;
}
```
由此他找到了688395641!=3141592651957527xxx....xxx。
但是他发现代码运行的有点慢，几经周折，[决定还是使用Stirling公式]
```bash
#include <gmp.h>
#include <mpfr.h>
#include <stdio.h>
#include <math.h>

#define DefaultBitLength 128

  mpfr_t ln10, A;
//=1/2ln(2π)+(n+1/2)ln(n)-n+1/12n
//n > 10^8 误差<1/360Z^3 = 10^(-26)
long double stirling(double n)
{
    mpfr_t a, b, c, s;
    long double r;
   
    mpfr_init2(a, DefaultBitLength);
    mpfr_init2(b, DefaultBitLength);
    mpfr_set_d(a, n, GMP_RNDD); //a=n
    mpfr_init2(c, DefaultBitLength);
    mpfr_set_d(b, 12.0, GMP_RNDD); //b=12
    mpfr_set_d(c, 1.0, GMP_RNDD); //c=1
    mpfr_init2(s, DefaultBitLength);

    mpfr_div(b, c, b, GMP_RNDD); // b = c / b = 1/12
   
    mpfr_div(b, b, a, GMP_RNDD); // b = b / a = 1/12n
   
    mpfr_sub(s, b, a, GMP_RNDD); //s = b - a = - n + 1/12n
   
    mpfr_log(c, a, GMP_RNDD); //c = ln(n)

    mpfr_set_d(b, 0.5, GMP_RNDD);
    mpfr_add(b, a, b, GMP_RNDD);
    mpfr_mul(b, b, c, GMP_RNDD); //b = (n + 1/2)ln(n)
   
    mpfr_add(s, s, b, GMP_RNDD); //s = (n + 1/2)ln(n) - n + 1/12n

    mpfr_add(s, s, A, GMP_RNDD); //最终结果s=1/2ln(2*pi) + (n + 1/2)ln(n) - n + 1/12n

    mpfr_div(s, s, ln10, GMP_RNDD); //转换为log10
    mpfr_floor(b, s);
    mpfr_sub(s, s, b, GMP_RNDD); //只要小数
   
    r = mpfr_get_ld(s, GMP_RNDD);
   
    mpfr_clear(a);
    mpfr_clear(b);
    mpfr_clear(c);
    mpfr_clear(s);
    return r;
}

int main(void)
{
  double a, b, low, hi, lf, t;
  double start;
  int find = 0;
  long i = 0, j = 0;
  
  mpfr_init2(ln10, DefaultBitLength);
  mpfr_init2(A, DefaultBitLength);
  mpfr_set_d(ln10, 10.0, GMP_RNDD);
  mpfr_log(ln10, ln10, GMP_RNDD);

  mpfr_const_pi(A, GMP_RNDD);
  mpfr_mul_ui(A, A, 2, GMP_RNDD);
  mpfr_log(A, A, GMP_RNDD);
  mpfr_div_ui(A, A, 2, GMP_RNDD);
  
  printf("请输入起始搜索数字：");
  scanf("%lf", &start);
  printf("请输入上界: ");
  scanf("%lf", &hi);
  printf("请输入下界: ");
  scanf("%lf", &low);
  
  hi = log10(hi);
  low = log10(low);
  
  printf("起始位置：%.0f\n当前界限[%.15f, %.15f]\n", start, low, hi);
  a = start;
  b = 1.0;
  while (a >= 10.0)
  {
    a /= 10.0;
    b *= 10.0;
  }

  lf = stirling(start);
  
  printf("a: %.15f\nb: %.0f\nlf: %.15f\n", a, b, lf);
  while (1)
  {
    start += 1.0;
    a = log10(start/b);
    
    if (a >= 1.0) 
    {
      b *= 10.0;
      a -= 1.0;
    }
    
    i ++;
    lf += a;
        
    if (lf >= 1.0)
        lf -= 1.0;

//    printf("[%.0f, %.15f]  ", start, lf);
   
    if ((lf >= low) && (lf < hi))
     {
       find = 1;
       break;
     }
   
    if (i >= 100)
    {
      lf = stirling(start);
      i = 0;
      j ++;
      if (j >= 100000)
      {
          j = 0;
          printf(".");
      }
    }
  }
  
  if (find)
  {
    printf("\n当前界限下的阶乘%.0f!, 对应的对数%.15f", start, lf);
  }
  
  mpfr_clear(ln10);
  mpfr_clear(A);
  return 1;
}
```
于是又得出了5777940569!=314159265301xxx...xxx。  

后面经过长时间挂机，[最终无心人给出]了如下结果
d = 3, n = 9
d = 31, n = 62
d = 314, n = 62
d = 3141, n = 10044
d =31415, n = 50583
d = 314159, n = 1490717
d = 3141592, n =   5573998  开始数字是3141592381
d = 31415926, n = 65630447 开始数字是3141592602
d = 314159265, n = 688395641 开始数字3141592651957527
d = 3141592653, n = 5777940569 起始数字314159265301
d = 31415926535, n = 77773146302, 开始 数字314159265353488
d = 314159265358,  n = 1154318938997开始数字3141592653584138
d = 3141592653589, n = 1544607046599开始数字31415926535899498

[于2008年12月提问]: https://bbs.emath.ac.cn/forum.php?mod=viewthread&tid=969&fromuid=20
[首先提议用斯特林公式去搜索]： https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=969&pid=12665&fromuid=20
[减少一位要求还是很容易]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=969&pid=12671&fromuid=20
[HugeCalc]: https://www.emath.ac.cn/software.htm
[medie2005的计算精度不够]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=969&pid=12693&fromuid=20
[只需要double类型数据]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=969&pid=12699&fromuid=20
[liangbch指出double类型精度是不够]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=969&pid=12706&fromuid=20
[最终无心人给出]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=969&pid=12711&fromuid=20
[第一个版本代码]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=969&pid=12726&fromuid=20
[使用了mpfr后的代码]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=969&pid=12842&fromuid=20
[决定还是使用Stirling公式]: https://bbs.emath.ac.cn/forum.php?mod=redirect&goto=findpost&ptid=969&pid=13275&fromuid=20
