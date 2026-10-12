#ifndef OV40_PRIVATE_H
#define OV40_PRIVATE_H

#include "global.h"

#include "message_format.h"
#include "sprite.h"

typedef struct Ov40State {
    u8 raw[0x4000];
} Ov40State;

void TouchHitboxController_IsTriggered(void *controller);
void TouchHitboxController_Destroy(void *controller);
void ManagedSprite_SetPositionXYWithSubscreenOffset(void *sprite, int x, int y, int offset);
void ManagedSprite_GetPositionXYWithSubscreenOffset(ManagedSprite *sprite, s16 *x, s16 *y, fx32 offset);
void ManagedSprite_SetDrawFlag(void *sprite, BOOL draw);
void Sprite_DeleteAndFreeResources(void *sprite);
void ClearWindowTilemapAndCopyToVram(void *window);
void RemoveWindow(void *window);
void GfGfx_SwapDisplay(void);
void GF_AssertFail(void);
void PlaySE(u16 seqNo);
void *Save_GameStats_Get(void *saveData);
void GameStats_AddScore(void *stats, int score);
void Pokepic_Delete(void *pic);
void FrontierCard_Delete(void *ptr);
void GlobalTerminalNet_Update(void *ptr);
void GlobalTerminalNet_Free(void *ptr);

void VsRecorder_SetMenuRowsDrawFlag(void *p, int value);
void VsRecorder_DeleteMenuRowSprites(void *p);
void VsRecorder_UnloadMenuRowChars(void *p);
void VsRecorder_InitBlend(void *p, int a1, int a2, int a3, int a4, int a5, void *a6);
void VsRecorder_CreateTouchScrollSpritesWithAnim(void *p, int a1, int count);
int ov40_022303B8(void *p);
void VsRecorder_PrintVideoInfo(void *p);
void VsRecorder_CreateVideoPartyIcons(void *p, int value);
void VsRecorder_ShowTouchIndicator(void *p);
void VsRecorder_FreeTextSprite(void *p);
BOOL ov40_0224222C(void *p);
BOOL ov40_02242490(void *p);
BOOL ov40_0224253C(void *p);
BOOL ov40_022428D4(void *p);
BOOL ov40_02242AEC(void *p);
void ov40_022425E8(void *p);
void ov40_02243B94(void *p);
void VsRecorder_SetSubState(void *p, int value);
void VsRecorder_SetStep(void *p, u32 value);
void VsRecorder_DeleteTouchScrollSprites(void *p);
void VsRecorder_SetProfileMonIconDrawFlag(void *p, BOOL draw);
void VsRecorder_SetProfileAvatarDrawFlag(void *p, BOOL draw);
int ov40_02230404(void *p);
void VsRecorder_RefreshChoiceRows(void *p);
void VsRecorder_PrintProfile(void *p, BOOL value);
void VsRecorder_CreateProfileAvatar(void *p, BOOL value);
void VsRecorder_CreateProfileMonIcon(void *p, BOOL value);
BOOL VsRecorder_IsNetIdle(void *p);
BOOL GlobalTerminalNet_IsIdle(void *p);
void ov40_0222D7DC(void *p);
void SysTask_Destroy(void *task);
void sub_02088030(void *p);
void ManagedSprite_SetPositionXY(void *sprite, int x, int y);

static inline void clear_windows(void *base, int count) {
    int i;
    for (i = 0; i < count; i++, base = (u8 *)base + 0x10) {
        ClearWindowTilemapAndCopyToVram(base);
        RemoveWindow(base);
    }
}


void ov40_02236158(void);
void ov40_02238EB8(void);
void ov40_02242E48(void);
void *VsRecorder_GetNet(void *p);
void VsRecorder_SetStep(void *p, u32 value);
void VsRecorder_SetSubState(void *p, int value);
void VsRecorder_ResetFade(void *p);
void ov40_0222DD8C(void *p, u32 value);
u32 ov40_0222DD94(void *p);
void *VsRecorder_IsFlipScreensDone(void *p);
void *VsRecorder_HasResumeScreen(void *p);
void *VsRecorder_IsVBlankPaused(void *p);
void VsRecorder_SetVBlankPaused(void *p, void *value);
void VsRecorder_SetBattleResult(void *p, u32 value);
void VsRecorder_FreeTouchScroll(void *p);
void VsRecorder_CloseMonIconNarc(void *narc);
void VsRecorder_UpdateMenuTouch(void *p);
void VsRecorder_SetProfileAvatarDrawFlag(void *p, BOOL draw);
void VsRecorder_ClearListWindow(void *p);
void VsRecorder_InitTouchScroll(void *p);
void *VsRecorder_OpenMonIconNarc(void);
int ov40_02230404(void *p);
void VsRecorder_SetSpritePos(void *sprite, int x, int y);
void VsRecorder_GetSpritePos(void *sprite, s16 *x, s16 *y);
void VsRecorder_CreateTouchScrollSprites(void *p, int value);
int ov40_0223A274(void);
int ov40_0223FD80(void *p);
int ov40_0224147C(void *p);
int ov40_02244054(int value, int flag);
void VsRecorder_SetResumeScreen(void *p, u32 value);
MessageFormat *VsRecorder_NewMessageFormat(enum HeapID heapId);
void VsRecorder_SetProfileMonIconDrawFlag(void *p, BOOL draw);
void VsRecorder_DeleteSprite(void *a0, void *a1, void *sprite);
void ov40_022306E0(void *p);
void VsRecorder_FreeLabeledSprite(void *p);
BOOL ov40_022371D4(u32 a, u32 mask);
void VsRecorder_SetProfileDrawFlag(void *p, BOOL draw);
void VsRecorder_RemoveListWindow(void *p);
void VsRecorder_DeleteTouchScrollSprites(void *p);
int ov40_02230410(void *p);
void VsRecorder_CreateVideoInfo(void *p, int value);
void VsRecorder_OnTouchConfirm(void *p);
void ov40_022439B8(void *p);
void VsRecorder_FreeMenuRows(void *p);
void VsRecorder_AddGameStatsScore(void *p, int score);
BOOL ov40_022361C8(u16 *p);
void ov40_02237564(void *p);
void VsRecorder_UpdateNet(void *p);
void VsRecorder_FreeNet(void *p);
void ov40_0224320C(void *p, u32 value);
int ov40_0224326C(void *p);
void VsRecorder_SetScreenIfDone(void *p, u32 value, BOOL valid, u32 *out);
void VsRecorder_RefreshMenu(void *p);
void VsRecorder_DestroyDecorTasks(void *p);
void VsRecorder_CreateProfile(void *p, BOOL value);
void VsRecorder_RemoveDressUpToggleWindow(void *p);
void ov40_0223655C(void *p);
void ov40_0223CCA0(void *p);
BOOL VsRecorder_IsNetIdle(void *p);
void VsRecorder_RemoveProfileWindows(void *p);
void VsRecorder_RemoveVideoInfoWindows(void *p);
void VsRecorder_RemoveProfileButton(void *p);
void ov40_02237C54(void *p);
void ov40_02241A34(void *p);
void VsRecorder_RemoveDressUpCategoryWindows(void *p);
void ov40_022393F4(void *p);
void ov40_02239514(void *p);
void ov40_02236FE0(void *p);
void ov40_02237008(void *p);
void ov40_02237C74(void *p);
void ov40_02237D6C(void *p);
void ov40_0223E848(void *p);
void ov40_0223CE38(void *p);
void ov40_02244494(void *p);
void VsRecorder_SetVideoPartyIconsDrawFlag(void *p, BOOL draw);
void VsRecorder_SetListArrowsY(void *p, int y1, int y2);

#endif
