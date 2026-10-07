#pragma force_active on
#include "global.h"
#pragma force_active on
struct Band {s16 start,end,step,current; s32 xy;};
struct Bands {struct Band list[16];void *handle;};
struct TaskState {u8 pad[24];int stop;u32 pad1c;struct Bands *bands;};
extern void Heap_Free(void *);
extern void SysTask_Destroy(void *);
extern void ov07_02222C60(void *);
extern u32 *ov07_02222C84(void *);
extern u32 ov07_02222D88(u16,u16);
void ov07_0221E87C(void *,struct TaskState *);
void ov07_0221E87C(void *task,struct TaskState *state) { struct Bands *bands=state->bands; int i,j; u32 *buffer; if(state->stop==1) { ov07_02222C60(bands->handle); Heap_Free(state->bands); Heap_Free(state); SysTask_Destroy(task); return; } buffer=ov07_02222C84(bands->handle); for(i=0;i<16;i++) { struct Band *band=&bands->list[i]; band->current+=band->step; for(j=band->start;j<band->end;j++) { buffer[j]=ov07_02222D88((u16)((s16)band->xy+band->current),(u16)(s16)(band->xy>>16)); } } }
