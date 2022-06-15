pup(k)={
    if(k>=688383,
         k*(log(k)+log(log(k))-1+(log(log(k))-2)/log(k)),
         k*log(k*log(k))
    )
}

plb(k)={
    k*(log(k)+log(log(k))-1+(log(log(k))-2.1)/log(k))
}

piup(x)={
   x/log(x)*(1+1/log(x)+2/log(x)^2+7.59/log(x)^3)
}

findl(TP)={
     local(logp2TP, p2TP, C, F, T);
     local(ph,lb,ub,ub2,lb2, pls, plogs, m2,m5,p2o,p5o, j1, j2,s1,s2, v, pres,s,sc,u2,u5,ec,es1,es2, pst,ped);
     p2TP = pup(2*TP);ec=0;ise=0;pst=-1;ped=-1;
     logp2TP = log(p2TP);
     C=ceil(p2TP^(1.0/5));
     print("C=" C);
     pls = primes(C);
     p2o=p5o=0;
     for(i=1,C,
         v=logp2TP/log(pls[i]);
         m2=floor(log(2.0*v)/log(2));
         m5=floor(log(5.0*v)/log(5));
         if(m2<=0,m2=1);if(m5<=0,m5=1);
         p2o++;p5o+=m5;
     );
     F=C+TP-p5o;
     T=F+TP-p2o;u2=u5=0;
     print("F=" F "; T="T"; N="TP);
     pres="";sc=0;
     for(i=1,C,
           ph=pls[i];
           plogs=log(pls[i]);
           lb = log(plb(T))/plogs;
           ub = 5*log(pup(TP))/plogs;
           lb = log(lb)/log(5);ub=log(ub)/log(5);
           ub2 = logp2TP/plogs;
           ub2 = log(ub2)/log(5);
           lb2 = log(plb(F))/plogs;
           lb2 = log(lb2)/log(5);
           if(ub2<1.0, if(pst>=0&&ped==-1,ped=i);break());
           j1=ceil(lb);j2=ceil(lb2);
           s1=5^j1-1;s2=2*5^j2-1;
           if(j1<ub && j2<ub2,
                   u5+=j1;es1=s1;es2=s2;
                   s=concat(concat(concat(concat("{",Str(s1)),","),Str(s2)),"}");
                   if(s!=pres,ec=ec+1;pst=i),
                   if(j1<ub,
                      s=s1;u5+=j1, 
                      if(j2<ub2,
                          u5+=j2;u2+=1;s=s2,
                          s="invalid"
                      )
                   )
           );
           if(s==pres,
                sc=sc+1,
                if(pst>=0 && ped==-1, ped=i-1);
                if(i>1,
                     print1(sc "*" pres ",")
                );sc=1;pres=s
           )
     );
     if(pst>=0&&ped==-1, ped=C);
     if(sc>0,
           print1(sc"*"pres","));
     u5=TP-u5;u2=TP-u2;
     print(u5"*4,"u2"*1")
}

