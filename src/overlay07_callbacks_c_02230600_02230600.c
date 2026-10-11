#include "overlay07_callbacks_w07_private.h"

#define WORD(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define HALF(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_02230600(void *owner) {
    void *data = BattleAnimUtil_AllocWork(owner, 0x34);
    WORD(data, 0) = (u32)owner;
    BattleAnimSystem_CreateTaskDefaultPriority(owner, ov07_0223049C, data);
}
