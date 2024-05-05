Computing searching result for [Wordle](https://www.nytimes.com/games/wordle/index.html)  
There're two different play modes for Wordle: Hard or Normal mode.  
Any revealed hints must be used in subsequent guesses in hard Mode.  
The target words must be in file w5.txt and player could use any words in file w5.txt or ta.txt when playing the game.  

t5.cpp is facility tool. Players could input their each guess and reponse from Wordle and the tool could shows how many candidate words are still left. In the response, we need input m for fully matched, u for unmatched and s (shift) for letter present but with wrong location.  

Other files like tt5.* ts5.* is computer searching results provide some optimized solutions.  
wf.cpp is a facility tool to guide player to use those optimized solutions.  
For example  

$./wf tse8.reast  
Next input reast  
Please input response 5  {(m)atch, (s)hift or (u)match}: uuuuu  
Next input godly  
Please input response 5  {(m)atch, (s)hift or (u)match}:  
 
The computer hints that the player should use reast for first round. If the Wordle game reponses all letters are unmatched (gray for all), the computer hints next the player could use godly.  

Some current optimized solutions as below  
 
Normal Mode  
|  file | Total count| Average count|max count|
|-------|------------|--------------|---------|
|ts5.salet| 8011 |3.4605|5|
|ts6.alien| 8356 |3.6095|5|
|ts6.flock| 9059 |3.9132|5|
|ts6.salet| 7991 |3.4518|5|
|tse8.reast| 7944|3.4315|5|

Hard Mode  
|  file | Total count| Average count|max count|
|-------|------------|--------------|---------|
|tse4.teals| 8175 |3.5313|6|
|tt5.adept.out|8568|3.7011|6|
|tt5.alien.out|8456|3.6527|6|
|tt5.flock| 8726 |3.76933|6|
|tt5.plate| 8249 |3.56328|6|
|tt5.trape| 8177 |3.53218|6|
|tse4.salet| 8144 |3.51793|6|
|tse4.80.salet|8125|3.50972|6|


bt.cpp is added to construct a game with chinese 4 characters idiom. A small database sid.sort is used which could be replaced by larger ones, suchas sid2.sort or sd.out

example:
````
$./bt sd.out 1
seed is 1

   * * * *
       *
       *
     * * * *
       *
   *   *
   * * * *
   *   *
   *   *
       *   *
       *   *
       *   *
       * * * *
         *
       * * * *
        身   *
         *   *
         *   *
         *   *
         *   *
         *   *
         *   *
         *   *
         *   *
         *   *
         *   *
         *   *
         *


俗容不不不风山人忍财国富落无名相肠脑面光玉出抽茅水发为真金金花致流败可改肥劳馀乎在苦满庐科胆铁闷初积咀咀嚼堆遁臬第焕隳耐


  落花流水
      风
      馀
    遁俗无闷
      不
  相  可
  忍苦耐劳
  为  人
  国  咀
      嚼  堆
      铁  金
      咀  积
      金科玉臬
        第
      初出茅庐
        身  山
        败  真
        名  面
        隳  不
        胆  改
        抽  容
        肠  光
        肥  焕
        脑  发
        满  财
        不  致
        在  富
        乎
````

````
$./bt sid2.sort 1
seed is 1

                 *
                 *   *
                 *   *
                 *   *
               * * * *
                 *
           *     *   *
           *     *   *
           *     *   *
           *   * * * *
           *     *
           *     *
           * * * *
               *
           * * * *
           *   *
           *
         *党 * *
           *
       * * * *
       *   *
       *
   * * * *
       *
       *
       *


之不不唱如情心心意意即即所有处然穷人故振振山严假实实词屈理理治私外欲虎虚安安士从宽归夫结稳稳转奋达泰妇随营忐忑擒纵瞬稍逝


                夫
                唱  虚
                妇  情
                随  假
              实心实意
                所
          忐    欲  转
          忑    擒  瞬
          不    故  即
          安  稍纵即逝
          安    虎
          稳    归
          稳如泰山
              然
          从宽处理
          严  之
          治
        结党营私
          外
      振奋人心
      振  士
      有
  理屈词穷
      不
      达
      意
````
file 岚.* provides excel version of the game.

files enumps*, mr*, nexterm.cpp, tracenext.cpp try to provides [6x6 squares so that product of rows/columns/diagonals are all same](https://www.zhihu.com/question/532559174).

files vd5.zip provides result of [transform digits of integers to powers](https://www.zhihu.com/question/533455093), such as 234=>4^3^2=262144, and 262144=>4^4^1^2^6^2=256 and so on. vd10.out vd10e.out only provides result of decimal system.
  The result of base below 12 are all provided.

cr2.cpp cr3.cpp: question https://www.zhihu.com/question/546784437 to find maximal number of black cells on m*n board where each black cell must have at least one neighbor white cell.

path4.cpp for A071983.  path4.2.cpp for A071984.

sumi*.cpp for Sporadic solutions such as A053974

gmpsrch.cpp for [是否不存在 2022397710683865089 的倍数，在 256 进制下每一位都小于 26](https://www.zhihu.com/question/654842645)
