#ifndef TO50_OV59_GAP3_PRIVATE_H
#define TO50_OV59_GAP3_PRIVATE_H

#include "global.h"

#include "sprite_system.h"
#include "system.h"
#include "touchscreen.h"

typedef struct Ov59Gap3Work {
    u8 raw[0x254];
    Sprite *sprites[15];
} Ov59Gap3Work;

extern const TouchscreenHitbox ov59_0223C940[];

void PlaySE(u32 seq);
int ov59_02239F38(Ov59Gap3Work *work);

#endif
