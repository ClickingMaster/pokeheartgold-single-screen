#ifndef POKEHEARTGOLD_OVERLAY14_BOX_ITEM_DISPLAY_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY14_BOX_ITEM_DISPLAY_PRIVATE_H

#include "global.h"

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

extern u32 ov14_021F5404(void *, void *);
extern void BgCellManager_PutWindowTiles(void *, u32, void *);
extern void ov14_021E88A4(void *);
extern void ov14_021E8944(void *);
extern void PCBox_ShowMonPic(void *, void *, u32);
extern void PCBox_SetSpriteVisible(void *, u32, u32);
extern void PCBox_DrawTypeIcons(void *, void *);
extern void PCBox_PrintMonInfo(void *, void *);
extern void PCBox_DrawMarkings(void *, u32);
extern u32 PCBox_PrintItemNameAndDesc(void *, u32);
extern void PCBox_LoadInfoItemIcon(void *, u32);
extern void *PCBox_GetSlotBoxMon(void *, u32, u32);
extern u32 AcquireBoxMonLock(void *);
extern void *PCBox_CreateMonInfo(void *);
extern void PCBox_FreeMonInfo(void *);
extern u32 ReleaseBoxMonLock(void *, u32);
extern void PCBox_ClearMonInfoWindows(void *);

void PCBox_DrawMonInfo(void *, void *);
void PCBox_DrawMonItemInfo(void *, void *);
BOOL PCBox_ShowSlotMonInfo(void *, u32);
void PCBox_RedrawSlotMonInfo(void *, u32);
void PCBox_ClearMonInfo(void *);

#endif
