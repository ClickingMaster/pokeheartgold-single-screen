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
SpriteSystem *PokeathlonUtil_GetSpriteSystem(void *ctx);
SpriteManager *PokeathlonUtil_GetSpriteManager(void *ctx);
void ov99_021E6400(Ov99ResourceWork *work);
void PokeathlonUtil_LoadMonIconResources(void *ctx, const int *ids, int count, int screen, BOOL flag);
extern const int ov99_021E9554[];
void ov99_021E6274(Ov99ResourceWork *work);
#endif
