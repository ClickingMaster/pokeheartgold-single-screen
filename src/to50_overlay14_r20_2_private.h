#ifndef TO50_OVERLAY14_R20_2_PRIVATE_H
#define TO50_OVERLAY14_R20_2_PRIVATE_H

#include "global.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct Ov14Motion {
    u8 x;
    u8 y;
    u8 dx;
    u8 dy;
    u32 xNegative : 1;
    u32 yNegative : 1;
    u32 frames : 30;
} Ov14Motion;

extern u32 sub_02019978(void *, u32);
extern void ManagedSprite_GetPositionXY(void *, s32 *, s32 *);
extern void ManagedSprite_SetPositionXY(void *, s32, s32);
extern u32 ov14_021E8514(void *);
extern u32 ov14_021E813C(void *);
extern u32 ov14_021E80A8(void *);
extern void ov14_021F69F0(void *, u32);
extern u32 System_GetTouchHeldCoords(u32 *, u32 *);
extern u32 ov14_021E7960(s16, s16);
extern u32 ov14_021E79AC(s16, s16, const void *);
extern const u8 ov14_021F7C08[];
extern void ov14_021E6F3C(void *, u32, u32);
extern void ov14_021E7034(void *, u32, u32);
extern void ov14_021F40E8(void *, u32);
extern u32 ov14_021E70B0(void *, u32);
extern void ov14_021E7FEC(void *);
extern void ov14_021E8434(void *);
extern void ov14_021E8824(void *);
extern void ov14_021E7FB8(void *);
extern void ov14_021E7F4C(void *);
extern void ov14_021E7ED0(void *);
extern void ov14_021E8328(void *);
extern void ov14_021F2F88(u32, s16 *, s16 *, u32);
extern void ov14_021F4174(void *);
extern u32 ov14_021E65C4(void *);
extern void ov14_021E7148(void *, void *);
extern u32 ov14_021E66F4(void *);
extern void ov14_021E765C(void *);
extern void ov14_021E7EE0(void *);
extern void ov14_021F3488(void *, u32, u32);
u32 ov14_021E9A24(void *);
u32 ov14_021E9F20(void *);
u32 ov14_021E9C88(void *);

#endif
