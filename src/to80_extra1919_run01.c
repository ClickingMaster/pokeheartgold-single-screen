#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u32 offset, size;
    void *data;
    u32 state;
} Entry;

typedef struct {
    u32 busy;
    u8 p[0x30 - 4];
    Entry entries[4];
} Cache;

typedef struct Task {
    struct Task *next;
    u32 flags;
    void (*run)(struct Task *), (*done)(struct Task *);
    u8 p[16];
} Task;

typedef struct {
    OSThread thread;
    Task *volatile queue;
    Task stop;
} Work;

extern Work *ov13_0224CF80;
extern void MBi_InitTaskInfo(Task *);
void MBi_AttachCacheBuffer(Cache *, u32, u32, void *, u32);
int MBi_ReadFromCache(Cache *, u32, void *, u32);
void MBi_TaskThread(void *);
void MBi_InitTaskThread(Work *, u32);

void MBi_AttachCacheBuffer(Cache *c, u32 offset, u32 size, void *data, u32 state) {
    Entry *end;
    OSIntrMode interrupts;
    Entry *e;
    interrupts = OS_DisableInterrupts();
    e = c->entries;
    end = c->entries + 4;
    for (;;) {
        if (e >= end) {
            OS_Terminate();
        }
        if (e->state == 0) {
            e->offset = offset;
            e->size = size;
            e->data = data;
            e->state = state;
            break;
        }
        e++;
    }
    OS_RestoreInterrupts(interrupts);
}
