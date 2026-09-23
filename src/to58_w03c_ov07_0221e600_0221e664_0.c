#include "global.h"
void Heap_Free(void *);
void SysTask_Destroy(void *);
void BgSetPosTextAndCommit(void *, int, int, int);
void GF_AssertFail(void);
void ov07_0221DD14(void *, int);
void ov07_0221E600(void *, u8 *);
void ov07_0221E664(u8 *);

void ov07_0221E600(void *task, u8 *data) {
    if (*(int *)(data + 0x18) == 1) {
        Heap_Free(data);
        SysTask_Destroy(task);
        return;
    }
    *(s16 *)(data + 4) += *(s16 *)(data + 8);
    *(s16 *)(data + 6) += *(s16 *)(data + 0xA);
    if (*(s16 *)(data + 8)) {
        BgSetPosTextAndCommit(*(void **)data, *(u32 *)(data + 0xC) & 0xFF, 0, *(s16 *)(data + 4));
    }
    if (*(s16 *)(data + 0xA)) {
        BgSetPosTextAndCommit(*(void **)data, *(u32 *)(data + 0xC) & 0xFF, 3, *(s16 *)(data + 6));
    }
}
