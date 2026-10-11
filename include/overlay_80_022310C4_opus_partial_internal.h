#ifndef OV80_022310C4_PRIVATE_H
#define OV80_022310C4_PRIVATE_H

#include "global.h"

#include "heap.h"

#include "overlay_80_022310C4_state.h"

extern const u8 ov80_0223BDFC[];
extern const u8 ov80_0223BE10[];

void *Save_Frontier_GetStatic(void *save);
u32 FrontierSave_GetStat(void *frontier, u32 stat, u32 substat);

u32 HallTypeRanks_Get(u8 index, u8 *bits);
void HallTypeRanks_Set(u8 index, u8 *bits, u32 value);
void HallTypeRanks_Clear(u8 *bits);
u32 sub_0205C0A0(u32 a, u32 b);
u32 sub_0205C268(u32 a);

void *FrontierTrainer_Load(TrHouseWork *work, u32 a, u32 heapId, u32 c);
void ov80_0222A30C(u8 a);
void ov80_0222AF10(void *a);
void ov80_0222AF54(void *a);
void ov80_0222AFB8(void *a);
void ov80_0222B024(void *a, u8 b);
void ov80_0222B070(void *a);
u8 ov80_02237920(u8 a);
u32 ov80_022379C8(TrHouse *s);
u32 ov80_02237A40(u32 a);

void ov80_022313C0(TrHouse *s);
void ov80_022313C8(TrHouse *s);
void ov80_022314A0(TrHouse *s);
void ov80_022314DC(TrHouse *s, TrHouseSrc *src);
u16 ov80_02231518(TrHouseSrc *src, u32 unused);
void ov80_0223157C(TrHouse *s, u32 mode);
u8 ov80_022317C0(TrHouse *s);
u8 ov80_022317CC(TrHouse *s);
void ov80_022317D0(TrHouse *s, u32 a);
void ov80_02231804(TrHouse *s);
void ov80_02231828(TrHouse *s);
void ov80_02231844(void *a, u32 kind, u32 c);
u8 ov80_02231888(TrHouse *s);
u16 ov80_022318D0(void *save, u32 a, s32 b, u16 *out1, u16 *out2);
void ov80_022319B0(TrHouse *s);
void ov80_02231A04(TrHouse *s);


/* Generated integration prototypes. */
void ov80_022313C0(TrHouse *s);
void ov80_022314A0(TrHouse *s);
void ov80_022314DC(TrHouse *s, TrHouseSrc *src);
u16 ov80_02231518(TrHouseSrc *src, u32 unused);
u8 ov80_022317C0(TrHouse *s);
u8 ov80_022317CC(TrHouse *s);
void ov80_022317D0(TrHouse *s, u32 a);
void ov80_02231804(TrHouse *s);
void ov80_02231828(TrHouse *s);
void ov80_02231844(void *a, u32 kind, u32 c);
u8 ov80_02231888(TrHouse *s);
u16 ov80_022318D0(void *save, u32 a, s32 b, u16 *out1, u16 *out2);
void ov80_022319B0(TrHouse *s);
void ov80_02231A04(TrHouse *s);
#endif
