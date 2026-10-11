#include "overlay15_bag_navigation_r42_private.h"

s32 BagApp_GetCursorItemId(BagWorkRaw *work) {
    BagContextRaw *ctx = PTR(BagContextRaw, work, 0x234);
    BagPocketEntryRaw *entry = &ctx->pockets[ctx->pocket];
    u32 index = entry->cursor + S32(work, 0x644) - 8;
    if (sPocketCapacities[ctx->pocket] <= index) {
        return 0;
    }
    return ((u16 *)entry->items)[index * 2];
}

void BagApp_PrintCursorDescription(BagWorkRaw *work) {
    s32 value = S32(work, 0x644);
    if (value >= 8 && value <= 13) {
        u32 item = BagApp_GetCursorItemId(work);
        if (item != 0) {
            BagApp_ShowItemDescription(work, work->raw + 4, item);
        } else {
            BagApp_ClearDescriptionWindow(work, work->raw + 4);
        }
    } else if (value >= 0 && value <= 7) {
        BagApp_ShowPocketDescription(work, work->raw + 4, value);
    } else {
        BagApp_ClearDescriptionWindow(work, work->raw + 4);
    }
}
