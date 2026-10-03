#pragma force_active on
#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x14];void*list;} Work;
extern Work*PokeathlonCourse_GetHeapAllocPtr4(void*);extern void*ov96_021EAA04(void*,u8);extern void ov96_021EAB38(void*,int),ov96_021EC790(void*,int);
void ov96_021ECA70(void*,int);
void ov96_021ECA70(void*course,int arg){Work*w=PokeathlonCourse_GetHeapAllocPtr4(course);u32 i;void*x,*y;i=0;for(;(s32)i<4;i++){x=ov96_021EAA04(w->list,(u8)(3*i+1));y=ov96_021EAA04(w->list,(u8)(3*i+2));ov96_021EAB38(x,0);ov96_021EAB38(y,0);}ov96_021EC790(course,arg);}
