#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work{int a;void*bg;u8 p[0x1204-8];void(*cb)(struct Work*),(*other)(struct Work*);}Work;
extern void DoScheduledBgGpuUpdates(void*),GF_RunVramTransferTasks(void),OamManager_ApplyAndResetBuffers(void);

void ov70_022387AC(Work*w);

void ov70_022387AC(Work*w){volatile u32*base=(volatile u32*)0x027e0000;if(w->cb){w->cb(w);w->cb=0;}if(w->other)w->other(w);DoScheduledBgGpuUpdates(w->bg);GF_RunVramTransferTasks();OamManager_ApplyAndResetBuffers();base[0x3ff8/4]|=1;}
