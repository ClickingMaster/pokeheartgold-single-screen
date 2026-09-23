#include "global.h"

extern void *Save_FriendGroup_Get(void *);
extern void *Heap_AllocAtEnd(u32, u32);
extern void *ov73_021E7960(void *);
void *ov73_021EA268(void *, u32, u32);

void *ov73_021EA268(void *save, u32 heap, u32 size) {
    void *friendGroup = Save_FriendGroup_Get(save);
    void *copy = Heap_AllocAtEnd(heap, size);
    void *src = ov73_021E7960(friendGroup);
    MIi_CpuCopyFast(src, copy, size);
    return copy;
}
