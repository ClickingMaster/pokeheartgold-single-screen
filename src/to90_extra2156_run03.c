#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
int CPSi_big_sign(u16*,int);
void CPSi_big_div(u16*,u16*,u16*,u16*,int,u16*),CPSi_big_mult(u16*,u16*,u16*,int),CPSi_big_sub(u16*,u16*,u16*,int),CPSi_big_add(u16*,u16*,u16*,int);
void CPSi_big_modinv(u16*,u16*,u16*,int,u16*);
void CPSi_big_modinv(u16*out,u16*input,u16*mod,int n,u16*scratch){u16 *quotient=scratch+n,*dividend=quotient+n,*coefficient=dividend+n,*previous=coefficient+n,*remainder=previous+n,*extra=remainder+n;int bytes=n*2;
 MI_CpuCopy8(input,scratch,bytes);MI_CpuCopy8(mod,dividend,bytes);*(u16*)((u8*)dividend+bytes)=1;
 while(CPSi_big_sign(scratch,n)>0){CPSi_big_div(quotient,dividend,scratch,remainder,n,extra);MI_CpuCopy8(scratch,dividend,bytes);MI_CpuCopy8(remainder,scratch,bytes);CPSi_big_mult(remainder,quotient,coefficient,n);CPSi_big_sub(remainder,previous,remainder,n);MI_CpuCopy8(coefficient,previous,bytes);MI_CpuCopy8(remainder,coefficient,bytes);}
 CPSi_big_add(previous,previous,mod,n);CPSi_big_div(0,previous,mod,out,n,extra);
}
