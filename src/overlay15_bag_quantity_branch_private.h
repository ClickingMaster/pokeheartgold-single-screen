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
void ov15_021FD574(BagWorkRaw *, s32, s32, s32);
void ov15_021FD43C(void *, int, int);
void ScheduleBgTilemapBufferTransfer(void *, int);
void ManagedSprite_SetDrawFlag(void *, BOOL);
void ov15_02200458(BagWorkRaw *, BOOL);
void ov15_021FD788(BagWorkRaw *, u32);
void ov15_021FF4EC(BagWorkRaw *, s32, s32);
void ov15_022002B4(BagWorkRaw *, s32);
void ov15_021FECA0(BagWorkRaw *, void *, u32);
void ov15_021FFF24(BagWorkRaw *);
String *NewString_ReadMsgData(void *, u32);
void StringExpandPlaceholders(MessageFormat *, String *, String *);
void String_Delete(String *);
u8 ov15_021FEF48(BagWorkRaw *, BOOL);
void ov15_021FF0FC(BagWorkRaw *, BOOL);
void ov15_021FF068(BagWorkRaw *);
void ov15_021FEDEC(BagWorkRaw *, u32);
int ov15_021FCB64(BagWorkRaw *);
#endif
