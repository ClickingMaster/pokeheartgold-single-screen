#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int a;VecFx32 entries[4];u16 head,tail;int count;}Queue;
extern void ov48_0225A2EC(Queue*);

void ov48_0225A2A0(Queue*w,const VecFx32*p);

void ov48_0225A2A0(Queue*w,const VecFx32*p){if((w->tail+1)%4==w->head)ov48_0225A2EC(w);w->entries[w->tail]=*p;w->tail=(w->tail+1)%4;w->count++;}
