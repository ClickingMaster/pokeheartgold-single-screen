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
void __num2dec(const void *form,double value,struct Dec *dec) {int precision=*(s16 *)((u8 *)form+2),i;__num2dec_internal(dec,value);if(dec->digits[0]>9)return;if(precision>32)precision=32;__rounddec(dec,precision);while(dec->length<precision){int index=dec->length++;dec->digits[index]=0;}dec->exponent-=dec->length-1;for(i=0;i<dec->length;i++)dec->digits[i]+='0';}
