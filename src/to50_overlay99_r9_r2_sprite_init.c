#include "to50_overlay99_r9_r2_sprite_init_private.h"

void ov99_021E7EF0(Ov99R9R2Work *work) {
    SpriteResourceCountsListUnion counts = ov99_021E9FC4;
    work->spriteContext = ov98_0221E5E0(work->heapId, &counts, 0x32);
    ov99_021E7F90(work);
    ov99_021E80D0(work);
    ov99_021E810C(work);
    ov99_021E8198(work);
    ov98_0221E6F0(work->spriteContext, work->sprites, ov99_021E9F60, ov99_021E724C(work), 5, 8, 0x98, TRUE);
}
