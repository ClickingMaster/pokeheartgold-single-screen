#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Key{int size;u8*n;int aSize;u8*a;int bSize;u8*b;int pSize;u8*p;int qSize;u8*q;int cSize;u8*c;}Key;
extern void*(*_021D4634)(int);extern void(*_021D4660)(void*);
extern void CPSi_big_from_char(u16*,const u8*,int,int),CPSi_big_montpower(u16*,u16*,u16*,int,u16*),CPSi_big_sub(u16*,u16*,u16*,int),CPSi_big_mult(u16*,u16*,u16*,int),CPSi_big_add(u16*,u16*,u16*,int),CPSi_big_negate(u16*,int),CPSi_big_div(u16*,u16*,u16*,u16*,int,u16*),CPSi_char_from_big(u8*,u16*,int,int);
extern int enter_computebound(void),CPSi_big_sign(u16*,int);extern void exit_computebound(int);
void decrypt_premaster_secret(u8*,const u8*,Key*);
void decrypt_premaster_secret(u8*out,const u8*input,Key*k){int len,token;u16*a,*b,*c,*d,*e,*f,*g,*h;if(!k||!k->size)return;len=((k->size<<1)/2)+1;a=_021D4634(len*20);if(!a)return;b=a+len;c=b+len;d=c+len;e=d+len;f=e+len;g=f+len;h=g+len;
 CPSi_big_from_char(a,input,k->size,len);CPSi_big_from_char(b,k->p,k->pSize,len);CPSi_big_from_char(f,k->a,k->aSize,len);token=enter_computebound();CPSi_big_montpower(d,a,b,len,f);CPSi_big_from_char(b,k->q,k->qSize,len);CPSi_big_from_char(f,k->b,k->bSize,len);CPSi_big_montpower(e,a,b,len,f);exit_computebound(token);CPSi_big_sub(a,d,e,len);CPSi_big_from_char(b,k->c,k->cSize,len);CPSi_big_mult(c,a,b,len);CPSi_big_from_char(b,k->b,k->bSize,len);CPSi_big_mult(a,c,b,len);CPSi_big_add(c,a,e,len);CPSi_big_from_char(b,k->n,k->size,len);
 if(CPSi_big_sign(c,len)<0){CPSi_big_negate(c,len);CPSi_big_div(0,c,b,g,len,h);CPSi_big_sub(g,b,g,len);}else CPSi_big_div(0,c,b,g,len,h);CPSi_char_from_big(out,g,48,len);_021D4660(a);
}
