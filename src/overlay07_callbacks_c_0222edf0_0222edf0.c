#include "overlay07_callbacks_w07_private.h"

#define WORD(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define HALF(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_0222EDF0(void *data) {
    WORD(data, 0x40) = 0;
    BattleAnimUtil_InitScaleMotionXY((u8 *)data + 0x1C, 10, 9, 10, 8, 10, 2);
}
