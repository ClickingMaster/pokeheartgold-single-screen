#include "to55_r2_t01_private.h"
void ov07_02223064(void *task, SimpleWork *w) {
    switch (w->state) {
    case 0:
        w->state++;
        break;
    case 1:
        ov07_0221C448(w->owner, task);
        Heap_Free(w);
        break;
    }
}
void ov07_0222308C(void *owner) {
    SimpleWork *w = Heap_Alloc(5, sizeof(SimpleWork));
    w->state = 0;
    w->owner = owner;
    ov07_0221C3F4(owner, ov07_02223064, w, 0x1000);
}
void ov07_022230B4(void *task, SimpleWork *w) {
    switch (w->state) {
    case 0:
        w->state++;
        break;
    case 1:
        ov07_0221C458(w->owner, task);
        Heap_Free(w);
        break;
    }
}
void ov07_022230DC(void *owner) {
    SimpleWork *w = Heap_Alloc(5, sizeof(SimpleWork));
    w->state = 0;
    w->owner = owner;
    ov07_0221C42C(owner, ov07_022230B4, w, 0x1000);
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
