#ifndef TO50_OV99_R2_R6_PRIVATE_H
#define TO50_OV99_R2_R6_PRIVATE_H
#include "global.h"

#include "filesystem.h"
#include "sprite_system.h"
typedef struct Ov99ResourceWork {
    u8 pad00[8];
    NARC *narc;
    u8 pad0c[8];
    void *spriteContext;
} Ov99ResourceWork;
SpriteSystem *ov98_0221E5C0(void *ctx);
SpriteManager *ov98_0221E5D0(void *ctx);
void ov99_021E6400(Ov99ResourceWork *work);
void ov98_0221E8A8(void *ctx, const int *ids, int count, int screen, BOOL flag);
extern const int ov99_021E9554[];
void ov99_021E6274(Ov99ResourceWork *work);
#endif
