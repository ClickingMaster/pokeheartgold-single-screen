#ifndef TO42_OVERLAY_15_SUFFIX_PRIVATE_H
#define TO42_OVERLAY_15_SUFFIX_PRIVATE_H

#include "global.h"

#include "bg_window.h"
#include "msgdata.h"
#include "render_text.h"
#include "sprite_system.h"

typedef struct BagWorkRaw {
    u8 raw[0x900];
} BagWorkRaw;

extern void BagApp_RemoveItemListWindows(BagWorkRaw *);
extern void BagApp_AddItemActionWindows(BagWorkRaw *);
extern void BagApp_RemoveItemActionWindows(BagWorkRaw *);
extern void BagApp_BufferSlotItemName(BagWorkRaw *, u32, u32);
extern void BagApp_BufferSlotItemNamePlural(BagWorkRaw *, u32, u32);
extern void BagApp_PrintItemDescription(BagWorkRaw *, u16);
extern void *BagApp_LoadListIconChars(BagWorkRaw *, NNSG2dCharacterData **);
extern void BagApp_PrintItemListEntry(BagWorkRaw *, Window *, String *, void *, u32);
extern void ov15_021FFEC0(BagWorkRaw *);
extern const u8 sQuantityCursorPositions[];
extern const u8 ov15_022009D5[];
extern const u8 ov15_022009D6[];
extern const u8 ov15_022009D7[];
extern const u8 sCursorPositions[];
extern const u8 ov15_02200AB9[];
extern const u8 ov15_02200ABA[];
extern const u8 ov15_02200ABB[];
extern void ClearFrameAndWindow2(Window *, BOOL);
extern void GF_AssertFail(void);

#endif
