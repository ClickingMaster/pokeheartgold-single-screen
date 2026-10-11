#ifndef TO46_OV15_GAP2_PRIVATE_H
#define TO46_OV15_GAP2_PRIVATE_H
#include "global.h"

#include "item.h"
#include "message_format.h"
#include "pm_string.h"
u16 Pocket_GetQuantity(ItemSlot *, u32, u16, u32);
typedef struct BagWorkRaw {
    u8 raw[0x900];
} BagWorkRaw;
#define P(p, o)   (*(void **)((p)->raw + (o)))
#define U32(p, o) (*(u32 *)((p)->raw + (o)))
#define S32(p, o) (*(s32 *)((p)->raw + (o)))
#define U16(p, o) (*(u16 *)((p)->raw + (o)))
#define S16(p, o) (*(s16 *)((p)->raw + (o)))
#define U8(p, o)  (*(u8 *)((p)->raw + (o)))
void BagApp_LoadSubScreenLayout(BagWorkRaw *, s32, s32, s32);
void BagApp_ApplyTilemapCommands(void *, int, int);
void ScheduleBgTilemapBufferTransfer(void *, int);
void ManagedSprite_SetDrawFlag(void *, BOOL);
void BagApp_ShowPocketTabSprites(BagWorkRaw *, BOOL);
void BagApp_ShowCancelButton(BagWorkRaw *, u32);
void BagApp_DrawSelectedItemEntry(BagWorkRaw *, s32, s32);
void BagApp_ShowSelectedItemIcon(BagWorkRaw *, s32);
void BagApp_ShowItemDescription(BagWorkRaw *, void *, u32);
void BagApp_HideCursor(BagWorkRaw *);
String *NewString_ReadMsgData(void *, u32);
void StringExpandPlaceholders(MessageFormat *, String *, String *);
void String_Delete(String *);
u8 BagApp_PrintMessage(BagWorkRaw *, BOOL);
void BagApp_PrintMoney(BagWorkRaw *, BOOL);
void BagApp_PrintSellPrice(BagWorkRaw *);
void BagApp_PrintQuantityDigits(BagWorkRaw *, u32);
int BagApp_State_StartSell(BagWorkRaw *);
#endif
