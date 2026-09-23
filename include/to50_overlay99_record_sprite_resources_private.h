#ifndef TO50_OV99_R2_R11_PRIVATE_H
#define TO50_OV99_R2_R11_PRIVATE_H
#include "global.h"
#include "filesystem.h"
#include "sprite_system.h"
typedef struct Ov99ResourceWork { u8 pad00[8]; NARC *narc; u8 pad0c[8]; void *spriteContext; } Ov99ResourceWork;
SpriteSystem *ov98_0221E5C0(void *ctx);
SpriteManager *ov98_0221E5D0(void *ctx);
void ov99_021E6A9C(Ov99ResourceWork *work);
#endif
