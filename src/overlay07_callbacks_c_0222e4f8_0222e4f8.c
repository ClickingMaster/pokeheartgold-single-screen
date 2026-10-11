#include "overlay07_callbacks_w07_private.h"

#define WORD(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define HALF(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_0222E4F8(void *owner, void *data) {
    switch (WORD(data, 0)) {
    case 0:
        if (ov07_0222E3E8(data)) {
            WORD(data, 0x48)++;
            if ((s32)WORD(data, 0x48) < 4) {
                WORD(data, 0x54) ^= 1;
                ov07_0222E3A4(data, WORD(data, 0x54));
                ov07_0222E3E8(data);
            } else {
                WORD(data, 0)++;
            }
        }
        break;
    case 1:
        BattleAnimSystem_DestroyTask(PTR(data, 4), owner);
        Heap_Free(data);
        break;
    }
}
