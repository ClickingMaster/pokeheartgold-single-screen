#ifndef TO45_SOL_R3_OV40_PC_MODE_PREFIX_H
#define TO45_SOL_R3_OV40_PC_MODE_PREFIX_H

#include "global.h"
#include <nitro/fs/overlay.h>

#define PTR(p, off)  (*(void **)((u8 *)(p) + (off)))
#define U32(p, off)  (*(u32 *)((u8 *)(p) + (off)))
#define S32(p, off)  (*(s32 *)((u8 *)(p) + (off)))
#define U16(p, off)  (*(u16 *)((u8 *)(p) + (off)))
#define S16(p, off)  (*(s16 *)((u8 *)(p) + (off)))
#define U8(p, off)   (*(u8 *)((u8 *)(p) + (off)))

typedef struct Block44 { u32 v[11]; } Block44;
typedef void (*HitboxCallback)(u32, int, void *);

void VsRecorder_LoadScreen(void *, int, int);
void GfGfx_EngineATogglePlanes(int, int);
void GfGfx_EngineBTogglePlanes(int, int);
void *TouchHitboxController_Create(const void *, int, HitboxCallback, void *, int);
int TouchHitboxController_IsTriggered(void *);
void TouchHitboxController_Destroy(void *);
int VsRecorder_StepFade(void *, int);
int VsRecorder_StepBlend(void *, void *, int, int);
void VsRecorder_ShowTopMessage(void *, int);
void ov40_02235FFC(void *);
void PaletteData_BlendPalettes(void *, int, int, int, int);
BOOL TouchscreenHitbox_TouchNewIsIn(const void *);
void VsRecorder_OnTouchConfirm(void *);
void ov40_0223610C(void);
void VsRecorder_HideTopMessage(void *);
void ov40_022359B4(void *, int);
void VsRecorder_SetVBlankPaused(void *, int);
void VsRecorder_CreateTouchScrollSprites(void *, void *);
void VsRecorder_ClearListWindow(void *);
void VsRecorder_InitList(void *, void *, void *, void *);
void VsRecorder_TouchScrollSetEnabled(void *, void *);
void VsRecorder_CreateListArrows(void *, void *, int);
void VsRecorder_UpdateTouchScroll(void *);
void VsRecorder_ListScroll(void *, s16);
int VsRecorder_UpdateMessageList(void *, void *);
void VsRecorder_FreeTouchScroll(void *);
void VsRecorder_RemoveListWindow(void *);
void VsRecorder_FreeListArrows(void *, void *);
void ov40_02235FA0(void *);
void SetBgPriority(int, int);
void Heap_Free(void *);
void FrontierCard_Delete(void *);
void BgClearTilemapBufferAndCommit(void *, int);
void ov40_02235B10(void *);
void VsRecorder_ResetFade(void *);
void VsRecorder_RebuildMenuRowsParent(void *);
void VsRecorder_StartFlipScreens(void *, int);
int VsRecorder_IsFlipScreensDone(void *);
void AccessoryPortrait_Destroy(void);
void VsRecorder_ResetBg2(void *);
void ov40_02235994(void);
void VsRecorder_ClearPaletteBlend(void *);
void VsRecorder_SetScreenIfDone(void *, int, int, void *);
void VsRecorder_SetSubState(void *, int);
void UnloadOverlayByID(int);
void Main_SetVBlankIntrCB(void (*)(void *), void *);
void VsRecorder_VBlankCB(void *);
int VsRecorder_IsNetIdle(void *);
void VsRecorder_PlaceOrbitCursor(void *, void *, int, int);
void OrbitCursor_SetVisible(void *, int);
void OrbitCursor_SetRadius(void *, int, int);
void PlaySE(int);
void *VsRecorder_GetNet(void *);
int GlobalTerminalNet_RequestDressUpDownload(void *, void *);
int GlobalTerminalNet_GetError(void *, void **);
void StopSE(int, int);
void VsRecorder_ShowMessageScreen(void *, int, void *, void *);
BOOL System_GetTouchNew(void);

extern const u8 ov40_02245708[];
extern const u8 ov40_022451C4[];
extern const Block44 ov40_022451F4;
extern u8 SDK_OVERLAY_OVY_41_ID[];
void ov40_02235FD0(u32, int, void *);

int ov40_02234A38(void *);
int ov40_02234BBC(void *);
int ov40_02234D94(void *);
int ov40_02234F98(void *);

#endif
