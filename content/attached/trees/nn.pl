#!/usr/bin/perl
$NUM_NODES=15;
for($i=1;$i<=$NUM_NODES;$i++){
   system("./abc $i");
}
system("cat data/s$NUM_NODES/* > data/output.all");
