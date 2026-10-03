#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern double copysign(double,double),pow(double,double),ldexp(double,int);
typedef struct {s8 sign,pad;s16 exponent;u8 length,digits[32];} Dec;
typedef union {double d;u64 bits;} Bits;
extern void __str2dec(Dec *,const char *,s16),__num2dec_internal(Dec *,double),__minus_dec(Dec *,const Dec *,const Dec *);
extern int __equals_dec(const Dec *,const Dec *),__less_dec(const Dec *,const Dec *),__fpclassifyd(double);
#pragma force_active on
#pragma dollar_identifiers on
extern const char fp_str$6F5C[];
extern const double digit_values[];
extern float __float_huge;
double __dec2num(const Dec *);
double __dec2num(const Dec *dec) {
 Dec input,limit,low,high,temp,delta;Bits nan;double approx;Bits adjacent;
 u8 *end,*next;u64 *step;struct {unsigned int digits;int count;} group;int exponent,j,up,down;
 if(dec->length==0)return copysign(0.0,dec->sign==0?1.0:-1.0);
 switch(dec->digits[0]) {
 case '0':return copysign(0.0,dec->sign==0?1.0:-1.0);
 case 'I':return copysign((double)__float_huge,dec->sign==0?1.0:-1.0);
 case 'N':{u64 *bits=&nan.bits;u32 *words=(u32 *)bits;words[0]=0;words[1]=0x7ff00000;if(dec->sign)*bits|=0x8000000000000000ULL;*bits|=0x8000000000000ULL;return nan.d;}
 }
 input=*dec;next=input.digits;end=next+input.length;while(next<end){*next-='0';next++;}
 input.exponent+=input.length-1;exponent=input.exponent;
 __str2dec(&limit,fp_str$6F5C,308);
 if(__less_dec(&limit,&input))return copysign((double)__float_huge,dec->sign==0?1.0:-1.0);
 next=input.digits;approx=(u32)*next++;
 while(next<end){
  double product,added;
  group.count=(end-next)%8;group.digits=0;if(group.count==0)group.count=8;
  for(j=0;j<group.count;j++){group.digits*=10;group.digits+=*next++;}
  product=approx*digit_values[group.count-1];added=product+(double)group.digits;
  if(group.digits!=0&&product==added)break;
  approx=added;exponent-=group.count;
 }
 if(exponent<0)approx/=pow(5.0,-exponent);else approx*=pow(5.0,exponent);
 approx=ldexp(approx,exponent);
 if(__fpclassifyd(approx)==2){(*(u64 *)&approx)=0x7fefffffffffffffULL;}
 step=&adjacent.bits;up=0;__num2dec_internal(&low,approx);
 if(!__equals_dec(&low,&input)){
  if(__less_dec(&low,&input))up=1;adjacent.d=approx;down=(up==0)?1:0;
  for(;;){
   if(!down){(*step)++;if(__fpclassifyd(adjacent.d)==2)goto done;}else (*step)--;
   __num2dec_internal(&high,adjacent.d);
   if(up&& !__less_dec(&high,&input))break;
   if(!up&&!__less_dec(&input,&high)){
    double swap;temp=low;low=high;high=temp;swap=approx;approx=adjacent.d;adjacent.d=swap;break;
   }
   low=high;approx=adjacent.d;
  }
  __minus_dec(&temp,&input,&low);__minus_dec(&delta,&high,&input);
  if(__equals_dec(&temp,&delta)){if((*(u64 *)&approx)&1)approx=adjacent.d;}
  else if(!__less_dec(&temp,&delta))approx=adjacent.d;
 }
 done:if(input.sign)approx=0.0-approx;return approx;
}
