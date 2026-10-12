#ifndef OV40_R33_A05_PRIVATE_H
#define OV40_R33_A05_PRIVATE_H
#include "global.h"
#define PTR(p, o)         (*(void **)((u8 *)(p) + (o)))
#define WORD(p, o)        (*(u32 *)((u8 *)(p) + (o)))
#define SWORD(p, o)       (*(s32 *)((u8 *)(p) + (o)))
#define SHALF(p, o)       (*(s16 *)((u8 *)(p) + (o)))
#define BLEND(p, w, a, b) PaletteData_BlendPalettes(PTR(p, 0x28), (a), (b), (u8)WORD(w, 8), (u16)WORD(p, 0x58))
void ManagedSprite_SetDrawFlag(void *, BOOL);
void TextOBJ_SetSpritesDrawFlag(void *, BOOL);
void ManagedSprite_SetPositionXY(void *, int, int);
void TextOBJ_SetPosition(void *, int, int);
void ClearWindowTilemapAndCopyToVram(void *);
void RemoveWindow(void *);
void *Heap_Alloc(int, u32);
void Heap_Free(void *);
void *memset(void *, int, u32);
void BgClearTilemapBufferAndCommit(void *, int);
void PaletteData_BlendPalettes(void *, int, int, int, int);
void SetBgPriority(int, int);
void GfGfx_EngineATogglePlanes(int, int);
void GfGfx_EngineBTogglePlanes(int, int);
void GfGfxLoader_LoadCharDataFromOpenNarc(void *, int, void *, int, int, int, int, int);
void InitWindow(void *);
void AddWindowParameterized(void *, void *, int, int, int, int, int, int, int);
void FillWindowPixelBuffer(void *, int);
void *NewString_ReadMsgData(void *, int);
u32 FontID_String_GetWidthMultiline(int, void *, int);
void AddTextPrinterParameterizedWithColor(void *, int, void *, int, int, int, u32, void *);
void ScheduleWindowCopyToVram(void *);
void String_Delete(void *);
void *TouchHitboxController_Create(const void *, int, const void *, void *, int);
void ManagedSprite_SetAnim(void *, int);
void PlaySE(u16);
void StopSE(u16, int);
BOOL TouchscreenHitbox_TouchNewIsIn(const void *);
BOOL BattleRecord_Exists(void);
void BattleRecord_Delete(void);
void WirelessStatusIcon_ShowOnBothScreens(int, int);
void OrbitCursor_SetVisible(void *, int);
void OrbitCursor_SetRadius(void *, int, int);
void OrbitCursor_MoveTarget(void *, int, int);
BOOL sub_02087E1C(void *);
void GF_AssertFail(void);
void VsRecorder_InitBgBlend(void *, void *, int);
BOOL VsRecorder_StepFade(void *, int);
BOOL VsRecorder_StepBlend(void *, void *, int, int);
void VsRecorder_ResetFade(void *);
void VsRecorder_SetSubState(void *, int);
void VsRecorder_SetScreenIfDone(void *, int, int, void *);
void VsRecorder_StartFlipScreens(void *, int);
void *VsRecorder_IsFlipScreensDone(void *);
int VsRecorder_HasResumeScreen(void);
void VsRecorder_SetVBlankPaused(void *, int);
void VsRecorder_FreeMenuRows(void *);
void ov40_0223A430(void *);
void VsRecorder_PlaceOrbitCursor(void *, void *, int, int);
void VsRecorder_ShowTopMessage(void *, int);
void *VsRecorder_GetNet(void *);
BOOL VsRecorder_IsNetIdle(void *);
BOOL ov39_022276A4(void *);
BOOL ov39_022276E0(void *);
BOOL ov39_02227648(void *);
BOOL ov39_022275E8(void *);
BOOL GlobalTerminalNet_GetError(void *, void **);
void VsRecorder_HideTopMessage(void *);
void VsRecorder_ShowMessageScreen(void *, int, int, int);
void VsRecorder_RebuildMenuRowsParent(void *);
void VsRecorder_ClearPaletteBlend(void *);
void ov40_0223B480(void *);
void ov40_0223D008(void *);
void ov40_0223D1AC(void *, int);
void ov40_0223D1F0(void *);
void ov40_0223D244(void *);
void VsRecorder_OnTouchConfirm(void *);
void VsRecorder_SetTextSpriteMsg(void *, void *, int);
void ov40_0223CD14(void *);
void ov40_0223CFA8(void *);
void VsRecorder_CreateVideoInfo(void *, void *);
int ov40_02230410(void *);
void VsRecorder_SetVideoPartyIconsDrawFlag(void *, int);
void VsRecorder_LoadScreen(void *, int, int);
void VsRecorder_UpdateTouchScroll(void *);
void VsRecorder_ListMoveCursor(void *, s16);
void VsRecorder_RedrawNameList(void *, void *);
void VsRecorder_FreeTouchScroll(void *);
void VsRecorder_RemoveListWindow(void *);
void VsRecorder_FreeListArrows(void *, void *);
void VsRecorder_CloseMonIconNarc(void *);
void VsRecorder_UnloadMonIconGfx(void *);
void VsRecorder_InitTouchScroll(void *);
void VsRecorder_ClearListWindow(void *);
void VsRecorder_LoadMonIconGfx(void *);
void *VsRecorder_OpenMonIconNarc(int);
void VsRecorder_InitList(void *, void *, int, void *);
void VsRecorder_TouchScrollSetEnabled(void *, void *);
void VsRecorder_CreateListArrows(void *, void *, int);
void VsRecorder_SetListArrowsY(void *, int, int);
void VsRecorder_CreateTouchScrollSprites(void *, void *);
typedef struct {
    u32 e[3];
} Ov40Msg3;
typedef struct {
    u32 e[11];
} Ov40Blk44;
typedef struct {
    u8 pad[0x2054];
    Ov40Blk44 block;
    void *obj;
    u32 pad2084;
    u32 value2088;
} Ov40BBWork;
typedef struct {
    u8 pad[0x2030];
    s32 count;
} Ov40Count2030;
typedef struct {
    u8 pad[0x202C];
    s32 count;
} Ov40Count202C;
extern const u32 ov40_02245494, ov40_02245498, ov40_0224549C, ov40_022454B0, ov40_022454E0;
extern const Ov40Blk44 ov40_022454F0;
extern const Ov40Msg3 ov40_022454A4;
void ov40_0223CE64(void);
void ov40_0223CF00(void);
void ov40_0223CF70(void);
void ov40_0223B4BC(void *, int);
void ov40_0223B538(void *);
void ov40_0223B574(void *);
int ov40_0223B5B0(void *);
int ov40_0223B62C(void *);
void ov40_0223BA70(void *);
int ov40_0223C0D8(void *);
int ov40_0223BF88(void *);
int ov40_0223BD98(void *);
int ov40_0223BB74(void *);
#endif
