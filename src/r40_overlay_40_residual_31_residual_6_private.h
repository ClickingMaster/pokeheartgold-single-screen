#ifndef SOL_R40_FOLLOWUP_OV40_31_6_PRIVATE_H
#define SOL_R40_FOLLOWUP_OV40_31_6_PRIVATE_H
#include "global.h"
#pragma require_prototypes off
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U8(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
typedef struct SolCtx40 {
    u8 pad[0x330];
    void *hitbox[5];
} SolCtx40;
void VsRecorder_SetVBlankPaused(void *, int);
void ov40_02237008(void *);
void ov40_02236534(void *);
void OrbitCursor_SetActive(void *, int);
void OrbitCursor_SetVisible(void *, int);
void VsRecorder_StartFlipScreens(void *, int);
int VsRecorder_IsFlipScreensDone(void);
void TouchHitboxController_Destroy(void *);
void VsRecorder_ResetFade(void *);
void VsRecorder_RebuildMenuRowsParent(void *);
void BgClearTilemapBufferAndCommit(void *, int);
int VsRecorder_StepFade(void *, int);
void VsRecorder_ClearPaletteBlend(void *);
void PaletteData_BlendPalettes(void *, int, int, int, u16);
void VsRecorder_SetScreenIfDone(void *, int, int, void *);
void VsRecorder_SetSubState(void *, int);
void FrontierCard_Delete(void *);
void Heap_Free(void *);
#endif
