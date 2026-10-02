#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
struct Dec {u8 sign,pad;s16 exponent;u8 length,digits[32];};
static inline int Minimum(int a,int b) {return a>b?b:a;}
static inline int IsZero(const struct Dec *d) {return d->digits[0]==0;}
int __must_round(struct Dec *,int);
void __dorounddecup(struct Dec *,int);
void __rounddec(struct Dec *,int);
void __ull2dec(struct Dec *,u64);
void __timesdec(struct Dec *,const struct Dec *,const struct Dec *);
void __str2dec(struct Dec *,const char *,s16);
int __equals_dec(const struct Dec *,const struct Dec *);
int __less_dec(const struct Dec *,const struct Dec *);
extern void __num2dec_internal(struct Dec *,double);
void __num2dec(const void *,double,struct Dec *);
int __must_round(struct Dec *dec,int length) {u8 *p=dec->digits+length,*end;if(*p>5)return 1;if(*p<5)return -1;end=dec->digits+dec->length;p++;while(p<end) {if(*p)return 1;p++;}return dec->digits[length-1]&1?1:-1;}
void __dorounddecup(struct Dec *dec,int length) {u8 *begin=dec->digits,*p=begin+length-1;for(;;) {if(*p<9){++*p;return;}if(p==begin){*p=1;dec->exponent++;return;}*p--=0;}}
void __rounddec(struct Dec *dec,int length) {int round;if(length<=0)return;if(length>=dec->length)return;round=__must_round(dec,length);dec->length=length;if(round<0)return;__dorounddecup(dec,length);}
void __ull2dec(struct Dec *dec,u64 value) {u8 *first,*last;dec->sign=0;dec->length=0;while(value!=0) {int index=dec->length++;dec->digits[index]=value%10;value/=10;}first=dec->digits;last=first+dec->length-1;while(first<last){u8 a=*last,b=*first;*first++=a;*last--=b;}dec->exponent=dec->length-1;}
