#include <gmpxx.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>

#define LINEBUF 2000000
char line[LINEBUF];
#define MAXRANGE 1000000
long next[MAXRANGE];
void init_logs(mpf_class logs[10])
{
	FILE *f=fopen("log2.txt","r");
	if(f==NULL){
		fprintf(stderr,"fail to open file log2.txt");
		exit(-1);
	}
	if(fscanf(f,"%s",line)==1){
		mpf_class l2(line);
		logs[2]=l2;
	}else{
		fprintf(stderr,"fail to read log2\n");
		exit(-1);
	}
	fclose(f);
	f=fopen("log3.txt","r");
	if(fscanf(f,"%s",line)==1){
		logs[3]=line;
	}else{
		fprintf(stderr,"fail to read log3\n");
		exit(-1);
	}
	fclose(f);
	f=fopen("log5.txt","r");
	fscanf(f,"%s",line);
	mpf_class l5(line);
	logs[5]=l5;
	fclose(f);
	f=fopen("log7.txt","r");
	fscanf(f,"%s",line);
	mpf_class l7(line);
	logs[7]=l7;
	fclose(f);
	logs[4]=2*logs[2];
	logs[8]=3*logs[2];
	logs[6]=logs[2]+logs[3];
	logs[9]=2*logs[3];
}

int powlim[]={0x7FFFFFFF,0x7FFFFFFF,3321928,2095903,1660964,1430676,1285097,1183294,1107309,1047951};

long get_leading(mpz_class& x)
{
	int length = mpz_sizeinbase(x.get_mpz_t(),10);
	if(length>10){
		length-=10;
		mpz_class div, base(10);
		mpz_pow_ui(div.get_mpz_t(),base.get_mpz_t(),length);
		x=x/div;
	}
	return x.get_si();
}

long getleading(long x)
{
	char db[40];
	sprintf(db,"%ld",x);
	int len = strlen(db);
	long r=0;
	int i;
	if(x==0)return 0;
	for(i=0;i<len;i++){
		if(db[i]=='1'){
			if(i==0)return 1L;
			return r;
		}else if(db[i]=='0'){
			int j;int c=0;
			for(j=i;j<len;j++){
				if(db[j]=='0')c++;
				else break;
			}
			if(c&1){
				if(i<=1)return 1;
				else return r/10;
			}else return r;
		}
		r*=10;r+=db[i]-'0';
	}
	return r;
}

long nexterm(long x, mpf_class logs[10])
{
	char db[40];
	sprintf(db,"%ld",x);
	int len=strlen(db);
	if(len>=6)return 0;
	int i;
	for(i=0;i<len;i++){
		db[i]-='0';
	}
	mpz_class prod(db[len-1]);
	for(i=len-2;i>=0;i--){
		if(db[i]==0){
			if(prod==0){
				prod=1;
			}else{
				prod=0;
			}
		}else if(db[i]==1){
			prod=1;
		}else if(prod<=powlim[db[i]]){
			mpz_class a(db[i]);
			mpz_pow_ui(prod.get_mpz_t(), a.get_mpz_t(), prod.get_si());
		}else if(i==0){
			mpf_class logv = prod*logs[db[i]];
			mpz_class integer_part = (mpz_class)logv;
			logv-=integer_part;
			double d = mpf_get_d(logv.get_mpf_t());
			d=pow(10.0,d)*100000000000L;
			return (long)(dz);
		}else{
			return 0;
		}
	}
	return get_leading(prod);
}

int main()
{
	long prec = 3321928;
	mpf_set_default_prec(prec);
	mpf_class logs[10];
	init_logs(logs);
	int i;
//	getleading(nexterm(1000,logs));
	for(i=10;i<MAXRANGE;i++){
		next[i]=getleading(nexterm(i,logs));
	}
	for(i=10;i<MAXRANGE;i++){
		long x=i;
		int r=0;
		printf("%ld",x);
		while(x>=10&&x!=5243&&x<MAXRANGE){
			x=next[x];
			printf("=>%ld",x);
			r++;
		}
		if(x>0&&x<10||x==5243){
			printf("=>%ld(A%d)\n",x,r+1);
		}else{
			printf("=>overflow(O%d)\n",r+1);
		}
	}
	return 0;
}
