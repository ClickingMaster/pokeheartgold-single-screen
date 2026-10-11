#include "r40_overlay_83_residual_49_private.h"

void *ov83_02247B7C(void *ctx) {
    void *ret = GridInputHandler_Create(ov83_022485C8, ov83_02248610, ov83_02248520, ctx, 1, 0, 0x6b);
    BattleCastlePartyApp_SetItemCursor(ctx, 0x40, 0x34, 3);
    BattleCastlePartyApp_ShowItemDescription(ctx, 0);
    return ret;
}
