#ifdef USE_GMP
#include <gmp.h>
#else
const int P=1073741783;
signed extGCD(signed a, signed b, signed & ar, signed & br)
{
	signed x = 0, y = 1;
	ar = 1, br = 0;
	signed t, q;
	while (b != 0)
	{
		t = b; q = a / b; b = a % b; a = t;
		t = x; x = ar - q * x; ar = t;
		t = y; y = br - q * y; br = t;
	}
	return (a);
}

class fpn{
	signed v;
	public:
	fpn(){v=0;}
	fpn(const fpn& n){v=n.v;}
	fpn(signed x){v=x%P;if(v<0)v+=P;}
	fpn& operator+=(const fpn& n);
	fpn& operator-=(const fpn& n);
	fpn& operator*=(const fpn& n);
	fpn& operator/=(const fpn& n);
	fpn operator+(const fpn& n)const{fpn x(*this);return x+=n;}
	fpn operator-(const fpn& n)const{fpn x(*this);return x-=n;}
	fpn operator*(const fpn& n)const{fpn x(*this);return x*=n;}
	fpn operator/(const fpn& n)const{fpn x(*this);return x/=n;}
	bool isZero()const{return v==0;}
	bool isOne()const{return v==1;}
	bool operator==(const fpn& n)const;
	bool operator!=(const fpn& n)const;
	fpn operator++(int){fpn f(*this);*this+=fpn(1);return f;}
	fpn& operator++(){return *this+=fpn(1);}
	fpn operator--(int){fpn f(*this);*this-=fpn(1);return f;}
	fpn& operator--(){return *this-=fpn(1);}
	fpn inv()const;
	fpn neg()const{fpn n;if(v!=0)n.v=P-v;else n.v=0;return n;}
	fpn& operator=(const fpn& n){v=n.v;return *this;}
        signed getv()const{return v;}
};

fpn& fpn::operator+=(const fpn& n)
{
	if(v>=P-n.v){
		v=v+n.v-P;
	}else{
		v+=n.v;
	}
	return *this;
}

fpn& fpn::operator-=(const fpn& n)
{
	if(v>=n.v)
		v-=n.v;
	else
		v=v+P-n.v;
	return *this;
}

fpn& fpn::operator*=(const fpn& n)
{
	unsigned long long t = (unsigned long long)v * (unsigned long long) n.v;
	t%=P;
	v=(unsigned)t;
	return *this;
}

fpn& fpn::operator/=(const fpn& n)
{
	return (*this) *=n.inv();
}

fpn fpn::inv()const
{
	signed ar,br;
	extGCD(v, P, ar, br);
	return ar;
}

bool fpn::operator==(const fpn& n)const
{
	return v==n.v;
}

bool fpn::operator!=(const fpn& n)const
{
	return v!=n.v;
}

typedef fpn mpq_t;
inline void mpq_init(mpq_t& x){
    x=0;
}
inline int mpq_sgn(const mpq_t& x){
   return x.isZero()?0:1;
}
inline void mpq_set(mpq_t& x, const mpq_t& y){
   x=y;
}

inline void mpq_set_ui(mpq_t& x, unsigned a, unsigned b){
    fpn u(a), v(b);
    x=u/v;
}
inline void mpq_mul(mpq_t& x, const mpq_t& a, const mpq_t& b){
    x=a*b;
}
inline void mpq_sub(mpq_t& x, const mpq_t& a, const mpq_t& b){
    x=a-b;
}
inline void mpq_div(mpq_t& x, const mpq_t& a, const mpq_t& b){
    x=a/b;
}
inline void mpq_add(mpq_t& x, const mpq_t& a, const mpq_t& b){
    x=a+b;
}
inline void mpq_clear(mpq_t& x){
}
inline void mpq_out_str(FILE *f, int base, const mpq_t& a)
{
    unsigned v=a.getv();
    if(v<=P/2)fprintf(f,"%d",v);
    else fprintf(f,"%d",v-P);
}
inline int mpq_cmp(const mpq_t& a, const mpq_t& b){
    mpq_t x=a-b;
    return mpq_sgn(x);
}
inline void mpq_neg(mpq_t& x, const mpq_t& a){
    x=a.neg();
}
mpq_t modInv(mpq_t& x)
{
    return x.inv();
}
#endif
