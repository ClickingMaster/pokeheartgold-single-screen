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
extern const W13Caps ov08_02225D14;

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
extern void ov08_02223B20(W13Work *);
extern void ov08_02223B48(W13Work *);
extern BOOL ov08_02223B78(W13Work *);
extern void ov08_02223BA8(W13Work *);
extern u16 ov08_02223CD4(W13Work *, int);
extern void ov08_02223D08(W13Work *);
extern void TextFlags_SetCanABSpeedUpPrint(BOOL);
extern u8 BattleSystem_GetTextFrameDelay(void *);
extern u8 AddTextPrinterParameterized(void *, u8, void *, u8, u8, u8, void *);
extern BOOL Bag_HasItem(void *, u16, u16, u32);
extern void ov08_02223D34(W13Work *);
extern void ov08_02223D80(W13Work *);
extern void ov08_02223F14(W13Work *);
extern void ov08_02223F34(W13Work *);
extern void ov08_022240A8(W13Work *);
extern void ov08_0222419C(W13Work *);
extern void GfGfx_EngineBTogglePlanes(u32, BOOL);
extern void *BattleSystem_GetSpriteSystem(void *);
extern void *SpriteManager_New(void);
extern void SpriteSystem_InitSprites(void *, void *, int);
extern void SpriteSystem_InitManagerWithCapacities(void *, void *, const void *);
extern void *ov08_02223EA4(W13Work *, int);
extern void ov08_02223E3C(W13Work *, int, int);
extern void ov08_02223E74(W13Work *, int, int);
extern int GetItemIndexMapping(int, int);
extern void SpriteSystem_ReplaceCharResObj(void *, void *, int, int, BOOL, int);
extern void PaletteData_LoadNarc(void *, int, int, int, int, int, int);
extern void Sprite_DeleteAndFreeResources(void *);
extern void ov08_02224108(W13Work *);
extern void ov08_022241F4(W13Work *);
extern void SpriteSystem_FreeResourcesAndManager(void *, void *);
extern u32 _u32_div_f(u32, u32);

extern void ov08_022239CC(W13Work *, int);
extern void ov08_02223A3C(W13Work *, int);
extern void *NewString_ReadMsgData(void *, int);
extern void BufferItemName(void *, int, u16);
extern void StringExpandPlaceholders(void *, void *, void *);
extern void String_Delete(void *);
extern void ScheduleWindowCopyToVram(void *);
extern u8 AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, int, int);
extern void *String_New(int, u32);
extern void GetItemDescIntoString(void *, u16, u32);
#endif
