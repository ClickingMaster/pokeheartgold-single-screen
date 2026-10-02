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
void __str2dec(struct Dec *dec,const char *text,s16 exponent) {int i=0,c;dec->exponent=exponent;dec->sign=0;while(i<32 && *text){dec->digits[i++]=*text++-'0';}dec->length=i;c=*text;if(c==0)return;if(c<5)return;if(c>5)goto round;c=text[1];text++;if(c){do{if(c!='0')goto round;c=*++text;}while(c);}if(!(dec->digits[i-1]&1))return;round:__dorounddecup(dec,dec->length);}
