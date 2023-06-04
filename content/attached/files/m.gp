matchinglen(a,b)=
{
    local(m);
    m=0;
    while(floor(a*10)==floor(b*10),
        m=m+1; a*=10;a-=floor(a);b*=10;b-=floor(b);
    );
    m
}

getm(n)=
{
    local(u1,v1,u2,v2,u,v,r,m,cm,l5,l2);
    u1=0;v1=1;u2=1;v2=1;m=0;
    for(h=1,n,
        u=u1+u2;v=v1+v2;
        r=log(2)/log(10)*v;
        if(abs(r-floor(r)-0.47)<0.1,
           l5=(log(5)/log(10)*v);l5-=floor(l5);
           l2=(log(2)/log(10)*v);l2-=floor(l2);
           cm=matchinglen(10.^l2,10.^l5);
           if(cm>m, m=cm;
              print(cm+1" "v)
           )
        );
        if((u+v)*log(2)>(v-u)*log(5),
            u2=u;v2=v,
            u1=u;v1=v
        )
    )
}

getconft(n)=
{
   contfracpnqn(contfrac(log(5/2)/log(10)),n)
}

findbest(cftl,n)=
{
    local(pnqn, cs,m,c,xp,d,dd,l5,l2,bt,u,bu,pass,eps);
    m=10^n;bt=-1;
    pnqn=getconft(cftl);cs=1;
    eps = -log(1-10^(-(n-1)))/log(10);
    c=log(5/2)/log(10);
    for(u=2,cftl,
        if(pnqn[2,u]>m, break());
        xp=-(pnqn[2,u]*c-pnqn[1,u])/(pnqn[2,u-1]*c-pnqn[1,u-1]);
        l2=(xp*pnqn[2,u-1]+pnqn[2,u])*eps;
        if(0<l2&&l2<xp&&0<xp&&xp<0.5,cs=u)
    );
    xp = -(pnqn[2,cs]*c-pnqn[1,cs])/(pnqn[2,cs-1]*c-pnqn[1,cs-1]);
    d=(pnqn[2,cs]+xp*pnqn[2,cs-1])*eps;
    u=1;
    while(1,
        if(bt>0&&pnqn[2,cs]*u+(xp-d/u)*u*pnqn[2,cs-1]>bt, break());
        for(v=ceil((xp-d/u)*u),floor((xp+d/u)*u),
           dd=pnqn[2,cs]*u+v*pnqn[2,cs-1];
           if(bt>0&&dd>=bt, break());
           l5=(log(5)/log(10)*dd);l5-=floor(l5);
           l2=(log(2)/log(10)*dd);l2-=floor(l2);
           if((md=matchinglen(10.^l2,10.^l5))>=n-1, bt=dd; bu=pnqn[1,cs]*u+v*pnqn[1,cs-1])
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
    if(pass, print(length(l2)":"l5[length(l2)]"=>" l2[length(l2)]));
    bt
}

\p 2000

for(u=8,1000, print(u " " findbest(3000,u) ));

