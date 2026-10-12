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
BOOL VsRecorder_FoundVideoToggleProfile(void *p);
BOOL VsRecorder_FoundVideoConfirmOverwrite(void *p);
BOOL VsRecorder_FoundVideoOverwriteNo(void *p);
BOOL VsRecorder_FoundVideoReturnToDetails(void *p);
BOOL VsRecorder_FoundVideoSlotsCancel(void *p);
void VsRecorder_FoundVideoSave(void *p);
void VsRecorder_NumberPadFreeSprites(void *p);
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
void VsRecorder_FreeButtonGfx(void *p);
void SysTask_Destroy(void *task);
void VsRecorderApp_CountMenuChoices(void *p);
void ManagedSprite_SetPositionXY(void *sprite, int x, int y);

static inline void clear_windows(void *base, int count) {
    int i;
    for (i = 0; i < count; i++, base = (u8 *)base + 0x10) {
        ClearWindowTilemapAndCopyToVram(base);
        RemoveWindow(base);
    }
}


void VsRecorder_OnDressUpUploaded(void);
void VsRecorder_OnBoxUploaded(void);
void VsRecorder_OnVideoSaveReported(void);
void *VsRecorder_GetNet(void *p);
void VsRecorder_SetStep(void *p, u32 value);
void VsRecorder_SetSubState(void *p, int value);
void VsRecorder_ResetFade(void *p);
void VsRecorder_SetBoxUploaded(void *p, u32 value);
u32 VsRecorder_IsBoxUploaded(void *p);
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
int VsRecorder_RankingsInvalid(void);
int VsRecorder_VideoSearchSave(void *p);
int VsRecorder_NumberSearchSave(void *p);
int VsRecorder_GetDigitAnim(int value, int flag);
void VsRecorder_SetResumeScreen(void *p, u32 value);
MessageFormat *VsRecorder_NewMessageFormat(enum HeapID heapId);
void VsRecorder_SetProfileMonIconDrawFlag(void *p, BOOL draw);
void VsRecorder_DeleteSprite(void *a0, void *a1, void *sprite);
void VsRecorder_ResetRecordSave(void *p);
void VsRecorder_FreeLabeledSprite(void *p);
BOOL VsRecorder_HasAllBits(u32 a, u32 mask);
void VsRecorder_SetProfileDrawFlag(void *p, BOOL draw);
void VsRecorder_RemoveListWindow(void *p);
void VsRecorder_DeleteTouchScrollSprites(void *p);
int ov40_02230410(void *p);
void VsRecorder_CreateVideoInfo(void *p, int value);
void VsRecorder_OnTouchConfirm(void *p);
void VsRecorder_NumberPadClearAction(void *p);
void VsRecorder_FreeMenuRows(void *p);
void VsRecorder_AddGameStatsScore(void *p, int score);
BOOL VsRecorder_BoxFormHasMon(u16 *p);
void VsRecorder_FreeBoxMonPic(void *p);
void VsRecorder_UpdateNet(void *p);
void VsRecorder_FreeNet(void *p);
void VsRecorder_NumberPadSetState(void *p, u32 value);
int VsRecorder_NumberPadFree(void *p);
void VsRecorder_SetScreenIfDone(void *p, u32 value, BOOL valid, u32 *out);
void VsRecorder_RefreshMenu(void *p);
void VsRecorder_DestroyDecorTasks(void *p);
void VsRecorder_CreateProfile(void *p, BOOL value);
void VsRecorder_RemoveDressUpToggleWindow(void *p);
void VsRecorder_SelectFirstBoxMon(void *p);
void VsRecorder_RemoveDetailsButtonWindow(void *p);
BOOL VsRecorder_IsNetIdle(void *p);
void VsRecorder_RemoveProfileWindows(void *p);
void VsRecorder_RemoveVideoInfoWindows(void *p);
void VsRecorder_RemoveProfileButton(void *p);
void VsRecorder_RemoveNextMonButton(void *p);
void VsRecorder_RemoveSearchDetailsButtonWindow(void *p);
void VsRecorder_RemoveDressUpCategoryWindows(void *p);
void VsRecorder_RemoveRankingsButtonLabels(void *p);
void VsRecorder_RemoveRankingsWindows(void *p);
void VsRecorder_RemoveBoxDataCategoryButtons(void *p);
void VsRecorder_RemoveBoxDataWindows(void *p);
void VsRecorder_RemoveBoxListButtons(void *p);
void VsRecorder_RemoveBoxListTitle(void *p);
void VsRecorder_RemoveSearchCategoryWindows(void *p);
void VsRecorder_RemoveOverwriteWindows(void *p);
void VsRecorder_HideDeleteYesNo(void *p);
void VsRecorder_SetVideoPartyIconsDrawFlag(void *p, BOOL draw);
void VsRecorder_SetListArrowsY(void *p, int y1, int y2);

#endif
