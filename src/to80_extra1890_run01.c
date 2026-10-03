#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int heap;void *course;u8 entries[3][60];u8 count;} State;
extern int PokeathlonCourse_GetParticipantCount(void *);extern void *Heap_Alloc(int,u32);extern void ov96_022108BC(void *,u8,void *);
static inline int GetParticipants(void *course){return PokeathlonCourse_GetParticipantCount(course);}
State *ov96_022107F0(void *,int);
State *ov96_022107F0(void *course,int heap){int i,participant;State *s;participant=GetParticipants(course);s=Heap_Alloc(heap,192);MI_CpuFill8(s,0,192);s->heap=heap;s->course=course;{i=0;s->count=4-participant;for(;i<s->count;i++){ov96_022108BC(s->entries[i],participant,course);participant++;}}return s;}
