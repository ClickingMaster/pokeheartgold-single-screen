#ifndef TO45_OV40_R27_U3_PRIVATE_H
#define TO45_OV40_R27_U3_PRIVATE_H

#include "global.h"

#define PTR(p, off) (*(void **)((u8 *)(p) + (off)))
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))

void BeginNormalPaletteFade(int, int, int, int, int, int, int);
BOOL IsPaletteFadeFinished(void);
void PaletteData_BlendPalettes(void *, int, int, int, u16);
void PlaySE(u16);
BOOL System_GetTouchHeld(void);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, int, void *, int, int, int, int, int);

BOOL VsRecorder_HasResumeScreen(void *);
void VsRecorder_SetSubState(void *, int);
u32 ov40_0222DAF0(void *);
void ov40_02230E34(void *);
void ov40_02230EB4(void *);
void ov40_02230964(void *, int);
void VsRecorder_LoadDecorGfx(void *);
void VsRecorder_CreateDecorSprites(void *);
void VsRecorder_LoadMenuRowGfx(void *);
void VsRecorder_CreateMenuRowSprites(void *);
void ov40_0222D2A0(void *);
void VsRecorder_RefreshChoiceRows(void *);
void VsRecorder_RefreshPathRows(void *);
void VsRecorder_SetScreenIfDone(void *, void *, int, void *);

void OrbitCursor_SetPosition(void *, s16, s16);
void OrbitCursor_SetTarget(void *, s16, s16);
void OrbitCursor_SetVisible(void *, int);
void OrbitCursor_SetActive(void *, int);
void sub_02087A30(void *);
void sub_02087A54(void *);

int ov40_02230ED8(void *);
int ov40_02231100(void *);
int ov40_0223131C(void *);

#endif
