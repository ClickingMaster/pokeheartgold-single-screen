#pragma force_active on
#include "global.h"
#pragma force_active on
extern u8*PokeathlonCourse_GetHeapAllocPtr4(void*);
u32 ov96_02215E94(void*w,int i);
u32 ov96_02215E94(void*w,int i){u8*p=PokeathlonCourse_GetHeapAllocPtr4(w)+i*168;return (*(u32*)(p+0x204)<<5)>>31;}
