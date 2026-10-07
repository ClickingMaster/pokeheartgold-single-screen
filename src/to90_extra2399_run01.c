#pragma force_active on
#pragma force_active on
#include "global.h"
typedef struct{void*arg;u8 p[0x59-4];u8 a,b,pad;u8 index;}Work;
extern void *PokeathlonCourse_GetGraphicsSystem(void*);extern int ov96_021E9524(void*);extern int ov96_021E94EC(void*,u8);extern void ov96_021F8830(void*,int,int,int);static inline int greater(u32 a,u32 b){return a>b;}u32 ov96_021F8360(Work*);void ov96_021F8378(Work*,void*,int,int);
u32 ov96_021F8360(Work*w){u32 result=0;if((u32)w->a>(u32)w->b)result=1;return result;}
