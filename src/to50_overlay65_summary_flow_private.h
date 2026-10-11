#ifndef TO50_OVERLAY65_SUMMARY_FLOW_PRIVATE_H
#define TO50_OVERLAY65_SUMMARY_FLOW_PRIVATE_H

#include "global.h"

#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S32(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

extern u8 gSystem[];
extern const u8 _0221FD34[];
extern const u8 gOverlayTemplate_PokemonSummary[];

extern u32 Party_GetCount(void *party);
extern u32 Comm_GetLocalNetId(void);
extern void *sub_02034818(u32 value);
extern void sub_0208AD34(void *args, void *profile);
extern void *sub_02088288(void *saveData);
extern void *Save_SpecialRibbons_Get(void *saveData);
extern void sub_02089D40(void *args, const u8 *data);
extern void *OverlayManager_New(const void *template, void *args, u32 heapId);
extern void FillBgTilemapRect(void *bgConfig, u32 bgId, u32 fillValue, u32 x, u32 y, u32 width, u32 height, u32 palette);
extern void ov65_0221FB90(void *window, u32 message, u32 framed, u32 messageFormat, void *msgData);
extern u32 ov65_0221FC54(void *bgConfig, void *a1, void *a2, void *a3, u32 a4);
extern void ov65_0221DE24(void *work, u32 a1, u32 a2);
extern int ov65_0221E050(void *work);
extern int ov65_0221E8EC(void *work);
extern void ov65_0221EF0C(void *work);

void ov65_0221E06C(void *work, u32 mode);
int ov65_0221E144(void *work);
int ov65_0221E194(void *work);
int ov65_0221E270(void *work);
int ov65_0221E2C0(void *work);

#endif
