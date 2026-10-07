#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct Task { struct Task *next; u32 flags; void (*run)(struct Task *); void (*done)(struct Task *); u8 p[16]; } Task;
typedef struct { OSThread thread; Task *queue; Task stop; u8 stack[]; } Work;
extern Work *ov13_0224CF80;
extern void MBi_InitTaskInfo(Task *);
void MBi_TaskThread(void *);
void MBi_InitTaskThread(Work *w, u32 size);
// @fn MBi_InitTaskThread
void MBi_InitTaskThread(Work *w, u32 size) {
    OSIntrMode e = OS_DisableInterrupts();
    if (ov13_0224CF80 == NULL) {
        u32 stack;
        ov13_0224CF80 = w;
        MBi_InitTaskInfo(&w->stop);
        w->queue = NULL;
        stack = (size - sizeof(Work)) & ~3;
        OS_CreateThread(&w->thread, MBi_TaskThread, w, (u8 *)w + sizeof(Work) + stack, stack, 0);
        OS_WakeupThreadDirect(&w->thread);
    }
    OS_RestoreInterrupts(e);
}
