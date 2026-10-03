#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{s8 sign;u8 pad;s16 exp;u8 len,digits[32],pad2;} Decimal;
int __signbitd(double);int __fpclassifyd(double);double frexp(double,int*);double ldexp(double,int);int __msl_generic_count_bits64(u64);void __two_exp(Decimal*,int);void __ull2dec(Decimal*,u64);void __timesdec(Decimal*,Decimal*,Decimal*);
void __num2dec_internal(Decimal*,double);
void __num2dec_internal(Decimal*out,double value){s8 sign=__signbitd(value)!=0;int exponent;union{double d;u64 bits;}u;Decimal temp,scale;double frac;u64 mant;int bits;if(value==0.0){out->sign=sign;out->exp=0;out->len=1;out->digits[0]=0;return;}if(__fpclassifyd(value)<=2){out->sign=sign;out->exp=0;out->len=1;out->digits[0]=__fpclassifyd(value)==1?'N':'I';return;}if(sign)value=0.0-value;frac=frexp(value,&exponent);u.d=frac;mant=u.bits|((u64)1<<52);bits=53-__msl_generic_count_bits64((mant&-mant)-1);__two_exp(&scale,exponent-bits);__ull2dec(&temp,(u64)ldexp(frac,bits));__timesdec(out,&temp,&scale);out->sign=sign;}
