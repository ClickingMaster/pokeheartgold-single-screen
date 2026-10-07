#pragma force_active on
#include "global.h"
#pragma force_active on
extern void*PokeathlonCourse_GetDataCopyArea(void*);
extern u8*ov96_021E8A20(void*);
extern int PokeathlonCourse_GetField3D8_AtIndex(void*,int);
extern void*PokeathlonCourse_GetGraphicsSystem(void*);
typedef struct Ov96R18Work Ov96R18Work;
extern u32 ov96_021E95D8(Ov96R18Work*,int,int);
u8 ov96_021ED6F8(void*,u8);
u32 ov96_021ED728(void*,u8,u8);
u8 ov96_021ED6F8(void*w,u8 index){void*p=PokeathlonCourse_GetDataCopyArea(w);u8*q=ov96_021E8A20((u8*)p+0xf0);if(w==0)GF_AssertFail();if(p==0)GF_AssertFail();if(q==0)GF_AssertFail();return q[index];}
u32 ov96_021ED728(void*w,u8 a,u8 b){u8 index=PokeathlonCourse_GetField3D8_AtIndex(w,b);return ov96_021E95D8(PokeathlonCourse_GetGraphicsSystem(w),index,a);}
