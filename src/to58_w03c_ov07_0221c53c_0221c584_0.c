#include "global.h"
void GF_AssertFail(void);
void *Heap_Alloc(int, int);
void *memset(void *, int, unsigned long);
void *SysTask_CreateOnMainQueue(void (*)(void *, u8 *), void *, int);
void Heap_Free(void *);
void SysTask_Destroy(void *);
extern int (*ov07_02234C08[])(void *);
void *ov07_0221C53C(u8 *);
void ov07_0221C56C(void *, u8 *);
void ov07_0221C584(void *, u8 *);

void *ov07_0221C53C(u8 *state) {
    void *allocated = Heap_Alloc(*(int *)state, 0x3C);
    if (allocated == NULL) {
        GF_AssertFail();
        return NULL;
    }
    memset(allocated, 0, 0x3C);
    ++*(u16 *)(state + 0x90);
    return allocated;
}

void ov07_0221C56C(void *task, u8 *data) {
    *(void **)(data + 0x38) = task;
    SysTask_CreateOnMainQueue(ov07_0221C584, data, 0x44C);
}

void ov07_0221C584(void *task, u8 *data) {
    if (!ov07_02234C08[*data](data)) {
        u8 *state = *(u8 **)(data + 0x38);
        if (*(u16 *)(state + 0x90) != 0) {
            --*(u16 *)(state + 0x90);
        }
        Heap_Free(data);
        SysTask_Destroy(task);
    }
}
