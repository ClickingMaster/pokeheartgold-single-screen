#include "to55_r2_t01_private.h"
void ov07_02223064(void *task, SimpleWork *w) {
    switch (w->state) {
    case 0:
        w->state++;
        break;
    case 1:
        BattleAnimSystem_DestroyTask(w->owner, task);
        Heap_Free(w);
        break;
    }
}
void ov07_0222308C(void *owner) {
    SimpleWork *w = Heap_Alloc(5, sizeof(SimpleWork));
    w->state = 0;
    w->owner = owner;
    BattleAnimSystem_CreateTask(owner, ov07_02223064, w, 0x1000);
}
void ov07_022230B4(void *task, SimpleWork *w) {
    switch (w->state) {
    case 0:
        w->state++;
        break;
    case 1:
        BattleAnimSystem_DestroySoundTask(w->owner, task);
        Heap_Free(w);
        break;
    }
}
void ov07_022230DC(void *owner) {
    SimpleWork *w = Heap_Alloc(5, sizeof(SimpleWork));
    w->state = 0;
    w->owner = owner;
    BattleAnimSystem_CreateSoundTask(owner, ov07_022230B4, w, 0x1000);
}
void ov07_02223104(void *task, SimpleWork *w) {
    switch (w->state) {
    case 0:
        w->state++;
        break;
    case 1:
        SysTask_Destroy(task);
        Heap_Free(w);
        break;
    }
}
void ov07_02223128(void *owner) {
    SimpleWork *w = Heap_Alloc(5, sizeof(SimpleWork));
    w->state = 0;
    w->owner = owner;
    SysTask_CreateOnMainQueue(ov07_02223104, w, 0x1000);
}
