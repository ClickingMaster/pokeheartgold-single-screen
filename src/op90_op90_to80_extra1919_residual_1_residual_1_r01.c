#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct { u32 offset; u32 size; u8 *data; u32 state; } CacheEntry;
typedef struct { u32 busy; u8 pad[0x2c]; CacheEntry entries[4]; } Cache;
typedef struct Task { struct Task *next; u32 flags; void (*run)(struct Task *); void (*done)(struct Task *); u8 p[16]; } Task;
typedef struct { OSThread thread; Task *volatile queue; Task stop; } Work;
BOOL MBi_ReadFromCache(Cache *cache, u32 offset, void *buf, u32 len);
void MBi_TaskThread(void *arg);
// @fn MBi_ReadFromCache
BOOL MBi_ReadFromCache(Cache *cache, u32 offset, void *buf, u32 len) {
    BOOL ret = FALSE;
    OSIntrMode e = OS_DisableInterrupts();
    CacheEntry *p;
    for (p = &cache->entries[0]; p < &cache->entries[4]; p++) {
        if (p->state >= 2) {
            int pos = offset - p->offset;
            if (pos >= 0 && pos + len <= p->size) {
                MI_CpuCopy8(p->data + pos, buf, len);
                cache->busy = 0;
                ret = TRUE;
                break;
            }
        }
    }
    OS_RestoreInterrupts(e);
    return ret;
}
// @fn MBi_TaskThread
void MBi_TaskThread(void *arg) {
    Work *w = arg;
    for (;;) {
        Task *task;
        void (*done)(Task *);
        u32 cur, prio;
        OSIntrMode e = OS_DisableInterrupts();
        while (w->queue == NULL) {
            OS_SetThreadPriority(&w->thread, 0);
            OS_SleepThread(NULL);
        }
        task = w->queue;
        w->queue = w->queue->next;
        OS_SetThreadPriority(&w->thread, task->flags >> 1);
        OS_RestoreInterrupts(e);
        done = task->run;
        if (done != NULL) {
            done(task);
        }
        e = OS_DisableInterrupts();
        done = task->done;
        cur = OS_GetThreadPriority(&w->thread);
        prio = (w->queue == NULL) ? 0 : ((w->queue->flags >> 1) > cur ? (w->queue->flags >> 1) : cur);
        if (prio != cur) {
            OS_SetThreadPriority(&w->thread, prio);
        }
        task->next = NULL;
        task->flags &= ~1;
        if (done != NULL) {
            done(task);
        }
        if (task == &w->stop) {
            break;
        }
        OS_RestoreInterrupts(e);
    }
    OS_ExitThread();
}
