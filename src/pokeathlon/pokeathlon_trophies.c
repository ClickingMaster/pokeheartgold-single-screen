#include "global.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0314.h"
#include "pokeathlon/pokeathlon_record_screens.h"

#include "gf_gfx_loader.h"
#include "heap.h"
#include "message_format.h"
#include "player_data.h"
#include "poke_overlay.h"
#include "sound_playback.h"
#include "system.h"

FS_EXTERN_OVERLAY(OVY_98);

extern const SpriteResourceCountsListUnion ov99_021E9DEC;
extern const WindowTemplate ov99_021E9ED0[15];
extern const char _021EA5CC[]; // "seq error!"
extern const GraphicsModes ov99_021E9D9C;
extern const PokeathlonTrophiesBgTemplates ov99_021E9E60;
extern const u8 ov99_021E9D88[4];
extern const ManagedSpriteTemplate ov99_021E9E2C;
extern const int ov99_021E9D8C[];
extern const int ov99_021E9DAC[];
extern const int ov99_021E9DBC[];
extern const int ov99_021E9DD4[];
extern const int ov99_021E9E04[10];

BOOL PokeathlonTrophies_Init(OverlayManager *man, int *state) {
    PokeathlonTrophies *work;
    PokeathlonTrophiesArgs *args;
    SpriteResourceCountsListUnion counts;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_98), OVY_LOAD_ASYNC);
    args = OverlayManager_GetArgs(man);
    GF_ASSERT(args != NULL);
    Heap_Create(HEAP_ID_3, HEAP_ID_132, 0x30000);
    work = OverlayManager_CreateAndGetData(man, sizeof(PokeathlonTrophies), HEAP_ID_132);
    MI_CpuFill8(work, 0, sizeof(PokeathlonTrophies));
    work->heapId = HEAP_ID_132;
    PokeathlonTrophies_CreateSummary(work, args);
    PokeathlonUtil_ResetDisplay();
    PokeathlonTrophies_InitBgs(work);
    PokeathlonTrophies_LoadBgGraphics(work);
    work->textContext = PokeathlonUtil_CreateTextContext(work->heapId, work->bgConfig, 15, ov99_021E9ED0, NARC_msg_msg_0314_bin);
    counts = ov99_021E9DEC;
    work->spriteContext = PokeathlonUtil_CreateSpriteContext(work->heapId, &counts, 67);
    PokeathlonTrophies_LoadSpriteResources(work);
    PokeathlonTrophies_DrawScreen(work);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    Main_SetVBlankIntrCB((GFIntrCB)PokeathlonTrophies_VBlankCallback, work);
    return TRUE;
}

BOOL PokeathlonTrophies_Exit(OverlayManager *man, int *state) {
    PokeathlonTrophies *work = OverlayManager_GetData(man);

    PokeathlonTrophies_FreeSummary(work->summary);
    PokeathlonUtil_FreeSpriteContext(work->spriteContext, work->sprites, 67);
    PokeathlonUtil_FreeTextContext(work->textContext, 15);
    PokeathlonTrophies_FreeBgs(work);
    PokeathlonUtil_DisableDisplay();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_132);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    return TRUE;
}

BOOL PokeathlonTrophies_Main(OverlayManager *man, int *state) {
    PokeathlonTrophies *work = OverlayManager_GetData(man);

    switch (*state) {
    case 0:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, work->heapId);
        *state = 1;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            *state = 2;
        }
        break;
    case 2:
        if (PokeathlonUtil_CheckExitInput()) {
            PlaySE(SEQ_SE_DP_SELECT);
            BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, work->heapId);
            *state = 3;
        }
        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            *state = 4;
        }
        break;
    case 4:
        return TRUE;
    default:
        GF_ASSERT(!_021EA5CC);
        break;
    }
    PokeathlonUtil_DrawSprites(work->spriteContext);
    return FALSE;
}

void PokeathlonTrophies_VBlankCallback(PokeathlonTrophies *work) {
    vu8 *dtcm = (vu8 *)0x027E0000;

    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(work->bgConfig);
    *(vu32 *)(dtcm + 0x3FF8) |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK), DTCM address as a literal
}

#pragma force_active on

void PokeathlonTrophies_InitBgs(PokeathlonTrophies *work) {
    GraphicsModes modes;
    PokeathlonTrophiesBgTemplates templates;
    int i;

    work->bgConfig = BgConfig_Alloc(work->heapId);
    modes = ov99_021E9D9C;
    SetBothScreensModesAndDisable(&modes);
    templates = ov99_021E9E60;
    for (i = 0; i < 4; i++) {
        InitBgFromTemplate(work->bgConfig, ov99_021E9D88[i], &templates.templates[i], GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(work->bgConfig, ov99_021E9D88[i]);
        BG_ClearCharDataRange(ov99_021E9D88[i], 32, 0, work->heapId);
    }
    work->narc = NARC_New(NARC_a_1_7_7, work->heapId);
}

#pragma force_active reset

void PokeathlonTrophies_LoadBgGraphics(PokeathlonTrophies *work) {
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 56, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x40, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 57, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 58, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 59, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 60, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 61, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
}

void PokeathlonTrophies_FreeBgs(PokeathlonTrophies *work) {
    u8 i;

    NARC_Delete(work->narc);
    for (i = 0; i < 4; i++) {
        FreeBgTilemapBuffer(work->bgConfig, ov99_021E9D88[i]);
    }
    Heap_Free(work->bgConfig);
}

void PokeathlonTrophies_LoadSpriteResources(PokeathlonTrophies *work) {
    SpriteSystem *spriteSystem = PokeathlonUtil_GetSpriteSystem(work->spriteContext);
    SpriteManager *spriteManager = PokeathlonUtil_GetSpriteManager(work->spriteContext);

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 17, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 16, FALSE, 2, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 18, TRUE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 19, TRUE, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 9, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 4);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 8, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 4);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 9, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 2);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 8, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 2);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 10, TRUE, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 11, TRUE, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 1, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 1);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 0, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 1, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 3);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 0, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 3);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 2, TRUE, 2);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 3, TRUE, 2);
}

void PokeathlonTrophies_CreatePlayerSprite(PokeathlonTrophies *work) {
    ManagedSprite *sprite = PokeathlonUtil_NewSprite(work->spriteContext, &ov99_021E9E2C);
    work->sprites[0] = sprite;
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
}

void PokeathlonTrophies_CreateNumberSprites(PokeathlonTrophies *work, s32 value, u32 index, u32 count, s16 x, s16 y, u8 spacing, BOOL mainScreen) {
    ManagedSpriteTemplate spriteTemplate = { 0 };
    u32 i;
    u32 digits = PokeathlonUtil_CountDecimalDigits(value);

    for (i = 0; i < count; i++) {
        ManagedSprite *sprite;
        spriteTemplate.animation = PokeathlonUtil_GetDecimalDigit(value, (u8)(count - i));
        MI_CpuCopy8(mainScreen ? ov99_021E9DBC : ov99_021E9DD4, spriteTemplate.resIdList, sizeof(spriteTemplate.resIdList));
        spriteTemplate.vram = mainScreen ? NNS_G2D_VRAM_TYPE_2DMAIN : NNS_G2D_VRAM_TYPE_2DSUB;
        spriteTemplate.x = x + i * spacing;
        spriteTemplate.y = y;
        sprite = PokeathlonUtil_NewSprite(work->spriteContext, &spriteTemplate);
        work->sprites[index + i] = sprite;
        ManagedSprite_SetAnimateFlag(sprite, TRUE);
        if (count - digits > i) {
            ManagedSprite_SetDrawFlag(work->sprites[index + i], FALSE);
        }
    }
}

void PokeathlonTrophies_PrintTitle(PokeathlonTrophies *work, u32 trophyCount) {
    GF_ASSERT(trophyCount <= 40);
    PokeathlonUtil_PrintAlignedMessage(work->textContext, 6, trophyCount / 2 + msg_0314_00033, FALSE);
}

void PokeathlonTrophies_PrintNextFriendshipTrophy(PokeathlonTrophies *work, u32 friendshipTrophies) {
    GF_ASSERT(friendshipTrophies <= 10);
    PokeathlonUtil_PrintAlignedMessage(work->textContext, 9, friendshipTrophies + msg_0314_00017, FALSE);
}

void PokeathlonTrophies_DrawScreen(PokeathlonTrophies *work) {
    MessageFormat *msgFormat;
    int gender;
    int medalTotal;
    int eventRecordTotal;
    int courseRecordTotal;
    int totalPoints;
    int collectiveTrophies;
    int trustTrophies;
    int potentialTrophies;
    int friendshipTrophies;
    u8 trophyCount;

    courseRecordTotal = PokeathlonTrophies_GetCourseRecordTotal(work->summary);
    eventRecordTotal = PokeathlonTrophies_GetEventRecordTotal(work->summary);
    medalTotal = PokeathlonTrophies_GetMedalTotal(work->summary);
    totalPoints = PokeathlonTrophies_GetTotalPoints(work->summary);
    gender = PokeathlonTrophies_GetGender(work->summary);
    collectiveTrophies = PokeathlonTrophies_CountCollectiveTrophies(work);
    trustTrophies = PokeathlonUtil_GetMedalistTrophyCount(PokeathlonTrophies_GetMedalistCount(work->summary));
    potentialTrophies = PokeathlonTrophies_GetPotentialTrophyCount(work->summary);
    friendshipTrophies = PokeathlonTrophies_CountFriendshipTrophies(totalPoints);
    trophyCount = collectiveTrophies + trustTrophies + potentialTrophies + friendshipTrophies;

    msgFormat = PokeathlonUtil_GetMessageFormat(work->textContext);
    BufferPlayersName(msgFormat, 0, PokeathlonTrophies_GetProfile(work->summary));
    PokeathlonUtil_PrintMessageAtXY(work->textContext, 0, msg_0314_00011, 1, 0);
    PokeathlonUtil_PrintAlignedMessage(work->textContext, 1, msg_0314_00012, FALSE);
    PokeathlonUtil_PrintAlignedMessage(work->textContext, 2, msg_0314_00013, FALSE);
    PokeathlonUtil_PrintAlignedMessage(work->textContext, 3, msg_0314_00014, FALSE);
    PokeathlonUtil_PrintAlignedMessage(work->textContext, 4, msg_0314_00015, FALSE);
    PokeathlonUtil_PrintMessageAtXY(work->textContext, 5, msg_0314_00032, 2, 0);
    PokeathlonUtil_PrintAlignedMessageAtY(work->textContext, 7, msg_0314_00000, TRUE, 0, 4);
    PokeathlonTrophies_PrintTitle(work, trophyCount);
    PokeathlonUtil_PrintAlignedMessage(work->textContext, 8, msg_0314_00016, FALSE);
    PokeathlonUtil_PrintAlignedMessage(work->textContext, 13, msg_0314_00031, FALSE);
    PokeathlonUtil_PrintAlignedMessage(work->textContext, 14, msg_0314_00001, FALSE);
    PokeathlonUtil_PrintMessageWithNumber(work->textContext, 10, msg_0314_00028, courseRecordTotal, 4, 0);
    PokeathlonUtil_PrintMessageWithNumber(work->textContext, 11, msg_0314_00029, eventRecordTotal, 4, 0);
    PokeathlonUtil_PrintMessageWithNumber(work->textContext, 12, msg_0314_00030, medalTotal, 4, 0);
    PokeathlonTrophies_PrintNextFriendshipTrophy(work, friendshipTrophies);
    PokeathlonTrophies_CreatePlayerSprite(work);
    if (gender == PLAYER_GENDER_FEMALE) {
        ManagedSprite_SetAnim(work->sprites[0], 1);
        ManagedSprite_SetPaletteOverride(work->sprites[0], 1);
    }
    PokeathlonTrophies_CreateNumberSprites(work, trophyCount, 1, 2, 200, 112, 32, TRUE);
    PokeathlonTrophies_CreateNumberSprites(work, totalPoints, 3, 4, 80, 160, 32, FALSE);
    PokeathlonUtil_CreateSpriteRow(work->spriteContext, work->sprites, ov99_021E9DAC, collectiveTrophies, 7, 16, 40, TRUE);
    PokeathlonUtil_CreateSpriteRow(work->spriteContext, work->sprites, ov99_021E9DAC, trustTrophies, 17, 16, 72, TRUE);
    PokeathlonUtil_CreateSpriteRow(work->spriteContext, work->sprites, ov99_021E9DAC, potentialTrophies, 27, 16, 104, TRUE);
    PokeathlonUtil_CreateSpriteRow(work->spriteContext, work->sprites, ov99_021E9DAC, friendshipTrophies, 37, 16, 136, TRUE);
    PokeathlonUtil_CreateSpriteRow(work->spriteContext, work->sprites, ov99_021E9D8C, friendshipTrophies, 57, 96, 120, FALSE);
}

u16 PokeathlonTrophies_CalcEventRecordTotal(PokeathlonSave *pokeathlon) {
    u16 sum = 0;
    u8 i;
    const u16 *records;

    GF_ASSERT(pokeathlon != NULL);
    records = PokeathlonSave_GetUnkAEC(pokeathlon);
    for (i = 0; i < 10; i++) {
        sum += records[i];
    }
    return sum;
}

u16 PokeathlonTrophies_CountMedals(const u8 *medals) {
    int i;
    u16 count = 0;

    GF_ASSERT(medals != NULL);
    for (i = 0; i < 493; i++) {
        count += MATH_CountPopulation(medals[i]);
    }
    return count;
}

void PokeathlonTrophies_CreateSummary(PokeathlonTrophies *work, PokeathlonTrophiesArgs *args) {
    PokeathlonTrophiesSummary *summary;
    PokeathlonSave *pokeathlon;
    PlayerProfile *profile;
    Pokeathlon_RecordsSolo *recordsSolo;
    void *medals;
    PokeathlonCourseRecordEntry *courseRecords;
    int i;

    summary = Heap_Alloc(work->heapId, sizeof(PokeathlonTrophiesSummary));
    MI_CpuFill8(summary, 0, sizeof(PokeathlonTrophiesSummary));
    pokeathlon = args->pokeathlon;
    profile = args->profile;
    courseRecords = (PokeathlonCourseRecordEntry *)PokeathlonSave_dummy1(pokeathlon);
    recordsSolo = PokeathlonSave_GetRecordsSolo(pokeathlon);
    medals = PokeathlonSave_GetUnkDC(pokeathlon);
    for (i = 0; i < 5; i++) {
        u16 score = courseRecords[i].totalScore;
        summary->courseScores[i] = score;
        summary->courseRecordTotal += score;
    }
    summary->profile = profile;
    summary->eventRecordTotal = PokeathlonTrophies_CalcEventRecordTotal(pokeathlon);
    summary->medalTotal = PokeathlonTrophies_CountMedals(medals);
    summary->medalistCount = PokeathlonUtil_CountSpeciesWithAllMedals(medals);
    summary->potentialTrophies = PokeathlonUtil_CountEventTrophies((const u16(*)[22])recordsSolo);
    summary->totalPoints = summary->courseRecordTotal + summary->eventRecordTotal + summary->medalTotal;
    summary->gender = PlayerProfile_GetTrainerGender(profile);
    work->summary = summary;
}

void PokeathlonTrophies_FreeSummary(PokeathlonTrophiesSummary *summary) {
    Heap_Free(summary);
}

u8 PokeathlonTrophies_GetPotentialTrophyCount(PokeathlonTrophiesSummary *summary) {
    GF_ASSERT(summary != NULL);
    return summary->potentialTrophies;
}

u8 PokeathlonTrophies_GetMedalistCount(PokeathlonTrophiesSummary *summary) {
    GF_ASSERT(summary != NULL);
    return summary->medalistCount;
}

u8 PokeathlonTrophies_GetGender(PokeathlonTrophiesSummary *summary) {
    GF_ASSERT(summary != NULL);
    return summary->gender;
}

PlayerProfile *PokeathlonTrophies_GetProfile(PokeathlonTrophiesSummary *summary) {
    GF_ASSERT(summary != NULL);
    return summary->profile;
}

u16 PokeathlonTrophies_GetMedalTotal(PokeathlonTrophiesSummary *summary) {
    GF_ASSERT(summary != NULL);
    return summary->medalTotal;
}

u16 PokeathlonTrophies_GetEventRecordTotal(PokeathlonTrophiesSummary *summary) {
    GF_ASSERT(summary != NULL);
    return summary->eventRecordTotal;
}

u16 PokeathlonTrophies_GetCourseRecordTotal(PokeathlonTrophiesSummary *summary) {
    GF_ASSERT(summary != NULL);
    return summary->courseRecordTotal;
}

u16 PokeathlonTrophies_GetTotalPoints(PokeathlonTrophiesSummary *summary) {
    GF_ASSERT(summary != NULL);
    return summary->totalPoints;
}

u16 PokeathlonTrophies_GetCourseScore(PokeathlonTrophiesSummary *summary, int course) {
    GF_ASSERT(summary != NULL);
    return summary->courseScores[course];
}

u8 PokeathlonTrophies_CountFriendshipTrophies(int totalPoints) {
    const int *thresholds = ov99_021E9E04;
    u8 i = 0;

    do {
        if (thresholds[i] <= totalPoints) {
            break;
        }
        i++;
    } while (i < 10);
    return 10 - i;
}

u8 PokeathlonTrophies_CountCollectiveTrophies(PokeathlonTrophies *work) {
    int i;
    u8 count = 0;

    for (i = 0; i < 5; i++) {
        count += PokeathlonUtil_GetCourseMedal(PokeathlonTrophies_GetCourseScore(work->summary, (u8)i));
    }
    return count;
}
