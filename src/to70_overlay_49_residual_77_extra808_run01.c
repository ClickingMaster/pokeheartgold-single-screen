#pragma force_active on
#include "global.h"
typedef struct{void*a,*b;u16 c,count;u32 d;}Work;extern void*Heap_Alloc(int,int),*ov45_0222B1DC(void*);extern u16 ov45_0222A35C(void*);extern int ov45_0222B1EC(void*);extern void ov49_0225E624(void*,int),ov49_02268D94(Work*),ov49_02268C74(Work*,void*,int);
Work*ov49_02268A0C(void*a,void*b,int heap);
Work*ov49_02268A0C(void*a,void*b,int heap){Work*w=Heap_Alloc(heap,16);void*p;u8*cursor=(u8*)w;u32 n=16;do{*cursor++=0;}while(--n);w->a=a;w->b=b;p=ov45_0222B1DC(a);w->count=ov45_0222A35C(a);if(w->count && p){ov49_0225E624(b,1);ov49_02268D94(w);ov49_02268C74(w,p,ov45_0222B1EC(a));}return w;}
