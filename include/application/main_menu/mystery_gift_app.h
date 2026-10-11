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
void ov74_02229E60(MysteryGiftAppData *data, BOOL visible);
void MysteryGiftApp_FreeGraphics(OverlayManager *manager);
void MysteryGiftApp_InitSprites(MysteryGiftAppData *data);
void MysteryGiftApp_SetSpriteAnim(MysteryGiftAppData *data, int animSeq);
void MysteryGiftApp_EraseWindowIfInUse(Window *window, BOOL dontCopyToVram);
void MysteryGiftApp_ClearWindowIfInUse(Window *window, BOOL dontCopyToVram);
int MysteryGiftApp_StartSearch(OverlayManager *manager);
int MysteryGiftApp_AcceptGift(OverlayManager *manager);
WonderCard *MysteryGiftApp_GetReceivedCard(OverlayManager *manager);
BOOL ov74_0222A0E4(WonderCard *card);
void MysteryGiftApp_WaitForSave(MysteryGiftAppData *data);
void MysteryGiftApp_SaveReceivedGift(OverlayManager *manager);
void MysteryGiftApp_ShowReceivePrompt(OverlayManager *manager, int mode);
int ov74_0222A2A4(OverlayManager *manager);
int MysteryGiftApp_SelectGetFromFriend(OverlayManager *manager);
int MysteryGiftApp_SelectGetViaWfc(OverlayManager *manager);
int MysteryGiftApp_SelectGetViaWireless(OverlayManager *manager);
void ov74_0222A43C(OverlayManager *manager);
int ov74_0222A494(OverlayManager *manager);
int ov74_0222A4CC(OverlayManager *manager);
int ov74_0222A504(OverlayManager *manager);
int ov74_0222A538(OverlayManager *manager);
int ov74_0222A560(OverlayManager *manager);
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
void ov74_0222B2C4(MysteryGiftAnimData *anim);
void MysteryGiftAnim_CreateRibbons(MysteryGiftAnimData *anim);
void MysteryGiftAnim_DeleteSparkles(MysteryGiftAnimData *anim);
void MysteryGiftAnim_DeleteRibbons(MysteryGiftAnimData *anim);
void ov74_0222B3A4(MysteryGiftAnimData *anim);
void ov74_0222B5F8(MysteryGiftAnimData *anim);
void ov74_0222B760(MysteryGiftAnimData *anim);
BOOL MysteryGiftAnim_StepTowards(s32 x, s32 y, float fromX, float fromY, float *outX, float *outY, float speed, int minDistance);
void MysteryGiftAnim_UpdateParticle(SysTask *task, MysteryGiftParticle *particle);
void MysteryGiftAnim_ApplyBrightnessMain(MysteryGiftAnimData *anim);
void MysteryGiftAnim_ApplyBrightnessSub(MysteryGiftAnimData *anim);
void MysteryGiftAnim_ApplyBrightness(MysteryGiftAnimData *anim);
void ov74_0222BF18(void);
BOOL MysteryGiftAnim_StepBrightness(MysteryGiftAnimData *anim, int mode, int step);
BOOL MysteryGiftAnim_StepBrightnessMain(MysteryGiftAnimData *anim, int mode, int step);
void MysteryGiftAnim_Init(MysteryGiftAnimData *anim);
void MysteryGiftAnim_Task(SysTask *task, MysteryGiftAnimData *anim);
BOOL MysteryGiftApp_Main(OverlayManager *manager, int *state);
OverlayManager *MysteryGiftApp_GetOverlayManager(void);
void MysteryGiftApp_SetOverlayManager(OverlayManager *manager);
BOOL MysteryGiftApp_Exit(OverlayManager *manager, int *state);
void ov74_0222CE10(MysteryGiftAppData *data);
void ov74_0222CE6C(int choice);
void ov74_0222CEC0(void);
void ov74_0222CEE0(MysteryGiftAppData *data);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_APP_H
