compless(a,b,errv)=
{
     if(a+errv<b, 1,
         if(a-errv>b, 0,
           print("Accuracy not enough in comparing"a"<"b "with error" errv);(a-b)<0
         )
     )
}

findbest(cftl,N,errv)=
{
    local(pnqn, ft, n, cs,m,c,xp,yp, d,dd,l2,bt,u,u2,bu,pass,eps,X, lgX, lgY,lg2,lg5,cerrv,gn,s, qs,qsm1,ps,psm1,rr);
    c = log(2)/log(10);
    ft = contfrac(c);
    pnqn = contfracpnqn(ft, cftl);
    cs=2;rr=[];
    gettime();
    for(n=4, N,
       m=10^n;bt=-1;
       X=floor(m/sqrt(10));
       lgX=log(X)/log(10)-n+1/2;
       lgY=log(X+1)/log(10)-n+1/2;
       gn = -lgX;if(gn>lgY,gn=lgY);
       cerrv=10^(-n)*errv;
       eps = 2*gn;
       for(u=cs+1,cftl,
          if(pnqn[2,u]>m, break());
          xp=-(pnqn[2,u]*c-pnqn[1,u])/(pnqn[2,u-1]*c-pnqn[1,u-1]);
          l2=(xp*pnqn[2,u-1]+pnqn[2,u])*eps;
          if(compless(0,xp,cerrv)&&compless(l2,xp, cerrv)&&compless(l2,1-xp,cerrv),cs=u)
       );
       xp = -(pnqn[2,cs]*c-pnqn[1,cs])/(pnqn[2,cs-1]*c-pnqn[1,cs-1]);
       yp = xp; if(1-xp<yp,yp=1-xp;s=1,s=0);
       d=(pnqn[2,cs]+xp*pnqn[2,cs-1])*eps;
       qs=bitand(pnqn[2,cs],1);qsm1=bitand(pnqn[2,cs-1],1);
       ps=bitand(pnqn[1,cs],1);psm1=bitand(pnqn[1,cs-1],1);
       u=1;
       while(1,
           v=floor(u/yp); if(s==1,u2=v-u,u2=u);
           if(bitand(v*ps+u2*psm1,1)==1&&bitand(v*qs+u2*qsm1,1)==0,
               if(compless(u,v*yp+d,errv), 
                    bt=pnqn[2,cs]*v+pnqn[2,cs-1]*u2; bu = pnqn[1,cs]*v+u2*pnqn[1,cs-1];break())
           );
           v=v+1; if(s==1,u2=v-u,u2=u);
           if(bitand(v*ps+u2*psm1,1)==1&&bitand(v*qs+u2*qsm1,1)==0,
              if(compless(v*yp-d,u,errv),
                   bt=pnqn[2,cs]*v+pnqn[2,cs-1]*u2; bu = pnqn[1,cs]*v+u2*pnqn[1,cs-1];break())
           );
           u=u+1
        );
        print(n" "bt/2)
    );
}

\p 22000
findbest(30000,10000,1E-100);

