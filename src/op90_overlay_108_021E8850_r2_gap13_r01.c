#pragma force_active on
#include "global.h"
#include "heap.h"
#include "sprite.h"
#include "sys_task_api.h"
typedef struct {
    enum HeapID heap;
    u8 pad[0x440];
    Sprite *sprites[18];
} W;
typedef struct {
    u32 *counter;
    W *w;
    u8 index;
    u8 pad[11];
} Job;
void PlaySE(u16);
void ov108_021EA5E4(SysTask *, void *);
void ov108_021EA584(W *w, u8 index, u32 *counter, u8 priority);
// @fn ov108_021EA584
void ov108_021EA584(W *w, u8 index, u32 *counter, u8 priority) {
    Job *j = Heap_Alloc(w->heap, sizeof(Job));
    MI_CpuFill8(j, 0, sizeof(Job));
    j->w = w;
    j->counter = counter;
    j->index = index;
    Sprite_SetAnimCtrlSeq(w->sprites[index + 2], index * 3 + 6);
    PlaySE(1505);
    if (j->counter != NULL) {
        (*j->counter)++;
    }
    SysTask_CreateOnMainQueue(ov108_021EA5E4, j, priority);
}
