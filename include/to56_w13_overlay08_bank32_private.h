#ifndef TO56_W13_OVERLAY08_BANK32_PRIVATE_H
#define TO56_W13_OVERLAY08_BANK32_PRIVATE_H
#include "global.h"
typedef struct W13Root {
    void *battleSystem;
    u32 unk04;
    void *bag;
    u32 heapId;
    u8 pad10[0x0C];
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u16 item;
    u8 pad22[5];
    u8 row[5];
    u8 page[5];
} W13Root;
typedef struct {
    u32 x[6];
} W13Caps;
extern const W13Caps sBattleBagSpriteCapacities;

typedef struct W13Work {
    W13Root *root;
    u8 pad04[4];
    void *palette;
    u8 pad0C[4];
    void *msgData;
    void *msgFmt;
    void *string;
    u8 pad1C[0x10];
    u8 windows[0x40];
    u8 pad6C[0x2A0];
    void *spriteManager;
    u8 pad310[0xE3D];
    u8 group;
    u8 pad114E;
    u8 counts[5];
    u8 maxRows[5];
    u8 pad1159[3];
    u16 items[5][36][2];
} W13Work;
extern void DrawFrameAndWindow2(void *, BOOL, u16, u8);
extern void FillWindowPixelBuffer(void *, u8);
extern void BattleBagMenu_ShowMessage(W13Work *);
extern void BattleBagMenu_PrintMessage(W13Work *);
extern BOOL BattleBagMenu_ValidateLastUsedItem(W13Work *);
extern void BattleBagMenu_SelectLastUsedItem(W13Work *);
extern u16 BattleBagMenu_GetItemOnPage(W13Work *, int);
extern void BattleBagMenu_InitSprites(W13Work *);
extern void TextFlags_SetCanABSpeedUpPrint(BOOL);
extern u8 BattleSystem_GetTextFrameDelay(void *);
extern u8 AddTextPrinterParameterized(void *, u8, void *, u8, u8, u8, void *);
extern BOOL Bag_HasItem(void *, u16, u16, u32);
extern void BattleBagMenu_CreateSpriteManager(W13Work *);
extern void BattleBagMenu_LoadItemIconResources(W13Work *);
extern void BattleBagMenu_CreateSprites(W13Work *);
extern void BattleBagMenu_FreeSprites(W13Work *);
extern void BattleBagMenu_CreateCursor(W13Work *);
extern void BattleBagMenu_CreateFinger(W13Work *);
extern void GfGfx_EngineBTogglePlanes(u32, BOOL);
extern void *BattleSystem_GetSpriteSystem(void *);
extern void *SpriteManager_New(void);
extern void SpriteSystem_InitSprites(void *, void *, int);
extern void SpriteSystem_InitManagerWithCapacities(void *, void *, const void *);
extern void *BattleBagMenu_CreateSprite(W13Work *, int);
extern void BattleBagMenu_SetItemIconGraphics(W13Work *, int, int);
extern void BattleBagMenu_SetItemIconPalette(W13Work *, int, int);
extern int GetItemIndexMapping(int, int);
extern void SpriteSystem_ReplaceCharResObj(void *, void *, int, int, BOOL, int);
extern void PaletteData_LoadNarc(void *, int, int, int, int, int, int);
extern void Sprite_DeleteAndFreeResources(void *);
extern void BattleBagMenu_FreeCursor(W13Work *);
extern void BattleBagMenu_FreeFinger(W13Work *);
extern void SpriteSystem_FreeResourcesAndManager(void *, void *);
extern u32 _u32_div_f(u32, u32);

extern void BattleBagMenu_PrintUseItemName(W13Work *, int);
extern void BattleBagMenu_PrintItemDescription(W13Work *, int);
extern void *NewString_ReadMsgData(void *, int);
extern void BufferItemName(void *, int, u16);
extern void StringExpandPlaceholders(void *, void *, void *);
extern void String_Delete(void *);
extern void ScheduleWindowCopyToVram(void *);
extern u8 AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, int, int);
extern void *String_New(int, u32);
extern void GetItemDescIntoString(void *, u16, u32);
#endif
