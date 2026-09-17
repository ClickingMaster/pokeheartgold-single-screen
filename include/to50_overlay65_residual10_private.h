#ifndef TO50_OV65_R10_PRIVATE_H
#define TO50_OV65_R10_PRIVATE_H
#include "global.h"
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define S32(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
#define FN(p, o)  (*(void (**)(void *))((u8 *)(p) + (o)))
typedef struct Ov65Interp {
    s32 x0, y0, unused, x1, y1;
} Ov65Interp;
typedef struct Ov65AnimFlags {
    u16 pad0;
    u16 flag2;
    u8 pad4[8];
    u32 flagC;
} Ov65AnimFlags;
typedef struct Ov65Point {
    s32 x;
    s32 y;
} Ov65Point;
extern void Sprite_SetMatrix(void *, const VecFx32 *);
extern void ov65_0221D5FC(void *, s32, s32, s32, s32);
extern void ov65_0221CADC(void *, u32);
extern void ClearWindowTilemapAndScheduleTransfer(void *);
extern void ov65_0221F748(void *, s32, s32);
extern void Sprite_SetDrawFlag(void *, u32);
extern void ScheduleWindowCopyToVram(void *);
extern void ov65_0221D8C4(void *, s32, void *);
extern void FillBgTilemapRect(void *, u32, u32, u32, u32, u32, u32, u32);
extern void *Party_GetMonByIndex(void *, u32);
extern void *Mon_GetBoxMon(void *);
extern void BufferBoxMonNickname(void *, u32, void *);
extern u32 ov65_0221FB90(void *, u32, u32, u32, void *);
extern const Ov65Point ov65_0221FF4C[];
extern void ov65_0221F120(void *);
void ov65_0221ED94(void *, void *, void *, s32, const Ov65Interp *, const Ov65AnimFlags *);
int ov65_0221EE18(void *);
void ov65_0221EF0C(void *);
int ov65_0221F028(void *);
#endif
