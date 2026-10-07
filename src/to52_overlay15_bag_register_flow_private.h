#ifndef TO52_OV15_BAG_REGISTER_PRIVATE_H
#define TO52_OV15_BAG_REGISTER_PRIVATE_H
#include "global.h"
struct ItemSlot;
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
typedef struct BagSelectionEntry {
    void *slots;
    u8 padding[5];
    u8 quantity;
    u8 tail[2];
} BagSelectionEntry;
typedef struct BagArgs {
    u8 padding[4];
    BagSelectionEntry entries[8];
    u8 selected;
    u8 padding65;
    u16 item;
} BagArgs;
extern u8 gSystem[];
extern BOOL TextPrinterCheckActive(u8);
extern void ClearFrameAndWindow2(void *, BOOL);
extern void ClearWindowTilemapAndScheduleTransfer(void *);
extern void ScheduleWindowCopyToVram(void *);
extern BOOL Pocket_TakeItem(struct ItemSlot *, u32, u16, u16, u32);
extern void ov15_021F9F08(void *);
extern s32 ov15_021FA074(void *);
extern void ov15_02200140(void *, void *, s32, BOOL);
extern void ov15_021FE868(void *);
extern void ov15_021FED3C(void *);
extern void ov15_021FB518(void *);
extern void ov15_021FA170(void *);
extern void ov15_021FD788(void *, s32);
extern s32 Bag_TryRegisterItem(void *, u16);
extern void Bag_UnregisterItem(void *, u16);
extern void *NewString_ReadMsgData(void *, s32);
extern void FillWindowPixelBuffer(void *, u32);
extern void StringExpandPlaceholders(void *, void *, void *);
extern void String_Delete(void *);
extern s32 ov15_021FEF48(void *, s32);
extern void ov15_02200294(void *);
extern void ov15_021FFF24(void *);
extern void ov15_021FF560(void *);
extern void ov15_021FF7AC(void *);
s32 ov15_021FC164(void *);
s32 ov15_021FC224(void *);
s32 ov15_021FC2E0(void *);
s32 ov15_021FC37C(void *);
#endif
