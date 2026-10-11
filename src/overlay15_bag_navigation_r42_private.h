#ifndef TO42_OV15_RESIDUAL7_PRIVATE_H
#define TO42_OV15_RESIDUAL7_PRIVATE_H

#include "global.h"
#include "to91_bag_choices41560_private.h"

#include "sys_vars.h"
#include "system.h"

typedef struct BagPocketEntryRaw {
    void *items;
    u16 unk_04;
    s16 cursor;
    u8 count;
    u8 unk_09;
    s16 scroll;
} BagPocketEntryRaw;

typedef struct BagItemSlotRaw {
    u16 item;
    u16 quantity;
} BagItemSlotRaw;

typedef struct BagContextRaw {
    u8 pad_000[4];
    BagPocketEntryRaw pockets[8];
    u8 pocket;
    u8 state;
    u16 item;
    u16 quantity;
} BagContextRaw;

typedef struct BagWorkRaw {
    u8 raw[0x900];
} BagWorkRaw;

#define PTR(type, p, o) (*(type **)((u8 *)(p) + (o)))
#define S32(p, o)       (*(s32 *)((u8 *)(p) + (o)))
#define S16(p, o)       (*(s16 *)((u8 *)(p) + (o)))
#define U16(p, o)       (*(u16 *)((u8 *)(p) + (o)))
#define U8(p, o)        (*(u8 *)((u8 *)(p) + (o)))

extern const u8 sButtonNavTable[];
extern const u8 ov15_02200641[];
extern const u8 ov15_02200642[];
extern const u8 ov15_02200643[];
extern const u8 sPocketCapacities[];
extern s32 _02201300;
extern s32 _02201480;

void PlaySE(u16);
BOOL System_GetTouchHeld(void);
void BagApp_ShowItemDescription(BagWorkRaw *, void *, u32);
void BagApp_ClearDescriptionWindow(BagWorkRaw *, void *);
void BagApp_ShowPocketDescription(BagWorkRaw *, void *, u32);
void BagApp_LoadSubScreenLayout(BagWorkRaw *, s32, s32, s32);
void BagApp_DrawSelectedItemEntry(BagWorkRaw *, s32, s32);
void BagApp_ShowSelectedItemIcon(BagWorkRaw *, s32);

void BagApp_RedrawMovePage(BagWorkRaw *, BagPocketEntryRaw *);
void BagApp_SetMoveCursorPosition(BagWorkRaw *, void *);
s32 BagApp_GetPageItemCount(BagWorkRaw *);
void BagApp_DrawItemList(BagWorkRaw *, s32, s32, s32);
void BagApp_PrintPageNumber(BagWorkRaw *, s32, s32, s32);
void BagApp_DrawItemIcons(BagWorkRaw *, BagPocketEntryRaw *, s32, s32);
s32 BagApp_GetCursorItemId(BagWorkRaw *);
void BagApp_PrintCursorDescription(BagWorkRaw *);
s32 BagApp_FindPocketIndex(BagWorkRaw *, s32);
s32 BagApp_GetAdjacentPocket(BagWorkRaw *, s32, s32);
void BagApp_RedrawPocketPage(BagWorkRaw *, BagPocketEntryRaw *);
void ov15_021FA620(BagWorkRaw *);
s32 ov15_021FA650(BagWorkRaw *);
s32 BagApp_State_OpenItemMenu(BagWorkRaw *);
s32 BagApp_State_ChangePage(BagWorkRaw *, s32);

#endif
