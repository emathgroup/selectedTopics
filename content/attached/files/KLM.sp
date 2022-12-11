LIB "solve.lib";
ring r0=0, (a,b,c,d,k,l,m,n,o),dp;
poly f0=1-1n+2ln-1ko-1no;
poly f1=1k+1ln-1ko-1no;
poly f2=1k2+1kl+2ln+1o-3ko-2no;
poly f3=1l-1km+1ln-1ko-1no;
poly f4=1kn-1ln+1ko;
poly f5=mn-o;
poly f6=km+k-l;

list nn1=1n,1k,1;
list nn2=0,1,1;
list nn3=1k+1n,1l,1;
list nn4=0,1,0;
list nn5=1+1n,1o,1;
list nn6=1,0,1;
list nn7=1,0,0;
list nn8=1-1k,k,1;
list nn9=1+1n,1,1;
list nn10=1,l,1;
list nn11=1,m,0;
list nn12=n,o,1;
list nn13=0,0,1;
list allnodes = nn1,nn2,nn3,nn4,nn5,nn6,nn7,nn8,nn9,nn10,nn11,nn12,nn13;

proc ptinline(list nodes, list index)
{
    matrix mm[3][3];
    int kk;
    for(kk=1;kk<=3;kk++){
    mm[kk,1]=nodes[index[kk]][1]; mm[kk,2]=nodes[index[kk]][2]; mm[kk,3]=nodes[index[kk]][3];
    }
    poly pp=det(mm);
    return (pp);
}
proc inonecv(list nodes, list index)
{
    matrix mm[6][6];
    int kk;
    list nn;
    for(kk=1;kk<=4;kk++){
        nn=nodes[ index[kk] ];
        mm[kk,1]=nn[1]*nn[1];
        mm[kk,2]=nn[1]*nn[2];
        mm[kk,3]=nn[1]*nn[3];
        mm[kk,4]=nn[2]*nn[2];
        mm[kk,5]=nn[2]*nn[3];
        mm[kk,6]=nn[3]*nn[3];
    }
    mm[5,1]=a2-b2;
    mm[6,1]=2ab;
    mm[5,2]=ac-bd;
    mm[6,2]=ad+bc;
    mm[5,3]=a;
    mm[6,3]=b;
    mm[5,4]=c2-d2;
    mm[6,4]=2cd;
    mm[5,5]=c;
    mm[6,5]=d;
    mm[5,6]=1;
    mm[6,6]=0;
    poly pp=det(mm);
    return (pp);
}

list ll0=8,9,1,2;
poly gg0=inonecv(allnodes, ll0);
list ll1=8,9,3,7;
poly gg1=inonecv(allnodes, ll1);
list ll2=8,9,4,6;
poly gg2=inonecv(allnodes, ll2);
list ll3=8,10,1,3;
poly gg3=inonecv(allnodes, ll3);
list ll4=8,10,4,7;
poly gg4=inonecv(allnodes, ll4);
list ll5=8,10,5,6;
poly gg5=inonecv(allnodes, ll5);
list ll6=8,11,1,4;
poly gg6=inonecv(allnodes, ll6);
list ll7=8,11,2,3;
poly gg7=inonecv(allnodes, ll7);
list ll8=8,11,5,7;
poly gg8=inonecv(allnodes, ll8);
list ll9=8,12,1,5;
poly gg9=inonecv(allnodes, ll9);
list ll10=8,12,2,4;
poly gg10=inonecv(allnodes, ll10);
list ll11=8,12,6,7;
poly gg11=inonecv(allnodes, ll11);
list ll12=8,13,1,6;
poly gg12=inonecv(allnodes, ll12);
list ll13=8,13,2,5;
poly gg13=inonecv(allnodes, ll13);
list ll14=8,13,3,4;
poly gg14=inonecv(allnodes, ll14);
list ll15=9,10,1,4;
poly gg15=inonecv(allnodes, ll15);
list ll16=9,10,2,3;
poly gg16=inonecv(allnodes, ll16);
list ll17=9,10,5,7;
poly gg17=inonecv(allnodes, ll17);
list ll18=9,11,1,5;
poly gg18=inonecv(allnodes, ll18);
list ll19=9,11,2,4;
poly gg19=inonecv(allnodes, ll19);
list ll20=9,11,6,7;
poly gg20=inonecv(allnodes, ll20);
list ll21=9,12,1,6;
poly gg21=inonecv(allnodes, ll21);
list ll22=9,12,2,5;
poly gg22=inonecv(allnodes, ll22);
list ll23=9,12,3,4;
poly gg23=inonecv(allnodes, ll23);
list ll24=9,13,1,7;
poly gg24=inonecv(allnodes, ll24);
list ll25=9,13,2,6;
poly gg25=inonecv(allnodes, ll25);
list ll26=9,13,3,5;
poly gg26=inonecv(allnodes, ll26);
list ll27=10,11,1,6;
poly gg27=inonecv(allnodes, ll27);
list ll28=10,11,2,5;
poly gg28=inonecv(allnodes, ll28);
list ll29=10,11,3,4;
poly gg29=inonecv(allnodes, ll29);
list ll30=10,12,1,7;
poly gg30=inonecv(allnodes, ll30);
list ll31=10,12,2,6;
poly gg31=inonecv(allnodes, ll31);
list ll32=10,12,3,5;
poly gg32=inonecv(allnodes, ll32);
list ll33=10,13,2,7;
poly gg33=inonecv(allnodes, ll33);
list ll34=10,13,3,6;
poly gg34=inonecv(allnodes, ll34);
list ll35=10,13,4,5;
poly gg35=inonecv(allnodes, ll35);
list ll36=11,12,2,7;
poly gg36=inonecv(allnodes, ll36);
list ll37=11,12,3,6;
poly gg37=inonecv(allnodes, ll37);
list ll38=11,12,4,5;
poly gg38=inonecv(allnodes, ll38);
list ll39=11,13,1,2;
poly gg39=inonecv(allnodes, ll39);
list ll40=11,13,3,7;
poly gg40=inonecv(allnodes, ll40);
list ll41=11,13,4,6;
poly gg41=inonecv(allnodes, ll41);
list ll42=12,13,1,3;
poly gg42=inonecv(allnodes, ll42);
list ll43=12,13,4,7;
poly gg43=inonecv(allnodes, ll43);
list ll44=12,13,5,6;
poly gg44=inonecv(allnodes, ll44);
print("end of inonecv");
ideal ii2=gg0,gg1,gg2,gg3,gg4,gg5,gg6,gg7,gg8,gg9,gg10,gg11,gg12,gg13,gg14,gg15,gg16,gg17,gg18,gg19,gg20,gg21,gg22,gg23,gg24,gg25,gg26,gg27,gg28,gg29,gg30,gg31,gg32,gg33,gg34,gg35,gg36,gg37,gg38,gg39,gg40,gg41,gg42,gg43,gg44,f0,f1,f2,f3,f4,f5,f6;
ideal si2=std(ii2);
dim(si2);
size(si2);
gg0=b;
ii2=sat(si2,gg0)[1];
si2=std(ii2);
gg0=d;
ii2=sat(si2,gg0)[1];
si2=std(ii2);
gg0=bc-ad;
ii2=sat(si2,gg0)[1];
si2=std(ii2);
gg0=-bc+ad+b;
ii2=sat(si2,gg0)[1];
si2=std(ii2);
gg0=-bc+ad-d;
ii2=sat(si2,gg0)[1];
si2=std(ii2);
dim(si2);
size(si2);
si2;

list lll;
link lk="";
int iii1,iii2,iii3,iii4;
for(iii1=1;iii1<=13;iii1++){
for(iii2=iii1+1;iii2<=13;iii2++){
for(iii3=iii2+1;iii3<=13;iii3++){
for(iii4=iii3+1;iii4<=13;iii4++){
lll = iii1,iii2,iii3,iii4;
gg0 = inonecv(allnodes, lll);
if(reduce(gg0, si2)==0){
fprintf(lk,"[%s,%s,%s,%s]",iii1,iii2,iii3,iii4);
}
}
lll = iii1,iii2,iii3;
gg0 = ptinline(allnodes, lll);
if(reduce(gg0, si2)==0){
fprintf(lk,"{%s,%s,%s}", iii1,iii2,iii3);
}
}
}
}
ring r1=0, (a,b,c,d,k,l,m,n,o),lp;
ideal si2 = fetch(r0,si2);
si2=std(si2);
dim(si2);
si2;
