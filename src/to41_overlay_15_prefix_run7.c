#include "to41_overlay_15_prefix_private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))

s32 BagApp_GetTouchedButton(void *work, u32 index) {
    return TouchscreenHitbox_FindRectAtTouchNew(sTouchHitboxTables[index]);
}

s32 ov15_021FAC40(void) {
    return -1;
}
