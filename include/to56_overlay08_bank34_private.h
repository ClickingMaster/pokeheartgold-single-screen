#ifndef TO56_R3_W12_PRIVATE_H
#define TO56_R3_W12_PRIVATE_H
#include "global.h"
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
extern void ManagedSprite_SetDrawFlag(void *, BOOL);
extern void BattleBagMenu_ShowSpriteAt(void *, int, int);
extern void BattleBagMenu_SetItemIconGraphics(void *, u32, u32);
extern void BattleBagMenu_SetItemIconPalette(void *, u32, u32, u32);
extern u32 BattleBagMenu_GetItemOnPage(void *, u32);
extern const s32 sBattleBagItemListIconPos[][2];
extern const u32 sBattleBagPageCursorBoxes[];
extern void *BattleDpadCursor_GetBattleCursor(void *);
extern void BattleDpadCursor_SetBoxes(void *, u32);
extern void BattleDpadCursor_SetPosition(void *, u32);
extern void BattleBagMenu_ShowLastUsedItemIcon(u8 *);
extern void BattleBagMenu_ShowItemListIcons(u8 *);
extern void BattleBagMenu_ShowUseItemIcon(u8 *);
void BattleBagMenu_ShowPageSprites(u8 *, u32);
void BattleBagMenu_CreateCursor(u8 *);
void BattleBagMenu_FreeCursor(u8 *);
extern void *BattleSystem_GetSpriteSystem(void *);
extern void BattleCursor_LoadResources(void *, void *, void *, u32, u32, u32, u32, u32);
extern void *BattleCursor_New(void *, void *, u32, u32, u32, u32, u32, u32, u32);
extern void BattleCursor_Delete(void *);
extern void BattleCursor_FreeResources(void *, u32, u32, u32, u32);
extern void BattleDpadCursor_SetBattleCursor(void *, void *);
void BattleBagMenu_SetPageCursor(u8 *, u32);
#endif
