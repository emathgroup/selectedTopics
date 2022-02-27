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
