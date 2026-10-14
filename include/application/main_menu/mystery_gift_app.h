#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_APP_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_APP_H

#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "mystery_gift.h"
#include "overlay_manager.h"
#include "sprite.h"
#include "sys_task.h"

typedef struct MysteryGiftAppData MysteryGiftAppData;
typedef struct MysteryGiftAnimData MysteryGiftAnimData;
typedef struct MysteryGiftParticle MysteryGiftParticle;

typedef int (*MysteryGiftMenuFunc)(OverlayManager *manager);

typedef struct MysteryGiftMenuItem {
    u32 msgId;
    MysteryGiftMenuFunc func;
} MysteryGiftMenuItem;

void MysteryGiftApp_SetWaitingIconVisible(MysteryGiftAppData *data, BOOL visible);
void thunk_MysteryGiftApp_SetWaitingIconVisible(MysteryGiftAppData *data, BOOL visible);
void MysteryGiftApp_FreeGraphics(OverlayManager *manager);
void MysteryGiftApp_InitSprites(MysteryGiftAppData *data);
void MysteryGiftApp_SetSpriteAnim(MysteryGiftAppData *data, int animSeq);
void MysteryGiftApp_EraseWindowIfInUse(Window *window, BOOL dontCopyToVram);
void MysteryGiftApp_ClearWindowIfInUse(Window *window, BOOL dontCopyToVram);
int MysteryGiftApp_StartSearch(OverlayManager *manager);
int MysteryGiftApp_AcceptGift(OverlayManager *manager);
WonderCard *MysteryGiftApp_GetReceivedCard(OverlayManager *manager);
BOOL MysteryGiftApp_IsSpecialCard(WonderCard *card);
void MysteryGiftApp_WaitForSave(MysteryGiftAppData *data);
void MysteryGiftApp_SaveReceivedGift(OverlayManager *manager);
void MysteryGiftApp_ShowReceivePrompt(OverlayManager *manager, int mode);
int MysteryGiftApp_DeclineSearch(OverlayManager *manager);
int MysteryGiftApp_SelectGetFromFriend(OverlayManager *manager);
int MysteryGiftApp_SelectGetViaWfc(OverlayManager *manager);
int MysteryGiftApp_SelectGetViaWireless(OverlayManager *manager);
void MysteryGiftApp_AskReceiveGift(OverlayManager *manager);
int MysteryGiftApp_DeclineAcceptGift(OverlayManager *manager);
int MysteryGiftApp_ConfirmWirelessSearch(OverlayManager *manager);
int MysteryGiftApp_ConfirmReceiveGift(OverlayManager *manager);
int MysteryGiftApp_DeclineReceiveGift(OverlayManager *manager);
int MysteryGiftApp_ConfirmWfcSearch(OverlayManager *manager);
int MysteryGiftApp_CancelReceiveMenu(OverlayManager *manager);
int MysteryGiftApp_SelectReceiveGift(OverlayManager *manager);
int MysteryGiftApp_SelectCheckCard(OverlayManager *manager);
int MysteryGiftApp_SelectViewLockCapsuleCard(OverlayManager *manager);
int MysteryGiftApp_SelectExit(OverlayManager *manager);
void MysteryGiftApp_InitBgLayer(BgConfig *bgConfig, GFBgLayer bgId, u32 screenOffset, u32 charOffset);
void MysteryGiftApp_InitBgLayers(BgConfig *bgConfig);
void MysteryGiftApp_LoadSubScreenBg(BgConfig *bgConfig);
void MysteryGiftApp_LoadMainScreenBg(BgConfig *bgConfig);
void MysteryGiftApp_CreateListMenu(OverlayManager *manager, const MysteryGiftMenuItem *items, int count, Window *window, int msgId);
void MysteryGiftApp_ShowMainMenu(OverlayManager *manager, int baseTile, int msgId);
void MysteryGiftApp_PrintMessage(OverlayManager *manager, Window *window, int msgId);
void MysteryGiftApp_PrintGiftTitle(OverlayManager *manager, Window *window, const u16 *text);
void MysteryGiftApp_HandleListMenuInput(OverlayManager *manager, int *state, MysteryGiftMenuFunc cancelFunc);
BOOL MysteryGiftApp_SetupScreen(OverlayManager *manager, MysteryGiftAppData *data);
void MysteryGiftApp_RunWfcDownload(OverlayManager *manager, int *state);
void MysteryGiftApp_ShowWfcError(MysteryGiftAppData *data);
int MysteryGiftApp_ShowCannotReceiveMessage(OverlayManager *manager);
int MysteryGiftApp_PrintMessageAndWait(OverlayManager *manager, Window *window, int msgId, int nextState);
void MysteryGiftApp_StopReceiving(MysteryGiftAppData *data);
BOOL MysteryGiftApp_Init(OverlayManager *manager, int *state);
void MysteryGiftAnim_LoadResources(MysteryGiftAnimData *anim);
Sprite *MysteryGiftAnim_CreateSprite(MysteryGiftAnimData *anim, NNS_G2D_VRAM_TYPE whichScreen);
void MysteryGiftAnim_CreateGiftMainSprite(MysteryGiftAnimData *anim);
void MysteryGiftAnim_CreateGiftSubSprite(MysteryGiftAnimData *anim);
void MysteryGiftAnim_DeleteGiftMainSprite(MysteryGiftAnimData *anim);
void MysteryGiftAnim_DeleteGiftSubSprite(MysteryGiftAnimData *anim);
void MysteryGiftAnim_UpdateSprites(MysteryGiftAnimData *anim);
void MysteryGiftAnim_CreateSparkles(MysteryGiftAnimData *anim);
void MysteryGiftAnim_CreateSparkleSprites(MysteryGiftAnimData *anim);
void MysteryGiftAnim_CreateRibbons(MysteryGiftAnimData *anim);
void MysteryGiftAnim_DeleteSparkles(MysteryGiftAnimData *anim);
void MysteryGiftAnim_DeleteRibbons(MysteryGiftAnimData *anim);
void MysteryGiftAnim_InitSparkles(MysteryGiftAnimData *anim);
void MysteryGiftAnim_StartSparkleTasks(MysteryGiftAnimData *anim);
void MysteryGiftAnim_StartRibbonTasks(MysteryGiftAnimData *anim);
BOOL MysteryGiftAnim_StepTowards(s32 x, s32 y, float fromX, float fromY, float *outX, float *outY, float speed, int minDistance);
void MysteryGiftAnim_UpdateParticle(SysTask *task, MysteryGiftParticle *particle);
void MysteryGiftAnim_ApplyBrightnessMain(MysteryGiftAnimData *anim);
void MysteryGiftAnim_ApplyBrightnessSub(MysteryGiftAnimData *anim);
void MysteryGiftAnim_ApplyBrightness(MysteryGiftAnimData *anim);
void MysteryGiftAnim_SetMaskColorsBlack(void);
BOOL MysteryGiftAnim_StepBrightness(MysteryGiftAnimData *anim, int mode, int step);
BOOL MysteryGiftAnim_StepBrightnessMain(MysteryGiftAnimData *anim, int mode, int step);
void MysteryGiftAnim_Init(MysteryGiftAnimData *anim);
void MysteryGiftAnim_Task(SysTask *task, MysteryGiftAnimData *anim);
BOOL MysteryGiftApp_Main(OverlayManager *manager, int *state);
OverlayManager *MysteryGiftApp_GetOverlayManager(void);
void MysteryGiftApp_SetOverlayManager(OverlayManager *manager);
BOOL MysteryGiftApp_Exit(OverlayManager *manager, int *state);
void MysteryGiftApp_UpdateBeaconWatch(MysteryGiftAppData *data);
void MysteryGiftApp_BeaconCallback(int choice);
void MysteryGiftApp_StopWirelessLink(void);
void MysteryGiftApp_UpdateWirelessSearch(MysteryGiftAppData *data);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_APP_H
