#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
extern void*(*_021D4634)(u32);extern void(*_021D4660)(void*);
extern int count_digits(const u16*,int);extern void CPSi_big_modinv(u16*,const u16*,const u16*,int,u16*),CPSi_big_mult(u16*,const u16*,const u16*,int),CPSi_big_sub_small(u16*,const u16*,int,int),CPSi_big_div(u16*,const u16*,const u16*,u16*,int,u16*),CPSi_big_montmult(u16*,u16*,const u16*,int,int,const u16*,u16*,u16*,u16*);
void CPSi_big_montpower(u16*,const u16*,const u16*,int,const u16*);
void CPSi_big_montpower(u16*out,const u16*base,const u16*exp,int n,const u16*mod){int digits,i;u16 *scratch,*inv,*prod,*a,*p,*tmp,*b;u32 total;p=_021D4634(n*22);if(p==0)return;MI_CpuFill8(p,0,n*22);inv=p+n;tmp=inv+n;prod=tmp+n;a=prod+n;b=a+n;scratch=b+n;digits=count_digits(mod,n);p[digits]=1;CPSi_big_modinv(inv,p,mod,n,tmp);CPSi_big_mult(prod,p,inv,n);CPSi_big_sub_small(inv,prod,1,n);CPSi_big_div(inv,inv,mod,0,n,scratch);CPSi_big_mult(tmp,base,p,n);CPSi_big_div(0,tmp,mod,tmp,n,scratch);CPSi_big_div(0,p,mod,out,n,scratch);i=0;total=digits<<4;for(;i<total;i++){CPSi_big_montmult(out,b,(u16*)1,n,digits,mod,inv,prod,a);if(exp[digits-(i>>4)-1]&(0x8000U>>(i&15)))CPSi_big_montmult(out,b,tmp,n,digits,mod,inv,prod,a);}CPSi_big_montmult(out,b,0,n,digits,mod,inv,prod,a);_021D4660(p);}
