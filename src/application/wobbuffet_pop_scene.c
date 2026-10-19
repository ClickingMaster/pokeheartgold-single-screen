#include "global.h"

#include "constants/sndseq.h"

#include "bg_window.h"
#include "communication_command.h"
#include "communication_core.h"
#include "error_handling.h"
#include "filesystem.h"
#include "font.h"
#include "font_sprite.h"
#include "gf_gfx_loader.h"
#include "math_util.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "overlay_manager.h"
#include "palette.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "text.h"
#include "wobbuffet_pop.h"

typedef struct WobbuffetPopRingSpeed {
    u8 steps;
    u8 delay;
    u8 timer;
    u8 unused;
} WobbuffetPopRingSpeed;

typedef struct WobbuffetPopInflationGfx {
    u16 charFile;
    u16 scrnFile;
    s32 scale;
} WobbuffetPopInflationGfx;

enum WobbuffetPopPlazaScreenState {
    WOBBUFFET_POP_PLAZA_SCREEN_START,
    WOBBUFFET_POP_PLAZA_SCREEN_CREATE_ENTRY,
    WOBBUFFET_POP_PLAZA_SCREEN_WAIT_ENTRY,
    WOBBUFFET_POP_PLAZA_SCREEN_CREATE_RESULTS,
    WOBBUFFET_POP_PLAZA_SCREEN_WAIT_RESULTS,
    WOBBUFFET_POP_PLAZA_SCREEN_DONE,
};

enum WobbuffetPopCommandId {
    WOBBUFFET_POP_CMD_PLAYER_VALUE = 23,
    WOBBUFFET_POP_CMD_EVENT,
    WOBBUFFET_POP_CMD_FINISHED,
};

enum WobbuffetPopBonusPopupState {
    WOBBUFFET_POP_BONUS_POPUP_START,
    WOBBUFFET_POP_BONUS_POPUP_DROP_IN,
    WOBBUFFET_POP_BONUS_POPUP_HIT,
    WOBBUFFET_POP_BONUS_POPUP_WAIT_HIT,
    WOBBUFFET_POP_BONUS_POPUP_RISE_OUT,
};

enum WobbuffetPopScoreDisplayState {
    WOBBUFFET_POP_SCORE_DISPLAY_CHECK,
    WOBBUFFET_POP_SCORE_DISPLAY_ROLL,
};

enum WobbuffetPopBonus {
    WOBBUFFET_POP_BONUS_NONE,
    WOBBUFFET_POP_BONUS_DOUBLE,
    WOBBUFFET_POP_BONUS_HALVE,
    WOBBUFFET_POP_BONUS_TRIPLE,
};

enum WobbuffetPopBonusTargetState {
    WOBBUFFET_POP_BONUS_TARGET_IDLE,
    WOBBUFFET_POP_BONUS_TARGET_ENTER,
    WOBBUFFET_POP_BONUS_TARGET_WALK,
    WOBBUFFET_POP_BONUS_TARGET_HIT,
};

enum WobbuffetPopBonusTargetEnterStep {
    WOBBUFFET_POP_BONUS_TARGET_ENTER_SHOW,
    WOBBUFFET_POP_BONUS_TARGET_ENTER_MOVE_IN,
};

enum WobbuffetPopBonusTargetHitStep {
    WOBBUFFET_POP_BONUS_TARGET_HIT_FLASH,
    WOBBUFFET_POP_BONUS_TARGET_HIT_WAIT,
    WOBBUFFET_POP_BONUS_TARGET_HIT_START_HOP,
    WOBBUFFET_POP_BONUS_TARGET_HIT_HOP,
};

extern const u32 gWobbuffetPopCommands[];
extern void (*const gWobbuffetPopEventFuncs[])(WobbuffetPop *game, WobbuffetPopEvent *event);

static void WobbuffetPopInflation_Start(BgConfig *bgConfig, int playerCount, int size, WobbuffetPopInflation *inflation);
static void WobbuffetPopInflation_Stop(BgConfig *bgConfig, WobbuffetPopInflation *inflation);
static void WobbuffetPopInflation_UpdateBg(BgConfig *bgConfig, WobbuffetPopInflation *inflation, int playerCount);
static void WobbuffetPopInflation_CalcScale(int playerCount, int size, int air, int *scaleX, int *scaleY);
static s32 WobbuffetPopInflation_AddAir(WobbuffetPop *game, WobbuffetPopInflation *inflation, s32 amount, s16 netId, u8 bonus);
static BOOL WobbuffetPopInflation_StepAir(WobbuffetPopInflation *inflation);
static s32 WobbuffetPop_GetInflationPercent(WobbuffetPop *game);
static u32 WobbuffetPopPuff_Init(WobbuffetPop *game, const WobbuffetPopPump *pump, WobbuffetPopPuff *puff);
static BOOL WobbuffetPopPuff_Move(WobbuffetPop *game, WobbuffetPopPuff *puff);
static ManagedSprite *WobbuffetPop_CreatePuffSprite(WobbuffetPop *game, const u8 *config);
static void WobbuffetPopConfetti_Create(WobbuffetPop *game, WobbuffetPopConfetti *confetti, WobbuffetPopInflation *inflation);
static BOOL WobbuffetPop_PopBalloon(WobbuffetPop *game);
static ManagedSprite *WobbuffetPop_CreateBalloonIconSprite(WobbuffetPop *game, u32 anim, u32 position);
static BOOL WobbuffetPopBalloonIcon_Init(WobbuffetPop *game, WobbuffetPopBalloonIcon *icon, u32 position, u32 number);
static void WobbuffetPopBalloonIcon_Slide(WobbuffetPopBalloonIcon *icon);
static void WobbuffetPopBalloonIcon_Update(WobbuffetPop *game, WobbuffetPopBalloonIcon *icon);
static u32 WobbuffetPopBalloonIcon_GetSize(s32 n);
static void WobbuffetPop_PopBalloonIcon(WobbuffetPop *game);
static inline int WobbuffetPop_FindPlayer(WobbuffetPop *game, int netId);
static inline int WobbuffetPop_GetSubBgPaletteIndex(WobbuffetPop *game, int i);
static inline int WobbuffetPop_GetSubObjPaletteIndex(WobbuffetPop *game, int i, int offset);
static WobbuffetPopPump *WobbuffetPop_DequeuePump(WobbuffetPop *game);
static BOOL WobbuffetPop_SetPumpBonus(WobbuffetPop *game, int bonus);
static u8 WobbuffetPop_FindBonusTargetAtFront(WobbuffetPop *game, WobbuffetPopBonusTarget **outTarget);
static void WobbuffetPopBonusTarget_SetState(WobbuffetPopBonusTarget *target, u8 state);
static u32 WobbuffetPopBonusTarget_Run(WobbuffetPop *game, WobbuffetPopBonusRing *ring, WobbuffetPopBonusTarget *target);
static inline void WobbuffetPopBonusTarget_StartHop(WobbuffetPopBonusTarget *target, fx32 sine, fx32 cosine, s16 x, s16 y, int duration);
static void WobbuffetPop_ShowBonusPopup(WobbuffetPop *game, WobbuffetPopBonusPopup *popups, int bonus, int netId, int delay);
static BOOL WobbuffetPopBonusPopup_Update(WobbuffetPopBonusPopup *popups, WobbuffetPopBonusPopup *popup);
static void WobbuffetPopScoreDisplay_SetScore(WobbuffetPopScoreDisplay *display, s32 score);
static void WobbuffetPopScoreDisplay_StartRoll(WobbuffetPopScoreDisplay *display);
static BOOL WobbuffetPopScoreDisplay_IsSettled(WobbuffetPopScoreDisplay *display);
static BOOL WobbuffetPopScoreDisplay_RollDigits(WobbuffetPopScoreDisplay *display);
static void WobbuffetPopDust_Create(WobbuffetPop *game, WobbuffetPopDust *dust);
static void WobbuffetPopDust_Free(WobbuffetPop *game, WobbuffetPopDust *dust);
static void WobbuffetPopDust_Start(WobbuffetPop *game, WobbuffetPopBonusTarget *target, WobbuffetPopDust *dust);
static void WobbuffetPopDust_Update(WobbuffetPop *game, WobbuffetPopBonusTarget *target, WobbuffetPopDust *dust);
static void WobbuffetPop_QueuePumpEvent(WobbuffetPop *game, const WobbuffetPopPump *pump);

void TextOBJ_SetPalOffset(TextOBJ *object);
void SetBothScreensBrightnessByColor(u16 color);

static u32 WobbuffetPopBonusTarget_Idle(WobbuffetPop *game, WobbuffetPopBonusRing *ring, WobbuffetPopBonusTarget *target);
static u32 WobbuffetPopBonusTarget_Enter(WobbuffetPop *game, WobbuffetPopBonusRing *ring, WobbuffetPopBonusTarget *target);
static u32 WobbuffetPopBonusTarget_Walk(WobbuffetPop *game, WobbuffetPopBonusRing *ring, WobbuffetPopBonusTarget *target);
static u32 WobbuffetPopBonusTarget_Hit(WobbuffetPop *game, WobbuffetPopBonusRing *ring, WobbuffetPopBonusTarget *target);
u32 WobbuffetPop_GetEventCommandSize(void);
u32 WobbuffetPop_GetPlayerValueCommandSize(void);
void WobbuffetPop_RecvEvent(int netId, int size, void *data, void *arg);
void WobbuffetPop_RecvFinished(int netId, int size, void *data, void *arg);
void WobbuffetPop_RecvPlayerValue(int netId, int size, const void *data, void *arg);
void WobbuffetPop_HandlePumpEvent(WobbuffetPop *game, WobbuffetPopEvent *event);
void WobbuffetPop_HandlePopEvent(WobbuffetPop *game, WobbuffetPopEvent *event);
void WobbuffetPop_HandleNewBalloonEvent(WobbuffetPop *game, WobbuffetPopEvent *event);
void WobbuffetPop_HandleCountdownEvent(WobbuffetPop *game, WobbuffetPopEvent *event);
void WobbuffetPop_HandleFinishEvent(WobbuffetPop *game, WobbuffetPopEvent *event);
void WobbuffetPop_HandleSyncEvent(WobbuffetPop *game, WobbuffetPopEvent *event);
void WobbuffetPop_HandleDemoEvent(WobbuffetPop *game, WobbuffetPopEvent *event);

static const u16 sWobbuffetPopConfettiPalettes[3] = { 6, 8, 7 };

static const u16 sWobbuffetPopBonusTargetAnims[4] = { 12, 12, 15, 18 };

static const u16 sWobbuffetPopSubObjColorIndices[4] = { 82, 81, 83, 84 };

static const u16 sWobbuffetPopSubBgColorIndices[4] = { 29, 13, 61, 45 };

static const u16 sWobbuffetPopSubObjPaletteBases[5] = { 32, 16, 48, 64, 0 };

static const s32 sWobbuffetPopPuffBonusFactors[4] = { 0x1000, 0x1800, 0xA00, 0x2000 };

static const s16 sWobbuffetPopSeatPositions[4][2] = {
    { 128, 164 },
    { 128, 28  },
    { 52,  96  },
    { 204, 96  },
};

static u32 (*const sWobbuffetPopBonusTargetStateFuncs[4])(WobbuffetPop *, WobbuffetPopBonusRing *, WobbuffetPopBonusTarget *) = {
    WobbuffetPopBonusTarget_Idle,
    WobbuffetPopBonusTarget_Enter,
    WobbuffetPopBonusTarget_Walk,
    WobbuffetPopBonusTarget_Hit,
};

static const u8 sWobbuffetPopPopupSpots[5][4] = {
    { 0, 0, 0, 0 },
    { 0, 2, 1, 3 },
    { 0, 1, 0, 0 },
    { 0, 2, 3, 0 },
    { 0, 2, 1, 3 },
};

static const WobbuffetPopRingSpeed sWobbuffetPopRingSpeeds[5] = {
    { 8, 20, 7, 0 },
    { 8, 17, 7, 0 },
    { 8, 14, 6, 0 },
    { 8, 10, 6, 0 },
    { 8, 6,  5, 0 },
};

static const WobbuffetPopInflationGfx sWobbuffetPopInflationGfx[3] = {
    { 11, 12, 0x1100 },
    { 9,  10, 0x1700 },
    { 7,  8,  0x1D00 },
};

static const u16 sWobbuffetPopConfettiAnims[5][4] = {
    { 22, 21, 23, 24 },
    { 22, 21, 23, 24 },
    { 22, 23, 0,  0  },
    { 22, 24, 23, 0  },
    { 22, 21, 23, 24 },
};

static const u16 sWobbuffetPopSubBgPaletteIndices[5][4] = {
    { 29, 45, 13, 61 },
    { 29, 45, 13, 61 },
    { 29, 13, 0,  0  },
    { 29, 61, 13, 0  },
    { 29, 45, 13, 61 },
};

static const u16 sWobbuffetPopSeatColorIndices[5][4] = {
    { 82, 81, 83, 84 },
    { 82, 81, 83, 84 },
    { 82, 81, 0,  0  },
    { 82, 83, 81, 0  },
    { 82, 84, 81, 83 },
};

static const u16 sWobbuffetPopSubObjPaletteIndices[5][4] = {
    { 32, 16, 48, 64 },
    { 32, 16, 48, 64 },
    { 32, 16, 0,  0  },
    { 32, 48, 16, 0  },
    { 32, 64, 16, 48 },
};

static const u16 sWobbuffetPopPopupPalettes[5][4] = {
    { 2, 4, 1, 3 },
    { 2, 4, 1, 3 },
    { 2, 1, 0, 0 },
    { 2, 3, 1, 0 },
    { 2, 4, 1, 3 },
};

static const ManagedSpriteTemplate sWobbuffetPopConfettiSpriteTemplate = {
    .x = 0x80,
    .y = 0x60,
    .z = 0,
    .animation = 0,
    .drawPriority = 12,
    .pal = 5,
    .vram = NNS_G2D_VRAM_TYPE_2DSUB,
    .resIdList = { 0x2714, 0x2716, 0x2714, 0x2714, -1, -1 },
    .bgPriority = 1,
    .vramTransfer = 0,
};

static const ManagedSpriteTemplate sWobbuffetPopBonusSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 16,
    .pal = 2,
    .vram = NNS_G2D_VRAM_TYPE_2DSUB,
    .resIdList = { 0x2714, 0x2716, 0x2714, 0x2714, -1, -1 },
    .bgPriority = 3,
    .vramTransfer = 0,
};

static const ManagedSpriteTemplate sWobbuffetPopBonusEffectSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 15,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DSUB,
    .resIdList = { 0x2714, 0x2716, 0x2714, 0x2714, -1, -1 },
    .bgPriority = 3,
    .vramTransfer = 0,
};

static const ManagedSpriteTemplate sWobbuffetPopDustSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 17,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DSUB,
    .resIdList = { 0x2714, 0x2716, 0x2714, 0x2714, -1, -1 },
    .bgPriority = 3,
    .vramTransfer = 0,
};

static const ManagedSpriteTemplate sWobbuffetPopScoreFrameSpriteTemplate = {
    .x = 0x28,
    .y = 0xB0,
    .z = 0,
    .animation = 0,
    .drawPriority = 13,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = { 0x2712, 0x2714, 0x2712, 0x2712, -1, -1 },
    .bgPriority = 3,
    .vramTransfer = 0,
};

static const ManagedSpriteTemplate sWobbuffetPopPuffSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 20,
    .pal = 1,
    .vram = NNS_G2D_VRAM_TYPE_2DSUB,
    .resIdList = { 0x2714, 0x2716, 0x2714, 0x2714, -1, -1 },
    .bgPriority = 3,
    .vramTransfer = 0,
};

static const ManagedSpriteTemplate sWobbuffetPopBalloonIconSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 11,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DSUB,
    .resIdList = { 0x2714, 0x2716, 0x2714, 0x2714, -1, -1 },
    .bgPriority = 1,
    .vramTransfer = 0,
};

static const ManagedSpriteTemplate sWobbuffetPopBonusShadowSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 18,
    .pal = 1,
    .vram = NNS_G2D_VRAM_TYPE_2DSUB,
    .resIdList = { 0x2714, 0x2716, 0x2714, 0x2714, -1, -1 },
    .bgPriority = 3,
    .vramTransfer = 0,
};

static const ManagedSpriteTemplate sWobbuffetPopDemoStylusSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 11,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = { 0x2711, 0x2712, 0x2711, 0x2711, -1, -1 },
    .bgPriority = 0,
    .vramTransfer = 0,
};

static const ManagedSpriteTemplate sWobbuffetPopSeatSpriteTemplate = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 19,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DSUB,
    .resIdList = { 0x2714, 0x2716, 0x2714, 0x2714, -1, -1 },
    .bgPriority = 3,
    .vramTransfer = 0,
};

static const s32 sWobbuffetPopInflationCapacities[5][3] = {
    { 3000, 6000,  8000  },
    { 1000, 1000,  1000  },
    { 6566, 8442,  10318 },
    { 8428, 10836, 13244 },
    { 9800, 12600, 15400 },
};

static const u8 sWobbuffetPopPuffConfigs[200] = {
    128, 0, 208, 0, 7, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 208, 0, 7, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 208, 0, 7, 0, 2, 2, 0, 0, 128, 0, 240, 255, 10, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 208, 0, 7, 0, 2, 2, 0, 0, 240, 255, 96, 0, 9, 0, 3, 3, 0, 0, 16, 1, 96, 0, 8, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 208, 0, 7, 0, 2, 2, 0, 0, 240, 255, 96, 0, 9, 0, 4, 3, 0, 0, 128, 0, 240, 255, 10, 0, 1, 0, 0, 0, 0, 1, 96, 0, 8, 0, 3, 1, 0, 0
};

static void WobbuffetPopInflation_Start(BgConfig *bgConfig, int playerCount, int size, WobbuffetPopInflation *inflation) {
    NARC *narc = NARC_New(NARC_a_1_9_9, HEAP_ID_117);

    GfGfxLoader_LoadCharDataFromOpenNarc(narc, sWobbuffetPopInflationGfx[size].charFile, bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_117);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, sWobbuffetPopInflationGfx[size].scrnFile, bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_117);
    NARC_Delete(narc);
    MI_CpuFill8(inflation, 0, sizeof(WobbuffetPopInflation));
    inflation->capacity = sWobbuffetPopInflationCapacities[playerCount][size];
    inflation->size = size;
    inflation->active = TRUE;
    inflation->showBg = TRUE;
    WobbuffetPopInflation_UpdateBg(bgConfig, inflation, playerCount);
    PlaySE(SEQ_SE_DP_BOX02);
}

static void WobbuffetPopInflation_Stop(BgConfig *bgConfig, WobbuffetPopInflation *inflation) {
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
    inflation->active = FALSE;
    inflation->hideBg = TRUE;
}

static void WobbuffetPopInflation_UpdateBg(BgConfig *bgConfig, WobbuffetPopInflation *inflation, int playerCount) {
    int x;
    int y;
    int dx;
    int dy;
    int jitter;
    int invy;
    int invx;

    if (inflation->active) {
        WobbuffetPopInflation_CalcScale(playerCount, inflation->size, inflation->air, &x, &y);
        if (x == FX32_ONE) {
            dx = 0;
        } else if (x < FX32_ONE) {
            dx = 128 - ((x << 7) / FX32_ONE);
        } else {
            dx = -(((x - FX32_ONE) << 7) / FX32_ONE);
        }
        if (y == FX32_ONE) {
            dy = 0;
        } else if (y < FX32_ONE) {
            dy = 128 - ((y << 7) / FX32_ONE);
        } else {
            dy = -(((y - FX32_ONE) << 7) / FX32_ONE);
        }
        if (inflation->pendingAir == 0) {
            jitter = 0;
        } else {
            jitter = (LCRandom() & 7) + 1;
            if (inflation->jitterToggle & 1) {
                jitter = -jitter;
            }
            inflation->jitterToggle ^= 1;
        }
        invx = FX_Inv(x);
        invy = FX_Inv(y);
        ScheduleSetBgAffineScale(bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_XSCALE, invx);
        ScheduleSetBgAffineScale(bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_YSCALE, invy);
        ScheduleSetBgPosText(bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_X, jitter - dx);
        ScheduleSetBgPosText(bgConfig, GF_BG_LYR_SUB_3, BG_POS_OP_SET_Y, 39 - dy);
    }
}

static void WobbuffetPopInflation_CalcScale(int playerCount, int size, int air, int *scaleX, int *scaleY) {
    int q = sWobbuffetPopInflationGfx[size].scale * air / sWobbuffetPopInflationCapacities[playerCount][size];
    int scale = q + 768;

    *scaleX = scale;
    *scaleY = scale;
    if (scale > FX32_ONE) {
        *scaleX += (int)(((s64)(q - 3328) * 4506 + 2048) >> 12);
    }
}

int WobbuffetPop_GetBalloonSize(int n) {
    if (n >= 10) {
        return 2;
    }
    if (n >= 5) {
        return 1;
    }
    return 0;
}

static s32 WobbuffetPopInflation_AddAir(WobbuffetPop *game, WobbuffetPopInflation *inflation, s32 amount, s16 netId, u8 bonus) {
    s32 seat;

    inflation->pendingAir += amount;
    inflation->airStep = inflation->pendingAir / 6;
    seat = WobbuffetPop_GetPlayerSeat(game, netId);
    inflation->playerAir[seat] += amount;
    if (netId == Comm_GetLocalNetId()) {
        game->score += amount;
        WobbuffetPopScoreDisplay_SetScore(&game->scoreDisplay, game->score);
    }
    return inflation->pendingAir;
}

static BOOL WobbuffetPopInflation_StepAir(WobbuffetPopInflation *inflation) {
    s32 step;

    if (inflation->pendingAir == 0) {
        return FALSE;
    }
    step = inflation->airStep;
    if (inflation->pendingAir < step) {
        step = inflation->pendingAir;
    }
    inflation->pendingAir -= step;
    inflation->air += step;
    return TRUE;
}

void WobbuffetPop_UpdateInflation(WobbuffetPop *game, BgConfig *bgConfig, WobbuffetPopInflation *inflation) {
    if (game->balloonInPlay != FALSE) {
        WobbuffetPopInflation_StepAir(inflation);
        WobbuffetPopInflation_UpdateBg(bgConfig, inflation, game->app->playerCount);
    }
}

static s32 WobbuffetPop_GetInflationPercent(WobbuffetPop *game) {
    WobbuffetPopInflation *inflation = &game->inflation;

    return inflation->air * 100 / inflation->capacity;
}

BOOL WobbuffetPop_LaunchPuff(WobbuffetPop *game, const WobbuffetPopPump *pump) {
    int i;
    u8 *scan;

    // Walks the puffs by moving the game pointer.
    for (i = 0, scan = (u8 *)game; i < 40; i++, scan += sizeof(WobbuffetPopPuff)) {
        if (((WobbuffetPop *)scan)->puffs[0].active == FALSE) {
            WobbuffetPop_ShowBonusPopup(game, game->bonusPopups, pump->bonus, pump->netId, WobbuffetPopPuff_Init(game, pump, &game->puffs[i]));
            return TRUE;
        }
    }
    GF_AssertFail();
    return FALSE;
}

static u32 WobbuffetPopPuff_Init(WobbuffetPop *game, const WobbuffetPopPump *pump, WobbuffetPopPuff *puff) {
    s16 x;
    s16 y;
    const u8 *config;
    s32 distance;

    if (puff->active != FALSE) {
        GF_AssertFail();
    }
    MI_CpuFill8(puff, 0, sizeof(*puff));
    config = sWobbuffetPopPuffConfigs + (game->app->playerCount * 40) + WobbuffetPop_GetPlayerSeat(game, pump->netId) * 10;
    puff->netId = pump->netId;
    puff->balloonIndex = pump->balloonIndex;
    puff->amount = pump->bonusAmount;
    puff->config = config;
    puff->bonus = pump->bonus;
    puff->sprite = WobbuffetPop_CreatePuffSprite(game, config);
    if (pump->netId == Comm_GetLocalNetId()) {
        WobbuffetPop_AddModelSlot(game, pump->amount);
        puff->delay = 25;
        ManagedSprite_SetDrawFlag(puff->sprite, FALSE);
    }
    puff->active = TRUE;
    distance = 0;
    ManagedSprite_GetPositionXYWithSubscreenOffset(puff->sprite, &x, &y, 0x160000);
    switch (config[7]) {
    case 0:
        distance = 28 - y;
        break;
    case 1:
        distance = x - 204;
        break;
    case 2:
        distance = y - 164;
        break;
    case 3:
        distance = 52 - x;
        break;
    }
    return (distance << 12) / (5 << 12);
}

void WobbuffetPop_UpdatePuffs(WobbuffetPop *game) {
    WobbuffetPopPuff *puff = game->puffs;
    int i;

    for (i = 0; i < 40; i++, puff++) {
        if (puff->active == TRUE) {
            if (puff->delay > 0) {
                puff->delay--;
                if (puff->delay == 0) {
                    ManagedSprite_SetDrawFlag(puff->sprite, TRUE);
                }
            } else if (WobbuffetPopPuff_Move(game, puff) == TRUE) {
                if (game->balloonInPlay == TRUE && game->gameOver == FALSE) {
                    WobbuffetPopInflation_AddAir(game, &game->inflation, puff->amount, puff->netId, puff->bonus);
                }
                Sprite_DeleteAndFreeResources(puff->sprite);
                puff->active = FALSE;
            }
        }
    }
}

static BOOL WobbuffetPopPuff_Move(WobbuffetPop *game, WobbuffetPopPuff *puff) {
    s16 nextX;
    s16 nextY;
    s16 x;
    s16 y;
    WobbuffetPopBonusTarget *target;
    WobbuffetPopPump *pump;
    int remaining;
    int previous;
    BOOL done = FALSE;
    s32 value;
    float scale;

    ManagedSprite_GetPositionXYWithSubscreenOffset(puff->sprite, &x, &y, 0x160000);
    switch (puff->config[7]) {
    case 0:
        previous = 28 - y;
        ManagedSprite_AddSpritePrecisePositionXY(puff->sprite, 0, 5 << 12);
        ManagedSprite_GetPositionXYWithSubscreenOffset(puff->sprite, &nextX, &nextY, 0x160000);
        remaining = 28 - nextY;
        if (nextY > 74) {
            done = TRUE;
        }
        break;
    case 1:
        previous = x - 204;
        ManagedSprite_AddSpritePrecisePositionXY(puff->sprite, -5 << 12, 0);
        ManagedSprite_GetPositionXYWithSubscreenOffset(puff->sprite, &nextX, &nextY, 0x160000);
        remaining = nextX - 204;
        if (nextX < 152) {
            done = TRUE;
        }
        break;
    case 2:
        previous = y - 164;
        ManagedSprite_AddSpritePrecisePositionXY(puff->sprite, 0, -5 << 12);
        ManagedSprite_GetPositionXYWithSubscreenOffset(puff->sprite, &nextX, &nextY, 0x160000);
        remaining = nextY - 164;
        if (nextY < 114) {
            done = TRUE;
        }
        break;
    case 3:
        previous = 52 - x;
        ManagedSprite_AddSpritePrecisePositionXY(puff->sprite, 5 << 12, 0);
        ManagedSprite_GetPositionXYWithSubscreenOffset(puff->sprite, &nextX, &nextY, 0x160000);
        remaining = 52 - nextX;
        if (nextX > 104) {
            done = TRUE;
        }
        break;
    }
    if (remaining >= 0) {
        if (remaining < 14) {
            value = (remaining << 12) / 14;
            if (value < (1 << 10)) {
                value = 1 << 10;
            }
            scale = (float)value / 4096.0f;
            ManagedSprite_SetAffineScale(puff->sprite, scale, scale);
        }
    } else if (remaining > -14) {
        value = (-remaining * sWobbuffetPopPuffBonusFactors[puff->bonus]) / 14;
        if (value < (1 << 10)) {
            value = 1 << 10;
        }
        scale = (float)value / 4096.0f;
        ManagedSprite_SetAffineScale(puff->sprite, scale, scale);
    }
    if ((s16)puff->netId == Comm_GetLocalNetId() && previous >= 0 && remaining <= 0) {
        puff->bonus = WobbuffetPop_FindBonusTargetAtFront(game, &target);
        switch (puff->bonus) {
        case WOBBUFFET_POP_BONUS_DOUBLE:
            puff->amount *= 2;
            break;
        case WOBBUFFET_POP_BONUS_HALVE:
            puff->amount /= 2;
            break;
        case WOBBUFFET_POP_BONUS_TRIPLE:
            puff->amount *= 3;
            break;
        }
        WobbuffetPop_SetPumpBonus(game, puff->bonus);
        if (puff->bonus != WOBBUFFET_POP_BONUS_NONE) {
            WobbuffetPopBonusTarget_SetState(target, WOBBUFFET_POP_BONUS_TARGET_HIT);
        }
        pump = WobbuffetPop_DequeuePump(game);
        if (pump != NULL) {
            WobbuffetPop_QueuePumpEvent(game, pump);
        }
    }
    if (done == TRUE) {
        return TRUE;
    }
    return FALSE;
}

static ManagedSprite *WobbuffetPop_CreatePuffSprite(WobbuffetPop *game, const u8 *config) {
    ManagedSpriteTemplate template = sWobbuffetPopPuffSpriteTemplate;
    ManagedSprite *sprite;

    template.x = ((const s16 *)config)[0];
    template.y = ((const s16 *)config)[1];
    template.pal = config[6];
    sprite = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &template);
    ManagedSprite_SetPositionXYWithSubscreenOffset(sprite, ((const s16 *)config)[0], ((const s16 *)config)[1], 0x160000);
    ManagedSprite_SetAnim(sprite, ((const u16 *)config)[2]);
    ManagedSprite_SetAffineOverwriteMode(sprite, 2);
    Sprite_TickFrame(sprite->sprite);
    return sprite;
}

void WobbuffetPop_FreePuffs(WobbuffetPop *game) {
    s32 i;
    u8 *p = (u8 *)game;

    for (i = 0; i < 40; i++) {
        if (((WobbuffetPop *)p)->puffs[0].active == TRUE) {
            Sprite_DeleteAndFreeResources(((WobbuffetPop *)p)->puffs[0].sprite);
            ((WobbuffetPop *)p)->puffs[0].active = FALSE;
        }
        p += sizeof(WobbuffetPopPuff);
    }
}

#pragma optimization_level 2
#pragma opt_common_subs off
static void WobbuffetPopConfetti_Create(WobbuffetPop *game, WobbuffetPopConfetti *confetti, WobbuffetPopInflation *inflation) {
    ManagedSpriteTemplate tmpl;
    s32 counts[4];
    s32 pendingAir;
    s32 air;
    int anim;
    int i;
    int j;
    int count;
    int seat;
    WobbuffetPopConfettiPiece *p;
    ManagedSprite *s;
    WobbuffetPopConfettiPiece *q;
    ManagedSprite *t;

    count = 0;
    tmpl = sWobbuffetPopConfettiSpriteTemplate;
    air = inflation->air;
    pendingAir = inflation->pendingAir;
    for (i = 0; i < game->app->playerCount; i++) {
        seat = WobbuffetPop_GetPlayerSeat(game, game->app->netIds[i]);
        counts[i] = 36 * inflation->playerAir[seat] / (air + pendingAir);
        anim = sWobbuffetPopConfettiAnims[game->app->playerCount][seat];
        for (j = 0; j < counts[i]; j++) {
            p = &confetti->playerPieces[count];
            s = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &tmpl);
            if (s == NULL) {
                break;
            }
            ManagedSprite_SetPositionXYWithSubscreenOffset(s, 128, 96, 22 << 16);
            ManagedSprite_SetAnim(s, anim);
            Sprite_TickFrame(s->sprite);
            p->radius = LCRandom() % 0x2000 + 0x2000;
            p->radiusSpeed = LCRandom() % 0x4000 + 0x2000;
            p->angle = (LCRandom() % 360) << 12;
            p->angleSpeed = LCRandom() % (5 << 14) + (10 << 12);
            p->life = LCRandom() % 15 + 20;
            confetti->playerPieces[count].sprite = s;
            count++;
        }
    }
    tmpl.drawPriority = 14;
    tmpl.pal = 0;
    for (i = 0; i < 8; i++) {
        q = &confetti->pieces[i];
        t = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &tmpl);
        if (t == NULL) {
            break;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(t, 128, 96, 22 << 16);
        ManagedSprite_SetAnim(t, LCRandom() % 3 + 28);
        Sprite_TickFrame(t->sprite);
        q->radius = LCRandom() % 0x2000 + 0x2000;
        q->radiusSpeed = LCRandom() % 0x4000 + 0x2000;
        q->angle = (LCRandom() % 360) << 12;
        q->angleSpeed = LCRandom() % (5 << 14) + (1 << 16);
        q->life = LCRandom() % 15 + 20;
        confetti->pieces[i].sprite = t;
    }
    tmpl.drawPriority = 13;
    tmpl.pal = sWobbuffetPopConfettiPalettes[inflation->size];
    for (i = 0; i < 3; i++) {
        q = &confetti->balloonPieces[i];
        t = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &tmpl);
        if (t == NULL) {
            break;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(t, 128, 96, 22 << 16);
        ManagedSprite_SetAnim(t, 11);
        Sprite_TickFrame(t->sprite);
        q->radius = LCRandom() % 0x2000 + 0x2000;
        q->radiusSpeed = LCRandom() % 0x4000 + 0x2000;
        q->angle = (LCRandom() % 360) << 12;
        q->angleSpeed = LCRandom() % (5 << 14) + (1 << 16);
        q->life = LCRandom() % 15 + 20;
        confetti->balloonPieces[i].sprite = t;
    }
}
#pragma opt_common_subs reset
#pragma optimization_level reset

void WobbuffetPop_FreeConfetti(WobbuffetPop *game) {
    s32 i;
    WobbuffetPopConfettiPiece *piece;

    for (i = 0, piece = game->confetti.playerPieces; i < 36; i++, piece++) {
        if (piece->sprite != NULL) {
            Sprite_DeleteAndFreeResources(piece->sprite);
        }
    }
    {
        WobbuffetPopConfettiPiece *piece2;
        s32 j;

        j = 0;
        piece2 = game->confetti.pieces;
        for (; j < 8; j++, piece2++) {
            if (piece2->sprite != NULL) {
                Sprite_DeleteAndFreeResources(piece2->sprite);
            }
        }
    }
    {
        s32 k = 0;
        WobbuffetPopConfettiPiece *piece3 = game->confetti.balloonPieces;

        for (; k < 3; k++, piece3++) {
            if (piece3->sprite != NULL) {
                Sprite_DeleteAndFreeResources(piece3->sprite);
            }
        }
    }
}

// Moves one confetti piece outwards on its spiral, deletes it when its life is over.
#define WOBBUFFET_POP_STEP_CONFETTI(piece, radiusCopy)                                                                                                                                   \
    do {                                                                                                                                                                                 \
        if ((piece)->sprite) {                                                                                                                                                           \
            if ((piece)->life == 0) {                                                                                                                                                    \
                Sprite_DeleteAndFreeResources((piece)->sprite);                                                                                                                          \
                (piece)->sprite = 0;                                                                                                                                                     \
            } else {                                                                                                                                                                     \
                (piece)->angle += (piece)->angleSpeed;                                                                                                                                   \
                (piece)->radius += (piece)->radiusSpeed;                                                                                                                                 \
                if ((piece)->radius >= 0xa0000) {                                                                                                                                        \
                    (piece)->radius = 0xa0000;                                                                                                                                           \
                }                                                                                                                                                                        \
                int radius = (piece)->radius;                                                                                                                                            \
                int sine = GF_SinDegFX32((piece)->angle);                                                                                                                                \
                radiusCopy = (piece)->radius;                                                                                                                                            \
                int cosine = GF_CosDegFX32((piece)->angle);                                                                                                                              \
                s64 xx = (s64)sine * radius;                                                                                                                                             \
                s64 yy = (s64)cosine * radiusCopy;                                                                                                                                       \
                ManagedSprite_SetPositionXYWithSubscreenOffset((piece)->sprite, (s16)((s32)((xx + 0x800) >> 12) / 4096 + 128), (s16)(-(s32)((yy + 0x800) >> 12) / 4096 + 96), 0x160000); \
                count++;                                                                                                                                                                 \
                (piece)->life--;                                                                                                                                                         \
            }                                                                                                                                                                            \
        }                                                                                                                                                                                \
    } while (0)

BOOL WobbuffetPop_UpdateConfetti(WobbuffetPop *game) {
    WobbuffetPopConfetti *confetti = &game->confetti;
    int count = 0;
    WobbuffetPopConfettiPiece *piece;
    int i;
    int radiusA;
    int radiusB;
    int radiusC;
    int k;
    int j;

    if (!confetti->active) {
        return FALSE;
    }
    for (i = 0, piece = confetti->playerPieces; i < 36; i++, piece++) {
        WOBBUFFET_POP_STEP_CONFETTI(piece, radiusA);
    }
    {
        for (j = 0, piece = confetti->pieces; j < 8; j++, piece++) {
            WOBBUFFET_POP_STEP_CONFETTI(piece, radiusB);
        }
    }
    {
        for (k = 0, piece = confetti->balloonPieces; k < 3; k++, piece++) {
            WOBBUFFET_POP_STEP_CONFETTI(piece, radiusC);
        }
    }
    if (count == 0) {
        confetti->active = FALSE;
    }
    return TRUE;
}

static BOOL WobbuffetPop_PopBalloon(WobbuffetPop *game) {
    WobbuffetPopConfetti *confetti = &game->confetti;
    s32 i;

    if (confetti->active == TRUE) {
        return FALSE;
    }
    MI_CpuFill8(confetti, 0, sizeof(WobbuffetPopConfetti));
    WobbuffetPopConfetti_Create(game, &game->confetti, &game->inflation);
    WobbuffetPopInflation_Stop(game->bgConfig, &game->inflation);
    WobbuffetPop_PopBalloonIcon(game);
    game->balloonInPlay = FALSE;
    game->inflation.air = 0;
    game->inflation.pendingAir = 0;
    for (i = 0; i < 4; i++) {
        game->inflation.playerAir[0] = 0;
        game = (WobbuffetPop *)((u8 *)game + 4);
    }
    PlaySE(SEQ_SE_PL_BALLOON07);
    confetti->active = TRUE;
    return TRUE;
}

static ManagedSprite *WobbuffetPop_CreateBalloonIconSprite(WobbuffetPop *game, u32 anim, u32 position) {
    ManagedSpriteTemplate template = sWobbuffetPopBalloonIconSpriteTemplate;
    ManagedSprite *sprite = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &template);

    ManagedSprite_SetPositionXYWithSubscreenOffset(sprite, 72 - position * 16, 32, 0x160000);
    ManagedSprite_SetAnim(sprite, anim);
    Sprite_TickFrame(sprite->sprite);
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}

void WobbuffetPop_InitBalloonIcons(WobbuffetPop *game) {
    s32 i;
    WobbuffetPopBalloonIcon *icon;
    u32 frames;

    for (i = 0, icon = game->balloonIcons, frames = 0; i < 5; i++, icon++, frames += 0x4000) {
        WobbuffetPopBalloonIcon_Init(game, icon, i, i);
        ManagedSprite_TickNFrames(icon->sprite, frames);
    }
}

void WobbuffetPop_FreeBalloonIcons(WobbuffetPop *game) {
    s32 i;
    u8 *p = (u8 *)game;

    for (i = 0; i < 5; i++) {
        Sprite_DeleteAndFreeResources(((WobbuffetPop *)p)->balloonIcons[0].sprite);
        p += sizeof(WobbuffetPopBalloonIcon);
    }
}

BOOL WobbuffetPop_UpdateBalloonIcons(WobbuffetPop *game) {
    s32 i;
    WobbuffetPopBalloonIcon *icon = game->balloonIcons;

    for (i = 0; i < 5; i++) {
        WobbuffetPopBalloonIcon_Slide(icon);
        WobbuffetPopBalloonIcon_Update(game, icon);
        icon++;
    }
    return TRUE;
}

static BOOL WobbuffetPopBalloonIcon_Init(WobbuffetPop *game, WobbuffetPopBalloonIcon *icon, u32 position, u32 number) {
    MI_CpuFill8(icon, 0, sizeof(WobbuffetPopBalloonIcon));
    icon->size = WobbuffetPopBalloonIcon_GetSize((s32)number);
    icon->state = 1;
    icon->sprite = WobbuffetPop_CreateBalloonIconSprite(game, icon->size, position);
    icon->position = position;
    icon->number = number;
    return TRUE;
}

static void WobbuffetPopBalloonIcon_Slide(WobbuffetPopBalloonIcon *icon) {
    s16 x;
    s16 y;
    s16 target = 72 - icon->position * 16;

    ManagedSprite_GetPositionXYWithSubscreenOffset(icon->sprite, &x, &y, 0x160000);
    if (target > x) {
        x += 2;
        if (x > target) {
            x = target;
        }
        ManagedSprite_SetPositionXYWithSubscreenOffset(icon->sprite, x, y, 0x160000);
    }
}

static void WobbuffetPopBalloonIcon_Update(WobbuffetPop *game, WobbuffetPopBalloonIcon *icon) {
    int i;
    WobbuffetPopBalloonIcon *other;
    u8 state;

    if (icon->position != 0) {
        return;
    }
    state = icon->state;
    if (state != 1) {
        if (state != 3) {
            return;
        }
        if (ManagedSprite_IsAnimated(icon->sprite) != FALSE) {
            return;
        }
        icon->number += 5;
        icon->size = WobbuffetPopBalloonIcon_GetSize(icon->number);
        icon->state = 1;
        ManagedSprite_SetAnim(icon->sprite, icon->size);
        ManagedSprite_SetPositionXYWithSubscreenOffset(icon->sprite, -8, 32, 0x160000);
        other = game->balloonIcons;
        for (i = 0; i < 5; i++, other++) {
            if (other->position == 0) {
                other->position = 4;
            } else {
                other->position--;
            }
        }
        return;
    }
    i = WobbuffetPop_GetInflationPercent(game);
    if (icon->state == 1 && i > 75) {
        icon->state = 2;
        ManagedSprite_SetAnim(icon->sprite, icon->size + 3);
        PlaySE(SEQ_SE_PL_ALERT4);
    }
}

static u32 WobbuffetPopBalloonIcon_GetSize(s32 n) {
    if (n >= 10) {
        return 2;
    }
    if (n >= 5) {
        return 1;
    }
    return 0;
}

static void WobbuffetPop_PopBalloonIcon(WobbuffetPop *game) {
    WobbuffetPopBalloonIcon *icon = game->balloonIcons;
    s32 i;

    for (i = 0; i < 5; i++) {
        if (icon->position == 0) {
            icon->state = 3;
            ManagedSprite_SetAnim(icon->sprite, 6);
            return;
        }
        icon++;
    }
}

void WobbuffetPop_SetupThreePlayerSubBg(WobbuffetPop *game) {
    if (game->app->playerCount == 3) {
        BgTilemapRectChangePalette(game->bgConfig, GF_BG_LYR_SUB_0, 0, 13, 12, 4, 3);
        BgTilemapRectChangePalette(game->bgConfig, GF_BG_LYR_SUB_0, 20, 13, 12, 4, 0);
    }
}

static inline int WobbuffetPop_FindPlayer(WobbuffetPop *game, int netId) {
    int i;

    for (i = 0; i < game->app->playerCount; i++) {
        if (netId == game->app->netIds[i]) {
            break;
        }
    }
    return i;
}

static inline int WobbuffetPop_GetSubBgPaletteIndex(WobbuffetPop *game, int i) {
    u8 count = game->app->playerCount;
    int seat = WobbuffetPop_GetPlayerSeat(game, game->app->netIds[i]);

    return sWobbuffetPopSubBgPaletteIndices[count][seat];
}

static inline int WobbuffetPop_GetSubObjPaletteIndex(WobbuffetPop *game, int i, int offset) {
    u8 count = game->app->playerCount;
    int seat = WobbuffetPop_GetPlayerSeat(game, game->app->netIds[i]);

    return offset * 16 + sWobbuffetPopSubObjPaletteIndices[count][seat];
}

void WobbuffetPop_SetPlayerSubBgColors(WobbuffetPop *game) {
    u16 colors[4][3];
    int i;
    int j;
    u16 *unfaded;
    u16 *faded;

    for (i = 0; i < game->app->playerCount; i++) {
        int base = sWobbuffetPopSubBgColorIndices[i];
        for (j = 0; j < 3; j++) {
            colors[i][j] = PaletteData_GetBufferColorAtIndex(game->paletteData, PLTTBUF_SUB_BG, PLTTSEL_OPAQUE, base + j);
        }
    }
    WobbuffetPop_FindPlayer(game, Comm_GetLocalNetId());
    unfaded = PaletteData_GetUnfadedBuf(game->paletteData, PLTTBUF_SUB_BG);
    faded = PaletteData_GetFadedBuf(game->paletteData, PLTTBUF_SUB_BG);
    for (i = 0; i < game->app->playerCount; i++) {
        int index = WobbuffetPop_GetSubBgPaletteIndex(game, i);
        int player = i;
        int k;
        for (k = 0; k < 3; k++, index++) {
            unfaded[index] = colors[player][k];
            faded[index] = colors[player][k];
        }
    }
}

void WobbuffetPop_SetPlayerSubObjPalettes(WobbuffetPop *game) {
    u16 colors[4][16];
    int i;
    int j;
    u16 *unfaded;
    u16 *faded;
    int offset;

    offset = SpriteManager_FindPlttResourceOffset(game->spriteManager, 10006, NNS_G2D_VRAM_TYPE_2DSUB);
    for (i = 0; i < game->app->playerCount; i++) {
        int base = sWobbuffetPopSubObjPaletteBases[i] + offset * 16;
        for (j = 0; j < 16; j++) {
            colors[i][j] = PaletteData_GetBufferColorAtIndex(game->paletteData, PLTTBUF_SUB_OBJ, PLTTSEL_OPAQUE, base);
            base++;
        }
    }
    WobbuffetPop_FindPlayer(game, Comm_GetLocalNetId());
    unfaded = PaletteData_GetUnfadedBuf(game->paletteData, PLTTBUF_SUB_OBJ);
    faded = PaletteData_GetFadedBuf(game->paletteData, PLTTBUF_SUB_OBJ);
    for (i = 0; i < game->app->playerCount; i++) {
        int index = WobbuffetPop_GetSubObjPaletteIndex(game, i, offset);
        int player = i;
        int k;
        for (k = 0; k < 16; k++, index++) {
            unfaded[index] = colors[player][k];
            faded[index] = colors[player][k];
        }
    }
}

void WobbuffetPop_SetPlayerSubObjColors(WobbuffetPop *game) {
    u16 colors[4];
    int i;
    u16 *unfaded;
    u16 *faded;
    int offset;

    offset = SpriteManager_FindPlttResourceOffset(game->spriteManager, 10006, NNS_G2D_VRAM_TYPE_2DSUB);
    for (i = 0; i < game->app->playerCount; i++) {
        colors[i] = PaletteData_GetBufferColorAtIndex(game->paletteData, PLTTBUF_SUB_OBJ, PLTTSEL_OPAQUE, sWobbuffetPopSubObjColorIndices[i] + offset * 16);
    }
    WobbuffetPop_FindPlayer(game, Comm_GetLocalNetId());
    unfaded = PaletteData_GetUnfadedBuf(game->paletteData, PLTTBUF_SUB_OBJ);
    faded = PaletteData_GetFadedBuf(game->paletteData, PLTTBUF_SUB_OBJ);
    for (i = 0; i < game->app->playerCount; i++) {
        int index = offset * 16 + sWobbuffetPopSeatColorIndices[game->app->playerCount][WobbuffetPop_GetPlayerSeat(game, game->app->netIds[i])];
        s32 player = i;
        unfaded[index] = colors[player];
        faded[index] = colors[player];
    }
}

void WobbuffetPop_InitPump(WobbuffetPop *game, u32 balloonIndex, u32 amount, WobbuffetPopPump *pump) {
    MI_CpuFill8(pump, 0, sizeof(WobbuffetPopPump));
    pump->netId = Comm_GetLocalNetId();
    pump->balloonIndex = balloonIndex;
    pump->amount = amount;
    pump->bonusAmount = amount;
    pump->bonus = WOBBUFFET_POP_BONUS_NONE;
    pump->bonusSet = FALSE;
}

BOOL WobbuffetPop_QueuePump(WobbuffetPop *game, const WobbuffetPopPump *pump) {
    if (game->pumpQueueWrite - game->pumpQueueRead >= 8) {
        return FALSE;
    }
    game->pumpQueue[game->pumpQueueWrite % 8] = *pump;
    game->pumpQueueWrite++;
    return TRUE;
}

static WobbuffetPopPump *WobbuffetPop_DequeuePump(WobbuffetPop *game) {
    s32 n = game->pumpQueueRead;

    if (n >= game->pumpQueueWrite) {
        return NULL;
    }
    game->pumpQueueRead = n + 1;
    return &game->pumpQueue[n % 8];
}

static BOOL WobbuffetPop_SetPumpBonus(WobbuffetPop *game, int bonus) {
    WobbuffetPopPump *pump = NULL;
    int start;
    int end;
    int i;

    if (game->pumpQueueRead == game->pumpQueueWrite || bonus == WOBBUFFET_POP_BONUS_NONE) {
        return FALSE;
    }
    start = game->pumpQueueRead % 8;
    end = game->pumpQueueWrite % 8;
    if (start < end) {
        for (i = start; i < end; i++) {
            if (!game->pumpQueue[i].bonusSet) {
                pump = &game->pumpQueue[i];
                break;
            }
        }
    } else {
        for (i = start; i < 8; i++) {
            if (!game->pumpQueue[i].bonusSet) {
                pump = &game->pumpQueue[i];
                break;
            }
        }
        if (!pump) {
            for (i = 0; i < end; i++) {
                if (!game->pumpQueue[i].bonusSet) {
                    pump = &game->pumpQueue[i];
                    break;
                }
            }
        }
    }
    if (!pump) {
        GF_AssertFail();
        return FALSE;
    }
    if (pump->bonus) {
        GF_AssertFail();
    }
    pump->bonus = bonus;
    pump->bonusSet = TRUE;
    switch (bonus) {
    case WOBBUFFET_POP_BONUS_DOUBLE:
        pump->bonusAmount = pump->amount * 2;
        break;
    case WOBBUFFET_POP_BONUS_HALVE:
        pump->bonusAmount = pump->amount / 2;
        break;
    case WOBBUFFET_POP_BONUS_TRIPLE:
        pump->bonusAmount = pump->amount * 3;
        break;
    }
    return TRUE;
}

void WobbuffetPop_CreateSeatSprites(WobbuffetPop *game, ManagedSprite **sprites) {
    ManagedSpriteTemplate template = sWobbuffetPopSeatSpriteTemplate;
    int i;

    for (i = 0; i < 4; i++) {
        sprites[i] = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &template);
        ManagedSprite_SetPositionXYWithSubscreenOffset(sprites[i], sWobbuffetPopSeatPositions[i][0], sWobbuffetPopSeatPositions[i][1], 0x160000);
        ManagedSprite_SetAnim(sprites[i], i / 2 + 31);
        Sprite_TickFrame(sprites[i]->sprite);
    }
    switch (game->app->playerCount) {
    case 2:
        ManagedSprite_SetDrawFlag(sprites[2], FALSE);
        ManagedSprite_SetDrawFlag(sprites[3], FALSE);
        break;
    case 3:
        ManagedSprite_SetDrawFlag(sprites[1], FALSE);
        break;
    }
}

void WobbuffetPop_FreeSeatSprites(WobbuffetPop *game, ManagedSprite **sprites) {
    s32 i;

    for (i = 0; i < 4; i++) {
        Sprite_DeleteAndFreeResources(*sprites);
        sprites++;
    }
}

void WobbuffetPopBonusRing_CreateSprites(WobbuffetPop *game, WobbuffetPopBonusRing *ring) {
    ManagedSpriteTemplate template = sWobbuffetPopBonusSpriteTemplate;
    int i;

    for (i = 0; i < 3; i++) {
        WobbuffetPopBonusTarget *target = &ring->targets[i];
        int x = 128 + FX_Mul(GF_SinDegFX32(ring->angle + target->angle), 76);
        int y = 98 - FX_Mul(GF_CosDegFX32(ring->angle + target->angle), 68);

        target->sprite = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &template);
        ManagedSprite_SetPositionXYWithSubscreenOffset(target->sprite, (s16)x, (s16)(y - 24), 0x160000);
        ManagedSprite_SetAnim(target->sprite, sWobbuffetPopBonusTargetAnims[i + 1]);
        Sprite_TickFrame(target->sprite->sprite);
        ManagedSprite_SetDrawFlag(target->sprite, FALSE);
        target->effect = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &sWobbuffetPopBonusEffectSpriteTemplate);
        ManagedSprite_SetPositionXYWithSubscreenOffset(target->effect, 0, 0, 0x160000);
        ManagedSprite_SetAnim(target->effect, 33);
        Sprite_TickFrame(target->effect->sprite);
        ManagedSprite_SetDrawFlag(target->effect, FALSE);
        target->shadow = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &sWobbuffetPopBonusShadowSpriteTemplate);
        ManagedSprite_SetPositionXYWithSubscreenOffset(target->shadow, (s16)x, (s16)y, 0x160000);
        ManagedSprite_SetOamMode(target->shadow, GX_OAM_MODE_XLU);
        ManagedSprite_SetAffineOverwriteMode(target->shadow, 1);
        ManagedSprite_SetAffineScale(target->shadow, 1.0f, 1.0f);
        ManagedSprite_SetAnim(target->shadow, i + 34);
        Sprite_TickFrame(target->shadow->sprite);
        ManagedSprite_SetDrawFlag(target->shadow, FALSE);
        WobbuffetPopDust_Create(game, &target->dust);
    }
    ManagedSprite_SetDrawFlag(ring->targets[0].sprite, TRUE);
    ManagedSprite_SetDrawFlag(ring->targets[0].shadow, TRUE);
}

void WobbuffetPopBonusRing_FreeSprites(WobbuffetPop *game, WobbuffetPopBonusRing *ring) {
    s32 i;
    WobbuffetPopBonusTarget *target = ring->targets;

    for (i = 0; i < 3; i++) {
        Sprite_DeleteAndFreeResources(target->sprite);
        Sprite_DeleteAndFreeResources(target->effect);
        Sprite_DeleteAndFreeResources(target->shadow);
        WobbuffetPopDust_Free(game, &target->dust);
        target++;
    }
}

#pragma opt_strength_reduction off
void WobbuffetPopBonusRing_Init(WobbuffetPop *game, WobbuffetPopBonusRing *ring) {
    ring->angleStep = 0x1e000 / sWobbuffetPopRingSpeeds[ring->speed].steps;
    int i = 0;
    int angle = i;
    WobbuffetPopBonusTarget *target = ring->targets;

    do {
        target->angle = angle << 12;
        i++;
        target->bonus = i;
        angle += 90;
        target++;
    } while (i < 3);
    ring->targets[0].state = WOBBUFFET_POP_BONUS_TARGET_WALK;
    ring->done = TRUE;
}
#pragma opt_strength_reduction reset

void WobbuffetPopBonusRing_Update(WobbuffetPop *game, WobbuffetPopBonusRing *ring) {
    int sum = 0;
    int i;

    if (ring->done == TRUE) {
        return;
    }
    ring->ticks++;
    if (ring->delay == 0) {
        ring->angle += ring->angleStep;
        ring->stepTimer++;
        if (ring->stepTimer >= sWobbuffetPopRingSpeeds[ring->speed].steps) {
            ring->stepTimer = 0;
            ring->position++;
            ring->angle = ring->position * 0x1e000;
            ring->delay = sWobbuffetPopRingSpeeds[ring->speed].delay;
        }
    } else {
        ring->delay--;
        if (ring->delay == 0 && ring->position >= 12) {
            ring->position = 0;
            ring->speed++;
            if (ring->speed >= 5) {
                ring->speed = 4;
            }
            ring->angleStep = 0x1e000 / sWobbuffetPopRingSpeeds[ring->speed].steps;
        }
    }
    if (ring->speed == 1) {
        {
            const WobbuffetPopRingSpeed *p = sWobbuffetPopRingSpeeds;
            for (i = 0; i < 2; i++, p++) {
                sum += (p->steps + p->delay) * 12;
            }
        }
        if (sum - 15 == ring->ticks) {
            WobbuffetPopBonusTarget_SetState(&ring->targets[1], WOBBUFFET_POP_BONUS_TARGET_ENTER);
        }
    } else if (ring->speed == 2) {
        {
            const WobbuffetPopRingSpeed *p = sWobbuffetPopRingSpeeds;
            for (i = 0; i < 3; i++, p++) {
                sum += (p->steps + p->delay) * 12;
            }
        }
        if (sum - 15 == ring->ticks) {
            WobbuffetPopBonusTarget_SetState(&ring->targets[2], WOBBUFFET_POP_BONUS_TARGET_ENTER);
        }
    }
    for (i = 0; i < 3; i++) {
        WobbuffetPopBonusTarget_Run(game, ring, &ring->targets[i]);
        WobbuffetPopDust_Update(game, &ring->targets[i], &ring->targets[i].dust);
    }
}

static u8 WobbuffetPop_FindBonusTargetAtFront(WobbuffetPop *game, WobbuffetPopBonusTarget **outTarget) {
    s32 i;
    WobbuffetPopBonusTarget *targets;
    WobbuffetPopBonusTarget *target;

    for (i = 0, targets = game->bonusRing.targets, target = targets; i < 3; i++) {
        if (target->atFront == TRUE) {
            *outTarget = &targets[i];
            return (*outTarget)->bonus;
        }
        target++;
    }
    *outTarget = NULL;
    return WOBBUFFET_POP_BONUS_NONE;
}

static void WobbuffetPopBonusTarget_SetState(WobbuffetPopBonusTarget *target, u8 state) {
    target->state = state;
    target->step = 0;
    target->timer = 0;
    target->duration = 0;
    target->atFront = FALSE;
}

static u32 WobbuffetPopBonusTarget_Run(WobbuffetPop *game, WobbuffetPopBonusRing *ring, WobbuffetPopBonusTarget *target) {
    return sWobbuffetPopBonusTargetStateFuncs[target->state](game, ring, target);
}

static u32 WobbuffetPopBonusTarget_Idle(WobbuffetPop *game, WobbuffetPopBonusRing *ring, WobbuffetPopBonusTarget *target) {
    target->atFront = FALSE;
    return TRUE;
}

static u32 WobbuffetPopBonusTarget_Enter(WobbuffetPop *game, WobbuffetPopBonusRing *ring, WobbuffetPopBonusTarget *target) {
    struct {
        s16 y;
        s16 x;
    } position;

    position.x = target->motion.home.x;
    position.y = target->motion.home.y;
    int offset;
    switch (target->step) {
    case WOBBUFFET_POP_BONUS_TARGET_ENTER_SHOW:
        ManagedSprite_SetDrawFlag(target->sprite, TRUE);
        ManagedSprite_SetDrawFlag(target->shadow, TRUE);
        ManagedSprite_GetPositionXYWithSubscreenOffset(target->sprite, &position.x, &position.y, 0x160000);
        target->motion.home.x = position.x;
        target->motion.home.y = position.y;
        target->step++;
    case WOBBUFFET_POP_BONUS_TARGET_ENTER_MOVE_IN:
        switch (target->angle) {
        case 0: {
            s16 delta = target->motion.home.y;
            u8 time = target->timer;
            delta += 32;
            delta = (s16)(time * delta / 15);
            position.y = delta - 32;
        } break;
        case 0x5a000: {
            s16 delta = target->motion.home.x;
            u8 time = target->timer;
            delta -= 288;
            delta = (s16)(time * delta / 15);
            position.x = delta + 288;
        } break;
        case 0xb4000: {
            s16 delta = target->motion.home.y;
            u8 time = target->timer;
            delta -= 228;
            delta = (s16)(time * delta / 15);
            position.y = delta + 228;
        } break;
        case 0x10e000: {
            s16 delta = target->motion.home.x;
            u8 time = target->timer;
            delta += 32;
            delta = (s16)(time * delta / 15);
            position.x = delta - 32;
        } break;
        default:
            GF_AssertFail();
            break;
        }
        if (target->timer >= 15) {
            ManagedSprite_SetPositionXYWithSubscreenOffset(target->sprite, target->motion.home.x, target->motion.home.y, 0x160000);
            ManagedSprite_SetPositionXYWithSubscreenOffset(target->shadow, target->motion.home.x, (s16)(target->motion.home.y + 24), 0x160000);
            WobbuffetPopBonusTarget_SetState(target, WOBBUFFET_POP_BONUS_TARGET_WALK);
            WobbuffetPopDust_Start(game, target, &target->dust);
            return TRUE;
        }
        offset = -((fx32)(((s64)GF_SinDegFX32(((180 * target->timer) << 12) / 15) * 0x18000 + 0x800) >> 12)) / 4096;
        ManagedSprite_SetPositionXYWithSubscreenOffset(target->sprite, position.x, (s16)(position.y + offset), 0x160000);
        ManagedSprite_SetPositionXYWithSubscreenOffset(target->shadow, position.x, (s16)(position.y + 24), 0x160000);
        {
            float scale = (float)(4096 - ((-offset / 3) * 4096) / 24) / 4096.0f;
            ManagedSprite_SetAffineScale(target->shadow, scale, scale);
        }
        break;
    default:
        break;
    }
    target->timer++;
    return TRUE;
}

static u32 WobbuffetPopBonusTarget_Walk(WobbuffetPop *game, WobbuffetPopBonusRing *ring, WobbuffetPopBonusTarget *target) {
    s32 bob;
    s32 x;
    s32 y;
    float scale;

    if (ring->stepTimer != 0) {
        ManagedSprite_TickNFrames(target->sprite, 0x2c000 / sWobbuffetPopRingSpeeds[ring->speed].steps);
    } else {
        ManagedSprite_SetAnimationFrame(target->sprite, 0);
    }
    bob = -FX_Mul(GF_SinDegFX32(((180 * ring->stepTimer) << FX32_SHIFT) / sWobbuffetPopRingSpeeds[ring->speed].steps), 0xc000) / FX32_ONE;
    x = 128 + FX_Mul(GF_SinDegFX32(ring->angle + target->angle), 76);
    y = 98 - FX_Mul(GF_CosDegFX32(ring->angle + target->angle), 68);
    ManagedSprite_SetPositionXYWithSubscreenOffset(target->sprite, (s16)x, (s16)(y - 24 + bob), 0x160000);
    ManagedSprite_SetPositionXYWithSubscreenOffset(target->shadow, (s16)x, (s16)y, 0x160000);
    scale = (float)(FX32_ONE - ((-bob / 3) << FX32_SHIFT) / 12) / (float)FX32_ONE;
    ManagedSprite_SetAffineScale(target->shadow, scale, scale);
    if (ring->stepTimer == sWobbuffetPopRingSpeeds[ring->speed].steps - 1) {
        WobbuffetPopDust_Start(game, target, &target->dust);
    }
    target->atFront = (((ring->angle + target->angle) >> FX32_SHIFT) % 360 == 180);
    return TRUE;
}

static inline void WobbuffetPopBonusTarget_StartHop(WobbuffetPopBonusTarget *target, fx32 sine, fx32 cosine, s16 x, s16 y, int duration) {
    target->motion.hop.dx = (s32)((u32)((s16)((s32)(((s64)sine * 76 + 0x800) >> 12) + 128) - x) << 12) / duration;
    target->motion.hop.dy = (s32)((u32)((s16)(98 - (s32)(((s64)cosine * 68 + 0x800) >> 12)) - y) << 12) / duration;
    target->motion.hop.x = (s32)((u32)x << 12);
    target->motion.hop.y = (s32)((u32)y << 12);
    target->timer = duration;
    target->duration = duration;
    target->step = (u8)(target->step + 1);
}

static u32 WobbuffetPopBonusTarget_Hit(WobbuffetPop *game, WobbuffetPopBonusRing *ring, WobbuffetPopBonusTarget *target) {
    s16 originX;
    s16 originY;
    s16 x;
    s16 y;

    switch (target->step) {
    case WOBBUFFET_POP_BONUS_TARGET_HIT_FLASH:
        PlaySE(SEQ_SE_PL_BALLOON05);
        ManagedSprite_GetPositionXYWithSubscreenOffset(target->sprite, &originX, &originY, 0x160000);
        ManagedSprite_SetPositionXYWithSubscreenOffset(target->effect, originX, originY - 32, 0x160000);
        ManagedSprite_SetAnim(target->effect, 33);
        ManagedSprite_SetDrawFlag(target->effect, TRUE);
        ManagedSprite_SetAnim(target->sprite, sWobbuffetPopBonusTargetAnims[target->bonus] + 2);
        target->timer = sWobbuffetPopRingSpeeds[ring->speed].timer;
        target->halfTime = sWobbuffetPopRingSpeeds[ring->speed].timer / 2;
        target->step = (u8)(target->step + 1);
        break;
    case WOBBUFFET_POP_BONUS_TARGET_HIT_WAIT:
        if (target->timer == target->halfTime) {
            ManagedSprite_SetAnim(target->sprite, sWobbuffetPopBonusTargetAnims[target->bonus] + 1);
            ManagedSprite_SetDrawFlag(target->effect, FALSE);
            ManagedSprite_TickNFrames(target->effect, 0x4000);
        }
        if (target->timer == 0) {
            ManagedSprite_SetDrawFlag(target->effect, FALSE);
            ManagedSprite_SetAnim(target->sprite, sWobbuffetPopBonusTargetAnims[target->bonus]);
            target->step = (u8)(target->step + 1);
        } else {
            target->timer--;
        }
        break;
    case WOBBUFFET_POP_BONUS_TARGET_HIT_START_HOP: {
        int duration;
        int start;
        int value = 0;
        s32 angle;
        s32 sine;
        s32 cosine;

        if (ring->delay == 0) {
            int period = sWobbuffetPopRingSpeeds[ring->speed].steps;
            value += period - ring->stepTimer;
            value += sWobbuffetPopRingSpeeds[ring->speed].delay;
            duration = value + period;
            start = ring->position + 2;
        } else {
            value += ring->delay;
            if (ring->position >= 12) {
                duration = value + sWobbuffetPopRingSpeeds[ring->speed + 1].steps;
                start = 1;
            } else {
                duration = value + sWobbuffetPopRingSpeeds[ring->speed].steps;
                start = ring->position + 1;
            }
        }
        start += target->angle / 0x1e000;
        start %= 12;
        angle = start * 0x1e000;
        sine = GF_SinDegFX32(angle);
        cosine = GF_CosDegFX32(angle);
        ManagedSprite_GetPositionXYWithSubscreenOffset(target->sprite, &x, &y, 0x160000);
        y += 24;
        WobbuffetPopBonusTarget_StartHop(target, sine, cosine, x, y, duration);
    }
    case WOBBUFFET_POP_BONUS_TARGET_HIT_HOP: {
        s64 height;

        target->motion.hop.x = (s32)((u32)target->motion.hop.x + (u32)target->motion.hop.dx);
        target->motion.hop.y = (s32)((u32)target->motion.hop.y + (u32)target->motion.hop.dy);
        height = (s64)GF_SinDegFX32(((target->timer * 180) << 12) / target->duration) * 0xc000;
        ManagedSprite_SetPositionXYWithSubscreenOffset(target->sprite, (s16)(target->motion.hop.x / 4096), (s16)(target->motion.hop.y / 4096 - 24 + (-(s32)((height + 0x800) >> 12)) / 4096), 0x160000);
        ManagedSprite_TickNFrames(target->sprite, 0x2c000 / target->duration);
        ManagedSprite_SetPositionXYWithSubscreenOffset(target->shadow, (s16)(target->motion.hop.x / 4096), (s16)(target->motion.hop.y / 4096), 0x160000);
        target->timer--;
        if (target->timer == 0) {
            ManagedSprite_SetAnim(target->sprite, sWobbuffetPopBonusTargetAnims[target->bonus]);
            ManagedSprite_SetAnimationFrame(target->sprite, 0);
            WobbuffetPopBonusTarget_SetState(target, WOBBUFFET_POP_BONUS_TARGET_WALK);
            return TRUE;
        }
        break;
    }
    }
    return TRUE;
}

void WobbuffetPop_CreateBonusPopupSprites(WobbuffetPop *game, WobbuffetPopBonusPopup *popups) {
    ManagedSpriteTemplate template = sWobbuffetPopBonusSpriteTemplate;
    int i;
    WobbuffetPopBonusPopup *popup;

    for (i = 0, popup = popups; i < 3; i++, popup++) {
        ManagedSprite *sprite;

        sprite = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &template);
        popup->sprite = sprite;
        ManagedSprite_SetPositionXYWithSubscreenOffset(sprite, 0, 0, 0x160000);
        ManagedSprite_SetDrawFlag(popup->sprite, FALSE);
        sprite = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &sWobbuffetPopBonusEffectSpriteTemplate);
        popup->effect = sprite;
        ManagedSprite_SetPositionXYWithSubscreenOffset(sprite, 0, 0, 0x160000);
        ManagedSprite_SetAnim(popup->effect, 33);
        Sprite_TickFrame(popup->effect->sprite);
        ManagedSprite_SetDrawFlag(popup->effect, FALSE);
    }
}

void WobbuffetPop_FreeBonusPopupSprites(WobbuffetPop *game, WobbuffetPopBonusPopup *popups) {
    WobbuffetPopBonusPopup *popup = popups;
    s32 i;

    for (i = 0; i < 3; i++) {
        Sprite_DeleteAndFreeResources(popup->sprite);
        Sprite_DeleteAndFreeResources(popup->effect);
        popup++;
    }
}

static void WobbuffetPop_ShowBonusPopup(WobbuffetPop *game, WobbuffetPopBonusPopup *popups, int bonus, int netId, int delay) {
    int bonusCopy = bonus;
    WobbuffetPopBonusPopup *popup = NULL;
    int i;
    int seat;
    int spot;

    if (bonusCopy == 0) {
        return;
    }
    if (netId == Comm_GetLocalNetId()) {
        return;
    }
    for (i = 0; i < 3; i++) {
        if (popups[i].active == FALSE) {
            popup = &popups[i];
            break;
        }
    }
    if (popup == NULL) {
        return;
    }
    seat = WobbuffetPop_GetPlayerSeat(game, netId);
    ManagedSprite_SetPaletteOverride(popup->sprite, sWobbuffetPopPopupPalettes[game->app->playerCount][seat]);
    ManagedSprite_SetAnim(popup->sprite, sWobbuffetPopBonusTargetAnims[bonusCopy]);
    spot = sWobbuffetPopPopupSpots[game->app->playerCount][seat];
    {
        s16 x = sWobbuffetPopSeatPositions[spot][0];
        popup->targetY = sWobbuffetPopSeatPositions[spot][1] - 24;
        ManagedSprite_SetPositionXYWithSubscreenOffset(popup->sprite, x, popup->targetY - 96, 22 << 16);
    }
    Sprite_TickFrame(popup->sprite->sprite);
    popup->bonus = bonusCopy;
    popup->delay = delay - 12;
    popup->state = WOBBUFFET_POP_BONUS_POPUP_START;
    popup->active = TRUE;
}

void WobbuffetPop_UpdateBonusPopups(WobbuffetPop *game, WobbuffetPopBonusPopup *popups) {
    s32 i;
    WobbuffetPopBonusPopup *popup;

    for (i = 0, popup = popups; i < 3; i++) {
        if (popup->active == TRUE) {
            WobbuffetPopBonusPopup_Update(popups, popup);
        }
        popup++;
    }
}

static BOOL WobbuffetPopBonusPopup_Update(WobbuffetPopBonusPopup *popups, WobbuffetPopBonusPopup *popup) {
    s16 x;
    s16 y;

    switch (popup->state) {
    case WOBBUFFET_POP_BONUS_POPUP_START:
        ManagedSprite_GetPositionXYWithSubscreenOffset(popup->sprite, &x, &y, 0x160000);
        popup->x = x << FX32_SHIFT;
        popup->y = y << FX32_SHIFT;
        popup->state++;
    case WOBBUFFET_POP_BONUS_POPUP_DROP_IN:
        if (popup->delay > 0) {
            popup->delay--;
            break;
        }
        ManagedSprite_SetDrawFlag(popup->sprite, TRUE);
        popup->y += 0x8000;
        if (popup->y / FX32_ONE >= popup->targetY) {
            popup->y = popup->targetY << FX32_SHIFT;
            popup->state++;
        }
        break;
    case WOBBUFFET_POP_BONUS_POPUP_HIT:
        ManagedSprite_GetPositionXYWithSubscreenOffset(popup->sprite, &x, &y, 0x160000);
        ManagedSprite_SetPositionXYWithSubscreenOffset(popup->effect, x, y - 32, 0x160000);
        ManagedSprite_SetAnim(popup->effect, 33);
        ManagedSprite_SetDrawFlag(popup->effect, TRUE);
        PlaySE(SEQ_SE_PL_BALLOON05);
        ManagedSprite_SetAnim(popup->sprite, sWobbuffetPopBonusTargetAnims[popup->bonus] + 2);
        popup->timer = 8;
        popup->state++;
        break;
    case WOBBUFFET_POP_BONUS_POPUP_WAIT_HIT:
        popup->timer--;
        if (popup->timer == 3) {
            ManagedSprite_TickNFrames(popup->effect, 0x4000);
        }
        if (popup->timer == 0) {
            ManagedSprite_SetDrawFlag(popup->effect, FALSE);
            ManagedSprite_SetAnim(popup->sprite, sWobbuffetPopBonusTargetAnims[popup->bonus]);
            popup->state++;
        }
        break;
    case WOBBUFFET_POP_BONUS_POPUP_RISE_OUT:
        popup->y -= 0x8000;
        if (popup->y / FX32_ONE <= popup->targetY - 96) {
            ManagedSprite_SetDrawFlag(popup->sprite, FALSE);
            popup->state = WOBBUFFET_POP_BONUS_POPUP_START;
            popup->active = FALSE;
            return FALSE;
        }
        break;
    }
    ManagedSprite_SetPositionXYWithSubscreenOffset(popup->sprite, popup->x / FX32_ONE, popup->y / FX32_ONE, 0x160000);
    return TRUE;
}

void WobbuffetPop_NewTextObj(BgConfig *bgConfig, SpriteManager *spriteManager, FontSystem *fontSystem, WobbuffetPopTextObj *textObj, String *string, int fontId, u32 color, int setPalOffset, int plttResId, int x, int y, int centered, int unk_20, int vram, int height) {
    TextOBJTemplate template;
    Window window;
    ObjCharSpaceReservation charAlloc;
    TextOBJ *obj;
    int width = FontID_String_GetWidthMultiline(fontId, string, 0);
    int tiles = width / 8;

    if (FX_ModS32(width, 8) != 0) {
        tiles++;
    }
    InitWindow(&window);
    AddTextWindowTopLeftCorner(bgConfig, &window, tiles, height, 0, 0);
    AddTextPrinterParameterizedWithColorAndSpacing(&window, fontId, string, 0, 0, TEXT_SPEED_NOTRANSFER, color, 0, 0, NULL);
    ObjCharTransfer_ReserveCharSpace(FontOAM_GetCharSize(&window, NNS_G2D_VRAM_TYPE_2DMAIN, HEAP_ID_117), TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, &charAlloc);
    if (centered == TRUE) {
        x -= width / 2;
    }
    template.fontSystem = fontSystem;
    template.window = &window;
    template.spriteList = SpriteManager_GetSpriteList(spriteManager);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(spriteManager, plttResId);
    template.sprite = NULL;
    template.offset = charAlloc.offset;
    template.x = x;
    template.y = y;
    template.bgPriority = unk_20;
    template.drawPriority = vram;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.heapID = HEAP_ID_117;
    obj = FontOAM_Create(&template);
    if (setPalOffset) {
        TextOBJ_SetPalOffset(obj);
    }
    TextOBJ_SetPosition(obj, x, y);
    RemoveWindow(&window);
    textObj->textObj = obj;
    textObj->charAlloc = charAlloc;
    textObj->width = width;
}

void WobbuffetPopTextObj_Free(WobbuffetPopTextObj *textObj) {
    FontOAM_Delete(textObj->textObj);
    ObjCharTransfer_FreeCharSpace(&textObj->charAlloc);
    textObj->textObj = NULL;
}

void WobbuffetPopScoreDisplay_PlaceDigits(WobbuffetPopScoreDisplay *display) {
    s32 x;
    s32 y;
    s32 i;
    s32 bottom;
    u8 *slot;
    s32 top;

    for (i = 0, slot = (u8 *)display; i < 6; i++, slot += 0x28) {
        s32 value = display->current[i];

        if (value < 64) {
            top = 64 - value;
            bottom = top - 80;
        } else if (value < 128) {
            bottom = 160 - value;
            bottom -= 16;
            top = bottom - 80;
        } else {
            value -= 160;
            top = 64 - value;
            bottom = top - 80;
        }
        TextOBJ_GetPosition(((WobbuffetPopScoreDisplay *)slot)->digits[0].top.textObj, &x, &y);
        TextOBJ_SetPosition(((WobbuffetPopScoreDisplay *)slot)->digits[0].top.textObj, x, 168 - top);
        TextOBJ_SetPosition(((WobbuffetPopScoreDisplay *)slot)->digits[0].bottom.textObj, x, 168 - bottom);
    }
}

void WobbuffetPopScoreDisplay_Update(WobbuffetPop *game, WobbuffetPopScoreDisplay *display) {
    BOOL done;

    if (display->delay > 0) {
        display->delay--;
        return;
    }
    switch (display->state) {
    case WOBBUFFET_POP_SCORE_DISPLAY_CHECK:
        if (WobbuffetPopScoreDisplay_IsSettled(display) == TRUE) {
            WobbuffetPopScoreDisplay_StartRoll(display);
            return;
        }
        display->state++;
    case WOBBUFFET_POP_SCORE_DISPLAY_ROLL:
        done = WobbuffetPopScoreDisplay_RollDigits(display);
        WobbuffetPopScoreDisplay_PlaceDigits(display);
        if (done == TRUE) {
            display->delay = 15;
            display->state--;
        }
        break;
    }
}

static void WobbuffetPopScoreDisplay_SetScore(WobbuffetPopScoreDisplay *display, s32 score) {
    s32 i;
    s32 divisor = 100000;

    for (i = 0; i < 6; i++) {
        s32 digit = score / divisor;
        display->next[i] = digit << 4;
        score -= digit * divisor;
        divisor /= 10;
    }
}

static void WobbuffetPopScoreDisplay_StartRoll(WobbuffetPopScoreDisplay *display) {
    s32 i;

    for (i = 0; i < 6; i++) {
        display->goal[i] = display->next[i];
        display->countdown[i] = (5 - i) * 2;
        display->passes[i] = 0;
    }
    display->settled = 0;
}

static BOOL WobbuffetPopScoreDisplay_IsSettled(WobbuffetPopScoreDisplay *display) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (display->current[i] != display->goal[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

#pragma opt_strength_reduction off
static BOOL WobbuffetPopScoreDisplay_RollDigits(WobbuffetPopScoreDisplay *display) {
    int digit = 5;
    int i;
    int prior;

    if (display->settled > 6) {
        return TRUE;
    }
    prior = 0;
    for (i = 0; i < 6; i++, digit--) {
        int saved = prior;
        WobbuffetPopScoreDisplay *d = (WobbuffetPopScoreDisplay *)((u8 *)display + digit);

        if (d->countdown[0] > 0) {
            d->countdown[0]--;
            prior = 0;
        } else if (d->passes[0] > 1 && d->current[0] == d->goal[0]) {
            prior = 1;
        } else {
            prior = 0;
            int old = d->current[0];
            d->current[0] += 23;
            int goal = d->goal[0];
            int lifted = goal + 160;
            if ((old <= goal && (u32)d->current[0] >= (u32)goal) || (old <= lifted && (int)d->current[0] >= lifted)) {
                if (d->passes[0] < 1) {
                    d->passes[0]++;
                } else if (digit == 5 || saved == 1) {
                    d->passes[0]++;
                    d->current[0] = d->goal[0];
                    display->settled++;
                    if (display->settled >= 6) {
                        return TRUE;
                    }
                }
            }
            d->current[0] = (int)d->current[0] % 160;
        }
    }
    return FALSE;
}
#pragma opt_strength_reduction reset

ManagedSprite *WobbuffetPop_CreateScoreFrameSprite(WobbuffetPop *game) {
    ManagedSprite *sprite = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &sWobbuffetPopScoreFrameSpriteTemplate);

    Sprite_TickFrame(sprite->sprite);
    return sprite;
}

void WobbuffetPop_CreateScoreLabels(WobbuffetPop *game) {
    int i;
    u32 score = game->score;
    u8 *cursor;
    u8 *other;
    WobbuffetPopTextObj *label;

    i = 5;
    cursor = (u8 *)game + 5 * sizeof(WobbuffetPopTextObj);
    other = (u8 *)game + 5 * 0x28;
    label = game->scoreDisplay.labels;
    label = (WobbuffetPopTextObj *)((u8 *)label + 5 * sizeof(WobbuffetPopTextObj));
    for (; i >= 0; i--) {
        String *string;
        int x;
        int y;

        if (((WobbuffetPop *)cursor)->scoreDisplay.labels[0].textObj != NULL) {
            GF_AssertFail();
        }
        string = NewString_ReadMsgData(game->msgData, score % 10 + 4);
        score /= 10;
        TextOBJ_GetPosition(((WobbuffetPop *)other)->scoreDisplay.digits[0].top.textObj, &x, &y);
        WobbuffetPop_NewTextObj(game->bgConfig, game->spriteManager, game->fontSystem, label, string, 0, MAKE_TEXT_COLOR(14, 15, 0), 0, 10003, x, 168, 0, 1, 12, 2);
        String_Delete(string);
        cursor -= sizeof(WobbuffetPopTextObj);
        other -= 0x28;
        label = (WobbuffetPopTextObj *)((u8 *)label - sizeof(WobbuffetPopTextObj));
    }
}

void WobbuffetPop_FreeScoreLabels(WobbuffetPop *game) {
    s32 i;
    u8 *p = (u8 *)game;
    WobbuffetPopTextObj *label = game->scoreDisplay.labels;

    for (i = 0; i < 6; i++) {
        if (((WobbuffetPop *)p)->scoreDisplay.labels[0].textObj != NULL) {
            WobbuffetPopTextObj_Free(label);
        }
        p += sizeof(WobbuffetPopTextObj);
        label++;
    }
}

void WobbuffetPop_DeleteScoreFrameSprite(WobbuffetPop *game, ManagedSprite *sprite) {
    Sprite_DeleteAndFreeResources(sprite);
}

ManagedSprite *WobbuffetPop_CreateDemoStylusSprite(WobbuffetPop *game) {
    ManagedSprite *sprite = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &sWobbuffetPopDemoStylusSpriteTemplate);

    ManagedSprite_SetDrawFlag(sprite, FALSE);
    Sprite_TickFrame(sprite->sprite);
    return sprite;
}

void WobbuffetPop_DeleteDemoStylusSprite(WobbuffetPop *game, ManagedSprite *sprite) {
    Sprite_DeleteAndFreeResources(sprite);
}

BOOL WobbuffetPop_RunDemo(WobbuffetPop *game, WobbuffetPopDemo *demo) {
    switch (demo->state) {
    case 0:
    case 5:
        ManagedSprite_SetPositionXYWithSubscreenOffset(demo->stylus, 128, 16, 22 << 16);
        ManagedSprite_SetDrawFlag(demo->stylus, TRUE);
        demo->state++;
        break;
    case 1:
    case 6:
        demo->timer++;
        if (demo->timer > 15) {
            demo->timer = 0;
            demo->touch[2] = TRUE;
            demo->touch[3] = TRUE;
            demo->touch[0] = 128;
            demo->touch[1] = 32;
            demo->state++;
        }
        break;
    case 2:
    case 7:
        demo->touch[3] = FALSE;
        demo->touch[1] += 6;
        ManagedSprite_OffsetPositionXY(demo->stylus, 0, 6);
        if (demo->touch[1] > 96) {
            demo->state++;
        }
        break;
    case 3:
    case 8:
        demo->touch[2] = FALSE;
        ManagedSprite_SetDrawFlag(demo->stylus, FALSE);
        demo->state++;
    case 4:
    case 9:
        demo->timer++;
        if (demo->timer > 30) {
            demo->timer = 0;
            demo->state++;
        }
        break;
    case 10:
        return TRUE;
    }
    return FALSE;
}

static void WobbuffetPopDust_Create(WobbuffetPop *game, WobbuffetPopDust *dust) {
    s32 i;
    ManagedSprite **out = dust->sprites;

    for (i = 0; i < 2; i++) {
        ManagedSprite *sprite = SpriteSystem_NewSprite(game->spriteSystem, game->spriteManager, &sWobbuffetPopDustSpriteTemplate);

        ManagedSprite_SetAnim(sprite, 27);
        ManagedSprite_SetOamMode(sprite, GX_OAM_MODE_XLU);
        ManagedSprite_SetDrawFlag(sprite, FALSE);
        Sprite_TickFrame(sprite->sprite);
        *out++ = sprite;
    }
}

static void WobbuffetPopDust_Free(WobbuffetPop *game, WobbuffetPopDust *dust) {
    ManagedSprite **sprites = dust->sprites;
    s32 i;

    for (i = 0; i < 2; i++) {
        Sprite_DeleteAndFreeResources(*sprites);
        sprites++;
    }
}

static void WobbuffetPopDust_Start(WobbuffetPop *game, WobbuffetPopBonusTarget *target, WobbuffetPopDust *dust) {
    s16 xy[2];
    int i;

    ManagedSprite_GetPositionXYWithSubscreenOffset(target->sprite, &xy[1], &xy[0], 0x160000);
    for (i = 0; i < 2; i++) {
        ManagedSprite_SetPositionXYWithSubscreenOffset(dust->sprites[i], xy[1], xy[0] + 28, 0x160000);
        ManagedSprite_SetAnim(dust->sprites[i], 27);
        ManagedSprite_SetOamMode(dust->sprites[i], GX_OAM_MODE_XLU);
        ManagedSprite_SetDrawFlag(dust->sprites[i], TRUE);
        dust->speedX[i] = LCRandom() % 4096 + 4096;
        dust->speedY[i] = LCRandom() % 1024 + 1024;
        if (i & 1) {
            dust->speedX[i] *= -1;
        }
    }
    dust->timer = 0;
    dust->active = TRUE;
}

static void WobbuffetPopDust_Update(WobbuffetPop *game, WobbuffetPopBonusTarget *target, WobbuffetPopDust *dust) {
    int i;

    switch (dust->active) {
    case 0:
        break;
    case 1:
        for (i = 0; i < 2; i++) {
            ManagedSprite_AddSpritePrecisePositionXY(dust->sprites[i], dust->speedX[i], -dust->speedY[i]);
            Sprite_TickFrame(dust->sprites[i]->sprite);
        }
        dust->timer++;
        if (dust->timer > 8) {
            for (i = 0; i < 2; i++) {
                ManagedSprite_SetDrawFlag(dust->sprites[i], FALSE);
            }
            dust->active = FALSE;
        }
        break;
    }
}

// The player list and results screens, run by WobbuffetPopApp before and after the game.
typedef struct WobbuffetPopPlazaScreen {
    WobbuffetPopApp *app;
    PlazaGameEntry *entry;
    PlazaGameResults *results;
} WobbuffetPopPlazaScreen;

BOOL WobbuffetPopPlazaScreen_Init(OverlayManager *man, int *state) {
    WobbuffetPopPlazaScreen *screen = OverlayManager_CreateAndGetData(man, sizeof(WobbuffetPopPlazaScreen), HEAP_ID_117);

    MI_CpuFill8(screen, 0, sizeof(WobbuffetPopPlazaScreen));
    screen->app = OverlayManager_GetArgs(man);
    return TRUE;
}

BOOL WobbuffetPopPlazaScreen_Main(OverlayManager *man, int *state) {
    WobbuffetPopPlazaScreen *screen = OverlayManager_GetData(man);

    if (screen->app->connectionLost == TRUE) {
        switch (screen->app->connectionLostState) {
        case WOBBUFFET_POP_CONNECTION_LOST_BLACK_OUT:
            if (IsPaletteFadeFinished() == TRUE) {
                StopPaletteFade();
            }
            SetBothScreensBrightnessByColor(0);
            screen->app->connectionLostState++;
            break;
        case WOBBUFFET_POP_CONNECTION_LOST_WAIT:
            if (PlazaGameRoster_ProcessConnectionLost(&screen->app->roster) == TRUE) {
                screen->app->connectionLostState++;
            }
            break;
        case WOBBUFFET_POP_CONNECTION_LOST_FREE:
        default:
            if (screen->entry) {
                PlazaGameEntry_Free(screen->entry);
                screen->entry = NULL;
            }
            if (screen->results) {
                PlazaGameResults_Free(screen->results);
                screen->results = NULL;
            }
            return TRUE;
        }
        return FALSE;
    }
    switch (*state) {
    case WOBBUFFET_POP_PLAZA_SCREEN_START:
        if (screen->app->screenId == WOBBUFFET_POP_SCREEN_PLAYER_LIST) {
            *state = WOBBUFFET_POP_PLAZA_SCREEN_CREATE_ENTRY;
        } else {
            *state = WOBBUFFET_POP_PLAZA_SCREEN_CREATE_RESULTS;
        }
        break;
    case WOBBUFFET_POP_PLAZA_SCREEN_CREATE_ENTRY:
        screen->entry = PlazaGameEntry_NewWobbuffetPop(&screen->app->roster, HEAP_ID_117);
        (*state)++;
        break;
    case WOBBUFFET_POP_PLAZA_SCREEN_WAIT_ENTRY:
        if (PlazaGameEntry_IsDone(screen->entry) == TRUE) {
            screen->app->selection = PlazaGameEntry_GetSelection(screen->entry);
            PlazaGameEntry_Free(screen->entry);
            screen->entry = NULL;
            *state = WOBBUFFET_POP_PLAZA_SCREEN_DONE;
        }
        break;
    case WOBBUFFET_POP_PLAZA_SCREEN_CREATE_RESULTS:
        screen->results = PlazaGameResults_NewWobbuffetPop(&screen->app->roster, &screen->app->results, HEAP_ID_117);
        (*state)++;
        break;
    case WOBBUFFET_POP_PLAZA_SCREEN_WAIT_RESULTS:
        if (PlazaGameResults_IsDone(screen->results) == TRUE) {
            screen->app->playAgain = PlazaGameResults_GetContinue(screen->results);
            PlazaGameResults_Free(screen->results);
            screen->results = NULL;
            *state = WOBBUFFET_POP_PLAZA_SCREEN_DONE;
        }
        break;
    case WOBBUFFET_POP_PLAZA_SCREEN_DONE:
    default:
        return TRUE;
    }
    return FALSE;
}

BOOL WobbuffetPopPlazaScreen_Exit(OverlayManager *man, int *state) {
    OverlayManager_GetData(man);
    OverlayManager_FreeData(man);
    return TRUE;
}

void WobbuffetPop_RegisterCommands(WobbuffetPop *game) {
    Comm_RegisterCommandTable(gWobbuffetPopCommands, 4, game);
}

u32 WobbuffetPop_GetEventCommandSize(void) {
    return sizeof(WobbuffetPopEvent);
}

u32 WobbuffetPop_GetPlayerValueCommandSize(void) {
    return 4;
}

void WobbuffetPop_RecvEvent(int netId, int size, void *data, void *arg) {
    WobbuffetPop_QueuePlayerEvent(arg, netId, data);
}

BOOL WobbuffetPop_SendEvent(WobbuffetPop *game, WobbuffetPopEvent *event) {
    if (Comm_SendCommand(WOBBUFFET_POP_CMD_EVENT, event, sizeof(WobbuffetPopEvent)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void WobbuffetPop_RecvFinished(int netId, int size, void *data, void *arg) {
    WobbuffetPop *game = arg;

    game->finished = TRUE;
}

BOOL WobbuffetPop_SendFinished(WobbuffetPop *game) {
    if (Comm_SendCommand(WOBBUFFET_POP_CMD_FINISHED, NULL, 0) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void WobbuffetPop_RecvPlayerValue(int netId, int size, const void *data, void *arg) {
    WobbuffetPop *game = arg;
    u32 value = *(u32 *)data;
    int i;

    for (i = 0; i < game->app->playerCount; i++) {
        if (netId == game->app->netIds[i]) {
            game->playerValues[i] = value;
            break;
        }
    }
    GF_ASSERT(i < game->app->playerCount);
    game->readyCount++;
}

BOOL WobbuffetPop_SendPlayerValue(void) {
    u32 value = 100;

    if (Comm_SendCommand(WOBBUFFET_POP_CMD_PLAYER_VALUE, &value, sizeof(value)) == TRUE) {
        return TRUE;
    }
    return FALSE;
}

BOOL WobbuffetPop_RunEvent(WobbuffetPop *game, WobbuffetPopEvent *event) {
    if (gWobbuffetPopEventFuncs[event->kind] != NULL) {
        gWobbuffetPopEventFuncs[event->kind](game, event);
        MI_CpuFill8(event, 0, sizeof(WobbuffetPopEvent));
        return TRUE;
    }
    return FALSE;
}

static void WobbuffetPop_QueuePumpEvent(WobbuffetPop *game, const WobbuffetPopPump *pump) {
    WobbuffetPopEvent event;

    MI_CpuFill8(&event, 0, sizeof(event));
    event.kind = WOBBUFFET_POP_EVENT_PUMP;
    *(WobbuffetPopPump *)&event.data = *pump;
    WobbuffetPop_QueueEvent(game, &event);
}

void WobbuffetPop_HandlePumpEvent(WobbuffetPop *game, WobbuffetPopEvent *event) {
    if (event->data.bytes.b4 != Comm_GetLocalNetId()) {
        WobbuffetPop_LaunchPuff(game, (WobbuffetPopPump *)&event->data);
    }
}

void WobbuffetPop_QueuePopEvent(WobbuffetPop *game, u32 balloonIndex, u8 syncId) {
    WobbuffetPopEvent event;

    MI_CpuFill8(&event, 0, sizeof(event));
    event.kind = WOBBUFFET_POP_EVENT_POP;
    event.data.halves.h4 = balloonIndex;
    event.data.halves.b6 = WOBBUFFET_POP_SYNC_POP;
    event.data.halves.b7 = syncId;
    WobbuffetPop_QueueEvent(game, &event);
}

void WobbuffetPop_HandlePopEvent(WobbuffetPop *game, WobbuffetPopEvent *event) {
    game->total++;
    WobbuffetPop_PopBalloon(game);
    WobbuffetPop_RequestSync(game, event->data.halves.b6, event->data.halves.b7);
}

void WobbuffetPop_QueueNewBalloonEvent(WobbuffetPop *game, int balloonIndex, int size, u8 syncId) {
    WobbuffetPopEvent event;

    MI_CpuFill8(&event, 0, sizeof(event));
    event.kind = WOBBUFFET_POP_EVENT_NEW_BALLOON;
    event.data.halves.h4 = balloonIndex;
    event.data.halves.b6 = size;
    event.data.halves.b7 = WOBBUFFET_POP_SYNC_NEW_BALLOON;
    event.data.halves.b8 = syncId;
    WobbuffetPop_QueueEvent(game, &event);
}

void WobbuffetPop_HandleNewBalloonEvent(WobbuffetPop *game, WobbuffetPopEvent *event) {
    game->balloonIndex = event->data.halves.h4;
    WobbuffetPopInflation_Start(game->bgConfig, game->app->playerCount, event->data.halves.b6, &game->inflation);
    game->balloonInPlay = TRUE;
    WobbuffetPop_RequestSync(game, event->data.halves.b7, event->data.halves.b8);
}

void WobbuffetPop_QueueCountdownEvent(WobbuffetPop *game, u8 syncId) {
    WobbuffetPopEvent event;

    MI_CpuFill8(&event, 0, sizeof(event));
    event.kind = WOBBUFFET_POP_EVENT_COUNTDOWN;
    event.data.bytes.b4 = WOBBUFFET_POP_SYNC_COUNTDOWN;
    event.data.bytes.b5 = syncId;
    WobbuffetPop_QueueEvent(game, &event);
}

void WobbuffetPop_HandleCountdownEvent(WobbuffetPop *game, WobbuffetPopEvent *event) {
    game->gameState = WOBBUFFET_POP_GAME_START_COUNTDOWN;
    WobbuffetPop_RequestSync(game, event->data.bytes.b4, event->data.bytes.b5);
}

void WobbuffetPop_QueueFinishEvent(WobbuffetPop *game, u8 syncId) {
    WobbuffetPopEvent event;

    MI_CpuFill8(&event, 0, sizeof(event));
    event.kind = WOBBUFFET_POP_EVENT_FINISH;
    event.data.bytes.b4 = WOBBUFFET_POP_SYNC_FINISH;
    event.data.bytes.b5 = syncId;
    WobbuffetPop_QueueEvent(game, &event);
}

void WobbuffetPop_HandleFinishEvent(WobbuffetPop *game, WobbuffetPopEvent *event) {
    game->gameOver = TRUE;
    game->gameState = WOBBUFFET_POP_GAME_START_FINISH;
    WobbuffetPop_RequestSync(game, event->data.bytes.b4, event->data.bytes.b5);
}

void WobbuffetPop_QueueSyncEvent(WobbuffetPop *game, int syncKind, u8 syncId) {
    WobbuffetPopEvent event;

    MI_CpuFill8(&event, 0, sizeof(event));
    event.kind = WOBBUFFET_POP_EVENT_SYNC;
    event.data.bytes.b4 = syncKind;
    event.data.bytes.b5 = syncId;
    WobbuffetPop_QueueEvent(game, &event);
}

void WobbuffetPop_HandleSyncEvent(WobbuffetPop *game, WobbuffetPopEvent *event) {
    WobbuffetPop_RequestSync(game, event->data.bytes.b4, event->data.bytes.b5);
}

void WobbuffetPop_QueueDemoEvent(WobbuffetPop *game, u8 syncId) {
    WobbuffetPopEvent event;

    MI_CpuFill8(&event, 0, sizeof(event));
    event.kind = WOBBUFFET_POP_EVENT_DEMO;
    event.data.bytes.b4 = WOBBUFFET_POP_SYNC_DEMO;
    event.data.bytes.b5 = syncId;
    WobbuffetPop_QueueEvent(game, &event);
}

void WobbuffetPop_HandleDemoEvent(WobbuffetPop *game, WobbuffetPopEvent *event) {
    game->gameState = WOBBUFFET_POP_GAME_START_DEMO;
    WobbuffetPop_RequestSync(game, event->data.bytes.b4, event->data.bytes.b5);
}
