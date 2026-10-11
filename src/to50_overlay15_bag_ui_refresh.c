#include "to50_overlay15_bag_ui_refresh_private.h"
s32 BagApp_State_CancelTrash(void *work) {
    BagApp_CloseItemMenuWindows(work);
    BagApp_RedrawPocket(work);
    BagApp_HideQuantityButtons(work);
    BagApp_PrintCancelButton(work, 0);
    ManagedSprite_SetAnimationFrame(PTR(work, 0x29C), 0);
    ManagedSprite_SetAnim(PTR(work, 0x29C), 0x10);
    return 1;
}
