#ifndef TO53_R3_W11_PRIVATE_H
#define TO53_R3_W11_PRIVATE_H

#include "global.h"

void sub_0200F450(void *);
void ManagedSprite_SetDrawFlag(void *, int);
void ManagedSprite_SetAnim(void *, int);
int ManagedSprite_GetActiveAnim(void *);
void ManagedSprite_SetDrawPriority(void *, int);
void ManagedSprite_SetAnimateFlag(void *, int);
void ManagedSprite_SetPositionXYWithSubscreenOffset(void *, int, int, int);
void ManagedSprite_SetPositionXY(void *, int, int);
int ov112_021E7668(void *);
int IsPaletteFadeFinished(void);
void Sound_SetSceneAndPlayBGM(int, int, int);
void PlayBGM(int);
int TextPrinterCheckActive(u8);
void ov112_021EC460(void *, int, int, int);
void GF_AssertFail(void);
void *WaitingIcon_New(void *, int);
void ManagedSprite_SetPaletteOverride(void *, int);
void ManagedSprite_SetPositionXYWithSubscreenOffset(void *, int, int, int);
extern u8 ov112_021FECE4[];
extern u8 ov112_021FECE5[];
extern u8 ov112_021FECE6[];
extern u8 ov112_021FECE7[];
void ov112_021EA17C(void *);
void ov112_021EA19C(void *);
void ov112_021EA4F4(void *);
void ov112_021EA51C(void *, int);
void ov112_021EA570(void *);
void ov112_021EA584(void *);
void ov112_021EA5A4(void *, int);
void ov112_021EA60C(void *);
void ov112_021EA64C(void *);
void ov112_021EA670(void *, int);
void ov112_021EA688(void *, int);
void ov112_021EA6A0(void *, int, int);
void ov112_021EA6B8(void *, int, int, int);
void ov112_021EA984(void *);
int ov112_021EA9A0(void *);
void ov112_021EA9BC(void *, const s16 *);
void ov112_021EAA10(void *);
void ov112_021EAA34(void *, int, int);
int ov112_021EAC18(void);
int ov112_021EAC58(void);
int ov112_021EAF48(void);
int ov112_021EB1D8(void *);
int ov112_021EB1E8(void *);
int ov112_021EB3A8(void *);
int ov112_021EB3B8(void *);
int ov112_021EB5AC(void *);
int ov112_021EB5BC(void *);

#endif
