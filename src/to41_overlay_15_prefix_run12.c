#include "to41_overlay_15_prefix_private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))

s32 BagApp_State_ShowTrashConfirm(void *work) {
    BagApp_PrintTrashConfirmMessage(work);
    BagApp_HideQuantityButtons(work);
    BagApp_HideCursor(work);
    BagApp_ClearQuantityLabel(work);
    BagApp_ShowCancelButton(work, 0);
    return 8;
}
