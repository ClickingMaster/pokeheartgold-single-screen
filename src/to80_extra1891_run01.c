#pragma force_active on
#include "global.h"
typedef struct{u32 heap;void*course;u8 entries[3][12];volatile u8 count;}Work;
extern int PokeathlonCourse_GetParticipantCount(void*);extern void*Heap_Alloc(u32,u32);extern void ov96_0221A00C(void*,u8,void*);Work*ov96_02219F7C(void*,u32);
Work*ov96_02219F7C(void*course,u32 heap){int i,n;Work*w;n=PokeathlonCourse_GetParticipantCount(course);w=Heap_Alloc(heap,48);MI_CpuFill8(w,0,48);w->heap=heap;w->course=course;i=0;w->count=4-n;if(i<w->count){u8*cursor=w->entries[0];do{ov96_0221A00C(cursor,(u8)n,course);i++;n++;cursor+=12;}while(i<w->count);}return w;}
