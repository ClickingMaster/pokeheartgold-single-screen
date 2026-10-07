#ifndef TO50_OVERLAY15_PREFIX_R17_PRIVATE_H
#define TO50_OVERLAY15_PREFIX_R17_PRIVATE_H

#include "global.h"
struct ItemSlot;

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define VU8(p, o) (*(volatile u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

extern u8 gSystem[];

extern u32 YesNoPrompt_HandleInput(void *);
extern void ov15_021FF058(void *);
extern void *NewString_ReadMsgData(void *, s32);
extern void BufferItemNamePlural(void *, u32, u16);
extern void BufferItemName(void *, u32, u16);
extern void BufferIntegerAsString(void *, u32, s32, u32, u32, u32);
extern void StringExpandPlaceholders(void *, void *, void *);
extern void String_Delete(void *);
extern s32 ov15_021FEF48(void *, s32);
extern void sub_0200E5D4(void *, s32);
extern void ClearFrameAndWindow2(void *, BOOL);
extern void ClearWindowTilemapAndScheduleTransfer(void *);
extern void ScheduleWindowCopyToVram(void *);
extern s32 ov15_021FA074(void *);
extern void ov15_02200140(void *, void *, s32, BOOL);
extern void ov15_021FE868(void *);
extern void ov15_021FED3C(void *);
extern void ov15_021FB518(void *);
extern void ov15_02200458(void *, s32);
extern void ov15_021FD788(void *, s32);
extern BOOL TextPrinterCheckActive(u8);
extern void PlaySE(u32);
extern void PlayerProfile_AddMoney(void *, u32);
extern BOOL Pocket_TakeItem(struct ItemSlot *, u32, u16, u16, u32);
extern void ov15_021F9F08(void *);
extern void ov15_021FA170(void *);

s32 ov15_021FD10C(void *);
s32 ov15_021FD24C(void *);
s32 ov15_021FD2FC(void *);

#endif
