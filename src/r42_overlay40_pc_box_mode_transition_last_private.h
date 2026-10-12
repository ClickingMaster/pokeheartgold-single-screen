#ifndef TO45_OV40_PC_MODE_LAST_PRIVATE_H
#define TO45_OV40_PC_MODE_LAST_PRIVATE_H

#include <nitro/fs/overlay.h>

#include "global.h"

#define PTR(p, off) (*(void **)((u8 *)(p) + (off)))
#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

int ov40_02234D94(void *);
int ov40_02234F98(void *);
int ov40_02234A38(void *);
void VsRecorder_LoadScreen(void *, int, int);
void GfGfx_EngineATogglePlanes(int, int);
void GfGfx_EngineBTogglePlanes(int, int);
typedef void (*HitboxCallback)(u32, int, void *);
void *TouchHitboxController_Create(const void *, int, HitboxCallback, void *, int);
int TouchHitboxController_IsTriggered(void *);
void TouchHitboxController_Destroy(void *);
void ov40_02235FFC(void *);
BOOL TouchscreenHitbox_TouchNewIsIn(const void *);
void VsRecorder_OnTouchConfirm(void *);
void ov40_0223610C(void);
void ov40_02235FD0(u32, int, void *);
extern const u8 ov40_02245708[];
extern const u8 ov40_022451C4[];
void SetBgPriority(int, int);
void Heap_Free(void *);
void FrontierCard_Delete(void *);
void BgClearTilemapBufferAndCommit(void *, int);
int VsRecorder_StepFade(void *, int);
int VsRecorder_StepBlend(void *, void *, int, int);
void VsRecorder_SetVBlankPaused(void *, int);
void ov40_02235B10(void *);
void PaletteData_BlendPalettes(void *, int, int, int, int);
void VsRecorder_ResetFade(void *);
void VsRecorder_RebuildMenuRowsParent(void *);
void VsRecorder_StartFlipScreens(void *, int);
int VsRecorder_IsFlipScreensDone(void *);
void AccessoryPortrait_Destroy(void);
void VsRecorder_ResetBg2(void *);
void ov40_02235994(void);
void VsRecorder_ClearPaletteBlend(void *);
void VsRecorder_SetScreenIfDone(void *, int, int, void *);
void UnloadOverlayByID(int);
void Main_SetVBlankIntrCB(void (*)(void *), void *);
void VsRecorder_VBlankCB(void *);
extern u8 SDK_OVERLAY_OVY_41_ID[];
int VsRecorder_IsNetIdle(void *);
void VsRecorder_PlaceOrbitCursor(void *, void *, int, int);
void OrbitCursor_SetVisible(void *, int);
void OrbitCursor_SetRadius(void *, int, int);
void VsRecorder_ShowTopMessage(void *, int);
void PlaySE(int);
void *VsRecorder_GetNet(void *);
int GlobalTerminalNet_RequestDressUpDownload(void *, void *);
void VsRecorder_HideTopMessage(void *);
int GlobalTerminalNet_GetError(void *, void **);
void StopSE(int, int);
void VsRecorder_ShowMessageScreen(void *, int, void *, void *);
void VsRecorder_SetSubState(void *, int);
BOOL System_GetTouchNew(void);

#endif
