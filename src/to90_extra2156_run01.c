#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern int count_digits(const u16 *,int);
extern u64 get48bits_3(const u16 *),get48bits_2(const u16 *),get48bits_1(const u16 *),get64bits(const u16 *);
extern void CPSi_big_mult_small(u16 *,const u16 *,u16,int),CPSi_big_sub(u16 *,const u16 *,const u16 *,int);
extern int CPSi_big_compare(const u16 *,const u16 *,int);
void CPSi_big_div(u16 *,const u16 *,const u16 *,u16 *,int,u16 *);
#pragma force_active on
void CPSi_big_div(u16 *quotient,const u16 *numerator,const u16 *denominator,u16 *remainder,int digits,u16 *scratch){
 int shift;u32 estimate;u16 *q=scratch+digits,*r=q+digits;int denominatorDigits,numeratorDigits;u64 divisor;
 MI_CpuFill8(q,0,digits*4);numeratorDigits=count_digits(numerator,digits);denominatorDigits=count_digits(denominator,digits);
 if(numeratorDigits>0&&denominatorDigits>0){
  shift=digits-numeratorDigits+denominatorDigits-1;
  if(shift>=digits){MI_CpuCopy8(numerator,r,digits*2);goto finish;}
  MI_CpuCopy8(numerator,q+shift,numeratorDigits*2);
  if(denominatorDigits>2)divisor=get48bits_3(denominator+denominatorDigits-1);
  else if(denominatorDigits>1)divisor=get48bits_2(denominator+denominatorDigits-1);
  else divisor=get48bits_1(denominator+denominatorDigits-1);
  for(;shift<digits;shift++){
   memmove(q+1,q,(digits*2-1)*2);estimate=(u32)(get64bits(r+denominatorDigits)/divisor);if(estimate>65535)estimate=65535;
   for(;;){CPSi_big_mult_small(scratch,denominator,(u16)estimate,digits);if(CPSi_big_compare(r,scratch,digits)>=0)break;estimate--;}
   CPSi_big_sub(r,r,scratch,digits);q[0]=(u16)estimate;
  }
 }
finish:if(quotient)MI_CpuCopy8(q,quotient,digits*2);if(remainder)MI_CpuCopy8(r,remainder,digits*2);
}
