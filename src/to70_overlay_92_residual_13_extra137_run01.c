#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u32 a,b,c,d,e,f;u8 p18[0x2228-24];u32 copy;void*ptr;u8 p2230[0x2aec-0x2230];u32 value;u8 p2af0[0x2b94-0x2af0];u32 zero;u8 p2b98[32];u32 last;};void*Heap_Alloc(int,u32);void*memset(void*,int,u32);void ov92_0225EBE0(struct S*);struct S*ov92_0225EB88(u32,u32,u32);
struct S*ov92_0225EB88(u32 a,u32 b,u32 c){struct S*s=Heap_Alloc(113,0x2bbc);memset(s,0,0x2bbc);s->b=a;s->a=b;s->c=0;s->d=0;s->f=c;s->last=0;s->copy=s->value;s->ptr=&s->value;s->zero=0;ov92_0225EBE0(s);return s;}
