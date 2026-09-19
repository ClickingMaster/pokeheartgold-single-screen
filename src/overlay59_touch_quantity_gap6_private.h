#ifndef TO50_OV59_GAP6_PRIVATE_H
#define TO50_OV59_GAP6_PRIVATE_H
#include "global.h"

#include "sprite_system.h"
#include "touchscreen.h"
typedef struct Ov59Gap6Work {
    u8 raw[0x254];
    Sprite *sprites[15];
} Ov59Gap6Work;
extern const TouchscreenHitbox ov59_0223C908[];
int ov59_0223A48C(Ov59Gap6Work *, int);
int ov59_0223A614(Ov59Gap6Work *, int *hit);
#endif
