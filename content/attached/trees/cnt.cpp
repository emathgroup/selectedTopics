#include <gmpxx.h>
#include <iostream>
#include <string.h>
#include <map>
#define CR 7
#define CG 8
#define CD 9
#define CY 10
#define CB 11
#define MAXLEVEL (45-9)
enum COLORBEAN { RED, GREEN, DARK, YELLOW, BLUE};

struct STATUS{
    char nred, ngreen, nyellow, nblue;
    COLORBEAN colors;
    bool operator<(const STATUS& s)const{
	    if(colors <s.colors)return true;
	    if(colors>s.colors)return false;
	    if(nred < s.nred)return true;
	    if(nred > s.nred)return false;
	    if(ngreen < s.ngreen)return true;
	    if(ngreen > s.ngreen)return false;
	    if(nyellow < s.nyellow)return true;
	    if(nyellow > s.nyellow)return false;
	    return nblue < s.nblue;
    }
};

typedef std::map<STATUS, mpz_class> STATUS_COUNT;

STATUS_COUNT sc[MAXLEVEL];
int fac[10];
int main()
{
	int i;
	fac[0]=1;
	for(i=1;i<=9;i++)fac[i]=fac[i-1]*i;
	STATUS s;
	memset(&s,0,sizeof(s));
	//initialize for single BEAN
	s.nred=1;s.colors=RED;
	sc[0][s]=1;
	s.nred=0;
	s.ngreen=1;s.colors=GREEN;
	sc[0][s]=1;
	s.ngreen=0;
	s.nyellow=1;s.colors=YELLOW;
	sc[0][s]=1;
	s.nyellow=0;
	s.nblue=1;s.colors=BLUE;
	sc[0][s]=1;
	s.nblue=0;
	for(i=1;i<MAXLEVEL;i++){
		STATUS_COUNT::iterator sit;
#ifdef DEBUG
		std::cerr << "Level " <<i-1<<std::endl;
#endif
		for(sit = sc[i-1].begin(); sit!=sc[i-1].end();++sit){
#ifdef DEBUG
			std::cerr << (int)sit->first.nred<<','<<(int)sit->first.ngreen<<','<<(int)sit->first.nyellow<<','<<(int)sit->first.nblue<<','<<sit->first.colors<<':'<<sit->second<<std::endl;
#endif
			s=sit->first;
			STATUS s2;
			if(s.nred<CR&&s.colors!=GREEN){
				s2=s;
				s2.nred++;
				s2.colors=RED;//add a red bean
				sc[i][s2]+=sit->second;
			}
			if(s.ngreen<CG&&s.colors!=RED){
				s2=s;
				s2.ngreen++;
				s2.colors=GREEN;//add a green bean
				sc[i][s2]+=sit->second;
			}
			if(s.nyellow<CY&&s.colors!=BLUE){
				s2=s;
				s2.nyellow++;
				s2.colors=YELLOW;
				sc[i][s2]+=sit->second;
			}
			if(s.nblue<CB&&s.colors!=YELLOW){
				s2=s;
				s2.nblue++;
				s2.colors=BLUE;
				sc[i][s2]+=sit->second;
			}
		}
	}
	mpz_class sum=0;
	for(i=45-18-1;i<=45-9-1;i++){
		STATUS_COUNT::iterator sit;
		for(sit=sc[i].begin();sit!=sc[i].end();++sit){
			s=sit->first;
			int space = 45-1-i-9;
			int v=fac[space];
			if(CR-s.nred+CG-s.ngreen+CB-s.nblue+CY-s.nyellow!=space){
				std::cerr << "Invalid\n";
			}
			v/=fac[CR-s.nred];
			v/=fac[CG-s.ngreen];
			v/=fac[CB-s.nblue];
			v/=fac[CY-s.nyellow];
			sum+=sit->second *v;
		}

	}
	std::cout << sum<<std::endl;
	return 0;
}
