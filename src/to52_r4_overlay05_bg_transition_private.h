#ifndef W07_OV05_PRIVATE_H
#define W07_OV05_PRIVATE_H
#include "global.h"
#include "bg_window.h"
#include "sound.h"
typedef struct W07Ov05Work {
    u8 pad00[0xC];
    BgConfig *bgConfig;
    u8 pad10[0xB6E];
    u8 direction;
    u8 state;
    u8 padB80[2];
    u8 frame;
} W07Ov05Work;
extern u8 ov05_0221EA6C[];
extern u8 ov05_0221EA6D[];
BOOL ov05_0221C5A8(W07Ov05Work *);
BOOL ov05_0221C5C4(W07Ov05Work *);
BOOL ov05_0221C6C8(W07Ov05Work *);
void ov05_0221D3AC(W07Ov05Work *, int, s16, s16);
void PlaySE(u32);
void ov05_0221D664(W07Ov05Work *, int, s16, s16);
#endif
