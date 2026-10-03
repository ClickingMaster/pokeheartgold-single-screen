#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int heap;void *palette;u8 indices[128];u16 durations[128];u8 count,member,timer,state,index;} Payload;
typedef struct {u8 pad0[0x18];int done;u8 pad1c[8];Payload *data;} Work;
extern void Heap_Free(void*);extern void SysTask_Destroy(void*);extern void PaletteData_LoadFromNarc(void*,int,int,int,int,int,int,int);
void ov07_0221E914(void*,Work*);
void ov07_0221E914(void*task,Work*w){Payload*d=w->data;if(w->done==1){Heap_Free(d);Heap_Free(w);SysTask_Destroy(task);return;}switch(d->state){case 0:d->timer=255;d->state++;case 1:if(d->timer<d->durations[d->index]-2)d->timer++;else {PaletteData_LoadFromNarc(d->palette,9,d->member,d->heap,0,32,144,(d->indices[d->index]&0xfff)<<4);d->timer=0;if(d->index>=d->count-1)d->index=0;else d->index++;}break;}}
