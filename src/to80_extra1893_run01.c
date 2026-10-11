#pragma force_active on
#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x14];void*list;} Work;
extern Work*PokeathlonCourse_GetHeapAllocPtr4(void*);extern void*PokeathlonCourseMonObjects_Get(void*,u8);extern void PokeathlonCourseMonObject_SetDrawFlag(void*,int),ov96_021EC790(void*,int);
void ov96_021ECA70(void*,int);
void ov96_021ECA70(void*course,int arg){Work*w=PokeathlonCourse_GetHeapAllocPtr4(course);u32 i;void*x,*y;i=0;for(;(s32)i<4;i++){x=PokeathlonCourseMonObjects_Get(w->list,(u8)(3*i+1));y=PokeathlonCourseMonObjects_Get(w->list,(u8)(3*i+2));PokeathlonCourseMonObject_SetDrawFlag(x,0);PokeathlonCourseMonObject_SetDrawFlag(y,0);}ov96_021EC790(course,arg);}
