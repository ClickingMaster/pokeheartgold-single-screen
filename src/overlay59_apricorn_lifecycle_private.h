#ifndef W12_OV59_R9_PRIVATE_H
#define W12_OV59_R9_PRIVATE_H
#include "global.h"
typedef u8 Ov59Work;
#define U8_(w,o) (*(u8 *)((w)+(o)))
#define S8_(w,o) (*(s8 *)((w)+(o)))
#define U16_(w,o) (*(u16 *)((w)+(o)))
#define PTR_(w,o) (*(void **)((w)+(o)))
u32 ov59_0223ABEC(Ov59Work *);
u32 ov59_0223AC48(Ov59Work *);
void ov59_0223ACD0(void);
void ov59_0223AD20(Ov59Work *);
extern BOOL ov59_0223AF9C(Ov59Work *);
extern BOOL ov59_0223B11C(Ov59Work *);
extern BOOL ov59_0223B164(Ov59Work *);
extern void Main_SetVBlankIntrCB(void *, void *);
extern void HBlankInterruptDisable(void);
extern void GfGfx_DisableEngineAPlanes(void);
extern void GfGfx_DisableEngineBPlanes(void);
extern void SetMasterBrightnessByColor(int, int);
extern void ResetVisibleHardwareWindows(int);
extern void ov59_0223B434(Ov59Work *);
extern void ov59_0223B590(Ov59Work *);
extern void ov59_0223B68C(Ov59Work *);
extern void ov59_0223B740(Ov59Work *);
extern void ov59_0223B8B0(Ov59Work *);
extern void *ov59_0223C298(void *);
extern void ov59_0223B3DC(void *);
#endif
