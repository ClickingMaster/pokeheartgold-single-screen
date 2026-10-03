#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x30];u32 value;}Work;extern void ov96_021EB03C(Work*,int,int,int,int);
void ov96_021EAF78(Work*w,int b,int c,int d,int e,u32*out);
void ov96_021EAF78(Work*w,int b,int c,int d,int e,u32*out){ov96_021EB03C(w,b,c,d,e);*out=w->value;}
