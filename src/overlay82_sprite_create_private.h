#ifndef TO51_R4_W06_PRIVATE_H
#define TO51_R4_W06_PRIVATE_H
#include "global.h"

#include "heap.h"
#include "sprite.h"
typedef struct Ov82Spr {
    u8 filler00[0xC];
    const u8 *animData;
    Sprite *sprite;
} Ov82Spr;
Sprite *ov82_0223FB08(void *work, u32 resource, u32 sequence, u32 priority, u8 mode);
Ov82Spr *ov82_0223FC48(void *work, u32 resource, u32 sequence, s32 x, u16 y, const u8 *animData);
#endif
