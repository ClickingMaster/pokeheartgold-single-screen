#pragma force_active on
#include "global.h"
typedef struct {u8 p[20];u32 f:20;u32 active:1;u32 rest:11;} Slot;typedef struct {u32 a;Slot slots[15];void*b,*c;} Work;
extern void ov96_021ECDEC(Slot*,void*,void*);
void ov96_021ED17C(Work*,int);
void ov96_021ED17C(Work*w,int n){int count=0,next;volatile int i;u32*flags;Slot*p;if(w==0)GF_AssertFail();i=0;flags=(u32*)((u8*)w+24);p=w->slots;do{if(((*flags<<11)>>31)==0){ov96_021ECDEC(p,w->b,w->c);if(++count>=n)break;}next=i+1;flags+=6;p++;i=next;}while(next<15);}
