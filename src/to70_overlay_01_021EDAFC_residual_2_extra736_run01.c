#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x97];u8 pad:6;u8 flagA:1;u8 flagB:1;u8 a,b,c,d;}Work;
extern u32 ov01_021EE2E4(Work*);extern void ov01_021EE01C(Work*,u32);
void ov01_021EDFA4(Work*);
void ov01_021EDFA4(Work*w){u32 count=ov01_021EE2E4(w),n;if((count&7)==0)n=count>>3;else n=(count>>3)+1;if(w->flagA)w->a-=n;if(w->flagB){if(w->d>8)w->b-=16;else w->b-=w->d*2;}ov01_021EE01C(w,n);}
