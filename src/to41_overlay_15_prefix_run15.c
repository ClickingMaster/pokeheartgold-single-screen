#include "to41_overlay_15_prefix_private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))

s32 BagApp_ItemAction_Move(void *work) {
    BagApp_StartMoveItem(work);
    return 3;
}

s32 BagApp_ItemAction_Give(void *work) {
    BagApp_CloseItemMenuWindows(work);
    BagApp_ShowCancelButton(work, 0);
    BagApp_HideCursor(work);
    sub_020880CC(1, 6);
    U16(PTR(work, 0x234), 0x68) = 2;
    return 0x25;
}
