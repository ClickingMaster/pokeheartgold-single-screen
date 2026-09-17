#ifndef TO50_OV99_R9_R2_PRIVATE_H
#define TO50_OV99_R9_R2_PRIVATE_H
#include "global.h"

#include "sprite_system.h"
typedef struct Ov99R9R2Work {
    u8 pad00[0xc];
    enum HeapID heapId;
    u8 pad10[0x3f4];
    void *spriteContext;
    void *sprites[1];
} Ov99R9R2Work;
extern const SpriteResourceCountsListUnion ov99_021E9FC4;
extern const int ov99_021E9F60[];
void *ov98_0221E5E0(enum HeapID heapId, const SpriteResourceCountsListUnion *counts, int maxSprites);
void ov98_0221E6F0(void *ctx, void **sprites, const int *resources, u32 activeCount, u32 startIndex, s16 x, s16 y, BOOL subScreen);
void ov99_021E7F90(Ov99R9R2Work *work);
void ov99_021E80D0(Ov99R9R2Work *work);
void ov99_021E810C(Ov99R9R2Work *work);
void ov99_021E8198(Ov99R9R2Work *work);
u32 ov99_021E724C(Ov99R9R2Work *work);
void ov99_021E7EF0(Ov99R9R2Work *work);
#endif
