compless(a,b,errv)=
{
     if(a+errv<b, 1,
         if(a-errv>b, 0,
           print("Accuracy not enough in comparing"a"<"b "with error" errv);(a-b)<0
         )
     )
}

findbest(cftl,n,errv)=
{
    local(pnqn, cs,m,c,xp,d,dd,l5,l2,bt,u,bu,pass,eps,X, lgX, lgY,lg2,lg5,cerrv);
    m=10^n;bt=-1;
    X=floor(m/sqrt(10));
    lgX=log(X)/log(10)-n;
    lgY=log(X+1)/log(10)-n;
    lg2=log(2)/log(10);lg5=1-lg2;
    cerrv=10^(-n)*errv;
    pnqn=contfracpnqn(contfrac(log(5/2)/log(10)),cftl);
    cs=1;
    eps = 1.0/(X*log(10));
    c=log(5/2)/log(10);
    for(u=2,cftl,
        if(pnqn[2,u]>m, break());
        xp=-(pnqn[2,u]*c-pnqn[1,u])/(pnqn[2,u-1]*c-pnqn[1,u-1]);
        l2=(xp*pnqn[2,u-1]+pnqn[2,u])*eps;
        if(compless(0,l2,cerrv)&&compless(l2,xp,cerrv)&&compless(0,xp,cerrv)&&compless(xp,0.5,cerrv),cs=u)
    );
    xp = -(pnqn[2,cs]*c-pnqn[1,cs])/(pnqn[2,cs-1]*c-pnqn[1,cs-1]);
    d=(pnqn[2,cs]+xp*pnqn[2,cs-1])*eps;
    u=1;
    while(1,
        if(bt>0&&bt<pnqn[2,cs]*u+(xp-d/u)*u*pnqn[2,cs-1]+errv, break());
        for(v=floor((xp-d/u)*u),ceil((xp+d/u)*u),
           dd=pnqn[2,cs]*u+v*pnqn[2,cs-1];
           if(bt>0&&dd>=bt, break());
           l2=dd*lg2; l2-=floor(l2);l5=dd*lg5;l5-=floor(l5);
           if(compless(lgX+1,l2,cerrv)&&compless(l2,lgY+1,cerrv)&&compless(lgX+1,l5,cerrv)&&compless(l5,lgY+1,cerrv), bt=dd; bu=pnqn[1,cs]*u+v*pnqn[1,cs-1])
        );
        u=u+1
    );
    l2=contfrac(bu/bt);
    l5=contfrac(log(5/2)/log(10));pass=1;
    for(u=1,length(l2)-1,
       if(l2[u]!=l5[u] ,
          printf("Wang assumption fail for " n);pass=0;
          print(u":");for(v=u,length(l2),print(l2[v]));
       )
    );
    if(pass&&l2[length(l2)]>l5[length(l2)]+1,  printf("Wang assuption fail for "n));
    if(pass, print(n"{"length(l2)-1":"l5[length(l2)]"=>"l2[length(l2)]"}"gettime()));
    bt
}

\p 22000
gettime();
for(u=4,10000,findbest(30000,u,1E-100));

