#include "pokeathlon/pokeathlon_block_smash.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"
#include "pokeathlon/pokeathlon_course_types_def.h"
#include "pokeathlon/pokeathlon_event_points.h"
#include "pokeathlon/pokeathlon_pennant_capture.h"

#include "assert.h"
#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "math_util.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon_icon_idx.h"
#include "screen_fade.h"
#include "sound_playback.h"
#include "sprite_resource_header.h"
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "wireless_status_icon.h"

enum {
    BLOCK_SMASH_STATE_INTRO,
    BLOCK_SMASH_STATE_PLAY,
    BLOCK_SMASH_STATE_FINISH,
};

enum PokeathlonBlockSmashInitState {
    BLOCK_SMASH_INIT_CREATE,
    BLOCK_SMASH_INIT_LOAD_BGS,
    BLOCK_SMASH_INIT_LOAD_MONS,
    BLOCK_SMASH_INIT_WAIT_MONS,
    BLOCK_SMASH_INIT_CREATE_BLOCKS,
    BLOCK_SMASH_INIT_CREATE_TEAMS,
    BLOCK_SMASH_INIT_SEND_REPORT,
    BLOCK_SMASH_INIT_WAIT_FADE_IN,
};

void PokeathlonCourse_GetMonInfo(PokeathlonCourseData *course, int participant, int member, void *out);
void *PokeathlonCourse_GetPerformance(PokeathlonCourseData *course, int participant, int member);
u32 PokeathlonCourse_GetSpriteSizeType(void *mon);
void PokeathlonCourse_CreateStartSprites(PokeathlonCourseData *course, int baseY, void *resources, void *spriteList, int whichScreen, int count, const void *position);
BOOL PokeathlonCourse_RunStartCountdown(PokeathlonCourseData *course);
void PokeathlonCourse_UpdateTimeUpCountdown(PokeathlonCourseData *course, int timer);
BOOL PokeathlonCourse_RunFinish(PokeathlonCourseData *course);
void PokeathlonCourse_SetHudPlanes(PokeathlonCourseData *course, int planes);
void **PokeathlonCourse_CreateHudSprites(PokeathlonCourseData *course, int baseY, PokeathlonCourseSpriteResources *resources, SpriteList *spriteList);
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u8 participant, int mon, int counter, int amount);
void PokeathlonCourse_SetRecordScore(PokeathlonCourseData *course, u16 score);
void PokeathlonCourseScoreList_Init(PokeathlonCourseScoreList *graphics);
void PokeathlonCourse_AddScoreEntry(PokeathlonCourseData *course, PokeathlonCourseScoreList *graphics, int participant, int score, int idx, int digits, int msgno);
void PokeathlonCourseScoreList_Rank(PokeathlonCourseScoreList *graphics, int mode);
void PokeathlonCourseScoreList_AddPointsToTotals(PokeathlonCourseScoreList *graphics);
void PokeathlonCourseScoreList_SaveEventPoints(PokeathlonCourseScoreList *graphics, int event);
PokeathlonCourseSpriteResources *PokeathlonCourseSprites_CreateResources(enum HeapID heapId, int flags, int charVram);
void PokeathlonCourseSprites_FreeResources(PokeathlonCourseSpriteResources *resources);
Sprite *PokeathlonCourse_CreateTimerDigitSprite(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId);
Sprite *PokeathlonCourseSprites_CreateTimeUpCountdown(PokeathlonCourseSpriteResources *resources, SpriteList *spriteList, int priority, enum HeapID heapId);
PokeathlonCourseMonObjects *PokeathlonCourseMonObjects_Create(enum HeapID heapId, int count, int baseDrawPriority, PokeathlonCourseSpriteResources *resources, SpriteList *spriteList);
void PokeathlonCourseMonObjects_Free(PokeathlonCourseMonObjects *monPics);
void PokeathlonCourseMonObjects_Load(void *monPics, int count, void *mons, void *params, int priority, int baseIndex);
BOOL PokeathlonCourseMonObjects_IsLoaded(PokeathlonCourseMonObjects *monPics);
void PokeathlonCourseObjects_PauseAnims(PokeathlonCourseMonObjects *monPics, int paused);
void PokeathlonCourse_LoadMonIcon(ManagedSprite *sprite, void *mon, u16 plttId, NARC *narc, u32 offset, enum HeapID heapId);

typedef struct PokeathlonBlockSmashBannerPosition {
    u16 x;
    u16 y;
} PokeathlonBlockSmashBannerPosition;

typedef struct PokeathlonBlockSmashBgTemplates {
    BgTemplate templates[6];
} PokeathlonBlockSmashBgTemplates;

PokeathlonBlockSmashMon *PokeathlonBlockSmashMon_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonCourseData *course);
void PokeathlonBlockSmashMon_Delete(PokeathlonBlockSmashMon *mon);
void PokeathlonBlockSmashMon_Update(PokeathlonBlockSmashMon *mon, int touched);
void PokeathlonBlockSmashMon_OnHit(PokeathlonBlockSmashMon *mon, int hitSpot, int broken);
void PokeathlonBlockSmashMon_StartSwitch(PokeathlonBlockSmashMon *mon);
void PokeathlonBlockSmashMon_SetAnimate(PokeathlonBlockSmashMon *mon, int animate);
u8 PokeathlonBlockSmashMon_GetMode(PokeathlonBlockSmashMon *mon);
void *PokeathlonBlockSmashMon_GetActiveMember(PokeathlonBlockSmashMon *mon);
BOOL PokeathlonBlockSmashMon_CanSwitch(PokeathlonBlockSmashMon *mon);
u32 PokeathlonBlockSmashMon_IsGaugeFull(PokeathlonBlockSmashMon *mon);
u32 PokeathlonBlockSmashMon_IsTired(PokeathlonBlockSmashMon *mon);
u16 PokeathlonBlockSmashMon_GetGauge(PokeathlonBlockSmashMon *mon);
u8 PokeathlonBlockSmashMon_GetActiveMemberIndex(PokeathlonBlockSmashMon *mon);
u32 PokeathlonBlockSmashMon_CanHit(PokeathlonBlockSmashMon *mon);
PokeathlonBlockSmashRivals *PokeathlonBlockSmashRivals_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonCourseData *course, PokeathlonCourseMonObjects *monPics);
void PokeathlonBlockSmashRivals_Delete(PokeathlonBlockSmashRivals *rivals);
void PokeathlonBlockSmashRivals_Update(PokeathlonBlockSmashRivals *rivals);
void PokeathlonBlockSmashRivals_Pause(PokeathlonBlockSmashRivals *rivals);
PokeathlonBlockSmashCpu *PokeathlonBlockSmashCpu_New(PokeathlonCourseData *course, enum HeapID heapId);
void PokeathlonBlockSmashCpu_Delete(PokeathlonBlockSmashCpu *cpu);
void PokeathlonBlockSmashCpu_Update(PokeathlonBlockSmashCpu *cpu);
void PokeathlonBlockSmashCpu_WriteReports(PokeathlonBlockSmashCpu *cpu);
ManagedSprite *PokeathlonBlockSmash_NewMainSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority);
ManagedSprite *PokeathlonBlockSmash_NewSubSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u16 anim, u8 priority);

// The sprites of a participant's row on the top screen
typedef struct PokeathlonBlockSmashRowSprites {
    u32 unk0;
    u32 bar;
    u32 star;
} PokeathlonBlockSmashRowSprites;

static void PokeathlonBlockSmash_SetVramBanks(void);
static void PokeathlonBlockSmash_InitSpriteSystem(PokeathlonBlockSmash *work);
static void PokeathlonBlockSmash_FreeSpriteSystem(PokeathlonBlockSmash *work);
static void PokeathlonBlockSmash_CreateHudSprites(PokeathlonBlockSmash *work);
static void PokeathlonBlockSmash_DeleteHudSprites(PokeathlonBlockSmash *work);
static void PokeathlonBlockSmash_LoadSpriteResources(PokeathlonBlockSmash *work);
static void PokeathlonBlockSmash_LoadMainAndSubSpriteRes(SpriteSystem *spriteSystem, SpriteManager *spriteManager);
static void PokeathlonBlockSmash_UpdateScoreDigits(PokeathlonBlockSmash *work);
static void PokeathlonBlockSmash_StopHudAnims(PokeathlonBlockSmash *work);
static void PokeathlonBlockSmash_InitBgs(BgConfig *bgConfig, enum HeapID heapId);
static void PokeathlonBlockSmash_FreeBgsAndText(PokeathlonBlockSmash *work);
static void PokeathlonBlockSmash_LoadBgGraphics(PokeathlonBlockSmash *work);
static void PokeathlonBlockSmash_InitText(PokeathlonCourseData *course);
static void PokeathlonBlockSmash_UpdateTimer(PokeathlonBlockSmash *work);
static void PokeathlonBlockSmash_PrintInitialText(PokeathlonBlockSmash *work);
static void PokeathlonBlockSmash_PrintMessage(Window *window, MessageFormat *msgFormat, MsgData *msgData, u32 msgId, enum HeapID heapId);
static void PokeathlonBlockSmash_CreateTeamRows(PokeathlonCourseData *course);
static void PokeathlonBlockSmash_DeleteTeamRows(PokeathlonBlockSmash *work);
static void PokeathlonBlockSmash_UpdateTeamRows(PokeathlonCourseData *course);
static void PokeathlonBlockSmash_SetTeamRowIcon(PokeathlonCourseData *course, u32 participant, u32 member);
static void PokeathlonBlockSmash_HandleSwipe(PokeathlonBlockSmash *work, BOOL anywhere);
static void PokeathlonBlockSmash_WriteReport(PokeathlonCourseData *course);
static void PokeathlonBlockSmash_UpdateLinkData(PokeathlonCourseData *course);
static void PokeathlonBlockSmash_PrintScore(PokeathlonCourseData *course, u32 score);
static void PokeathlonBlockSmash_StartShake(PokeathlonBlockSmash *work, int offset, int decay);

static const PokeathlonBlockSmashBannerPosition sPokeathlonBlockSmashBannerPosition = { 120, 132 };

static const TouchscreenHitbox sPokeathlonBlockSmashSwipeHitboxes[] = {
    { .rect = { 8, 32, 0, 64 } },
    { .rect = { TOUCHSCREEN_RECTLIST_END } },
};

static const GraphicsModes sPokeathlonBlockSmashGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

static const OamCharTransferParam sPokeathlonBlockSmashOamCharTransferParam = {
    .maxTasks = 0,
    .sizeMain = 0x40000,
    .sizeSub = 0x4000,
    .charModeMain = GX_OBJVRAMMODE_CHAR_1D_256K,
    .charModeSub = GX_OBJVRAMMODE_CHAR_1D_256K,
};

static const u32 sPokeathlonBlockSmashBgLayers[6] = { GF_BG_LYR_MAIN_0, GF_BG_LYR_MAIN_1, GF_BG_LYR_MAIN_2, GF_BG_LYR_SUB_0, GF_BG_LYR_SUB_1, GF_BG_LYR_SUB_2 };

static const SpriteResourceCountsListUnion sPokeathlonBlockSmashSpriteResourceCounts = {
    .numChar = 9,
    .numPltt = 10,
    .numCell = 7,
    .numAnim = 7,
    .numMcel = 0,
    .numManm = 0,
};

static const WindowTemplate sPokeathlonBlockSmashWindowTemplates[4] = {
    { GF_BG_LYR_MAIN_1, 6,  22, 4,  2, 15, 1    },
    { GF_BG_LYR_MAIN_1, 25, 22, 7,  2, 15, 9    },
    { GF_BG_LYR_MAIN_2, 0,  20, 32, 2, 15, 0x17 },
    { GF_BG_LYR_SUB_2,  0,  22, 15, 2, 15, 1    },
};

static const OamManagerParam sPokeathlonBlockSmashOamManagerParam = {
    .fromOBJmain = 0,
    .numOBJmain = 126,
    .fromAffineMain = 0,
    .numAffineMain = 32,
    .fromOBJsub = 1,
    .numOBJsub = 126,
    .fromAffineSub = 0,
    .numAffineSub = 32,
};

static const GraphicsBanks sPokeathlonBlockSmashGraphicsBanks = {
    .bg = GX_VRAM_BG_64_E,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_256_AB,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_NONE,
    .texpltt = GX_VRAM_TEXPLTT_NONE,
};

static const PokeathlonBlockSmashRowSprites sPokeathlonBlockSmashRowSprites[4] = {
    { 1,  2,  3  },
    { 4,  5,  6  },
    { 7,  8,  9  },
    { 10, 11, 12 },
};

static const PokeathlonBlockSmashBgTemplates sPokeathlonBlockSmashBgTemplates = {
    {
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0000,
            .charBase = GX_BG_CHARBASE_0x04000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 3,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0800,
            .charBase = GX_BG_CHARBASE_0x08000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 1,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x1000,
            .charBase = GX_BG_CHARBASE_0x0c000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 0,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0000,
            .charBase = GX_BG_CHARBASE_0x04000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 3,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x0800,
            .charBase = GX_BG_CHARBASE_0x0c000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 1,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
     {
            .x = 0,
            .y = 0,
            .bufferSize = GF_BG_BUF_SIZE_256x256_4BPP,
            .baseTile = 0,
            .size = GF_BG_SCR_SIZE_256x256,
            .colorMode = GX_BG_COLORMODE_16,
            .screenBase = GX_BG_SCRBASE_0x1000,
            .charBase = GX_BG_CHARBASE_0x14000,
            .bgExtPltt = GX_BG_EXTPLTT_01,
            .priority = 0,
            .areaOver = GX_BG_AREAOVER_XLU,
            .mosaic = FALSE,
        },
     },
};

BOOL PokeathlonPennantCaptureCpu_IsNearCellTarget(PokeathlonPennantCaptureCpuCell *cell, s32 x, s32 y) {
    s32 left = cell->targetX - 16;
    s32 right = cell->targetX + 16;
    s32 top = cell->targetY - 16;
    s32 bottom = cell->targetY + 16;
    if (left <= x && x < right && top <= y && y < bottom) {
        return TRUE;
    }
    return FALSE;
}

// The field is narrower below y = 392 (the goal opening).
BOOL PokeathlonPennantCaptureCpu_IsCellTargetOnField(PokeathlonPennantCaptureCpuCell *cell) {
    s32 y = cell->targetY;
    BOOL right;
    BOOL left;
    if (y < 104) {
        return FALSE;
    }
    if (y <= 392) {
        right = cell->targetX >= 144;
        left = cell->targetX <= 368;
        if (right & left) {
            return TRUE;
        }
    } else {
        right = cell->targetX >= 208;
        left = cell->targetX <= 304;
        if (right & left) {
            return TRUE;
        }
    }
    return FALSE;
}

enum PokeathlonBlockSmashIntroState {
    BLOCK_SMASH_INTRO_WAIT_BLOCKS,
    BLOCK_SMASH_INTRO_COUNTDOWN,
};

// Event state 0: wait for the first stack, then show the start banner.
BOOL PokeathlonBlockSmash_Intro(PokeathlonCourseData *course, u8 *state) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetWork(course);
    switch (*state) {
    case BLOCK_SMASH_INTRO_WAIT_BLOCKS:
        PokeathlonBlockSmashBlocks_Update(work->blocks);
        if (PokeathlonBlockSmashBlocks_GetState(work->blocks) == BLOCKS_STATE_READY) {
            PokeathlonBlockSmashBannerPosition position;
            SpriteList *spriteList;
            position = sPokeathlonBlockSmashBannerPosition;
            spriteList = SpriteManager_GetSpriteList(work->spriteManager);
            PokeathlonCourse_CreateStartSprites(course, 0, work->iconResources, spriteList, 1, 1, &position);
            (*state)++;
        }
        break;
    case BLOCK_SMASH_INTRO_COUNTDOWN:
        if (PokeathlonCourse_RunStartCountdown(course)) {
            PokeathlonCourseObjects_PauseAnims(work->monPics, 0);
            PokeathlonCourse_SetNextState(course, BLOCK_SMASH_STATE_PLAY);
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return FALSE;
}

// Returns TRUE once the host's frame says the event is over.
static BOOL PokeathlonBlockSmash_UpdateGame(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetWork(course);
    int state;
    BOOL touched;
    u32 *member;
    u32 strongHit;
    int hitSpot;
    int broken;
    if (((PokeathlonBlockSmashFrame *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + 0xF0))->finished == 1) {
        PokeathlonBlockSmash_PrintScore(course, PokeathlonBlockSmashBlocks_GetScore(work->blocks));
        PokeathlonBlockSmash_StopHudAnims(work);
        PokeathlonBlockSmashMon_SetAnimate(work->mon, 0);
        PokeathlonBlockSmashRivals_Pause(work->rivals);
        PokeathlonBlockSmashBlocks_Pause(work->blocks);
        PokeathlonCourseObjects_PauseAnims(work->monPics, 1);
        return TRUE;
    }
    if (System_GetTouchNew()) {
        PokeathlonCourse_AddMonCounter(course, (u8)PokeathlonCourse_GetNetId(course), 3, 0, 1);
    }
    PokeathlonBlockSmash_UpdateTimer(work);
    PokeathlonBlockSmashBlocks_Update(work->blocks);
    state = PokeathlonBlockSmashBlocks_GetState(work->blocks);
    PokeathlonBlockSmashMon_Update(work->mon, state == BLOCKS_STATE_DROPPING);
    if (PokeathlonCourse_GetNetId(course) == 0) {
        PokeathlonBlockSmashCpu_Update(work->cpu);
    }
    PokeathlonBlockSmashRivals_Update(work->rivals);
    touched = PokeathlonBlockSmashBlocks_IsTopBlockTouched(work->blocks);
    PokeathlonBlockSmash_HandleSwipe(work, touched);
    if (PokeathlonBlockSmashMon_CanHit(work->mon) && state == BLOCKS_STATE_READY && touched) {
        member = PokeathlonBlockSmashMon_GetActiveMember(work->mon);
        strongHit = PokeathlonBlockSmashMon_IsGaugeFull(work->mon);
        hitSpot = PokeathlonBlockSmashBlocks_IsSpotTouched(work->blocks, ((PokeathlonBlockSmashMember *)member)->kind);
        {
            int count = PokeathlonBlockSmashBlocks_Hit(work->blocks, PokeathlonBlockSmashMon_GetActiveMemberIndex(work->mon), (s16)(u16)((PokeathlonBlockSmashMember *)member)->power, hitSpot, strongHit, &broken);
            PokeathlonBlockSmashMon_OnHit(work->mon, hitSpot, count);
        }
        PokeathlonBlockSmash_UpdateScoreDigits(work);
        if (hitSpot) {
            PokeathlonBlockSmash_StartShake(work, 16, 2);
        } else if (broken) {
            PokeathlonBlockSmash_StartShake(work, 4, 1);
        } else if (PokeathlonBlockSmashMon_IsGaugeFull(work->mon)) {
            PokeathlonBlockSmash_StartShake(work, 4, 1);
        }
    }
    if (work->timer > 0) {
        work->timer--;
    }
    PokeathlonCourse_UpdateTimeUpCountdown(course, work->timer);
    if (work->timer == 0 && PokeathlonCourse_GetNetId(course) == 0) {
        PokeathlonBlockSmashFrame *frame = (PokeathlonBlockSmashFrame *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + 0x28);
        frame->finished = TRUE;
    }
    return FALSE;
}

enum PokeathlonBlockSmashPlayState {
    BLOCK_SMASH_PLAY_RUN,
    BLOCK_SMASH_PLAY_FINISH,
};

// Event state 1: play until the host ends the event and the shake is over, then fade out.
BOOL PokeathlonBlockSmash_Play(PokeathlonCourseData *course, u8 *state) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetWork(course);
    switch (*state) {
    case BLOCK_SMASH_PLAY_RUN:
        PokeathlonBlockSmash_UpdateLinkData(course);
        PokeathlonBlockSmash_UpdateTeamRows(course);
        if (PokeathlonBlockSmash_UpdateGame(course)) {
            (*state)++;
        }
        break;
    case BLOCK_SMASH_PLAY_FINISH:
        if (PokeathlonCourse_RunFinish(course) && work->shakeTask == NULL) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, work->heapId);
            PokeathlonCourse_SetNextState(course, BLOCK_SMASH_STATE_FINISH);
        }
        break;
    }
    return FALSE;
}

// Event state 2: after the fade, store the scores (the host also those of the CPU participants).
BOOL PokeathlonBlockSmash_Finish(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetWork(course);
    u32 participant;
    s32 count;
    u32 offset;
    u16 score;
    if (IsPaletteFadeFinished()) {
        participant = PokeathlonCourse_GetNetId(course);
        score = PokeathlonBlockSmashBlocks_GetScore(work->blocks);
        PokeathlonCourse_SetEventScore(course, (u8)participant, score);
        if (participant == 0) {
            count = PokeathlonCourse_GetParticipantCount(course);
            if (count < 4) {
                offset = count * 0x28;
                do {
                    u8 *block = PokeathlonCourse_GetDataCopyArea(course) + 0x50 + offset;
                    PokeathlonCourse_SetEventScore(course, (u8)count, ((PokeathlonBlockSmashReport *)PokeathlonCourse_ToBytes(block))->score);
                    count++;
                    offset += 0x28;
                } while (count < 4);
            }
        }
        return TRUE;
    }
    return FALSE;
}

typedef struct PokeathlonBlockSmashMonPicParams {
    u32 loadAtOnce;
    u32 priority;
    u32 vramTransfer;
    u32 vram;
    u32 whichScreen;
    u32 ids[9];
    u32 unk38[3];
} PokeathlonBlockSmashMonPicParams;

// PokeathlonCourse_GetParticipantMons: the participant's team.
typedef struct PokeathlonBlockSmashTeamMon {
    u8 unk0[0x12];
    u16 nickname[11];
} PokeathlonBlockSmashTeamMon;

// What PokeathlonCourse_GetMonInfo reports about one Pokemon of a team.
typedef struct PokeathlonBlockSmashMonInfo {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
} PokeathlonBlockSmashMonInfo;

// Event template entry 0: sets up the event over several frames; TRUE when done.
BOOL PokeathlonBlockSmash_Init(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetWork(course);
    int i, j;
    switch (PokeathlonCourse_GetEventInitState(course)) {
    case BLOCK_SMASH_INIT_CREATE:
        Heap_Create(HEAP_ID_POKEATHLON, HEAP_ID_141, 0x60000);
        Main_SetVBlankIntrCB(NULL, NULL);
        Main_SetHBlankIntrCB(NULL, NULL);
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
        reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
        PokeathlonBlockSmash_SetVramBanks();
        gSystem.screensFlipped = TRUE;
        GfGfx_SwapDisplay();
        work = PokeathlonCourse_AllocWork(course, sizeof(PokeathlonBlockSmash));
        MI_CpuFill8(work, 0, sizeof(PokeathlonBlockSmash));
        work->heapId = HEAP_ID_141;
        work->timer = 900;
        PokeathlonCourse_AdvanceEventInitState(course);
        break;
    case BLOCK_SMASH_INIT_LOAD_BGS:
        work->bgConfig = BgConfig_Alloc(work->heapId);
        PokeathlonCourse_SetHudPlanes(course, 4);
        PokeathlonBlockSmash_InitBgs(work->bgConfig, work->heapId);
        PokeathlonBlockSmash_InitText(course);
        PokeathlonBlockSmash_LoadBgGraphics(work);
        PokeathlonBlockSmash_InitSpriteSystem(work);
        work->monPics = PokeathlonCourseMonObjects_Create(work->heapId, 9, 32, work->iconResources, SpriteManager_GetSpriteList(work->spriteManager));
        PokeathlonCourse_AdvanceEventInitState(course);
        break;
    case BLOCK_SMASH_INIT_LOAD_MONS: {
        PokeathlonBlockSmashMonInfo mons[9];
        int n = 0;
        PokeathlonBlockSmashMonPicParams params = { 0 };
        for (i = 0; i < 4; i++) {
            if (i != PokeathlonCourse_GetNetId(course)) {
                for (j = 0; j < 3; j++) {
                    GF_ASSERT(n < 9);
                    PokeathlonCourse_GetMonInfo(course, i, j, &mons[n]);
                    params.ids[n] = PokeathlonCourse_GetSpriteSizeType(PokeathlonCourse_GetPerformance(course, i, j));
                    n++;
                }
            }
        }
        params.priority = 1;
        params.vram = 1;
        params.whichScreen = 1;
        PokeathlonCourseMonObjects_Load(work->monPics, 9, mons, &params, 0, 0);
        PokeathlonCourse_AdvanceEventInitState(course);
        break;
    }
    case BLOCK_SMASH_INIT_WAIT_MONS:
        if (PokeathlonCourseMonObjects_IsLoaded(work->monPics)) {
            PokeathlonBlockSmash_LoadSpriteResources(work);
            PokeathlonBlockSmash_LoadMainAndSubSpriteRes(work->spriteSystem, work->spriteManager);
            Sprite_SetDrawPriority(*PokeathlonCourse_CreateHudSprites(course, 0, work->iconResources, SpriteManager_GetSpriteList(work->spriteManager)), 0);
            PokeathlonCourse_AdvanceEventInitState(course);
        }
        break;
    case BLOCK_SMASH_INIT_CREATE_BLOCKS:
        PokeathlonBlockSmash_CreateHudSprites(work);
        work->blocks = PokeathlonBlockSmashBlocks_New(work->heapId, work->spriteSystem, work->spriteManager, course);
        PokeathlonCourse_AdvanceEventInitState(course);
        break;
    case BLOCK_SMASH_INIT_CREATE_TEAMS:
        work->mon = PokeathlonBlockSmashMon_New(work->heapId, work->spriteSystem, work->spriteManager, course);
        work->rivals = PokeathlonBlockSmashRivals_New(work->heapId, work->spriteSystem, work->spriteManager, course, work->monPics);
        PokeathlonCourseObjects_PauseAnims(work->monPics, 1);
        if (PokeathlonCourse_GetNetId(course) == 0) {
            work->cpu = PokeathlonBlockSmashCpu_New(course, work->heapId);
        }
        PokeathlonBlockSmash_PrintInitialText(work);
        PokeathlonBlockSmash_CreateTeamRows(course);
        PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData *)work->bgConfig); // the callback gets the BgConfig
        WirelessStatusIcon_CreateOnScreenIfConnected(1);
        PokeathlonCourse_SetEventDrawEnabled(course, 1);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        if (PokeathlonCourse_GetNetId(course) == 0) {
            PokeathlonBlockSmashCpu_WriteReports(work->cpu);
        }
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 6, 1, work->heapId);
        PokeathlonCourse_AdvanceEventInitState(course);
        break;
    case BLOCK_SMASH_INIT_SEND_REPORT:
        if (PokeathlonCourse_GetNetId(course) == 0) {
            PokeathlonBlockSmash_WriteReport(course);
        }
        PokeathlonBlockSmash_UpdateLinkData(course);
        PokeathlonCourse_AdvanceEventInitState(course);
        break;
    case BLOCK_SMASH_INIT_WAIT_FADE_IN:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return FALSE;
}

// Event template entry 1.
BOOL PokeathlonBlockSmash_Draw(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetWork(course);
    SpriteSystem_DrawSprites(work->spriteManager);
    return TRUE;
}

// Event template entry 2.
BOOL PokeathlonBlockSmash_ShowResults(PokeathlonCourseData *course) {
    PokeathlonCourseScoreList *graphics = PokeathlonCourse_GetGraphicsSystem(course);
    int i;
    PokeathlonCourseScoreList_Init(graphics);
    for (i = 0; i < 4; i++) {
        u16 value = PokeathlonCourse_GetReceivedScore(course, (u8)i);
        PokeathlonCourse_AddScoreEntry(course, graphics, i, value, 0, 3, msg_0309_00294);
    }
    PokeathlonCourseScoreList_Rank(graphics, 1);
    PokeathlonEvent_CalcPoints(graphics, 3);
    PokeathlonCourseScoreList_AddPointsToTotals(graphics);
    PokeathlonCourseScoreList_SaveEventPoints(graphics, 3);
    return TRUE;
}

// Event template entry 3: frees everything Init created.
BOOL PokeathlonBlockSmash_Exit(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetWork(course);
    PokeathlonBlockSmash_DeleteTeamRows(work);
    if (!PokeathlonCourse_GetNetId(course)) {
        PokeathlonBlockSmashCpu_Delete(work->cpu);
    }
    PokeathlonBlockSmashBlocks_Delete(work->blocks);
    PokeathlonBlockSmashRivals_Delete(work->rivals);
    PokeathlonBlockSmashMon_Delete(work->mon);
    PokeathlonBlockSmash_DeleteHudSprites(work);
    PokeathlonBlockSmash_FreeSpriteSystem(work);
    PokeathlonBlockSmash_FreeBgsAndText(work);
    WirelessStatusIcon_Destroy();
    Main_SetVBlankIntrCB(NULL, NULL);
    Main_SetHBlankIntrCB(NULL, NULL);
    PokeathlonCourse_FreeWork(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    Heap_Destroy(HEAP_ID_141);
    return TRUE;
}

static void PokeathlonBlockSmash_SetVramBanks(void) {
    GraphicsBanks banks = sPokeathlonBlockSmashGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void PokeathlonBlockSmash_InitSpriteSystem(PokeathlonBlockSmash *work) {
    SpriteResourceCountsListUnion counts = sPokeathlonBlockSmashSpriteResourceCounts;
    OamManagerParam oamParam = sPokeathlonBlockSmashOamManagerParam;
    OamCharTransferParam transferParam = sPokeathlonBlockSmashOamCharTransferParam;
    transferParam.maxTasks = 128;
    work->spriteSystem = SpriteSystem_Alloc(work->heapId);
    work->spriteManager = SpriteManager_New(work->spriteSystem);
    SpriteSystem_Init(work->spriteSystem, &oamParam, &transferParam, 32);
    SpriteSystem_InitSprites(work->spriteSystem, work->spriteManager, 256);
    SpriteSystem_InitManagerWithCapacities(work->spriteSystem, work->spriteManager, &counts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(work->spriteSystem), 0, FX32_CONST(256));
    work->iconResources = PokeathlonCourseSprites_CreateResources(work->heapId, 743, 1);
    work->icons.narc = NARC_New(NARC_poketool_icongra_poke_icon, work->heapId);
}

static void PokeathlonBlockSmash_FreeSpriteSystem(PokeathlonBlockSmash *work) {
    NARC_Delete(work->icons.narc);
    PokeathlonCourseMonObjects_Free(work->monPics);
    PokeathlonCourseSprites_FreeResources(work->iconResources);
    SpriteSystem_FreeResourcesAndManager(work->spriteSystem, work->spriteManager);
    SpriteSystem_Free(work->spriteSystem);
}

// The timer (top screen), the score frame and the digits.
static void PokeathlonBlockSmash_CreateHudSprites(PokeathlonBlockSmash *work) {
    VecFx32 position = { 0, 0, 0 };
    int i;
    int x = FX32_CONST(8);
    for (i = 0; i < 3; i++) {
        position.x = x;
        position.y = FX32_CONST(176);
        work->scoreDigits[i] = PokeathlonCourseSprites_CreateTimeUpCountdown(work->iconResources, SpriteManager_GetSpriteList(work->spriteManager), 0, work->heapId);
        Sprite_SetMatrix(work->scoreDigits[i], &position);
        Sprite_SetDrawPriority(work->scoreDigits[i], 2);
        Sprite_SetAnimCtrlSeq(work->scoreDigits[i], 1);
        Sprite_SetDrawFlag(work->scoreDigits[i], 1);
        x += FX32_CONST(16);
    }
    work->timerFrame = PokeathlonBlockSmash_NewMainSprite(work->spriteSystem, work->spriteManager, 192, 184, 25, 2);
    {
        int digitX;
        PokeathlonBlockSmash *cursor;
        i = 0;
        cursor = work;
        digitX = 212;
        for (; i < 2; i++) {
            VecFx32 digitPosition = { 0, 0, 0 };
            cursor->timerDigits[0] = PokeathlonCourse_CreateTimerDigitSprite(work->iconResources, SpriteManager_GetSpriteList(work->spriteManager), 2, work->heapId);
            digitPosition.x = (u32)digitX << FX32_SHIFT;
            digitPosition.y = 184 << FX32_SHIFT;
            digitPosition.z = 0;
            Sprite_SetMatrix(cursor->timerDigits[0], &digitPosition);
            Sprite_SetDrawFlag(cursor->timerDigits[0], 1);
            cursor = (PokeathlonBlockSmash *)((u8 *)cursor + sizeof(ManagedSprite *));
            digitX += 16;
        }
    }
    work->switchButton = PokeathlonBlockSmash_NewMainSprite(work->spriteSystem, work->spriteManager, 32, 16, 19, 2);
}

static void PokeathlonBlockSmash_DeleteHudSprites(PokeathlonBlockSmash *work) {
    s32 i;
    GF_ASSERT(work != NULL);
    for (i = 0; i < 2; i++, work = (PokeathlonBlockSmash *)((u8 *)work + sizeof(ManagedSprite *))) {
        GF_ASSERT(work->timerFrame != NULL);
        Sprite_DeleteAndFreeResources(work->timerFrame);
    }
}

static void PokeathlonBlockSmash_LoadSpriteResources(PokeathlonBlockSmash *work) {
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    spriteManager = work->spriteManager;
    spriteSystem = work->spriteSystem;
    NNS_G2D_VRAM_TYPE vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 8, TRUE, vram, 10000);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 7, FALSE, 2, vram, 10000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 9, TRUE, 10000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 10, TRUE, 10000);
    vram = NNS_G2D_VRAM_TYPE_2DSUB;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 12, TRUE, vram, 10001);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 11, FALSE, 3, vram, 10001);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 13, TRUE, 10001);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 14, TRUE, 10001);
    work->icons.plttId = SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), FALSE, 3, vram, 10002);
}

#pragma opt_propagation off

static void PokeathlonBlockSmash_LoadMainAndSubSpriteRes(SpriteSystem *spriteSystem, SpriteManager *spriteManager) {
    int i;
    int vram;
    vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 16, TRUE, vram, 10002);
    SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 15, FALSE, 1, vram, 10003);
    vram = NNS_G2D_VRAM_TYPE_2DSUB;
    i = 0;
    do {
        SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 16, TRUE, vram, 10003 + i);
        SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 15, FALSE, 1, vram, 10004 + i);
        i++;
    } while (i < 2);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 17, TRUE, 10002);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_2_3_2, 18, TRUE, 10002);
}

#pragma opt_propagation reset

static void PokeathlonBlockSmash_UpdateScoreDigits(PokeathlonBlockSmash *work) {
    s32 i;
    u16 score;
    GF_ASSERT(work != NULL);
    score = PokeathlonBlockSmashBlocks_GetScore(work->blocks);
    for (i = 0; i < 3; i++) {
        Sprite_SetAnimCtrlSeq(work->scoreDigits[2 - i], (u16)(score % 10) + 1);
        if (score < 10) {
            break;
        }
        score = (u16)(score / 10);
    }
}

static void PokeathlonBlockSmash_StopHudAnims(PokeathlonBlockSmash *work) {
    s32 i;
    for (i = 0; i < 2; i++, work = (PokeathlonBlockSmash *)((u8 *)work + sizeof(ManagedSprite *))) {
        ManagedSprite_SetAnimateFlag(work->timerFrame, FALSE);
    }
}

static void PokeathlonBlockSmash_InitBgs(BgConfig *bgConfig, enum HeapID heapId) {
    GraphicsModes modes = sPokeathlonBlockSmashGraphicsModes;
    s32 i;
    SetBothScreensModesAndDisable(&modes);
    {
        PokeathlonBlockSmashBgTemplates templates = sPokeathlonBlockSmashBgTemplates;
        for (i = 0; i < 6; i++) {
            InitBgFromTemplate(bgConfig, (u8)sPokeathlonBlockSmashBgLayers[i], &templates.templates[i], GF_BG_TYPE_TEXT);
            BgClearTilemapBufferAndCommit(bgConfig, (u8)sPokeathlonBlockSmashBgLayers[i]);
            BG_ClearCharDataRange((u8)sPokeathlonBlockSmashBgLayers[i], 0x20, 0, heapId);
        }
        FontID_Alloc(4, heapId);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, 16);
    }
}

static void PokeathlonBlockSmash_FreeBgsAndText(PokeathlonBlockSmash *work) {
    s32 i;
    PokeathlonBlockSmash *cursor;
    for (i = 0, cursor = work; i < 3; i++, cursor = (PokeathlonBlockSmash *)((u8 *)cursor + sizeof(String *))) {
        String_Delete(cursor->names[0]);
    }
    MessageFormat_Delete(work->msgFormat);
    DestroyMsgData(work->msgData);
    for (i = 0; i < 4; i++) {
        RemoveWindow(&work->windows[i]);
    }
    for (i = 0; i < 6; i++) {
        FreeBgTilemapBuffer(work->bgConfig, (u8)sPokeathlonBlockSmashBgLayers[i]);
    }
    FontID_Release(4);
    Heap_Free(work->bgConfig);
}

static void PokeathlonBlockSmash_LoadBgGraphics(PokeathlonBlockSmash *work) {
    GfGfxLoader_LoadCharData(NARC_a_2_3_2, 1, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_2, 2, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_2, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, work->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_2, 4, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharData(NARC_a_2_3_2, 4, work->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_2, 5, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnData(NARC_a_2_3_2, 6, work->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPal(NARC_a_2_3_2, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, work->heapId);
}

static void PokeathlonBlockSmash_InitText(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetWork(course);
    int i;
    PokeathlonBlockSmashTeamMon *mons;
    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, work->heapId);
    work->msgFormat = MessageFormat_New(work->heapId);
    for (i = 0; i < 4; i++) {
        AddWindow(work->bgConfig, &work->windows[i], &sPokeathlonBlockSmashWindowTemplates[i]);
        FillWindowPixelBuffer(&work->windows[i], 0);
        ClearWindowTilemap(&work->windows[i]);
    }
    mons = (PokeathlonBlockSmashTeamMon *)PokeathlonCourse_GetParticipantMons(course, PokeathlonCourse_GetNetId(course));
    for (i = 0; i < 3; i++) {
        work->names[i] = String_New(11, work->heapId);
        CopyU16ArrayToString(work->names[i], mons[i].nickname);
    }
}

// The seconds left and the name of the Pokemon next in line.
static void PokeathlonBlockSmash_UpdateTimer(PokeathlonBlockSmash *work) {
    int seconds;
    u8 value;
    GF_ASSERT(work != NULL);
    GF_ASSERT(work->mon != NULL);
    seconds = work->timer / 30;
    if (seconds < 0) {
        seconds = 0;
    }
    value = seconds % 100;
    Sprite_SetAnimCtrlSeq(work->timerDigits[0], (u8)(value / 10) + 1);
    Sprite_SetAnimCtrlSeq(work->timerDigits[1], (u8)(value % 10) + 1);
    u8 next = (PokeathlonBlockSmashMon_GetActiveMemberIndex(work->mon) + 1) % 3;
    BufferString(work->msgFormat, 0, work->names[next], 2, 1, 2);
    PokeathlonBlockSmash_PrintMessage(&work->windows[3], work->msgFormat, work->msgData, msg_0309_00154, work->heapId);
}

static void PokeathlonBlockSmash_PrintInitialText(PokeathlonBlockSmash *work) {
    PokeathlonBlockSmash_PrintMessage(&work->windows[0], work->msgFormat, work->msgData, msg_0309_00301, work->heapId);
    PokeathlonBlockSmash_UpdateTimer(work);
}

static void PokeathlonBlockSmash_PrintMessage(Window *window, MessageFormat *msgFormat, MsgData *msgData, u32 msgId, enum HeapID heapId) {
    String *string;
    FillWindowPixelBuffer(window, 0);
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msgId, heapId);
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    CopyWindowToVram(window);
}

// The four participants' rows on the top screen.
static void PokeathlonBlockSmash_CreateTeamRows(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work;
    int i;
    PokeathlonBlockSmashTeamRow *row;
    int x;
    const PokeathlonBlockSmashRowSprites *sprites;
    int j;
    int y;
    s16 barX;
    work = PokeathlonCourse_GetWork(course);
    GF_ASSERT(work != NULL);
    GF_ASSERT(work->spriteSystem != NULL);
    GF_ASSERT(work->spriteManager != NULL);
    GF_ASSERT(work->rivals != NULL);
    for (i = 0, row = work->icons.rows, x = 120, sprites = sPokeathlonBlockSmashRowSprites; i < 4; row++, x += 32, sprites++, i++) {
        s16 iconX = x;
        row->icon = PokeathlonBlockSmash_NewSubSprite(work->spriteSystem, work->spriteManager, iconX, 152, i + 13, 3);
        PokeathlonBlockSmash_SetTeamRowIcon(course, (u8)i, 0);
        row->switching = PokeathlonBlockSmash_NewSubSprite(work->spriteSystem, work->spriteManager, iconX, 152, 18, 3);
        ManagedSprite_SetDrawFlag(row->switching, FALSE);
        for (j = 0, y = 0; j < 5; y += 16, j++) {
            row->stars[j] = PokeathlonBlockSmash_NewSubSprite(work->spriteSystem, work->spriteManager, (s16)x + 6, (s16)(136 - y) - 20, sprites->star, 44 - j);
            ManagedSprite_SetDrawFlag(row->stars[j], FALSE);
        }
        barX = x;
        for (j = 0; j < 20; j++) {
            row->bars[j] = PokeathlonBlockSmash_NewSubSprite(work->spriteSystem, work->spriteManager, barX, 136 - j * 4, sprites->bar, 24 - j);
            ManagedSprite_SetDrawFlag(row->bars[j], FALSE);
        }
    }
}

static void PokeathlonBlockSmash_DeleteTeamRows(PokeathlonBlockSmash *work) {
    s32 i, j;
    PokeathlonBlockSmashTeamRow *row;
    PokeathlonBlockSmashTeamRow *cursor;
    for (i = 0, row = work->icons.rows; i < 4; i++, row++) {
        GF_ASSERT(row->icon != NULL);
        Sprite_DeleteAndFreeResources(row->icon);
        GF_ASSERT(row->switching != NULL);
        Sprite_DeleteAndFreeResources(row->switching);
        for (j = 0, cursor = row; j < 20; j++, cursor = (PokeathlonBlockSmashTeamRow *)((u8 *)cursor + sizeof(ManagedSprite *))) {
            GF_ASSERT(cursor->bars[0] != NULL);
            Sprite_DeleteAndFreeResources(cursor->bars[0]);
        }
        cursor = NULL;
        j = (s32)row;
        do {
            GF_ASSERT(((PokeathlonBlockSmashTeamRow *)j)->stars[0] != NULL);
            Sprite_DeleteAndFreeResources(((PokeathlonBlockSmashTeamRow *)j)->stars[0]);
            cursor = (PokeathlonBlockSmashTeamRow *)((u8 *)cursor + 1);
            j += sizeof(ManagedSprite *);
        } while ((s32)cursor < 5);
    }
}

// Shows each participant's score as bars (one per 5 blocks) and its active Pokemon.
static void PokeathlonBlockSmash_UpdateTeamRows(PokeathlonCourseData *course) {
    PokeathlonBlockSmashTeamRow *row;
    int participant;
    PokeathlonBlockSmashReport *report;
    GF_ASSERT(course != NULL);
    row = PokeathlonCourse_GetWork(course);
    report = (PokeathlonBlockSmashReport *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course) + 0xF0);
    participant = 0;
    row = ((PokeathlonBlockSmash *)row)->icons.rows;
    do {
        int bars = (int)(report->score / 5u);
        int i;
        if (bars > 20) {
            if (!ManagedSprite_GetDrawFlag(row->stars[0])) {
                for (i = 0; i < 5; i++) {
                    ManagedSprite_SetDrawFlag(row->stars[i], TRUE);
                }
                for (i = 0; i < 20; i++) {
                    ManagedSprite_SetDrawFlag(row->bars[i], FALSE);
                }
            }
            bars -= 20;
        }
        if (bars > 20) {
            bars = 20;
        }
        {
            int j;
            for (j = 0; j < bars; j++) {
                if (!ManagedSprite_GetDrawFlag(row->bars[j])) {
                    ManagedSprite_ResetSpriteAnimCtrlState(row->bars[j]);
                    ManagedSprite_SetDrawFlag(row->bars[j], TRUE);
                }
            }
        }
        if (report->switchingFrame) {
            int member = report->member;
            if (member != row->member) {
                PokeathlonBlockSmash_SetTeamRowIcon(course, (u8)participant, (u8)member);
                row->member = report->member;
            }
            ManagedSprite_SetDrawFlag(row->icon, FALSE);
            ManagedSprite_SetDrawFlag(row->switching, TRUE);
        } else {
            ManagedSprite_SetDrawFlag(row->switching, FALSE);
            ManagedSprite_SetDrawFlag(row->icon, TRUE);
        }
        row++;
        report++;
        participant++;
    } while (participant < 4);
}

static void PokeathlonBlockSmash_SetTeamRowIcon(PokeathlonCourseData *course, u32 participant, u32 member) {
    PokeathlonBlockSmashMonInfo info;
    PokeathlonBlockSmash *work = PokeathlonCourse_GetWork(course);
    PokeathlonBlockSmashTeamIcons *icons = &work->icons;
    PokeathlonCourse_GetMonInfo(course, participant, member, &info);
    PokeathlonCourse_LoadMonIcon(icons->rows[participant].icon, &info, icons->plttId, work->icons.narc, participant << 9, work->heapId);
}

// Swiping the active Pokemon up (from inside the block area unless `anywhere`) switches to the next
// one; the switch button does the same.
static void PokeathlonBlockSmash_HandleSwipe(PokeathlonBlockSmash *work, BOOL anywhere) {
    u32 x, y, heldX, heldY;
    int anim;
    PokeathlonBlockSmashSwipe *swipe;
    PokeathlonBlockSmashMon_GetMode(work->mon);
    swipe = &work->swipe;
    if (System_GetTouchNew()) {
        BOOL inside, inTop, inX;
        System_GetTouchNewCoords(&x, &y);
        inside = FALSE;
        inTop = FALSE;
        inX = FALSE;
        if (x >= 96 && x <= 176) {
            inX = TRUE;
        }
        if (inX && y >= 72) {
            inTop = TRUE;
        }
        if (inTop && y <= 152) {
            inside = TRUE;
        }
        if (anywhere || inside) {
            swipe->startY = y;
            swipe->y = y;
            swipe->frames = 1;
            swipe->active = TRUE;
        }
    } else if (swipe->active && !System_GetTouchHeld()) {
        if ((int)(swipe->startY - swipe->y) >= 32 && PokeathlonBlockSmashMon_CanSwitch(work->mon)) {
            PokeathlonBlockSmashMon_StartSwitch(work->mon);
        }
        swipe->active = FALSE;
    } else if (swipe->active) {
        System_GetTouchHeldCoords(&heldX, &heldY);
        swipe->y = heldY;
        swipe->frames++;
        if (swipe->frames >= 10) {
            swipe->active = FALSE;
        }
    }
    if (System_GetTouchNew() || System_GetTouchHeld() <= 0) {
        if (TouchscreenHitbox_FindRectAtTouchNew(sPokeathlonBlockSmashSwipeHitboxes) != -1) {
            if (PokeathlonBlockSmashMon_CanSwitch(work->mon)) {
                anim = 20;
                PlaySE(SEQ_SE_GS_CHANGE1);
                PokeathlonBlockSmashMon_StartSwitch(work->mon);
            } else {
                anim = 22;
                PlaySE(SEQ_SE_GS_CHANGE2);
            }
        } else if (!PokeathlonBlockSmashMon_CanSwitch(work->mon)) {
            anim = 21;
        } else if (PokeathlonBlockSmashMon_IsTired(work->mon)) {
            anim = 23;
        } else {
            anim = 19;
        }
        ManagedSprite_SetAnimNoRestart(work->switchButton, anim);
    }
}

// Fills in the own report of the link data.
static void PokeathlonBlockSmash_WriteReport(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetWork(course);
    PokeathlonBlockSmashReport *report = (PokeathlonBlockSmashReport *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course));
    PokeathlonBlockSmashMember *member = PokeathlonBlockSmashMon_GetActiveMember(work->mon);
    u32 mode = PokeathlonBlockSmashMon_GetMode(work->mon);
    report->score = PokeathlonBlockSmashBlocks_GetScore(work->blocks);
    report->stamina = member->stamina;
    report->gauge = PokeathlonBlockSmashMon_GetGauge(work->mon);
    report->gaugeFull = PokeathlonBlockSmashMon_IsGaugeFull(work->mon);
    report->switching = mode == 3;
    report->member = PokeathlonBlockSmashMon_GetActiveMemberIndex(work->mon);
}

// The host builds the frame from all reports; every participant then writes its own report.
static void PokeathlonBlockSmash_UpdateLinkData(PokeathlonCourseData *course) {
    BOOL participant = PokeathlonCourse_GetNetId(course);
    PokeathlonBlockSmash *work = PokeathlonCourse_GetWork(course);
    u8 *data = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonBlockSmashReport *out;
    int i;
    GF_ASSERT(work != NULL);
    GF_ASSERT(work->mon != NULL);
    GF_ASSERT(work->blocks != NULL);
    if (participant == 0) {
        out = (PokeathlonBlockSmashReport *)PokeathlonCourse_ToBytes(data + 0x28);
        *(PokeathlonBlockSmashLinkBlock *)PokeathlonCourse_ToBytes(data + 0x50) = *(PokeathlonBlockSmashLinkBlock *)PokeathlonCourse_ToBytes(data);
        data += 0x50;
        for (i = 0; i < 4; i++, data += 0x28, out++) {
            PokeathlonBlockSmashReport *report = (PokeathlonBlockSmashReport *)PokeathlonCourse_ToBytes(data);
            out->score = report->score;
            out->stamina = report->stamina;
            out->gauge = report->gauge;
            out->member = report->member;
            out->gaugeFull = report->gaugeFull;
            out->switchingFrame = report->switching;
        }
    }
    PokeathlonBlockSmash_WriteReport(course);
}

static void PokeathlonBlockSmash_PrintScore(PokeathlonCourseData *course, u32 score) {
    PokeathlonBlockSmash *work;
    MessageFormat *msgFormat;
    MsgData *msgData;
    String *string;
    work = PokeathlonCourse_GetWork(course);
    msgFormat = work->msgFormat;
    msgData = work->msgData;
    PokeathlonCourse_SetRecordScore(course, (u16)score);
    FillWindowPixelBuffer(&work->windows[2], 0);
    BufferIntegerAsString(msgFormat, 0, score, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msg_0309_00159, work->heapId);
    AddTextPrinterParameterizedWithColor(&work->windows[2], 4, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    CopyWindowToVram(&work->windows[2]);
}

// Shakes the touch screen up and down with a shrinking amplitude.
static void PokeathlonBlockSmash_ShakeTask(SysTask *task, void *data) {
    PokeathlonBlockSmash *work = data;
    s32 offset;
    s32 sign;
    if (work->shakeSide) {
        sign = 1;
    } else {
        sign = -1;
    }
    offset = work->shakeOffset * sign;
    ScheduleSetBgPosText(work->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, offset);
    ScheduleSetBgPosText(work->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, offset);
    G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(work->spriteSystem), 0, -offset << FX32_SHIFT);
    work->shakeOffset = (u8)(work->shakeOffset - work->shakeDecay);
    work->shakeSide = (u8)(work->shakeSide ^ 1);
    if (work->shakeOffset <= 0) {
        ScheduleSetBgPosText(work->bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, 16);
        ScheduleSetBgPosText(work->bgConfig, GF_BG_LYR_MAIN_1, BG_POS_OP_SET_Y, 0);
        G2dRenderer_SetMainSurfaceCoords(SpriteSystem_GetRenderer(work->spriteSystem), 0, 0);
        work->shakeTask = NULL;
        SysTask_Destroy(task);
    }
}

static void PokeathlonBlockSmash_StartShake(PokeathlonBlockSmash *work, int offset, int decay) {
    if (work->shakeTask != NULL) {
        SysTask_Destroy(work->shakeTask);
    }
    work->shakeSide = 0;
    work->shakeOffset = (u8)offset;
    work->shakeDecay = (u8)decay;
    work->shakeTask = SysTask_CreateOnMainQueue(PokeathlonBlockSmash_ShakeTask, work, 2);
}
