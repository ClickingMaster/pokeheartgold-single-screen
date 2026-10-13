#include "pokeathlon/pokeathlon_block_smash.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0309.h"
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
#include "sprite_system.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "wireless_status_icon.h"

// Course helpers (OVY_96, not yet tidied).
u8 *PokeathlonCourse_ToBytes(void *block);
void PokeathlonCourse_GetMonInfo(PokeathlonCourseData *course, int participant, int member, void *out);
void *PokeathlonCourse_GetPerformance(PokeathlonCourseData *course, int participant, int member);
u32 PokeathlonCourse_GetSpriteSizeType(void *mon);
void PokeathlonCourse_CreateStartSprites(PokeathlonCourseData *course, int a1, void *resources, void *spriteList, int a4, int a5, const void *position);
BOOL PokeathlonCourse_RunStartCountdown(PokeathlonCourseData *course);
void PokeathlonCourse_UpdateTimeUpCountdown(PokeathlonCourseData *course, int timer);
BOOL PokeathlonCourse_RunFinish(PokeathlonCourseData *course);
void PokeathlonCourse_SetHudPlanes(PokeathlonCourseData *course, int a1);
void **PokeathlonCourse_CreateHudSprites(PokeathlonCourseData *course, int a1, void *resources, void *spriteList);
void PokeathlonCourse_AddMonCounter(PokeathlonCourseData *course, u8 participant, int a2, int a3, int a4);
void PokeathlonCourse_SetRecordScore(PokeathlonCourseData *course, u16 score);
void PokeathlonCourseScoreList_Init(void *graphics);
void PokeathlonCourse_AddScoreEntry(PokeathlonCourseData *course, void *graphics, int participant, int a3, int a4, int a5, int a6);
void PokeathlonCourseScoreList_Rank(void *graphics, int a1);
void PokeathlonCourseScoreList_AddPointsToTotals(void *graphics);
void PokeathlonCourseScoreList_SaveEventPoints(void *graphics, int a1);
void *PokeathlonCourseSprites_CreateResources(enum HeapID heapId, int a1, int a2);
void PokeathlonCourseSprites_FreeResources(void *resources);
void *ov96_021EA374(void *resources, void *spriteList, int a2, enum HeapID heapId);
void *PokeathlonCourseSprites_CreateTimeUpCountdown(void *resources, void *spriteList, int a2, enum HeapID heapId);
void *PokeathlonCourseMonObjects_Create(enum HeapID heapId, int count, int a2, void *resources, void *spriteList);
void PokeathlonCourseMonObjects_Free(void *monPics);
void PokeathlonCourseMonObjects_Load(void *monPics, int count, void *mons, void *params, int a4, int a5);
BOOL PokeathlonCourseMonObjects_IsLoaded(void *monPics);
void PokeathlonCourseObjects_PauseAnims(void *monPics, int a1);
void PokeathlonCourse_LoadMonIcon(ManagedSprite *sprite, void *mon, u16 plttId, NARC *narc, u32 offset, enum HeapID heapId);
void PokeathlonEvent_CalcPoints(void *graphics, int a1);

typedef struct PokeathlonBlockSmashBannerPosition {
    u16 x;
    u16 y;
} PokeathlonBlockSmashBannerPosition;

extern const PokeathlonBlockSmashBannerPosition ov96_0221CC00;
extern const TouchscreenHitbox ov96_0221CC04[];
extern const GraphicsModes ov96_0221CC0C;
extern const OamCharTransferParam ov96_0221CC1C;
extern const SpriteResourceCountsListUnion ov96_0221CC30;
extern const u32 ov96_0221CC48[6];
extern const WindowTemplate ov96_0221CC60[4];
extern const OamManagerParam ov96_0221CC80;
extern const GraphicsBanks ov96_0221CCA0;

typedef struct PokeathlonBlockSmashBgTemplates {
    BgTemplate templates[6];
} PokeathlonBlockSmashBgTemplates;

extern const PokeathlonBlockSmashBgTemplates ov96_0221CCF8;

// sound_system.c (no header declares it)
void GF_SndSetTrackPitchBySeqNo(u16 seqNo, u32 trackMask, int pitch);

// The rest of Block Smash (OVY_96, not yet tidied), declared as the calls were built.
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
PokeathlonBlockSmashRivals *PokeathlonBlockSmashRivals_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonCourseData *course, void *monPics);
void PokeathlonBlockSmashRivals_Delete(PokeathlonBlockSmashRivals *rivals);
void PokeathlonBlockSmashRivals_Update(PokeathlonBlockSmashRivals *rivals);
void PokeathlonBlockSmashRivals_Pause(PokeathlonBlockSmashRivals *rivals);
PokeathlonBlockSmashCpu *PokeathlonBlockSmashCpu_New(PokeathlonCourseData *course, enum HeapID heapId);
void PokeathlonBlockSmashCpu_Delete(PokeathlonBlockSmashCpu *cpu);
void PokeathlonBlockSmashCpu_Update(PokeathlonBlockSmashCpu *cpu);
void PokeathlonBlockSmashCpu_WriteReports(PokeathlonBlockSmashCpu *cpu);
ManagedSprite *PokeathlonBlockSmash_NewMainSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, u16 x, u16 y, u16 anim, u16 priority);
ManagedSprite *PokeathlonBlockSmash_NewSubSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u16 anim, u8 priority);

extern const TouchscreenHitbox *ov96_0221DC68[3];
extern const u8 ov96_0221CDA0[3];
extern const s16 ov96_0221CDC8[4][2]; // x, y of the four weak spot positions
extern const s16 ov96_0221CDCA[];     // their y (the second half of each ov96_0221CDC8 entry)
extern const s16 ov96_0221CDD8[4][2];

// The sprites of a participant's row on the top screen
typedef struct PokeathlonBlockSmashRowSprites {
    u32 unk0;
    u32 bar;
    u32 star;
} PokeathlonBlockSmashRowSprites;

extern const PokeathlonBlockSmashRowSprites ov96_0221CCC8[4];

// The Pennant Capture CPU's cell checks (used by pokeathlon_pennant_capture_cpu.c) are compiled at the
// head of this unit.
BOOL PokeathlonPennantCaptureCpu_IsNearCellTarget(PokeathlonPennantCaptureCpuCell *cell, s32 x, s32 y);
BOOL PokeathlonPennantCaptureCpu_IsCellTargetOnField(PokeathlonPennantCaptureCpuCell *cell);

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

// Event state 0: wait for the first stack, then show the start banner.
BOOL PokeathlonBlockSmash_Intro(PokeathlonCourseData *course, u8 *state) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (*state) {
    case 0:
        PokeathlonBlockSmashBlocks_Update(work->blocks);
        if (PokeathlonBlockSmashBlocks_GetState(work->blocks) == 2) {
            PokeathlonBlockSmashBannerPosition position;
            void *spriteList;
            position = ov96_0221CC00;
            spriteList = SpriteManager_GetSpriteList(work->spriteManager);
            PokeathlonCourse_CreateStartSprites(course, 0, work->iconResources, spriteList, 1, 1, &position);
            (*state)++;
        }
        break;
    case 1:
        if (PokeathlonCourse_RunStartCountdown(course)) {
            PokeathlonCourseObjects_PauseAnims(work->monPics, 0);
            PokeathlonCourse_SetStateField07(course, 1);
        }
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    return FALSE;
}

#pragma force_active on

// Returns TRUE once the host's frame says the event is over.
BOOL PokeathlonBlockSmash_UpdateGame(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetHeapAllocPtr4(course);
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
    PokeathlonBlockSmashMon_Update(work->mon, state == 1);
    if (PokeathlonCourse_GetNetId(course) == 0) {
        PokeathlonBlockSmashCpu_Update(work->cpu);
    }
    PokeathlonBlockSmashRivals_Update(work->rivals);
    touched = PokeathlonBlockSmashBlocks_IsTopBlockTouched(work->blocks);
    PokeathlonBlockSmash_HandleSwipe(work, touched);
    if (PokeathlonBlockSmashMon_CanHit(work->mon) && state == 2 && touched) {
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

#pragma force_active reset

// Event state 1: play until the host ends the event and the shake is over, then fade out.
BOOL PokeathlonBlockSmash_Play(PokeathlonCourseData *course, u8 *state) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    switch (*state) {
    case 0:
        PokeathlonBlockSmash_UpdateLinkData(course);
        PokeathlonBlockSmash_UpdateTeamRows(course);
        if (PokeathlonBlockSmash_UpdateGame(course)) {
            (*state)++;
        }
        break;
    case 1:
        if (PokeathlonCourse_RunFinish(course) && work->shakeTask == NULL) {
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, work->heapId);
            PokeathlonCourse_SetStateField07(course, 2);
        }
        break;
    }
    return FALSE;
}

// Event state 2: after the fade, store the scores (the host also those of the CPU participants).
BOOL PokeathlonBlockSmash_Finish(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    u32 participant;
    s32 count;
    u32 offset;
    u16 score;
    if (IsPaletteFadeFinished()) {
        participant = PokeathlonCourse_GetNetId(course);
        score = PokeathlonBlockSmashBlocks_GetScore(work->blocks);
        PokeathlonCourse_SetField5E0_AtIndex(course, (u8)participant, score);
        if (participant == 0) {
            count = PokeathlonCourse_GetParticipantCount(course);
            if (count < 4) {
                offset = count * 0x28;
                do {
                    u8 *block = PokeathlonCourse_GetDataCopyArea(course) + 0x50 + offset;
                    PokeathlonCourse_SetField5E0_AtIndex(course, (u8)count, ((PokeathlonBlockSmashReport *)PokeathlonCourse_ToBytes(block))->score);
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
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    u32 ids[9];
    u32 unk38[3];
} PokeathlonBlockSmashMonPicParams;

// PokeathlonCourse_GetParticipantUnk04: the participant's team.
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

#pragma force_active on

// Event template entry 0: sets up the event over several frames; TRUE when done.
BOOL PokeathlonBlockSmash_Init(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i, j;
    switch (PokeathlonCourse_GetField1ED(course)) {
    case 0:
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
        work = PokeathlonCourse_AllocPtr4FromHeap(course, sizeof(PokeathlonBlockSmash));
        MI_CpuFill8(work, 0, sizeof(PokeathlonBlockSmash));
        work->heapId = HEAP_ID_141;
        work->timer = 900;
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 1:
        work->bgConfig = BgConfig_Alloc(work->heapId);
        PokeathlonCourse_SetHudPlanes(course, 4);
        PokeathlonBlockSmash_InitBgs(work->bgConfig, work->heapId);
        PokeathlonBlockSmash_InitText(course);
        PokeathlonBlockSmash_LoadBgGraphics(work);
        PokeathlonBlockSmash_InitSpriteSystem(work);
        work->monPics = PokeathlonCourseMonObjects_Create(work->heapId, 9, 32, work->iconResources, SpriteManager_GetSpriteList(work->spriteManager));
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 2: {
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
        params.unk4 = 1;
        params.unkC = 1;
        params.unk10 = 1;
        PokeathlonCourseMonObjects_Load(work->monPics, 9, mons, &params, 0, 0);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    }
    case 3:
        if (PokeathlonCourseMonObjects_IsLoaded(work->monPics)) {
            PokeathlonBlockSmash_LoadSpriteResources(work);
            ov96_02209B04(work->spriteSystem, work->spriteManager);
            Sprite_SetDrawPriority(*PokeathlonCourse_CreateHudSprites(course, 0, work->iconResources, SpriteManager_GetSpriteList(work->spriteManager)), 0);
            PokeathlonCourse_IncrementField1ED(course);
        }
        break;
    case 4:
        PokeathlonBlockSmash_CreateHudSprites(work);
        work->blocks = PokeathlonBlockSmashBlocks_New(work->heapId, work->spriteSystem, work->spriteManager, course);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 5:
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
        PokeathlonCourse_SetField1F4(course, 1);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
        if (PokeathlonCourse_GetNetId(course) == 0) {
            PokeathlonBlockSmashCpu_WriteReports(work->cpu);
        }
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 6, 1, work->heapId);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 6:
        if (PokeathlonCourse_GetNetId(course) == 0) {
            PokeathlonBlockSmash_WriteReport(course);
        }
        PokeathlonBlockSmash_UpdateLinkData(course);
        PokeathlonCourse_IncrementField1ED(course);
        break;
    case 7:
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

#pragma force_active reset

// Event template entry 1.
BOOL PokeathlonBlockSmash_Draw(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    SpriteSystem_DrawSprites(work->spriteManager);
    return TRUE;
}

#pragma force_active on

// Event template entry 2.
BOOL PokeathlonBlockSmash_ShowResults(PokeathlonCourseData *course) {
    void *graphics = PokeathlonCourse_GetGraphicsSystem(course);
    int i;
    PokeathlonCourseScoreList_Init(graphics);
    for (i = 0; i < 4; i++) {
        u16 value = PokeathlonCourse_GetField5F0_AtIndex(course, (u8)i);
        PokeathlonCourse_AddScoreEntry(course, graphics, i, value, 0, 3, 294);
    }
    PokeathlonCourseScoreList_Rank(graphics, 1);
    PokeathlonEvent_CalcPoints(graphics, 3);
    PokeathlonCourseScoreList_AddPointsToTotals(graphics);
    PokeathlonCourseScoreList_SaveEventPoints(graphics, 3);
    return TRUE;
}

// Event template entry 3: frees everything Init created.
BOOL PokeathlonBlockSmash_Exit(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetHeapAllocPtr4(course);
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
    PokeathlonCourse_FreePtr4HeapAlloc(course);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    Heap_Destroy(HEAP_ID_141);
    return TRUE;
}

void PokeathlonBlockSmash_SetVramBanks(void) {
    GraphicsBanks banks = ov96_0221CCA0;
    GfGfx_SetBanks(&banks);
}

void PokeathlonBlockSmash_InitSpriteSystem(PokeathlonBlockSmash *work) {
    SpriteResourceCountsListUnion counts = ov96_0221CC30;
    OamManagerParam oamParam = ov96_0221CC80;
    OamCharTransferParam transferParam = ov96_0221CC1C;
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

#pragma force_active reset

void PokeathlonBlockSmash_FreeSpriteSystem(PokeathlonBlockSmash *work) {
    NARC_Delete(work->icons.narc);
    PokeathlonCourseMonObjects_Free(work->monPics);
    PokeathlonCourseSprites_FreeResources(work->iconResources);
    SpriteSystem_FreeResourcesAndManager(work->spriteSystem, work->spriteManager);
    SpriteSystem_Free(work->spriteSystem);
}

#pragma force_active on

// The timer (top screen), the score frame and the digits.
void PokeathlonBlockSmash_CreateHudSprites(PokeathlonBlockSmash *work) {
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
            cursor->timerDigits[0] = ov96_021EA374(work->iconResources, SpriteManager_GetSpriteList(work->spriteManager), 2, work->heapId);
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

#pragma force_active reset

void PokeathlonBlockSmash_DeleteHudSprites(PokeathlonBlockSmash *work) {
    s32 i;
    GF_ASSERT(work != NULL);
    for (i = 0; i < 2; i++, work = (PokeathlonBlockSmash *)((u8 *)work + sizeof(ManagedSprite *))) {
        GF_ASSERT(work->timerFrame != NULL);
        Sprite_DeleteAndFreeResources(work->timerFrame);
    }
}

#pragma force_active on

void PokeathlonBlockSmash_LoadSpriteResources(PokeathlonBlockSmash *work) {
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

void ov96_02209B04(SpriteSystem *spriteSystem, SpriteManager *spriteManager) {
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
#pragma force_active reset

void PokeathlonBlockSmash_UpdateScoreDigits(PokeathlonBlockSmash *work) {
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

void PokeathlonBlockSmash_StopHudAnims(PokeathlonBlockSmash *work) {
    s32 i;
    for (i = 0; i < 2; i++, work = (PokeathlonBlockSmash *)((u8 *)work + sizeof(ManagedSprite *))) {
        ManagedSprite_SetAnimateFlag(work->timerFrame, FALSE);
    }
}

void PokeathlonBlockSmash_InitBgs(BgConfig *bgConfig, enum HeapID heapId) {
    GraphicsModes modes = ov96_0221CC0C;
    s32 i;
    SetBothScreensModesAndDisable(&modes);
    {
        PokeathlonBlockSmashBgTemplates templates = ov96_0221CCF8;
        for (i = 0; i < 6; i++) {
            InitBgFromTemplate(bgConfig, (u8)ov96_0221CC48[i], &templates.templates[i], GF_BG_TYPE_TEXT);
            BgClearTilemapBufferAndCommit(bgConfig, (u8)ov96_0221CC48[i]);
            BG_ClearCharDataRange((u8)ov96_0221CC48[i], 0x20, 0, heapId);
        }
        FontID_Alloc(4, heapId);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        BgSetPosTextAndCommit(bgConfig, GF_BG_LYR_MAIN_0, BG_POS_OP_SET_Y, 16);
    }
}

void PokeathlonBlockSmash_FreeBgsAndText(PokeathlonBlockSmash *work) {
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
        FreeBgTilemapBuffer(work->bgConfig, (u8)ov96_0221CC48[i]);
    }
    FontID_Release(4);
    Heap_Free(work->bgConfig);
}

void PokeathlonBlockSmash_LoadBgGraphics(PokeathlonBlockSmash *work) {
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

#pragma force_active on

void PokeathlonBlockSmash_InitText(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    int i;
    PokeathlonBlockSmashTeamMon *mons;
    work->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0309_bin, work->heapId);
    work->msgFormat = MessageFormat_New(work->heapId);
    for (i = 0; i < 4; i++) {
        AddWindow(work->bgConfig, &work->windows[i], &ov96_0221CC60[i]);
        FillWindowPixelBuffer(&work->windows[i], 0);
        ClearWindowTilemap(&work->windows[i]);
    }
    mons = (PokeathlonBlockSmashTeamMon *)PokeathlonCourse_GetParticipantUnk04(course, PokeathlonCourse_GetNetId(course));
    for (i = 0; i < 3; i++) {
        work->names[i] = String_New(11, work->heapId);
        CopyU16ArrayToString(work->names[i], mons[i].nickname);
    }
}

// The seconds left and the name of the Pokemon next in line.
void PokeathlonBlockSmash_UpdateTimer(PokeathlonBlockSmash *work) {
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

#pragma force_active reset

void PokeathlonBlockSmash_PrintInitialText(PokeathlonBlockSmash *work) {
    PokeathlonBlockSmash_PrintMessage(&work->windows[0], work->msgFormat, work->msgData, msg_0309_00301, work->heapId);
    PokeathlonBlockSmash_UpdateTimer(work);
}

void PokeathlonBlockSmash_PrintMessage(Window *window, MessageFormat *msgFormat, MsgData *msgData, u32 msgId, enum HeapID heapId) {
    String *string;
    FillWindowPixelBuffer(window, 0);
    string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msgId, heapId);
    AddTextPrinterParameterizedWithColor(window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 14, 0), NULL);
    String_Delete(string);
    CopyWindowToVram(window);
}

#pragma force_active on

// The four participants' rows on the top screen.
void PokeathlonBlockSmash_CreateTeamRows(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work;
    int i;
    PokeathlonBlockSmashTeamRow *row;
    int x;
    const PokeathlonBlockSmashRowSprites *sprites;
    int j;
    int y;
    s16 barX;
    work = PokeathlonCourse_GetHeapAllocPtr4(course);
    GF_ASSERT(work != NULL);
    GF_ASSERT(work->spriteSystem != NULL);
    GF_ASSERT(work->spriteManager != NULL);
    GF_ASSERT(work->rivals != NULL);
    for (i = 0, row = work->icons.rows, x = 120, sprites = ov96_0221CCC8; i < 4; row++, x += 32, sprites++, i++) {
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

#pragma force_active reset

void PokeathlonBlockSmash_DeleteTeamRows(PokeathlonBlockSmash *work) {
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

#pragma force_active on

// Shows each participant's score as bars (one per 5 blocks) and its active Pokemon.
void PokeathlonBlockSmash_UpdateTeamRows(PokeathlonCourseData *course) {
    PokeathlonBlockSmashTeamRow *row;
    int participant;
    PokeathlonBlockSmashReport *report;
    GF_ASSERT(course != NULL);
    row = PokeathlonCourse_GetHeapAllocPtr4(course);
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

#pragma force_active reset

void PokeathlonBlockSmash_SetTeamRowIcon(PokeathlonCourseData *course, u32 participant, u32 member) {
    PokeathlonBlockSmashMonInfo info;
    PokeathlonBlockSmash *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonBlockSmashTeamIcons *icons = &work->icons;
    PokeathlonCourse_GetMonInfo(course, participant, member, &info);
    PokeathlonCourse_LoadMonIcon(icons->rows[participant].icon, &info, icons->plttId, work->icons.narc, participant << 9, work->heapId);
}

#pragma force_active on

// Swiping the active Pokemon up (from inside the block area unless `anywhere`) switches to the next
// one; the switch button does the same.
void PokeathlonBlockSmash_HandleSwipe(PokeathlonBlockSmash *work, BOOL anywhere) {
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
        if (TouchscreenHitbox_FindRectAtTouchNew(ov96_0221CC04) != -1) {
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

#pragma force_active reset

// Fills in the own report of the link data.
void PokeathlonBlockSmash_WriteReport(PokeathlonCourseData *course) {
    PokeathlonBlockSmash *work = PokeathlonCourse_GetHeapAllocPtr4(course);
    PokeathlonBlockSmashReport *report = (PokeathlonBlockSmashReport *)PokeathlonCourse_ToBytes(PokeathlonCourse_GetDataCopyArea(course));
    PokeathlonBlockSmashMember *member = PokeathlonBlockSmashMon_GetActiveMember(work->mon);
    u32 mode = PokeathlonBlockSmashMon_GetMode(work->mon);
    report->score = PokeathlonBlockSmashBlocks_GetScore(work->blocks);
    report->stamina = member->stamina;
    report->gauge = PokeathlonBlockSmashMon_GetGauge(work->mon);
    report->unk1A = PokeathlonBlockSmashMon_IsGaugeFull(work->mon);
    report->switching = mode == 3;
    report->member = PokeathlonBlockSmashMon_GetActiveMemberIndex(work->mon);
}

// The host builds the frame from all reports; every participant then writes its own report.
void PokeathlonBlockSmash_UpdateLinkData(PokeathlonCourseData *course) {
    BOOL participant = PokeathlonCourse_GetNetId(course);
    PokeathlonBlockSmash *work = PokeathlonCourse_GetHeapAllocPtr4(course);
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
            out->unk1A = report->unk1A;
            out->switchingFrame = report->switching;
        }
    }
    PokeathlonBlockSmash_WriteReport(course);
}

void PokeathlonBlockSmash_PrintScore(PokeathlonCourseData *course, u32 score) {
    PokeathlonBlockSmash *work;
    MessageFormat *msgFormat;
    MsgData *msgData;
    String *string;
    work = PokeathlonCourse_GetHeapAllocPtr4(course);
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
void PokeathlonBlockSmash_ShakeTask(SysTask *task, void *data) {
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

void PokeathlonBlockSmash_StartShake(PokeathlonBlockSmash *work, int offset, int decay) {
    if (work->shakeTask != NULL) {
        SysTask_Destroy(work->shakeTask);
    }
    work->shakeSide = 0;
    work->shakeOffset = (u8)offset;
    work->shakeDecay = (u8)decay;
    work->shakeTask = SysTask_CreateOnMainQueue(PokeathlonBlockSmash_ShakeTask, work, 2);
}

PokeathlonBlockSmashBlocks *PokeathlonBlockSmashBlocks_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PokeathlonCourseData *course) {
    PokeathlonBlockSmashBlocks *blocks;
    s32 i;
    blocks = Heap_Alloc(heapId, sizeof(PokeathlonBlockSmashBlocks));
    MI_CpuFill8(blocks, 0, sizeof(PokeathlonBlockSmashBlocks));
    blocks->state = 1;
    blocks->count = 0;
    blocks->spriteSystem = spriteSystem;
    blocks->spriteManager = spriteManager;
    blocks->course = course;
    blocks->hitSprite = PokeathlonBlockSmash_NewMainSprite(spriteSystem, spriteManager, 0, 0, 7, 1);
    ManagedSprite_SetDrawFlag(blocks->hitSprite, FALSE);
    blocks->missSprite = PokeathlonBlockSmash_NewMainSprite(spriteSystem, spriteManager, 0, 0, 18, 1);
    ManagedSprite_SetDrawFlag(blocks->missSprite, FALSE);
    for (i = 0; i < 10; i++) {
        blocks->blocks[i] = PokeathlonBlockSmashBlock_New(heapId, spriteSystem, spriteManager, (u16)i);
    }
    PokeathlonBlockSmashFragments_Init(blocks->fragments, spriteSystem, spriteManager);
    return blocks;
}

void PokeathlonBlockSmashBlocks_Delete(PokeathlonBlockSmashBlocks *blocks) {
    s32 i;
    GF_ASSERT(blocks != NULL);
    GF_ASSERT(blocks->hitSprite != NULL);
    Sprite_DeleteAndFreeResources(blocks->hitSprite);
    Sprite_DeleteAndFreeResources(blocks->missSprite);
    for (i = 0; i < 10; i++) {
        PokeathlonBlockSmashBlock_Delete(blocks->blocks[i]);
    }
    PokeathlonBlockSmashFragments_Delete(blocks->fragments);
    Heap_Free(blocks);
}

void PokeathlonBlockSmashBlocks_Update(PokeathlonBlockSmashBlocks *blocks) {
    GF_ASSERT(blocks != NULL);
    switch (blocks->state) {
    case 1:
        PokeathlonBlockSmashBlocks_UpdateDrop(blocks);
        break;
    case 2:
        PokeathlonBlockSmashBlocks_CheckEmpty(blocks);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
    PokeathlonBlockSmashFragments_Update(blocks->fragments);
}

void PokeathlonBlockSmashBlocks_Pause(PokeathlonBlockSmashBlocks *blocks) {
    s32 i;
    ManagedSprite_SetAnimateFlag(blocks->missSprite, FALSE);
    ManagedSprite_SetAnimateFlag(blocks->hitSprite, FALSE);
    for (i = 0; i < 10; i++) {
        if (blocks->blocks[i]->active) {
            ManagedSprite_SetAnimateFlag(blocks->blocks[i]->sprite, FALSE);
            ManagedSprite_SetAnimateFlag(blocks->blocks[i]->spot, FALSE);
        }
    }
    PokeathlonBlockSmashFragments_Pause(blocks->fragments);
}

// Whether the touch hits the weak spot of the top block (`kind`: the spot's shape).
BOOL PokeathlonBlockSmashBlocks_IsSpotTouched(PokeathlonBlockSmashBlocks *blocks, int kind) {
    GF_ASSERT(blocks != NULL);
    GF_ASSERT(blocks->count != 0);
    if (PokeathlonBlockSmashBlock_IsSpotTouched(blocks->blocks[blocks->count - 1], kind)) {
        return TRUE;
    }
    return FALSE;
}

typedef struct PokeathlonBlockSmashVec {
    fx32 x;
    fx32 y;
    fx32 z;
} PokeathlonBlockSmashVec;

// Whether the new touch is on the top block: within its height and inside the trapezoid
// ov96_0221CDD8 (offsets from the block's position).
BOOL PokeathlonBlockSmashBlocks_IsTopBlockTouched(PokeathlonBlockSmashBlocks *blocks) {
    PokeathlonBlockSmashVec points[4];
    u32 touchX;
    u32 touchY;
    s16 position[2];
    fx32 fixedX;
    fx32 fixedY;
    int i;
    fx32 leftEdgeY;
    fx32 rightEdgeY;
    float converted;
    GF_ASSERT(blocks != NULL);
    if (blocks->state == 1) {
        return FALSE;
    }
    GF_ASSERT(blocks->count != 0);
    PokeathlonBlockSmashBlock_GetPosition(blocks->blocks[blocks->count - 1], &position[1], &position[0]);
    System_GetTouchNewCoords(&touchX, &touchY);
    if (touchY >= position[0] - 18 && touchY <= position[0] + 12) {
        if (touchX != 0) {
            converted = (float)(touchX << FX32_SHIFT) + 0.5f;
        } else {
            converted = (float)(touchX << FX32_SHIFT) - 0.5f;
        }
        fixedX = (s32)converted;
        if (touchY != 0) {
            converted = (float)(touchY << FX32_SHIFT) + 0.5f;
        } else {
            converted = (float)(touchY << FX32_SHIFT) - 0.5f;
        }
        fixedY = (s32)converted;
        for (i = 0; i < 4; i++) {
            if (position[1] + ov96_0221CDD8[i][0] > 0) {
                converted = (float)((position[1] + ov96_0221CDD8[i][0]) << FX32_SHIFT) + 0.5f;
            } else {
                converted = (float)((position[1] + ov96_0221CDD8[i][0]) << FX32_SHIFT) - 0.5f;
            }
            points[i].x = (s32)converted;
            if (position[0] + ov96_0221CDD8[i][1] > 0) {
                converted = (float)((position[0] + ov96_0221CDD8[i][1]) << FX32_SHIFT) + 0.5f;
            } else {
                converted = (float)((position[0] + ov96_0221CDD8[i][1]) << FX32_SHIFT) - 0.5f;
            }
            points[i].y = (s32)converted;
        }
        leftEdgeY = FX_Div(FX_Mul(points[1].y - points[0].y, fixedX - points[0].x), points[1].x - points[0].x);
        leftEdgeY += points[0].y;
        rightEdgeY = FX_Div(FX_Mul(points[3].y - points[2].y, fixedX - points[2].x), points[3].x - points[2].x);
        rightEdgeY += points[2].y;
        if ((fixedX >= points[1].x && fixedX <= points[2].x) || (fixedY >= leftEdgeY && fixedX >= points[0].x && fixedX <= points[1].x) || (fixedY <= rightEdgeY && fixedX >= points[2].x && fixedX <= points[3].x)) {
            return TRUE;
        }
    }
    return FALSE;
}

#pragma force_active on

// A tap on the top block: `power` comes off the blocks from the top down (more on the weak spot or
// a strong hit), broken blocks throw fragments. Returns the number of blocks broken; `*cleared`
// tells whether that emptied the stack.
u16 PokeathlonBlockSmashBlocks_Hit(PokeathlonBlockSmashBlocks *blocks, int member, s16 power, int hitSpot, int strongHit, BOOL *cleared) {
    int count;
    int broken = 0, i, anim;
    int x, y;

    struct {
        s16 y, x;
    } position;

    GF_ASSERT(blocks != NULL);
    if (!blocks->count) {
        return 0;
    }
    System_GetTouchNewCoords((u32 *)&x, (u32 *)&y);
    anim = 7;
    ManagedSprite_SetPositionXY(blocks->hitSprite, (s16)x, (s16)y);
    if (hitSpot || strongHit) {
        anim = 18;
    }
    ManagedSprite_SetAnim(blocks->hitSprite, anim);
    ManagedSprite_SetDrawFlag(blocks->hitSprite, TRUE);
    if (strongHit) {
        power += 150;
        PlaySE(SEQ_SE_GS_KOUKA_H);
    } else if (hitSpot) {
        power += 160;
        PlaySE(SEQ_SE_GS_KOUKA_H);
    }
    count = blocks->count;
    for (i = 0; i < count; i++) {
        PokeathlonBlockSmashBlock *block = blocks->blocks[count - (i + 1)];
        if (power <= 0) {
            break;
        }
        block->health -= power;
        if (block->health > 0) {
            break;
        }
        if (block->health <= 0) {
            blocks->count--;
            blocks->score++;
            if (blocks->score > 200) {
                blocks->score = 200;
            }
            ManagedSprite_SetAnim(block->sprite, 0);
            ManagedSprite_GetPositionXY(block->sprite, &position.x, &position.y);
            ManagedSprite_SetDrawFlag(block->spot, FALSE);
            power = -block->health;
            broken++;
            PokeathlonBlockSmashFragments_Spawn(blocks->fragments, 4, position.x, position.y);
            PokeathlonCourse_AddMonCounter(blocks->course, (u8)PokeathlonCourse_GetNetId(blocks->course), member, 3, 1);
        }
    }
    if (broken) {
        PlaySE(SEQ_SE_GS_W085);
    } else {
        PlaySE(SEQ_SE_GS_W260);
        ManagedSprite_SetPositionXY(blocks->missSprite, (s16)x, (s16)y);
        ManagedSprite_SetAnim(blocks->missSprite, 6);
        ManagedSprite_SetDrawFlag(blocks->missSprite, TRUE);
    }
    if (!broken && blocks->count) {
        PokeathlonBlockSmashBlock *block = blocks->blocks[blocks->count - 1];
        if (block->health <= 20) {
            ManagedSprite_SetAnim(block->sprite, 3);
        } else if (block->health <= 60) {
            ManagedSprite_SetAnim(block->sprite, 2);
            PlaySE(SEQ_SE_GS_W260);
            GF_SndSetTrackPitchBySeqNo(SEQ_SE_GS_W260, 0xFFFF, 64 - ((int)LCRandom() % 128));
        }
    }
    *cleared = broken > 0 && blocks->count == 0;
    return (u16)broken;
}

#pragma force_active reset

u32 PokeathlonBlockSmashBlocks_GetState(PokeathlonBlockSmashBlocks *blocks) {
    GF_ASSERT(blocks != NULL);
    return blocks->state;
}

u16 PokeathlonBlockSmashBlocks_GetScore(PokeathlonBlockSmashBlocks *blocks) {
    GF_ASSERT(blocks != NULL);
    return blocks->score;
}

#pragma force_active on

// Drops the next block every three frames; once all ten are dropped and none is falling any more,
// the stack is ready.
void PokeathlonBlockSmashBlocks_UpdateDrop(PokeathlonBlockSmashBlocks *blocks) {
    int i;
    u32 timer;
    if (blocks->count >= 10) {
        for (i = 0; i < 10; i++) {
            if (PokeathlonBlockSmashBlock_GetState(blocks->blocks[i]) == 1) {
                break;
            }
        }
        if (i == 10) {
            PlaySE(SEQ_SE_GS_KI_GASYAN);
            blocks->state = 2;
        }
    } else {
        timer = blocks->dropTimer;
        blocks->dropTimer++;
        if (timer >= 2) {
            blocks->dropTimer = 0;
            PokeathlonBlockSmashBlock_Drop(blocks->blocks[blocks->count]);
            PlaySE(SEQ_SE_GS_KAWARATUMU);
            blocks->count++;
        }
    }
    for (i = 0; i < 10; i++) {
        PokeathlonBlockSmashBlock_Update(blocks->blocks[i]);
    }
}

#pragma force_active reset

// A cleared stack is rebuilt.
void PokeathlonBlockSmashBlocks_CheckEmpty(PokeathlonBlockSmashBlocks *blocks) {
    if (blocks->count == 0) {
        blocks->state = 1;
    }
}

PokeathlonBlockSmashBlock *PokeathlonBlockSmashBlock_New(enum HeapID heapId, SpriteSystem *spriteSystem, SpriteManager *spriteManager, u32 index) {
    PokeathlonBlockSmashBlock *block;
    u16 priority;
    GF_ASSERT(spriteSystem != NULL);
    GF_ASSERT(spriteManager != NULL);
    block = Heap_Alloc(heapId, sizeof(PokeathlonBlockSmashBlock));
    MI_CpuFill8(block, 0, sizeof(PokeathlonBlockSmashBlock));
    block->index = index;
    priority = (u16)(22 - index * 2);
    block->spot = PokeathlonBlockSmash_NewMainSprite(spriteSystem, spriteManager, 0, 0, 4, priority);
    ManagedSprite_SetDrawFlag(block->spot, FALSE);
    block->sprite = PokeathlonBlockSmash_NewMainSprite(spriteSystem, spriteManager, 120, 0, 1, (u16)(priority + 1));
    ManagedSprite_SetDrawFlag(block->sprite, FALSE);
    ManagedSprite_SetAnimateFlag(block->sprite, TRUE);
    return block;
}

#pragma force_active on

void PokeathlonBlockSmashBlock_Drop(PokeathlonBlockSmashBlock *block) {
    GF_ASSERT(block != NULL);
    block->health = 100;
    block->fallSpeed = 0;
    block->state = 1;
    block->active = TRUE;
    ManagedSprite_SetAnim(block->sprite, 1);
    ManagedSprite_SetPositionXY(block->sprite, 120, 0);
    ManagedSprite_SetDrawFlag(block->sprite, TRUE);
    PokeathlonBlockSmashBlock_MoveSpot(block);
    PokeathlonBlockSmashBlock_UpdateSpot(block);
    ManagedSprite_SetDrawFlag(block->spot, TRUE);
}

#pragma force_active reset

void PokeathlonBlockSmashBlock_Delete(PokeathlonBlockSmashBlock *block) {
    GF_ASSERT(block != NULL);
    GF_ASSERT(block->sprite != NULL);
    GF_ASSERT(block->spot != NULL);
    Sprite_DeleteAndFreeResources(block->sprite);
    Sprite_DeleteAndFreeResources(block->spot);
    Heap_Free(block);
}

#pragma force_active on

// Falls with increasing speed until it lands on the block below (4 pixels per block).
void PokeathlonBlockSmashBlock_Update(PokeathlonBlockSmashBlock *block) {
    if (block->state == 1) {
        u32 bottom = 168 - block->index * 4;
        s16 x, y;
        block->fallSpeed += 20;
        ManagedSprite_OffsetPositionXY(block->sprite, 0, block->fallSpeed);
        ManagedSprite_OffsetPositionXY(block->spot, 0, block->fallSpeed);
        ManagedSprite_GetPositionXY(block->spot, &x, &y);
        ManagedSprite_GetPositionXY(block->sprite, &x, &y);
        if ((u32)y >= bottom) {
            y = bottom;
            ManagedSprite_SetPositionXY(block->sprite, x, y);
            PokeathlonBlockSmashBlock_UpdateSpot(block);
            block->state = 2;
        }
    }
}

// Moves the weak spot to one of the other three positions, with some randomness.
void PokeathlonBlockSmashBlock_MoveSpot(PokeathlonBlockSmashBlock *block) {
    u8 slot;
    GF_ASSERT(block != NULL);
    slot = MTRandom() % 3;
    if (slot >= block->spotSlot) {
        slot++;
    }
    block->spotSlot = slot;
    block->spotX = ov96_0221CDC8[block->spotSlot][0] + (MTRandom() & 15);
    block->spotY = ov96_0221CDCA[block->spotSlot * 2] + MTRandom() % 12;
}

#pragma force_active reset

void PokeathlonBlockSmashBlock_UpdateSpot(PokeathlonBlockSmashBlock *block) {
    s16 position[2];
    GF_ASSERT(block != NULL);
    PokeathlonBlockSmashBlock_GetPosition(block, &position[1], &position[0]);
    position[1] += block->spotX;
    position[0] += block->spotY;
    ManagedSprite_SetPositionXY(block->spot, position[1], position[0]);
}

#pragma force_active on

// The touch areas of the weak spot (ov96_0221DC68[kind], ov96_0221CDA0[kind] entries) around it.
BOOL PokeathlonBlockSmashBlock_IsSpotTouched(PokeathlonBlockSmashBlock *block, int kind) {
    TouchscreenHitbox hitboxes[3] = {
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }
    };

    struct {
        s16 x, y;
    } position;

    u32 count;
    const TouchscreenHitbox *area;
    int i;
    GF_ASSERT(kind < 3);
    area = ov96_0221DC68[kind];
    count = (u8)(ov96_0221CDA0[kind] - 1);
    ManagedSprite_GetPositionXY(block->spot, &position.y, &position.x);
    i = 0;
    if ((s32)count > 0) {
        do {
            hitboxes[i].rect.top = position.x + area->rect.top;
            hitboxes[i].rect.bottom = position.x + area->rect.bottom;
            hitboxes[i].rect.left = position.y + area->rect.left;
            hitboxes[i].rect.right = position.y + area->rect.right;
            i++;
            area++;
        } while (i < (s32)count);
    }
    hitboxes[count].rect.top = TOUCHSCREEN_RECTLIST_END;
    if (TouchscreenHitbox_FindRectAtTouchNew(hitboxes) != -1) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

void PokeathlonBlockSmashBlock_GetPosition(PokeathlonBlockSmashBlock *block, s16 *x, s16 *y) {
    GF_ASSERT(block != NULL);
    ManagedSprite_GetPositionXY(block->sprite, x, y);
}

#pragma force_active on

u32 PokeathlonBlockSmashBlock_GetState(PokeathlonBlockSmashBlock *block) {
    GF_ASSERT(block != NULL);
    return block->state;
}

void PokeathlonBlockSmashFragments_Init(PokeathlonBlockSmashFragment *fragments, SpriteSystem *spriteSystem, SpriteManager *spriteManager) {
    int i;
    for (i = 0; i < 16; i++, fragments++) {
        fragments->sprite = PokeathlonBlockSmash_NewMainSprite(spriteSystem, spriteManager, 0, 0, (u16)(i % 3 + 28), 1);
        ManagedSprite_SetDrawFlag(fragments->sprite, FALSE);
    }
}

#pragma force_active reset

void PokeathlonBlockSmashFragments_Delete(PokeathlonBlockSmashFragment *fragments) {
    s32 i;
    for (i = 0; i < 16; i++, fragments++) {
        GF_ASSERT(fragments->sprite != NULL);
        Sprite_DeleteAndFreeResources(fragments->sprite);
    }
}

#pragma force_active on

// Throws `count` free fragments from (x, y), alternately to the right and to the left.
void PokeathlonBlockSmashFragments_Spawn(PokeathlonBlockSmashFragment *fragments, u8 count, s16 x, s16 y) {
    int i;
    int spawned = 0;
    for (i = 0; i < 16; i++, fragments++) {
        if (fragments->active == 0) {
            fragments->active = 1;
            fragments->x = x;
            fragments->y = y;
            fragments->velocityX = FX32_CONST(MTRandom() % 8 + 4) * (i % 2 == 0 ? 1 : -1);
            fragments->velocityY = FX32_CONST(MTRandom() % 8 + 8);
            ManagedSprite_SetPositionXY(fragments->sprite, x, y);
            ManagedSprite_ResetSpriteAnimCtrlState(fragments->sprite);
            ManagedSprite_SetDrawFlag(fragments->sprite, TRUE);
            spawned++;
        }
        if (spawned >= count) {
            break;
        }
    }
}

// TRUE once the fragment left the screen.
BOOL PokeathlonBlockSmashFragment_Update(PokeathlonBlockSmashFragment *fragment) {
    fragment->x += (fragment->velocityX >> FX32_SHIFT);
    fragment->velocityX = (fragment->velocityX << 2) / 3;
    fragment->y -= (fragment->velocityY >> FX32_SHIFT);
    fragment->velocityY -= 4;
    ManagedSprite_SetPositionXY(fragment->sprite, fragment->x, fragment->y);
    if (fragment->x < 0 || fragment->x > 256) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active reset

void PokeathlonBlockSmashFragments_Update(PokeathlonBlockSmashFragment *fragments) {
    s32 i;
    for (i = 0; i < 16; i++, fragments++) {
        if (fragments->active == 1 && PokeathlonBlockSmashFragment_Update(fragments)) {
            ManagedSprite_SetDrawFlag(fragments->sprite, FALSE);
            fragments->active = 0;
        }
    }
}

void PokeathlonBlockSmashFragments_Pause(PokeathlonBlockSmashFragment *fragments) {
    s32 i;
    for (i = 0; i < 16; i++, fragments++) {
        if (fragments->active == 1) {
            ManagedSprite_SetAnimateFlag(fragments->sprite, FALSE);
        }
    }
}
