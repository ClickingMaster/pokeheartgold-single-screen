#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern void ov08_02224BC0(void*),ov12_0226BAFC(void*,int,int,int,int);void ov08_02224BCC(void*,u8*);
void ov08_02224BCC(void*w,u8*d){ov08_02224BC0(w);P(w,4)=d;*(int*)((u8*)w+12)=-1;if(*(u8*)((u8*)w+8)==1){d=P(w,4);ov12_0226BAFC(P(w,0),d[0],d[2],d[1],d[3]);}}
