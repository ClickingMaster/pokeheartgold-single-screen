#ifndef TO53_SOL_R1_W20_PRIVATE_H
#define TO53_SOL_R1_W20_PRIVATE_H

#include "global.h"

extern const u8 ov91_02262754[];

void GF_SndHandleSetTempoRatio(int, int);
void Comm_RegisterCommandTable(const void *, int, int);
BOOL Comm_GetLocalNetId(void);
u32 ov91_0225CA64(u32, u32, u32);
u32 ov91_0225CA6C(u32, u32, u32);
void ov91_0225CA74(u32, u32, u32);
void ov91_0225CA90(u32, u32, u32);
void ov91_0225CA9C(u32, u32, u32);
void ov91_0225CABC(u32, u32, u32);
void ov91_0225CAE0(u32, u32);
void ov91_0225CAEC(u32, u32, u32);
u32 ov91_0225CB50(u32, u32, u32);
u32 PlaySE(u32);
void Camera_GetAngle(void *, void *);
void Camera_SetAnglePos(void *, void *);
u32 Sprite_Delete(void *);
void Sprite_SetDrawFlag(void *, BOOL);
void sub_020182A0(void *, int);
void sub_020180F8(void *, void *);
void sub_02018068(void *);
void sub_020182A0(void *, int);
void sub_020181B0(void *, void *);
u32 sub_0201815C(void *, u32, u32, u32);
void sub_020181D4(void *, u32);
void sub_020181E0(void *, u32);

void ov91_02261B10(void);
const u8 *ov91_02261B20(void);
int ov91_02261B28(void);
void ov91_02261B2C(int);
int ov91_02261B48(void);
u32 ov91_02261B4C(u32, u32, u32, u32);
u32 ov91_02261B58(u32, u32, u32, u32);
void ov91_02261B64(u32, u32, u32, u32);
void ov91_02261B8C(u32, u32, const u32 *, u32);
void ov91_02261B9C(u32, u32, u32, u32);
void ov91_02261BAC(u32, u32, const u32 *, u32);
u32 ov91_02261BBC(u32, u32, u32, u32);
void ov91_02261BC8(u32, u32, const u32 *, u32);
int ov91_02261BD8(void);
int ov91_02261BDC(void);
int ov91_02261BE0(void);
int ov91_02261BE4(void);

void ov91_02261200(u8 *, s32);
void ov91_0226137C(u8 *, u32);
void ov91_02261384(u8 *, u16);
void ov91_022613AC(u8 *, u8);
void ov91_022614D4(u8 *);
u32 ov91_022619E8(u8 *);
u32 ov91_02260B48(u8 *, u32);
void ov91_02260830(u32, u8 *);
void ov91_02260A10(void);
void ov91_02260A2C(u8 *);
u32 ov91_02260CA8(u8 *);
void ov91_02260CB4(u8 *);
void ov91_02260D00(u8 *);
void ov91_02260D80(u32, u8 *);
void ov91_02261070(u8 *, u32, void *);
void ov91_022611D0(u8 *);
u32 ov91_0226031C(u8 *, u32, u32, u32);
void ov91_02260334(u8 *, u16);
void ov91_02260364(u8 *, u16);
void ov91_02260370(u8 *, u16);
void ov91_02260400(int, int, s32 *);

#endif
