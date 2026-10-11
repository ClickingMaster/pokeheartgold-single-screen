#include "overlay15_bag_navigation_r42_private.h"

s32 BagApp_GetAdjacentPocket(BagWorkRaw *work, s32 pocket, s32 direction) {
    s32 i;
    for (i = 0; i < 8; i++) {
        pocket += direction;
        if (pocket < 0) {
            pocket = 7;
        } else if (pocket == 8) {
            pocket = 0;
        }
        if (BagApp_FindPocketIndex(work, pocket) != -1) {
            break;
        }
    }
    return pocket;
}

void BagApp_RedrawPocketPage(BagWorkRaw *work, BagPocketEntryRaw *entry) {
    s32 count = BagApp_GetPageItemCount(work);
    BagApp_LoadSubScreenLayout(work, 0, count, 0);
    BagApp_DrawItemList(work, entry->cursor, -1, 0);
    BagApp_PrintPageNumber(work, entry->unk_09, entry->cursor, 0);
    count = BagApp_GetPageItemCount(work);
    BagApp_DrawItemIcons(work, entry, count, 1);
}
