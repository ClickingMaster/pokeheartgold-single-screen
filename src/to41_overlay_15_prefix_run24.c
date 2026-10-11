#include "to41_overlay_15_prefix_private.h"

#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))

void BagApp_BufferSlotItemName(void *work, u32 item, u32 slot) {
    u16 value = BagApp_GetCurrentPocketSlotValue(work, (u16)item, FALSE);
    BufferItemName(PTR(work, 0x2f4), slot, value);
}

void BagApp_BufferSlotItemNamePlural(void *work, u32 item, u32 slot) {
    u16 value = BagApp_GetCurrentPocketSlotValue(work, (u16)item, FALSE);
    BufferItemNamePlural(PTR(work, 0x2f4), slot, value);
}
