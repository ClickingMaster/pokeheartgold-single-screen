#include "overlay07_callbacks_w07_private.h"

#define WORD(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define HALF(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_0222E6A8(void *owner, void *data) {
    switch (WORD(data, 4)) {
    case 0:
        if (ov07_0222E5E8(data)) {
            WORD(data, 4)++;
            WORD(data, 8) = 0x10;
        }
        break;
    case 1:
        WORD(data, 8)--;
        if ((s32)WORD(data, 8) < 0) {
            WORD(data, 4)++;
        }
        break;
    case 2:
        BattleAnimSystem_DestroyTask(PTR(data, 0), owner);
        Heap_Free(data);
        break;
    }
}
