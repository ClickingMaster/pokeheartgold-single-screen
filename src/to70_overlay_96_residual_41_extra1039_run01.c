#pragma force_active on
#include "global.h"
#pragma force_active on
extern void*PokeathlonCourse_GetDataCopyArea(void*);
extern u8*ov96_021E8A20(void*);
extern int PokeathlonCourse_GetField3D8_AtIndex(void*,int);
extern void*PokeathlonCourse_GetGraphicsSystem(void*);
extern void ov96_021E95D8(void*,int,int);
u8 ov96_021ED6F8(void*,int);
void ov96_021ED728(void*,int,int);
u8 ov96_021ED6F8(void*w,int index){void*p=PokeathlonCourse_GetDataCopyArea(w);u8*q=ov96_021E8A20((u8*)p+0xf0);if(w==0)GF_AssertFail();if(p==0)GF_AssertFail();if(q==0)GF_AssertFail();return q[index];}
void ov96_021ED728(void*w,int a,int b){u8 index=PokeathlonCourse_GetField3D8_AtIndex(w,b);ov96_021E95D8(PokeathlonCourse_GetGraphicsSystem(w),index,a);}
