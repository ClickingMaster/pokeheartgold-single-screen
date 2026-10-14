#include "battle/battle_hp_bar.h"

#include "global.h"

#include "battle/battle_system.h"
#include "msgdata/msg/msg_0197.h"

#include "math_util.h"
#include "ui_util.h"

typedef enum HPBarType {
    HP_BAR_TYPE_SINGLE_PLAYER,
    HP_BAR_TYPE_SINGLE_ENEMY,
    HP_BAR_TYPE_DOUBLE_PLAYER_LHS,
    HP_BAR_TYPE_DOUBLE_ENEMY_LHS,
    HP_BAR_TYPE_DOUBLE_PLAYER_RHS,
    HP_BAR_TYPE_DOUBLE_ENEMY_RHS,
    HP_BAR_TYPE_SAFARI,
    HP_BAR_TYPE_PALPARK,
} HPBarType;

#ifdef FAST_HP_BARS
#define USE_SUBPIXELS_TEST (TRUE)
#else
#define USE_SUBPIXELS_TEST (maxHp < pixelsWide)
#endif // FAST_HP_BARS

ALIGN(4)
static const s8 sHpBarArrowXOffsets[] = {
    72,
    0,
    72,
    0,
    72,
    0,
};

typedef struct HpBarVramRegion {
    u16 offset;
    u16 size;
} HpBarVramRegion;

static const HpBarVramRegion sNicknameVramRegions[][4] = {
    {
     { 0x260, 0xA0 },
     { 0x360, 0xA0 },
     { 0xA00, 0x60 },
     { 0xB00, 0x60 },
     },
    {
     { 0x220, 0xE0 },
     { 0x320, 0xE0 },
     { 0xA00, 0x20 },
     { 0xB00, 0x20 },
     },
    {
     { 0x240, 0xC0 },
     { 0x340, 0xC0 },
     { 0xA00, 0x40 },
     { 0xB00, 0x40 },
     },
    {
     { 0x220, 0xE0 },
     { 0x320, 0xE0 },
     { 0xA00, 0x20 },
     { 0xB00, 0x20 },
     },
    {
     { 0x240, 0xC0 },
     { 0x340, 0xC0 },
     { 0xA00, 0x40 },
     { 0xB00, 0x40 },
     },
    {
     { 0x220, 0xE0 },
     { 0x320, 0xE0 },
     { 0xA00, 0x20 },
     { 0xB00, 0x20 },
     },
};

static const HpBarVramRegion sGenderIconVramRegions[][2] = {
    {
     { 0xA60, 0x40 },
     { 0xB60, 0x40 },
     },
    {
     { 0xA20, 0x40 },
     { 0xB20, 0x40 },
     },
    {
     { 0xA40, 0x40 },
     { 0xB40, 0x40 },
     },
    {
     { 0xA20, 0x40 },
     { 0xB20, 0x40 },
     },
    {
     { 0xA40, 0x40 },
     { 0xB40, 0x40 },
     },
    {
     { 0xA20, 0x40 },
     { 0xB20, 0x40 },
     },
};

static const HpBarVramRegion sLevelVramRegions[][2] = {
    {
     { 0xAA0, 0x60 },
     { 0xBA0, 0x60 },
     },
    {
     { 0xA60, 0x60 },
     { 0xB60, 0x60 },
     },
    {
     { 0xA80, 0x60 },
     { 0xB80, 0x60 },
     },
    {
     { 0xA60, 0x60 },
     { 0xB60, 0x60 },
     },
    {
     { 0xA80, 0x60 },
     { 0xB80, 0x60 },
     },
    {
     { 0xA60, 0x60 },
     { 0xB60, 0x60 },
     },
};

static const HpBarVramRegion sCurrentHpVramRegions[][2] = {
    {
     { 0, 0 },
     { 0xD00, 0x60 },
     },
    {
     { 0x620, 0x60 },
     { 0, 0 },
     },
    {
     { 0, 0 },
     { 0xC00, 0x60 },
     },
    {
     { 0x620, 0x60 },
     { 0, 0 },
     },
    {
     { 0, 0 },
     { 0xC00, 0x60 },
     },
    {
     { 0x620, 0x60 },
     { 0, 0 },
     },
};

static const HpBarVramRegion sMaxHpVramRegions[] = {
    { 0xD80, 0x60 },
    { 0x6A0, 0x60 },
    { 0xC80, 0x60 },
    { 0x6A0, 0x60 },
    { 0xC80, 0x60 },
    { 0x6A0, 0x60 },
};

static const HpBarVramRegion sHpGaugeVramRegions[][2] = {
    {
     { 0x4E0, 0 },
     { 0xC20, 0xC0 },
     },
    {
     { 0x4E0, 0x20 },
     { 0xC00, 0xA0 },
     },
    {
     { 0x4E0, 0 },
     { 0xC00, 0xC0 },
     },
    {
     { 0x4E0, 0x20 },
     { 0xC00, 0xA0 },
     },
    {
     { 0x4E0, 0 },
     { 0xC00, 0xC0 },
     },
    {
     { 0x4E0, 0x20 },
     { 0xC00, 0xA0 },
     },
};

static const HpBarVramRegion sCaughtIconVramRegions[] = {
    { 0x460, 0x20 },
    { 0x420, 0x20 },
    { 0x440, 0x20 },
    { 0x420, 0x20 },
    { 0x440, 0x20 },
    { 0x420, 0x20 },
};

static const HpBarVramRegion sStatusIconVramRegions[] = {
    { 0x480, 0x60 },
    { 0x440, 0x60 },
    { 0x460, 0x60 },
    { 0x440, 0x60 },
    { 0x460, 0x60 },
    { 0x440, 0x60 },
};

static const HpBarVramRegion sBallsLabelVramRegions[] = {
    { 0x240, 0xC0 },
    { 0x340, 0xC0 },
    { 0xA00, 0xE0 },
    { 0xB00, 0xE0 },
};

static const HpBarVramRegion sBallsLeftVramRegions[] = {
    { 0x440, 0xC0 },
    { 0x540, 0xC0 },
    { 0xC00, 0xE0 },
    { 0xD00, 0xE0 },
};

static const HpBarVramRegion ov12_0226D3A8[] = {
    { 0,     0    },
    { 0,     0    },
    { 0x4C0, 0x40 },
    { 0,     0    },
    { 0x4C0, 0x40 },
    { 0,     0    },
};

static const HpBarVramRegion ov12_0226D3D8[] = {
    { 0,     0    },
    { 0,     0    },
    { 0xCC0, 0x20 },
    { 0,     0    },
    { 0xCC0, 0x20 },
    { 0,     0    },
};

static const HpBarVramRegion ov12_0226D408[] = {
    { 0,     0    },
    { 0,     0    },
    { 0xC60, 0x20 },
    { 0,     0    },
    { 0xC60, 0x20 },
    { 0,     0    },
};

static void BattleHpBar_LoadBoxResources(SpriteSystem *spriteSystem, SpriteManager *spriteManager, NARC *narc, PaletteData *plttData, int barType);
static void BattleHpBar_LoadArrowResources(SpriteSystem *spriteSystem, SpriteManager *spriteManager, NARC *narc, PaletteData *plttData, int barType);
static ManagedSprite *BattleHpBar_CreateBoxSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int barType);
static void BattleHpBar_DeleteBoxSprite(BattleHpBar *hpBar);
static void BattleHpBar_DeleteArrowSprite(BattleHpBar *hpBar);
static void BattleHpBar_UnloadBoxResources(BattleHpBar *hpBar);
static void BattleHpBar_UnloadArrowResources(BattleHpBar *hpBar);
static void BattleHpBar_SetArrowDrawFlag(BattleHpBar *hpBar, int draw);
static void BattleHpBar_SetOffset(BattleHpBar *hpBar, int x, int y);
static void Task_BattleHpBarSlide(SysTask *task, void *data);
static void BattleHpBar_DrawNickname(BattleHpBar *hpBar);
static void BattleHpBar_DrawGenderIcon(BattleHpBar *hpBar);
static void BattleHpBar_DrawLevel(BattleHpBar *hpBar);
static void BattleHpBar_DrawCurrentHp(BattleHpBar *hpBar, u32 num);
static void BattleHpBar_DrawMaxHp(BattleHpBar *hpBar);
static void BattleHpBar_DrawCaughtIcon(BattleHpBar *hpBar);
static void BattleHpBar_DrawStatusIcon(BattleHpBar *hpBar, int componentId);
static void BattleHpBar_PrintSafariOrParkBallsString(BattleHpBar *hpBar, u32 flag);
static void BattleHpBar_PrintNumRemainingSafariOrParkBalls(BattleHpBar *hpBar, u32 flag);
static int BattleHpBar_StepGauge(BattleHpBar *hpBar, BOOL isExp);
static void BattleHpBar_DrawGauge(BattleHpBar *hpBar, u8 isExp);
static int BattleHpBar_CalculatePixelsChangeFrame(s32 maxHp, s32 curHp, s32 deltaHp, s32 *pHpCalc, u8 tilesWide, u16 hpChange);
static u8 BattleHpBar_Util_MakeHpBarPixelBuffer(s32 maxHp, s32 hp, s32 deltaHp, s32 *pHpCalc, u8 *pixelBuf, u8 tilesWide);
static u32 BattleHpBar_Util_GetPixelsToGain(s32 exp, s32 gainedExp, s32 maxExp, u8 tilesWide);
static const u8 *BattleHpBar_Util_GetComponentRawGraphic(int componentId);
static const ManagedSpriteTemplate *BattleHpBar_Util_GetHpBoxSpriteTemplate(u8 barType);
static const ManagedSpriteTemplate *BattleHpBar_Util_GetArrowSpriteTemplate(u8 barType);
static void Task_ExpBarFullFlash(SysTask *task, void *data);
static void BattleHpBar_StartBobTask(BattleHpBar *hpBar);
static void BattleHpBar_StopBobTask(BattleHpBar *hpBar);
static void Task_BattleHpBarBob(SysTask *task, void *data);

static const ManagedSpriteTemplate sSpriteTemplate_HpBarSinglePlayer = {
    .x = 0xC0,
    .y = 0x74,
    .z = 0,
    .animation = 0,
    .drawPriority = 0x17,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = {
                  0xBF,
                  20006,
                  0xBE,
                  0xBD,
                  -1,
                  -1,
                  },
    .bgPriority = 0,
    .vramTransfer = 0
};

static const ManagedSpriteTemplate sSpriteTemplate_HpBarSingleEnemy = {
    .x = 0x3A,
    .y = 0x24,
    .z = 0,
    .animation = 0,
    .drawPriority = 0x18,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = {
                  0xBC,
                  20006,
                  0xBB,
                  0xBA,
                  -1,
                  -1,
                  },
    .bgPriority = 0,
    .vramTransfer = 0
};

static const ManagedSpriteTemplate sSpriteTemplate_HpBarDoublePlayerLHS = {
    .x = 0xC0,
    .y = 0x67,
    .z = 0,
    .animation = 0,
    .drawPriority = 0x1C,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = {
                  0xC8,
                  20006,
                  0xC7,
                  0xC6,
                  -1,
                  -1,
                  },
    .bgPriority = 0,
    .vramTransfer = 0
};

static const ManagedSpriteTemplate sSpriteTemplate_HpBarDoubleEnemyLHS = {
    .x = 0x40,
    .y = 0x10,
    .z = 0,
    .animation = 0,
    .drawPriority = 0x19,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = {
                  0xC2,
                  20006,
                  0xC1,
                  0xC0,
                  -1,
                  -1,
                  },
    .bgPriority = 0,
    .vramTransfer = 0
};

static const ManagedSpriteTemplate sSpriteTemplate_HpBarDoublePlayerRHS = {
    .x = 0xC6,
    .y = 0x84,
    .z = 0,
    .animation = 0,
    .drawPriority = 0x1A,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = {
                  0xCB,
                  20006,
                  0xCA,
                  0xC9,
                  -1,
                  -1,
                  },
    .bgPriority = 0,
    .vramTransfer = 0
};

static const ManagedSpriteTemplate sSpriteTemplate_HpBarDoubleEnemyRHS = {
    .x = 0x3A,
    .y = 0x2D,
    .z = 0,
    .animation = 0,
    .drawPriority = 0x1B,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = {
                  0xC5,
                  20006,
                  0xC4,
                  0xC3,
                  -1,
                  -1,
                  },
    .bgPriority = 0,
    .vramTransfer = 0
};

static const ManagedSpriteTemplate sSpriteTemplate_Arrow = {
    .x = 0,
    .y = 0,
    .z = 0,
    .animation = 0,
    .drawPriority = 0x11,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = {
                  0xB9,
                  20006,
                  0xB8,
                  0xB7,
                  -1,
                  -1,
                  },
    .bgPriority = 0,
    .vramTransfer = 0
};

static const ManagedSpriteTemplate sSpriteTemplate_HpBarSafariOrPark = {
    .x = 0xC0,
    .y = 0x74,
    .z = 0,
    .animation = 0,
    .drawPriority = 0x17,
    .pal = 0,
    .vram = NNS_G2D_VRAM_TYPE_2DMAIN,
    .resIdList = {
                  0xFD,
                  20008,
                  0xFE,
                  0xFF,
                  -1,
                  -1,
                  },
    .bgPriority = 0,
    .vramTransfer = 0
};

#include "battle/battle_hp_bar_data.h"

static void BattleHpBar_LoadBoxResources(SpriteSystem *spriteSystem, SpriteManager *spriteManager, NARC *narc, PaletteData *plttData, int barType) {
    const ManagedSpriteTemplate *pRes = BattleHpBar_Util_GetHpBoxSpriteTemplate(barType);

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, pRes->resIdList[GF_GFX_RES_TYPE_CHAR], TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, pRes->resIdList[GF_GFX_RES_TYPE_CHAR]);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 71, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 20006);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, pRes->resIdList[GF_GFX_RES_TYPE_CELL], TRUE, pRes->resIdList[GF_GFX_RES_TYPE_CELL]);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, pRes->resIdList[GF_GFX_RES_TYPE_ANIM], TRUE, pRes->resIdList[GF_GFX_RES_TYPE_ANIM]);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 71, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 20007);
    if (barType == 6 || barType == 7) {
        SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 81, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 20008);
    }
}

static void BattleHpBar_LoadArrowResources(SpriteSystem *spriteSystem, SpriteManager *spriteManager, NARC *narc, PaletteData *plttData, int barType) {
    const ManagedSpriteTemplate *pRes = BattleHpBar_Util_GetArrowSpriteTemplate(barType);

    if (pRes != NULL) {
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, pRes->resIdList[GF_GFX_RES_TYPE_CHAR], TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, pRes->resIdList[GF_GFX_RES_TYPE_CHAR]);
        SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, 71, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 20006);
        SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, pRes->resIdList[GF_GFX_RES_TYPE_CELL], TRUE, pRes->resIdList[GF_GFX_RES_TYPE_CELL]);
        SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, pRes->resIdList[GF_GFX_RES_TYPE_ANIM], TRUE, pRes->resIdList[GF_GFX_RES_TYPE_ANIM]);
    }
}

static ManagedSprite *BattleHpBar_CreateBoxSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, int barType) {
    const ManagedSpriteTemplate *pRes = BattleHpBar_Util_GetHpBoxSpriteTemplate(barType);

    ManagedSprite *ret = SpriteSystem_NewSprite(spriteSystem, spriteManager, pRes);
    Sprite_TickFrame(ret->sprite);
    return ret;
}

void BattleHpBar_Draw(BattleHpBar *hpBar, u32 num, u32 flag) {
    GF_ASSERT(hpBar->boxObj != NULL);
    if (hpBar->type == HP_BAR_TYPE_SAFARI) {
        flag &= 0xC00;
    } else if (hpBar->type == HP_BAR_TYPE_PALPARK) {
        flag &= 0x3000;
    } else {
        flag &= ~0x3C00;
    }
    switch (hpBar->type) {
    case HP_BAR_TYPE_SINGLE_ENEMY:
    case HP_BAR_TYPE_DOUBLE_ENEMY_LHS:
    case HP_BAR_TYPE_DOUBLE_ENEMY_RHS:
        flag &= ~0x26;
        break;
    case HP_BAR_TYPE_DOUBLE_PLAYER_LHS:
    case HP_BAR_TYPE_DOUBLE_PLAYER_RHS:
        flag &= ~0x220;
        if (hpBar->showHpNumbers == 0) {
            flag &= ~6;
        } else {
            flag &= ~1;
        }
        break;
    case HP_BAR_TYPE_SINGLE_PLAYER:
        flag &= ~0x200;
        break;
    case HP_BAR_TYPE_SAFARI:
    case HP_BAR_TYPE_PALPARK:
        break;
    }
    if (BattleSystem_GetBattleType(hpBar->battleSystem) & BATTLE_TYPE_TRAINER) {
        flag &= ~0x200;
    }

    if (flag & 1) {
        BattleHpBar_InitHpChange(hpBar, 0);
        BattleHpBar_StepGauge(hpBar, FALSE);
    }

    if (flag & 2) {
        BattleHpBar_DrawCurrentHp(hpBar, num);
    }

    if (flag & 4) {
        BattleHpBar_DrawMaxHp(hpBar);
    }

    if (flag & 0x80 || flag & 0x40) {
        BattleHpBar_DrawGenderIcon(hpBar);
    }

    if (flag & 8) {
        BattleHpBar_DrawLevel(hpBar);
    }

    if (flag & 0x10) {
        BattleHpBar_DrawNickname(hpBar);
    }

    if (flag & 0x20) {
        BattleHpBar_InitExpChange(hpBar, 0);
        BattleHpBar_StepGauge(hpBar, TRUE);
    }

    if (flag & 0x200) {
        BattleHpBar_DrawCaughtIcon(hpBar);
    }

    if (flag & 0x100) {
        switch (hpBar->status) {
        case 0:
        default:
            BattleHpBar_DrawStatusIcon(hpBar, 38);
            break;
        case 1:
            BattleHpBar_DrawStatusIcon(hpBar, 47);
            break;
        case 2:
            BattleHpBar_DrawStatusIcon(hpBar, 50);
            break;
        case 3:
            BattleHpBar_DrawStatusIcon(hpBar, 53);
            break;
        case 4:
            BattleHpBar_DrawStatusIcon(hpBar, 44);
            break;
        case 5:
            BattleHpBar_DrawStatusIcon(hpBar, 41);
            break;
        }
    }

    if (flag & 0x1400) {
        BattleHpBar_PrintSafariOrParkBallsString(hpBar, flag);
    }

    if (flag & 0x2800) {
        BattleHpBar_PrintNumRemainingSafariOrParkBalls(hpBar, flag);
    }
}

static void BattleHpBar_DeleteBoxSprite(BattleHpBar *hpBar) {
    if (hpBar->sysTask != NULL) {
        SysTask_Destroy(hpBar->sysTask);
        hpBar->sysTask = NULL;
    }
    if (hpBar->boxObj != NULL) {
        Sprite_DeleteAndFreeResources(hpBar->boxObj);
        hpBar->boxObj = NULL;
    }
}

static void BattleHpBar_DeleteArrowSprite(BattleHpBar *hpBar) {
    if (hpBar->arrowObj != NULL) {
        Sprite_DeleteAndFreeResources(hpBar->arrowObj);
        hpBar->arrowObj = NULL;
    }
}

static void BattleHpBar_UnloadBoxResources(BattleHpBar *hpBar) {
    const ManagedSpriteTemplate *tmplate = BattleHpBar_Util_GetHpBoxSpriteTemplate(hpBar->type);
    SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(hpBar->battleSystem);
    SpriteManager *spriteManager = BattleSystem_GetSpriteManager(hpBar->battleSystem);
    SpriteManager_UnloadCharObjById(spriteManager, tmplate->resIdList[GF_GFX_RES_TYPE_CHAR]);
    SpriteManager_UnloadCellObjById(spriteManager, tmplate->resIdList[GF_GFX_RES_TYPE_CELL]);
    SpriteManager_UnloadAnimObjById(spriteManager, tmplate->resIdList[GF_GFX_RES_TYPE_ANIM]);
}

static void BattleHpBar_UnloadArrowResources(BattleHpBar *hpBar) {
    const ManagedSpriteTemplate *tmplate = BattleHpBar_Util_GetArrowSpriteTemplate(hpBar->type);
    if (tmplate != NULL) {
        SpriteSystem *spriteSystem = BattleSystem_GetSpriteSystem(hpBar->battleSystem);
        SpriteManager *spriteManager = BattleSystem_GetSpriteManager(hpBar->battleSystem);
        SpriteManager_UnloadCharObjById(spriteManager, tmplate->resIdList[GF_GFX_RES_TYPE_CHAR]);
        SpriteManager_UnloadCellObjById(spriteManager, tmplate->resIdList[GF_GFX_RES_TYPE_CELL]);
        SpriteManager_UnloadAnimObjById(spriteManager, tmplate->resIdList[GF_GFX_RES_TYPE_ANIM]);
    }
}

void BattleHpBar_LoadResources(BattleHpBar *hpBar) {
    const ManagedSpriteTemplate *tmplate;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    PaletteData *plttData;
    NARC *narc;

    narc = NARC_New(NARC_a_0_0_8, HEAP_ID_BATTLE);

    spriteSystem = BattleSystem_GetSpriteSystem(hpBar->battleSystem);
    spriteManager = BattleSystem_GetSpriteManager(hpBar->battleSystem);
    plttData = BattleSystem_GetPaletteData(hpBar->battleSystem);

    tmplate = BattleHpBar_Util_GetHpBoxSpriteTemplate(hpBar->type);

    BattleHpBar_LoadBoxResources(spriteSystem, spriteManager, narc, plttData, hpBar->type);
    hpBar->boxObj = BattleHpBar_CreateBoxSprite(spriteSystem, spriteManager, hpBar->type);

    BattleHpBar_LoadArrowResources(spriteSystem, spriteManager, narc, plttData, hpBar->type);
    if (hpBar->arrowObj) {
        Sprite_SetPositionXY(hpBar->arrowObj->sprite, tmplate->x - sHpBarArrowXOffsets[hpBar->type], tmplate->y + 0);
    }
    NARC_Delete(narc);
}


void BattleHpBar_FreeResources(BattleHpBar *hpBar) {
    BattleHpBar_DeleteBoxSprite(hpBar);
    BattleHpBar_UnloadBoxResources(hpBar);
    BattleHpBar_DeleteArrowSprite(hpBar);
    BattleHpBar_UnloadArrowResources(hpBar);
}

void BattleHpBar_ToggleHpNumbers(BattleHpBar *hpBar) {
    const u8 *src;
    void *vramBaseAddr;
    NNSG2dImageProxy *imgProxy;

    switch (hpBar->type) {
    case HP_BAR_TYPE_DOUBLE_PLAYER_LHS:
    case HP_BAR_TYPE_DOUBLE_PLAYER_RHS:
        hpBar->showHpNumbers ^= 1;
        vramBaseAddr = G2_GetOBJCharPtr();
        imgProxy = Sprite_GetImageProxy(hpBar->boxObj->sprite);
        if (hpBar->showHpNumbers == 1) {
            src = BattleHpBar_Util_GetComponentRawGraphic(70);
            MI_CpuCopy16(src, (void *)((u32)vramBaseAddr + ov12_0226D3A8[hpBar->type].offset + 0x20 + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), 0x20);
            src = BattleHpBar_Util_GetComponentRawGraphic(71);
            MI_CpuCopy16(src, (void *)((u32)vramBaseAddr + ov12_0226D3D8[hpBar->type].offset + 0x20 + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), 0x20);
            src = BattleHpBar_Util_GetComponentRawGraphic(69);
            MI_CpuCopy16(src, (void *)((u32)vramBaseAddr + ov12_0226D408[hpBar->type].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), ov12_0226D408[hpBar->type].size);
            BattleHpBar_Draw(hpBar, hpBar->hp, 6);
        } else {
            src = BattleHpBar_Util_GetComponentRawGraphic(66);
            MI_CpuCopy16(src, (void *)((u32)vramBaseAddr + ov12_0226D3A8[hpBar->type].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), ov12_0226D3A8[hpBar->type].size);
            src = BattleHpBar_Util_GetComponentRawGraphic(68);
            MI_CpuCopy16(src, (void *)((u32)vramBaseAddr + ov12_0226D3D8[hpBar->type].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), ov12_0226D3D8[hpBar->type].size);
            src = BattleHpBar_Util_GetComponentRawGraphic(38);
            MI_CpuCopy16(src, (void *)((u32)vramBaseAddr + ov12_0226D3D8[hpBar->type].offset + 0x20 + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), 0x20);
            BattleHpBar_Draw(hpBar, hpBar->hp, 1);
        }
    }
}

void BattleHpBar_InitHpChange(BattleHpBar *hpBar, int hp) {
    hpBar->hpCalc = 0x80000000;
    if (hpBar->hp + hp < 0) {
        hp -= (hpBar->hp + hp);
    }
    if (hpBar->hp + hp > hpBar->maxHp) {
        hp -= ((hpBar->hp + hp) - hpBar->maxHp);
    }
    hpBar->gainedHp = -hp;
    if (hpBar->hp < 0) {
        hpBar->hp = 0;
    }
    if (hpBar->hp > hpBar->maxHp) {
        hpBar->hp = hpBar->maxHp;
    }
}

int BattleHpBar_UpdateHpChange(BattleHpBar *hpBar) {
    int r4 = BattleHpBar_StepGauge(hpBar, FALSE);
    if (r4 == -1) {
        hpBar->hp -= hpBar->gainedHp;
        BattleHpBar_Draw(hpBar, hpBar->hp, 2);
    } else {
        BattleHpBar_Draw(hpBar, r4, 2);
    }
    return r4;
}

void BattleHpBar_InitExpChange(BattleHpBar *hpBar, int exp) {
    hpBar->expCalc = 0x80000000;
    if (hpBar->exp + exp < 0) {
        exp -= (hpBar->exp + exp);
    }
    if (hpBar->exp + exp > hpBar->maxExp) {
        exp -= ((hpBar->exp + exp) - hpBar->maxExp);
    }
    hpBar->gainedExp = -exp;
    if (hpBar->exp < 0) {
        hpBar->exp = 0;
    }
    if (hpBar->exp > hpBar->maxExp) {
        hpBar->exp = hpBar->maxExp;
    }
}

int BattleHpBar_UpdateExpChange(BattleHpBar *hpBar) {
    int ret = BattleHpBar_StepGauge(hpBar, TRUE);
    if (ret == -1) {
        hpBar->exp -= hpBar->gainedExp;
    }
    return ret;
}

void BattleHpBar_StartActiveEffect(BattleHpBar *hpBar) {
    if (hpBar->arrowObj != NULL) {
        Sprite_SetAnimActiveFlag(hpBar->arrowObj->sprite, TRUE);
        BattleHpBar_SetArrowDrawFlag(hpBar, 1);
    }
    if (!(BattleSystem_GetBattleType(hpBar->battleSystem) & (BATTLE_TYPE_PAL_PARK | BATTLE_TYPE_SAFARI))) {
        BattleHpBar_StartBobTask(hpBar);
    }
}

void BattleHpBar_StopActiveEffect(BattleHpBar *hpBar) {
    if (hpBar->arrowObj != NULL) {
        Sprite_SetAnimActiveFlag(hpBar->arrowObj->sprite, FALSE);
        Sprite_SetAnimationFrame(hpBar->arrowObj->sprite, 0);
        BattleHpBar_SetArrowDrawFlag(hpBar, 0);
    }
    BattleHpBar_StopBobTask(hpBar);
}

void BattleHpBar_SetPriority(BattleHpBar *hpBar, int prio) {
    if (hpBar->boxObj != NULL) {
        ManagedSprite_SetPriority(hpBar->boxObj, prio);
        if (hpBar->arrowObj != NULL) {
            ManagedSprite_SetPriority(hpBar->arrowObj, prio);
        }
    }
}

static void BattleHpBar_SetArrowDrawFlag(BattleHpBar *hpBar, int draw) {
    if (hpBar->arrowObj != NULL) {
        if (!(BattleSystem_GetBattleType(hpBar->battleSystem) & (BATTLE_TYPE_PAL_PARK | BATTLE_TYPE_SAFARI)) || draw != TRUE) {
            ManagedSprite_SetDrawFlag(hpBar->arrowObj, draw);
        }
    }
}

void BattleHpBar_SetEnabled(BattleHpBar *hpBar, BOOL enabled) {
    if (hpBar->boxObj != NULL) {
        ManagedSprite_SetDrawFlag(hpBar->boxObj, enabled);
        BattleHpBar_SetArrowDrawFlag(hpBar, enabled);
    }
}

static void BattleHpBar_SetOffset(BattleHpBar *hpBar, int x, int y) {
    const ManagedSpriteTemplate *tmplate;

    GF_ASSERT(hpBar->boxObj);

    tmplate = BattleHpBar_Util_GetHpBoxSpriteTemplate(hpBar->type);

    Sprite_SetPositionXY(hpBar->boxObj->sprite, (s16)((u32)(s32)tmplate->x + (u32)x), (s16)((u32)(s32)tmplate->y + (u32)y));
    if (hpBar->arrowObj) {
        Sprite_SetPositionXY(hpBar->arrowObj->sprite,
            (s16)((u32)(s32)tmplate->x + (u32)x - (u32)(s32)sHpBarArrowXOffsets[hpBar->type]),
            (s16)((u32)(s32)tmplate->y + (u32)y));
    }
}


void BattleHpBar_StartSlide(BattleHpBar *hpBar, BOOL slideOut) {
    GF_ASSERT(hpBar != NULL);
    GF_ASSERT(hpBar->boxObj != NULL);
    hpBar->slideDone = FALSE;
    hpBar->slideOut = slideOut;
    if (!slideOut) {
        switch (hpBar->type) {
        case HP_BAR_TYPE_SINGLE_PLAYER:
        case HP_BAR_TYPE_DOUBLE_PLAYER_LHS:
        case HP_BAR_TYPE_DOUBLE_PLAYER_RHS:
        case HP_BAR_TYPE_SAFARI:
        case HP_BAR_TYPE_PALPARK:
            BattleHpBar_SetOffset(hpBar, 160, 0);
            break;
        default:
            BattleHpBar_SetOffset(hpBar, -160, 0);
            break;
        }
    } else {
        BattleHpBar_SetOffset(hpBar, 0, 0);
    }
    SysTask_CreateOnMainQueue(Task_BattleHpBarSlide, hpBar, 990);
}

static void Task_BattleHpBarSlide(SysTask *task, void *data) {
    BattleHpBar *hpBar = data;
    s16 x, y;
    const ManagedSpriteTemplate *r6;
    int r4;

    r4 = 0;
    r6 = BattleHpBar_Util_GetHpBoxSpriteTemplate(hpBar->type);
    ManagedSprite_GetPositionXY(hpBar->boxObj, &x, &y);

    switch (hpBar->type) {
    case HP_BAR_TYPE_SINGLE_PLAYER:
    case HP_BAR_TYPE_DOUBLE_PLAYER_LHS:
    case HP_BAR_TYPE_DOUBLE_PLAYER_RHS:
    case HP_BAR_TYPE_SAFARI:
    case HP_BAR_TYPE_PALPARK:
        if (hpBar->slideOut == 0) {
            x -= 24;
            if (x < r6->x) {
                x = r6->x;
                r4++;
            }
        } else {
            x += 24;
            if (x > r6->x + 160) {
                x = r6->x + 160;
                r4++;
            }
        }
        break;
    default:
        if (hpBar->slideOut == 0) {
            x += 24;
            if (x > r6->x) {
                x = r6->x;
                r4++;
            }
        } else {
            x -= 24;
            if (x < r6->x - 24) {
                x = r6->x - 24;
                r4++;
            }
        }
        break;
    }
    ManagedSprite_SetPositionXY(hpBar->boxObj, x, y);
    if (hpBar->arrowObj) {
        ManagedSprite_SetPositionXY(hpBar->arrowObj, x - sHpBarArrowXOffsets[hpBar->type], y + 0);
    }

    if (r4 > 0) {
        hpBar->slideDone = TRUE;
        SysTask_Destroy(task);
        return;
    }
}


static void BattleHpBar_DrawNickname(BattleHpBar *hpBar) {
    BgConfig *bgConfig;
    u8 *srcBuf;
    NNSG2dImageProxy *imgProxy;
    Window window;
    MsgData *msgData;
    String *string;
    String *string2;
    Pokemon *mon;
    BoxPokemon *boxMon;
    MessageFormat *msgFormat;

    bgConfig = BattleSystem_GetBgConfig(hpBar->battleSystem);
    msgData = BattleSystem_GetMessageLoader(hpBar->battleSystem);
    msgFormat = BattleSystem_GetMessageFormat(hpBar->battleSystem);
    string = String_New(22, HEAP_ID_BATTLE);
    string2 = NewString_ReadMsgData(msgData, msg_0197_00964);

    mon = BattleSystem_GetPartyMon(hpBar->battleSystem, hpBar->battlerId, hpBar->monId);
    boxMon = Mon_GetBoxMon(mon);
    BufferBoxMonNickname(msgFormat, 0, boxMon);
    StringExpandPlaceholders(msgFormat, string, string2);

    AddTextWindowTopLeftCorner(bgConfig, &window, 8, 2, 0, 15);
    AddTextPrinterParameterizedWithColorAndSpacing(&window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 2, 15), 0, 0, NULL);
    srcBuf = window.pixelBuffer;

    {
        void *vramAddr;
        u8 *pixelBuffer;
        u8 *pixelBuffer2;

        vramAddr = G2_GetOBJCharPtr();
        imgProxy = Sprite_GetImageProxy(hpBar->boxObj->sprite);
        pixelBuffer = srcBuf;
        pixelBuffer2 = srcBuf + 0x100;

        MI_CpuCopy16(pixelBuffer, (void *)((u32)vramAddr + sNicknameVramRegions[hpBar->type][0].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sNicknameVramRegions[hpBar->type][0].size);
        MI_CpuCopy16(pixelBuffer2, (void *)((u32)vramAddr + sNicknameVramRegions[hpBar->type][1].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sNicknameVramRegions[hpBar->type][1].size);
        MI_CpuCopy16(pixelBuffer + sNicknameVramRegions[hpBar->type][0].size, (void *)((u32)vramAddr + sNicknameVramRegions[hpBar->type][2].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sNicknameVramRegions[hpBar->type][2].size);
        MI_CpuCopy16(pixelBuffer2 + sNicknameVramRegions[hpBar->type][1].size, (void *)((u32)vramAddr + sNicknameVramRegions[hpBar->type][3].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sNicknameVramRegions[hpBar->type][3].size);
    }
    RemoveWindow(&window);
    String_Delete(string);
    String_Delete(string2);
}

static void BattleHpBar_DrawGenderIcon(BattleHpBar *hpBar) {
    int r0, r4;

    if (hpBar->gender == 0) {
        r0 = 74;
        r4 = 62;
    } else if (hpBar->gender == 1) {
        r0 = 72;
        r4 = 60;
    } else {
        r0 = 76;
        r4 = 64;
    }
    const u8 *sp0 = BattleHpBar_Util_GetComponentRawGraphic(r0);
    const u8 *r7 = BattleHpBar_Util_GetComponentRawGraphic(r4);
    NNSG2dImageProxy *imgProxy = Sprite_GetImageProxy(hpBar->boxObj->sprite);
    void *vramAddr = G2_GetOBJCharPtr();
    MI_CpuCopy16(r7, (void *)((u32)vramAddr + sGenderIconVramRegions[hpBar->type][0].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sGenderIconVramRegions[hpBar->type][0].size);
    MI_CpuCopy16(sp0, (void *)((u32)vramAddr + sGenderIconVramRegions[hpBar->type][1].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sGenderIconVramRegions[hpBar->type][1].size);
}

static void BattleHpBar_DrawLevel(BattleHpBar *hpBar) {
    int j, i, k;
    u8 *r4 = Heap_Alloc(HEAP_ID_BATTLE, 0x60);
    u8 *r7 = Heap_Alloc(HEAP_ID_BATTLE, 0xC0);
    MI_CpuFill8(r4, 0xFF, 0x60);
    BattleNumberPrinter_PrintUInt(BattleSystem_GetLevelNumPrinter(hpBar->battleSystem), hpBar->level, 3, PRINTING_MODE_LEFT_ALIGN, (void *)r4);
    NNSG2dImageProxy *imgProxy = Sprite_GetImageProxy(hpBar->boxObj->sprite);
    void *vramAddr = G2_GetOBJCharPtr();
    MI_CpuCopy16((void *)((u32)vramAddr + sLevelVramRegions[hpBar->type][0].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), r7, sLevelVramRegions[hpBar->type][0].size);
    MI_CpuCopy16((void *)((u32)vramAddr + sLevelVramRegions[hpBar->type][1].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), r7 + 0x60, sLevelVramRegions[hpBar->type][1].size);
    k = 0;
    for (i = 0; i < 0x60; i += 0x20) {
        for (j = 0; j < 0x10; ++j) {
            r7[i + 0x10 + j] = r4[k];
            r7[i + j + 0x60] = r4[k + 0x10];
            ++k;
        }
        k += 0x10;
    }
    u8 *buf1 = r7;
    u8 *buf2 = r7 + 0x60;
    MI_CpuCopy16(buf1, (void *)((u32)vramAddr + sLevelVramRegions[hpBar->type][0].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sLevelVramRegions[hpBar->type][0].size);
    MI_CpuCopy16(buf2, (void *)((u32)vramAddr + sLevelVramRegions[hpBar->type][1].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sLevelVramRegions[hpBar->type][1].size);
    Heap_Free(r4);
    Heap_Free(r7);
}

static void BattleHpBar_DrawCurrentHp(BattleHpBar *hpBar, u32 num) {
    u8 *r4 = Heap_Alloc(HEAP_ID_BATTLE, 0x60);
    MI_CpuFill8(r4, 0xFF, 0x60);
    BattleNumberPrinter_PrintUInt(BattleSystem_GetHpNumPrinter(hpBar->battleSystem), num, 3, PRINTING_MODE_RIGHT_ALIGN, (void *)r4);
    NNSG2dImageProxy *imgProxy = Sprite_GetImageProxy(hpBar->boxObj->sprite);
    void *vramAddr = G2_GetOBJCharPtr();

    MI_CpuCopy16(r4, (void *)((u32)vramAddr + sCurrentHpVramRegions[hpBar->type][0].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sCurrentHpVramRegions[hpBar->type][0].size);
    MI_CpuCopy16(r4 + sCurrentHpVramRegions[hpBar->type][0].size, (void *)((u32)vramAddr + sCurrentHpVramRegions[hpBar->type][1].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sCurrentHpVramRegions[hpBar->type][1].size);
    Heap_Free(r4);
}

static void BattleHpBar_DrawMaxHp(BattleHpBar *hpBar) {
    u8 *r4 = Heap_Alloc(HEAP_ID_BATTLE, 0x60);
    MI_CpuFill8(r4, 0xFF, 0x60);
    BattleNumberPrinter_PrintUInt(BattleSystem_GetHpNumPrinter(hpBar->battleSystem), hpBar->maxHp, 3, PRINTING_MODE_LEFT_ALIGN, (void *)r4);
    NNSG2dImageProxy *imgProxy = Sprite_GetImageProxy(hpBar->boxObj->sprite);
    void *vramAddr = G2_GetOBJCharPtr();

    MI_CpuCopy16(r4, (void *)((u32)vramAddr + sMaxHpVramRegions[hpBar->type].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sMaxHpVramRegions[hpBar->type].size);
    Heap_Free(r4);
}

static void BattleHpBar_DrawCaughtIcon(BattleHpBar *hpBar) {
    const u8 *r4;
    if (hpBar->caught == 1) {
        r4 = BattleHpBar_Util_GetComponentRawGraphic(59);
    } else {
        r4 = BattleHpBar_Util_GetComponentRawGraphic(38);
    }
    NNSG2dImageProxy *imgProxy = Sprite_GetImageProxy(hpBar->boxObj->sprite);
    void *vramAddr = G2_GetOBJCharPtr();

    MI_CpuCopy16(r4, (void *)((u32)vramAddr + sCaughtIconVramRegions[hpBar->type].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sCaughtIconVramRegions[hpBar->type].size);
}

static void BattleHpBar_DrawStatusIcon(BattleHpBar *hpBar, int componentId) {
    const u8 *r4 = BattleHpBar_Util_GetComponentRawGraphic(componentId);
    NNSG2dImageProxy *imgProxy = Sprite_GetImageProxy(hpBar->boxObj->sprite);
    void *vramAddr = G2_GetOBJCharPtr();

    MI_CpuCopy16(r4, (void *)((u32)vramAddr + sStatusIconVramRegions[hpBar->type].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sStatusIconVramRegions[hpBar->type].size);
}

static void BattleHpBar_PrintSafariOrParkBallsString(BattleHpBar *hpBar, u32 flag) {
    BgConfig *bgConfig;
    u8 *windowBuf;
    NNSG2dImageProxy *imgProxy;
    Window window;
    MsgData *msgData;
    String *string;

    bgConfig = BattleSystem_GetBgConfig(hpBar->battleSystem);
    msgData = BattleSystem_GetMessageLoader(hpBar->battleSystem);

    if (flag & 0x400) {
        string = NewString_ReadMsgData(msgData, msg_0197_00950); // SAFARI BALLS
    } else {
        string = NewString_ReadMsgData(msgData, msg_0197_01220); // PARK BALLS
    }
    AddTextWindowTopLeftCorner(bgConfig, &window, 13, 2, 0, 15);
    AddTextPrinterParameterizedWithColorAndSpacing(&window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 2, 15), 0, 0, NULL);
    windowBuf = window.pixelBuffer;

    {
        void *vramAddr;
        u8 *ptr1;
        u8 *ptr2;

        vramAddr = G2_GetOBJCharPtr();
        imgProxy = Sprite_GetImageProxy(hpBar->boxObj->sprite);
        ptr1 = windowBuf;
        ptr2 = windowBuf + 0x1A0;
        MI_CpuCopy16(ptr1, (void *)((u32)vramAddr + sBallsLabelVramRegions[0].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sBallsLabelVramRegions[0].size);
        MI_CpuCopy16(ptr2, (void *)((u32)vramAddr + sBallsLabelVramRegions[1].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sBallsLabelVramRegions[1].size);
        MI_CpuCopy16(ptr1 + sBallsLabelVramRegions[0].size, (void *)((u32)vramAddr + sBallsLabelVramRegions[2].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sBallsLabelVramRegions[2].size);
        MI_CpuCopy16(ptr2 + sBallsLabelVramRegions[1].size, (void *)((u32)vramAddr + sBallsLabelVramRegions[3].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sBallsLabelVramRegions[3].size);
    }

    RemoveWindow(&window);
    String_Delete(string);
}

static void BattleHpBar_PrintNumRemainingSafariOrParkBalls(BattleHpBar *hpBar, u32 flag) {
    BgConfig *bgConfig;
    u8 *windowBuf;
    NNSG2dImageProxy *imgProxy;
    Window window;
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *string;
    String *string2;

    bgConfig = BattleSystem_GetBgConfig(hpBar->battleSystem);
    msgData = BattleSystem_GetMessageLoader(hpBar->battleSystem);
    msgFormat = BattleSystem_GetMessageFormat(hpBar->battleSystem);

    string = String_New(30, HEAP_ID_BATTLE);
    if (flag & 0x400) {
        string2 = NewString_ReadMsgData(msgData, msg_0197_00951); // Left: $1
    } else {
        string2 = NewString_ReadMsgData(msgData, msg_0197_01221); // Left: $1
    }
    BufferIntegerAsString(msgFormat, 0, hpBar->safariBallCount, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    StringExpandPlaceholders(msgFormat, string, string2);
    AddTextWindowTopLeftCorner(bgConfig, &window, 13, 2, 0, 15);
    AddTextPrinterParameterizedWithColorAndSpacing(&window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 2, 15), 0, 0, NULL);
    windowBuf = window.pixelBuffer;

    {
        void *vramAddr;
        u8 *ptr1;
        u8 *ptr2;

        vramAddr = G2_GetOBJCharPtr();
        imgProxy = Sprite_GetImageProxy(hpBar->boxObj->sprite);
        ptr1 = windowBuf;
        ptr2 = windowBuf + 0x1A0;
        MI_CpuCopy16(ptr1, (void *)((u32)vramAddr + sBallsLeftVramRegions[0].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sBallsLeftVramRegions[0].size);
        MI_CpuCopy16(ptr2, (void *)((u32)vramAddr + sBallsLeftVramRegions[1].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sBallsLeftVramRegions[1].size);
        MI_CpuCopy16(ptr1 + sBallsLeftVramRegions[0].size, (void *)((u32)vramAddr + sBallsLeftVramRegions[2].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sBallsLeftVramRegions[2].size);
        MI_CpuCopy16(ptr2 + sBallsLeftVramRegions[1].size, (void *)((u32)vramAddr + sBallsLeftVramRegions[3].offset + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), sBallsLeftVramRegions[3].size);
    }

    RemoveWindow(&window);
    String_Delete(string);
    String_Delete(string2);
}

static int BattleHpBar_StepGauge(BattleHpBar *hpBar, BOOL isExp) {
    int ret;
    if (isExp == FALSE) {
        ret = BattleHpBar_CalculatePixelsChangeFrame(hpBar->maxHp, hpBar->hp, hpBar->gainedHp, &hpBar->hpCalc, 6, 1);
    } else {
        // Supposedly this will make the exp bar move at a consistent speed regardless of the gauge size.
        int denom = BattleHpBar_Util_GetPixelsToGain(hpBar->exp, hpBar->gainedExp, hpBar->maxExp, 12);
        if (denom == 0) {
            denom = 1;
        }
        ret = BattleHpBar_CalculatePixelsChangeFrame(hpBar->maxExp, hpBar->exp, hpBar->gainedExp, &hpBar->expCalc, 12, abs(hpBar->gainedExp / denom));
    }
    if (isExp != FALSE || hpBar->showHpNumbers != TRUE) {
        BattleHpBar_DrawGauge(hpBar, isExp);
    }
    if (ret == -1) {
        // we done
        if (isExp == FALSE) {
            hpBar->hpCalc = 0;
        } else {
            hpBar->expCalc = 0;
        }
    }
    return ret;
}

static void BattleHpBar_DrawGauge(BattleHpBar *hpBar, u8 isExp) {
    u8 i;
    u8 pixelBuffer[12];
    u8 tmp;
    const u8 *src;
    void *vramAddr;
    NNSG2dImageProxy *imgProxy;
    int sizeTop;

    vramAddr = G2_GetOBJCharPtr();
    imgProxy = Sprite_GetImageProxy(hpBar->boxObj->sprite);
    switch (isExp) {
    case FALSE:
        // hp
        switch (HpBar_GetColorIdx(BattleHpBar_Util_MakeHpBarPixelBuffer(hpBar->maxHp, hpBar->hp, hpBar->gainedHp, &hpBar->hpCalc, pixelBuffer, 6), 0x30)) {
        case 3: // Green
            tmp = 2;
            break;
        case 2: // Yellow
            tmp = 11;
            break;
        case 1: // Red
        default:
            tmp = 20;
            break;
        }
        src = BattleHpBar_Util_GetComponentRawGraphic(tmp);
        sizeTop = sHpGaugeVramRegions[hpBar->type][0].size / 32;
        for (i = 0; i < 6; ++i) {
            if (i < sizeTop) {
                MI_CpuCopy16(src + pixelBuffer[i] * 32, (void *)((u32)vramAddr + sHpGaugeVramRegions[hpBar->type][0].offset + i * 32 + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), 0x20);
            } else {
                MI_CpuCopy16(src + pixelBuffer[i] * 32, (void *)((u32)vramAddr + sHpGaugeVramRegions[hpBar->type][1].offset + (i - sizeTop) * 32 + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), 0x20);
            }
        }
        break;
    case TRUE:
        // exp
        BattleHpBar_Util_MakeHpBarPixelBuffer(hpBar->maxExp, hpBar->exp, hpBar->gainedExp, &hpBar->expCalc, pixelBuffer, 12);
        if (hpBar->level == 100) {
            // Don't fill an exp bar for a level 100 mon
            for (i = 0; i < 12; ++i) {
                pixelBuffer[i] = 0;
            }
        }
        src = BattleHpBar_Util_GetComponentRawGraphic(29);
        for (i = 0; i < 12; ++i) {
            if (i < 5) {
                MI_CpuCopy16(src + pixelBuffer[i] * 32, (void *)((u32)vramAddr + 0x660 + i * 32 + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), 0x20);
            } else {
                MI_CpuCopy16(src + pixelBuffer[i] * 32, (void *)((u32)vramAddr + 0xE00 + (i - 5) * 32 + imgProxy->vramLocation.baseAddrOfVram[NNS_G2D_VRAM_TYPE_2DMAIN]), 0x20);
            }
        }
        break;
    }
}

static int BattleHpBar_CalculatePixelsChangeFrame(s32 maxHp, s32 curHp, s32 deltaHp, s32 *pHpCalc, u8 tilesWide, u16 hpChange) {
    s32 nowHp;
    s32 ret;
    u8 pixelsWide;
    s32 hpPerPixel;

    pixelsWide = tilesWide * 8;
    if (*pHpCalc == 0x80000000) {
        // Initialize
        if (USE_SUBPIXELS_TEST) {
            *pHpCalc = curHp << 8;
        } else {
            *pHpCalc = curHp;
        }
    }
    nowHp = curHp - deltaHp;
    if (nowHp < 0) {
        nowHp = 0;
    } else if (nowHp > maxHp) {
        nowHp = maxHp;
    }
    if (USE_SUBPIXELS_TEST) {
        // Use subpixel mode
        if (nowHp == (*pHpCalc >> 8) && (*pHpCalc & 0xFF) == 0) {
            return -1;
        }
    } else {
        // Use integer pixel mode
        if (nowHp == *pHpCalc) {
            return -1;
        }
    }
    if (USE_SUBPIXELS_TEST) {
        hpPerPixel = (maxHp << 8) / pixelsWide;
        if (deltaHp < 0) {
            *pHpCalc += hpPerPixel;
            ret = *pHpCalc >> 8;
            if (ret >= nowHp) {
                *pHpCalc = nowHp << 8;
                ret = nowHp;
            }
        } else {
            *pHpCalc -= hpPerPixel;
            ret = *pHpCalc >> 8;
            if ((*pHpCalc & 0xFF) > 0) {
                ++ret;
            }
            if (ret <= nowHp) {
                *pHpCalc = nowHp << 8;
                ret = nowHp;
            }
        }
    } else {
        if (deltaHp < 0) {
            *pHpCalc += hpChange;
            if (*pHpCalc > nowHp) {
                *pHpCalc = nowHp;
            }
        } else {
            *pHpCalc -= hpChange;
            if (*pHpCalc < nowHp) {
                *pHpCalc = nowHp;
            }
        }
        ret = *pHpCalc;
    }
    return ret;
}

static u8 BattleHpBar_Util_MakeHpBarPixelBuffer(s32 maxHp, s32 hp, s32 deltaHp, s32 *pHpCalc, u8 *pixelBuf, u8 tilesWide) {
    int i;
    int targetHp;
    u32 pixelsWide;
    u32 curPixels;
    u32 ret;

    targetHp = hp - deltaHp;
    if (targetHp < 0) {
        targetHp = 0;
    } else if (targetHp > maxHp) {
        targetHp = maxHp;
    }
    pixelsWide = tilesWide * 8;
    for (i = 0; i < tilesWide; ++i) {
        pixelBuf[i] = 0;
    }
    if (USE_SUBPIXELS_TEST) {
        curPixels = (*pHpCalc * pixelsWide / maxHp) >> 8;
    } else {
        curPixels = *pHpCalc * pixelsWide / maxHp;
    }
    ret = curPixels;
    if (curPixels == 0 && targetHp > 0) {
        pixelBuf[0] = 1;
        ret = 1;
    } else {
        for (i = 0; i < tilesWide; ++i) {
            if (curPixels >= 8) {
                pixelBuf[i] = 8;
                curPixels -= 8;
            } else {
                pixelBuf[i] = curPixels;
                break;
            }
        }
    }
    return ret;
}

static u32 BattleHpBar_Util_GetPixelsToGain(s32 exp, s32 gainedExp, s32 maxExp, u8 tilesWide) {
    s8 curPixels, targetPixels;
    u8 pixelsWide;
    s32 targetExp;

    pixelsWide = tilesWide * 8;

    targetExp = exp - gainedExp;
    if (targetExp < 0) {
        targetExp = 0;
    } else if (targetExp > maxExp) {
        targetExp = maxExp;
    }
    curPixels = exp * pixelsWide / maxExp;
    targetPixels = targetExp * pixelsWide / maxExp;
    return abs(curPixels - targetPixels);
}

static const u8 *BattleHpBar_Util_GetComponentRawGraphic(int componentId) {
    return gBattleHpBar_RawGraphicComponents + componentId * 32;
}

u8 BattleHpBar_Util_GetBarTypeFromBattlerSide(u8 bside, u32 battleType) {
    switch (bside) {
    case 0:
        if (battleType & BATTLE_TYPE_PAL_PARK) {
            return HP_BAR_TYPE_PALPARK;
        }
        if (battleType & BATTLE_TYPE_SAFARI) {
            return HP_BAR_TYPE_SAFARI;
        }
        return HP_BAR_TYPE_SINGLE_PLAYER;
    case 1:
        return HP_BAR_TYPE_SINGLE_ENEMY;
    case 2:
        return HP_BAR_TYPE_DOUBLE_PLAYER_LHS;
    case 3:
        return HP_BAR_TYPE_DOUBLE_ENEMY_LHS;
    case 4:
        return HP_BAR_TYPE_DOUBLE_PLAYER_RHS;
    case 5:
        return HP_BAR_TYPE_DOUBLE_ENEMY_RHS;
    default:
        GF_ASSERT(FALSE);
        return HP_BAR_TYPE_SINGLE_PLAYER;
    }
}

static const ManagedSpriteTemplate *BattleHpBar_Util_GetHpBoxSpriteTemplate(u8 barType) {
    const ManagedSpriteTemplate *ret;
    switch (barType) {
    case HP_BAR_TYPE_SINGLE_PLAYER:
        ret = &sSpriteTemplate_HpBarSinglePlayer;
        break;
    case HP_BAR_TYPE_SINGLE_ENEMY:
        ret = &sSpriteTemplate_HpBarSingleEnemy;
        break;
    case HP_BAR_TYPE_DOUBLE_PLAYER_LHS:
        ret = &sSpriteTemplate_HpBarDoublePlayerLHS;
        break;
    case HP_BAR_TYPE_DOUBLE_ENEMY_LHS:
        ret = &sSpriteTemplate_HpBarDoubleEnemyLHS;
        break;
    case HP_BAR_TYPE_DOUBLE_PLAYER_RHS:
        ret = &sSpriteTemplate_HpBarDoublePlayerRHS;
        break;
    case HP_BAR_TYPE_DOUBLE_ENEMY_RHS:
        ret = &sSpriteTemplate_HpBarDoubleEnemyRHS;
        break;
    case HP_BAR_TYPE_SAFARI:
    case HP_BAR_TYPE_PALPARK:
        ret = &sSpriteTemplate_HpBarSafariOrPark;
        break;
    default:
        GF_ASSERT(FALSE);
        return NULL;
    }
    return ret;
}

static const ManagedSpriteTemplate *BattleHpBar_Util_GetArrowSpriteTemplate(u8 barType) {
    switch (barType) {
    case HP_BAR_TYPE_SINGLE_PLAYER:
    case HP_BAR_TYPE_DOUBLE_PLAYER_LHS:
    case HP_BAR_TYPE_DOUBLE_PLAYER_RHS:
    case HP_BAR_TYPE_SAFARI:
    case HP_BAR_TYPE_PALPARK:
        return &sSpriteTemplate_Arrow;
    case HP_BAR_TYPE_SINGLE_ENEMY:
    case HP_BAR_TYPE_DOUBLE_ENEMY_LHS:
    case HP_BAR_TYPE_DOUBLE_ENEMY_RHS:
        return NULL;
    default:
        GF_ASSERT(FALSE);
        return NULL;
    }
}

typedef struct BattleHpBarExpBarFullFlashEffectTaskData {
    BattleHpBar *hpBar;
    u8 *pDoneFlag;
    u8 state;
    u8 plttNum;
    s8 ev;
} BattleHpBarExpBarFullFlashEffectTaskData;

SysTask *BattleHpBar_BeginExpBarFullFlashEffect(BattleHpBar *hpBar, u8 *doneFlag) {
    *doneFlag = 0;
    BattleHpBarExpBarFullFlashEffectTaskData *taskData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(BattleHpBarExpBarFullFlashEffectTaskData));
    MI_CpuFill8(taskData, 0, sizeof(BattleHpBarExpBarFullFlashEffectTaskData));
    taskData->hpBar = hpBar;
    taskData->pDoneFlag = doneFlag;
    return SysTask_CreateOnMainQueue(Task_ExpBarFullFlash, taskData, 1000);
}

static void Task_ExpBarFullFlash(SysTask *task, void *data) {
    BattleHpBarExpBarFullFlashEffectTaskData *taskData = data;
    SpriteManager *spriteManager = BattleSystem_GetSpriteManager(taskData->hpBar->battleSystem);
    int plttNum;
    PaletteData *plttData = BattleSystem_GetPaletteData(taskData->hpBar->battleSystem);

    switch (taskData->state) {
    case 0:
        plttNum = SpriteManager_FindPlttResourceOffset(spriteManager, 20007, NNS_G2D_VRAM_TYPE_2DMAIN);
        ManagedSprite_SetPaletteOverride(taskData->hpBar->boxObj, plttNum);
        taskData->plttNum = plttNum;
        ++taskData->state;
        // break;
    case 1:
        taskData->ev += 2;
        if (taskData->ev >= 10) {
            taskData->ev = 10;
            ++taskData->state;
        }
        PaletteData_BlendPalette(plttData, PLTTBUF_MAIN_OBJ, 16 * taskData->plttNum, 0x10, taskData->ev, RGB(5, 29, 28));
        break;
    case 2:
        taskData->ev -= 2;
        if (taskData->ev <= 0) {
            taskData->ev = 0;
            ++taskData->state;
        }
        PaletteData_BlendPalette(plttData, PLTTBUF_MAIN_OBJ, 16 * taskData->plttNum, 0x10, taskData->ev, RGB(5, 29, 28));
        break;
    default:
        plttNum = SpriteManager_FindPlttResourceOffset(spriteManager, 20006, NNS_G2D_VRAM_TYPE_2DMAIN);
        ManagedSprite_SetPaletteOverride(taskData->hpBar->boxObj, plttNum);
        *taskData->pDoneFlag = 1;
        Heap_Free(taskData);
        SysTask_Destroy(task);
        break;
    }
}

void BattleHpBar_MenuOpenDummy(BattleHpBar *hpBar) {
}

void BattleHpBar_MenuCloseDummy(BattleHpBar *hpBar) {
}

static void BattleHpBar_StartBobTask(BattleHpBar *hpBar) {
    if (hpBar->sysTask == NULL) {
        hpBar->bobAngle = 0;
        hpBar->sysTask = SysTask_CreateOnMainQueue(Task_BattleHpBarBob, hpBar, 1010);
    }
}

static void BattleHpBar_StopBobTask(BattleHpBar *hpBar) {
    if (hpBar->sysTask != NULL) {
        SysTask_Destroy(hpBar->sysTask);
        hpBar->sysTask = NULL;
    }
    hpBar->bobAngle = 0;
    BattleHpBar_SetOffset(hpBar, 0, 0);
}

static void Task_BattleHpBarBob(SysTask *task, void *data) {
    BattleHpBar *hpBar = data;
    hpBar->bobAngle += 20;
    if (hpBar->bobAngle >= 360) {
        hpBar->bobAngle -= 360;
    }
    BattleHpBar_SetOffset(hpBar, 0, FX_Mul(GF_SinDegNoWrap(hpBar->bobAngle), FX32_CONST(1.5)) / FX32_ONE);
}
