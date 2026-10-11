#ifndef TO45_OV40_R27_U8_PRIVATE_H
#define TO45_OV40_R27_U8_PRIVATE_H

#include "global.h"

#define PTR(p, off)  (*(void **)((u8 *)(p) + (off)))
#define WORD(p, off) (*(u32 *)((u8 *)(p) + (off)))

void BeginNormalPaletteFade(int, int, int, int, int, int, int);
void BgClearTilemapBufferAndCommit(void *, int);
void GfGfxLoader_LoadScrnDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void GfGfx_EngineATogglePlanes(int, int);
void GfGfx_EngineBTogglePlanes(int, int);
void Heap_Free(void *);
BOOL IsPaletteFadeFinished(void);
void PaletteData_BlendPalettes(void *, int, u16, u8, u16);
void SaveGameNormal(void *);
BOOL Save_NumModifiedPCBoxesIsMany(void *);
void *TouchHitboxController_Create(const void *, int, const void *, void *, int);
void TouchHitboxController_Destroy(void *);
BOOL TouchscreenHitbox_TouchNewIsIn(const void *);
void VsRecorder_SetScreenIfDone(void *, int, int, void *);
void VsRecorder_SetSubState(void *, int);
void VsRecorder_ClearResumeState(void *);
void VsRecorder_SetResumeScreen(void *, void *);
void VsRecorder_RebuildMenuRowsParent(void *);
void VsRecorder_RebuildMenuRows(void *);
BOOL VsRecorder_StepBlend(void *, void *, int, int);
BOOL VsRecorder_StepFade(void *, int);
void VsRecorder_ResetFade(void *);
void VsRecorder_ClearPaletteBlend(void *);
void VsRecorder_ShowTopMessage(void *, int);
void VsRecorder_SetMessageText(void *, int);
void VsRecorder_HideTopMessage(void);
void VsRecorder_FreeProfile(void *, void *);
void VsRecorder_StartFlipScreens(void *, int);
BOOL VsRecorder_IsFlipScreensDone(void);
void VsRecorder_LoadSavedVideos(void *);
void VsRecorder_FreeSavedVideos(void *);
int ov40_02230410(void *);
void VsRecorder_CreateVideoInfo(void *, void *);
void VsRecorder_FreeVideoInfo(void *, void *);
void VsRecorder_SetVideoPartyIconsDrawFlag(void *, int);
void VsRecorder_LoadScreen(void *, int, int);
void VsRecorder_OnTouchConfirm(void *);
void VsRecorder_SetVBlankPaused(void *, int);
void ov40_022326DC(void);
void ov40_022327BC(void *);
void ov40_02232F50(void *);
void ov40_02232F88(void *);
void ov40_02232FCC(void *);
void ov40_02233044(void *);
void ov40_022330B8(void *);
void ov40_02244BBC(void *, void *);
void BattleRecord_Delete(void);
void BattleRecord_Load(void *, int, void *, int, void *);
u64 BattleRecordHeader_GetField(void *, int, int);
void sub_02087A84(void *, int, int);

extern const u8 ov40_0224512C[];
extern const u8 ov40_02245130[];
extern const u8 ov40_02245134[];

void ov40_02232ED4(int, int, void *);
int ov40_022327F0(void *);
int ov40_02232A48(void *);
int ov40_02232AF8(void *);
int ov40_02232BD8(void *);
int ov40_02232D44(void *);

#endif
