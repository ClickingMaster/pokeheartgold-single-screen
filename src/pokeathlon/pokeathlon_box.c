#include "pokeathlon/pokeathlon_box.h"

#include "constants/species.h"

#include "font.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "pokemon.h"
#include "save_arrays.h"
#include "screen_fade.h"
#include "system.h"
#include "text.h"
#include "wireless_status_icon.h"

enum {
    BOX_STATE_START,
    BOX_STATE_FADE_IN,
    BOX_STATE_WAIT_FADE_IN,
    BOX_STATE_SELECT,
    BOX_STATE_FADE_OUT,
    BOX_STATE_WAIT_FADE_OUT,
};

// Functions in assembly
int BoxMonSelector_Run(int);
void BoxMonSelector_RenderSprites(u32);
void BoxMonSelector_Delete(u32);
u32 BoxMonSelector_New(u32, u32, u32);
void BoxMonSelector_Init(u32, BgConfig *, PCStorage *, Party *, u32, u32, BOOL, u32, void *, void *, void *, void *, PokeathlonBox *);
void BoxMonSelector_VBlank(u32);
s8 PokemonSummary_GetPokeathlonStarStates(void *, u8, u8, u32, s8);

static BOOL PokeathlonBox_RunSetup(OverlayManager *manager);
static BOOL PokeathlonBox_Setup(OverlayManager *manager);
static void PokeathlonBox_SetGraphicsBanks();
static void PokeathlonBox_SetGraphicsModes(BgConfig *bgConfig);
static void PokeathlonBox_SetObjCharTransferTemplate();
static void PokeathlonBox_CreateOamManager(PokeathlonBox *data);
static void PokeathlonBox_VBlankCallback(void *data);
static BOOL PokeathlonBox_GetLightBoxMon(PCStorage *storage, u32 boxno, u32 slotno, PokeathlonBox_BoxMon *ptr);
static void PokeathlonBox_GetBoxName(String *dest, PCStorage *storage, u32 boxno);
static void PokeathlonBox_CopySelectedMons(PokeathlonBox *data, PokeathlonBoxArgs *args);
static void PokeathlonBox_GetPartyMonStats(Party *party, u8 slot, PokeathlonBox_MonStats *stats);
static void PokeathlonBox_GetBoxMonStats(BoxPokemon *boxMon, PokeathlonBox_MonStats *stats);
static void PokeathlonBox_ShowMonInfo(int boxno, u8 slot, PokeathlonBox *data);
static void PokeathlonBox_CalcStatChanges(PokeathlonBox_MonStats *stats1, PokeathlonBox_MonStats *stats2, PartyAprijuiceModifier *aprijuiceModifier, PokeathlonBox_MonInfo *mon);

static const ObjCharTransferTemplate sPokeathlonBoxObjCharTransferTemplate = {
    .maxTasks = 40,
    .sizeMain = 0x10000,
    .sizeSub = 0x4000,
    .heapID = HEAP_ID_POKEATHLON,
};

static const GraphicsModes sPokeathlonBoxGraphicsModes = {
    .dispMode = GX_DISPMODE_GRAPHICS,
    .bgMode = GX_BGMODE_0,
    .subMode = GX_BGMODE_0,
    ._2d3dMode = GX_BG0_AS_2D,
};

static const u32 sPokeathlonBoxMedalShifts[5] = { 0, 1, 2, 3, 4 };

static const GraphicsBanks sPokeathlonBoxGraphicsBanks = {
    .bg = GX_VRAM_BG_128_A,
    .bgextpltt = GX_VRAM_BGEXTPLTT_NONE,
    .subbg = GX_VRAM_SUB_BG_128_C,
    .subbgextpltt = GX_VRAM_SUB_BGEXTPLTT_NONE,
    .obj = GX_VRAM_OBJ_64_E,
    .objextpltt = GX_VRAM_OBJEXTPLTT_NONE,
    .subobj = GX_VRAM_SUB_OBJ_16_I,
    .subobjextpltt = GX_VRAM_SUB_OBJEXTPLTT_NONE,
    .tex = GX_VRAM_TEX_0_B,
    .texpltt = GX_VRAM_TEXPLTT_01_FG,
};

BOOL PokeathlonBox_Init(OverlayManager *manager) {
    if (PokeathlonBox_RunSetup(manager)) {
        return TRUE;
    }
    return FALSE;
}

BOOL PokeathlonBox_Main(OverlayManager *manager) {
    PokeathlonBox *data = OverlayManager_GetData(manager);

    switch (data->state) {
    case BOX_STATE_START:
        data->state = BOX_STATE_FADE_IN;
        break;

    case BOX_STATE_FADE_IN:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_POKEATHLON);
        data->state = BOX_STATE_WAIT_FADE_IN;
        break;

    case BOX_STATE_WAIT_FADE_IN:
        if (IsPaletteFadeFinished()) {
            data->state = BOX_STATE_SELECT;
        }
        break;

    case BOX_STATE_SELECT:
        int res = BoxMonSelector_Run(data->monSelector);
        if (res == 2) {
            data->cancelled = FALSE;
            data->state = BOX_STATE_FADE_OUT;
        } else if (res == 3) {
            data->cancelled = TRUE;
            data->state = BOX_STATE_FADE_OUT;
        }
        break;

    case BOX_STATE_FADE_OUT:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_POKEATHLON);
        data->state = BOX_STATE_WAIT_FADE_OUT;
        break;

    case BOX_STATE_WAIT_FADE_OUT:
        if (IsPaletteFadeFinished()) {
            PokeathlonBoxArgs *args = OverlayManager_GetArgs(manager);
            PokeathlonBox_CopySelectedMons(data, args);
            return TRUE;
        }
        break;
    }

    BoxMonSelector_RenderSprites(data->monSelector);
    PokeathlonBox_RenderAndAnimateSprites(data->graphics);
    return FALSE;
}

BOOL PokeathlonBox_Exit(OverlayManager *manager, int *state) {
    PokeathlonBox *data = OverlayManager_GetData(manager);

    Main_SetVBlankIntrCB(NULL, NULL);
    PokeathlonBox_ReleaseGraphics(data->graphics);
    BoxMonSelector_Delete(data->monSelector);
    FontID_Release(2);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    Heap_Free(data->bgConfig);
    Heap_Free(data);

    return TRUE;
}

static BOOL PokeathlonBox_RunSetup(OverlayManager *manager) {
    PokeathlonBoxArgs *args = OverlayManager_GetArgs(manager);

    do {
        if (PokeathlonBox_Setup(manager)) {
            return TRUE;
        }
    } while (args->courseArgs->mode == 0);

    return FALSE;
}

static BOOL PokeathlonBox_Setup(OverlayManager *manager) {
    PokeathlonBox *data;
    PokeathlonCourseArgs *courseArgs;
    PokeathlonBoxArgs *args;

    args = OverlayManager_GetArgs(manager);
    courseArgs = args->courseArgs;
    data = OverlayManager_CreateAndGetData(manager, sizeof(PokeathlonBox), HEAP_ID_POKEATHLON);

    memset(data, 0, sizeof(PokeathlonBox));

    data->bgConfig = BgConfig_Alloc(HEAP_ID_POKEATHLON);
    data->saveData = courseArgs->saveData;
    data->courseId = args->courseId;

    PokeathlonBox_SetGraphicsBanks();

    data->monSelector = BoxMonSelector_New(HEAP_ID_POKEATHLON, HEAP_ID_3, 0x13);
    data->graphics = PokeathlonBox_InitGraphics(HEAP_ID_POKEATHLON);

    PokeathlonBox_SetGraphicsModes(data->bgConfig);
    PokeathlonBox_SetObjCharTransferTemplate();

    PokeathlonBox_CreateOamManager(data);

    BOOL isMode0 = FALSE;
    if (args->courseArgs->mode == 0) {
        isMode0 = TRUE;
    }

    PCStorage *pcStorage = SaveArray_PCStorage_Get(courseArgs->saveData);
    Party *party = SaveArray_Party_Get(courseArgs->saveData);
    BoxMonSelector_Init(data->monSelector, data->bgConfig, pcStorage, party, 0, 0, isMode0, 18, &PokeathlonBox_GetLightBoxMon, &PokeathlonBox_GetBoxName, data->selectedMons, &PokeathlonBox_ShowMonInfo, data);

    WirelessStatusIcon_CreateOnScreenIfConnected(2);
    PokeathlonBox_SetupGraphics(data->graphics, data->bgConfig, data->courseId, courseArgs->isSupreme);
    Main_SetVBlankIntrCB(&PokeathlonBox_VBlankCallback, data);
    data->state = BOX_STATE_START;

    ResetAllTextPrinters();
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_POKEATHLON);
    FontID_Alloc(2, HEAP_ID_POKEATHLON);

    return TRUE;
}

static void PokeathlonBox_SetGraphicsBanks() {
    GraphicsBanks banks = sPokeathlonBoxGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

static void PokeathlonBox_SetGraphicsModes(BgConfig *bgConfig) {
    GraphicsModes modes = sPokeathlonBoxGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
}

static void PokeathlonBox_SetObjCharTransferTemplate() {
    ObjCharTransferTemplate template = sPokeathlonBoxObjCharTransferTemplate;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(0x14, HEAP_ID_POKEATHLON);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

static void PokeathlonBox_CreateOamManager(PokeathlonBox *data) {
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 1, 126, 0, 32, HEAP_ID_POKEATHLON);
}

static void PokeathlonBox_VBlankCallback(void *data) {
    BoxMonSelector_VBlank(((PokeathlonBox *)data)->monSelector);
}

static BOOL PokeathlonBox_GetBoxMon(PCStorage *storage, u32 boxno, u32 slotno, PokeathlonBox_BoxMon *ptr) {
    BoxPokemon *boxMon = PCStorage_GetMonByIndexPair(storage, boxno, slotno);
    if (GetBoxMonData(boxMon, MON_DATA_SPECIES_EXISTS, NULL)) {
        ptr->species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
        ptr->personality = GetBoxMonData(boxMon, MON_DATA_PERSONALITY, NULL);
        ptr->isEgg = GetBoxMonData(boxMon, MON_DATA_IS_EGG, NULL);
        ptr->form = GetBoxMonData(boxMon, MON_DATA_FORM, NULL);
        ptr->unkC = 0;
        ptr->unkE = 0;
        GetBoxMonData(boxMon, MON_DATA_NICKNAME, ptr->nickname);
        ptr->isShiny = BoxMonIsShiny(boxMon);
        ptr->gender = GetBoxMonData(boxMon, MON_DATA_GENDER, NULL);
        return TRUE;
    }
    ptr->species = SPECIES_NONE;
    ptr->personality = 0;
    ptr->isEgg = 0;
    ptr->form = 0;
    ptr->unkC = 0;
    ptr->unkE = 0;
    ptr->isShiny = 0;
    ptr->gender = 0;
    return FALSE;
}

static BOOL PokeathlonBox_GetLightBoxMon(PCStorage *storage, u32 boxno, u32 slotno, PokeathlonBox_BoxMon *ptr) {
    BoxPokemon *boxMon = PCStorage_GetMonByIndexPair(storage, boxno, slotno);
    if (GetBoxMonData(boxMon, MON_DATA_SPECIES_EXISTS, NULL)) {
        ptr->species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
        ptr->personality = 0;
        ptr->isEgg = GetBoxMonData(boxMon, MON_DATA_IS_EGG, NULL);
        ptr->form = GetBoxMonData(boxMon, MON_DATA_FORM, NULL);
        ptr->unkC = 0;
        ptr->unkE = 0;
        ptr->isShiny = 0;
        ptr->gender = 0;
        return TRUE;
    }
    ptr->species = SPECIES_NONE;
    ptr->personality = 0;
    ptr->isEgg = 0;
    ptr->form = 0;
    ptr->unkC = 0;
    ptr->unkE = 0;
    ptr->isShiny = 0;
    ptr->gender = 0;
    return FALSE;
}

static void PokeathlonBox_GetBoxName(String *dest, PCStorage *storage, u32 boxno) {
    PCStorage_GetBoxName(storage, boxno, dest);
}

static void PokeathlonBox_CopySelectedMons(PokeathlonBox *data, PokeathlonBoxArgs *args) {
    PokeathlonBox_BoxMon boxMon;

    args->cancelled = data->cancelled;
    if (data->cancelled == 0) {
        for (int i = 0; i < 3; i++) {
            if (data->selectedMons[i].boxno == NUM_BOXES) {
                Pokemon *partyMon = Party_GetMonByIndex(SaveArray_Party_Get(args->courseArgs->saveData), data->selectedMons[i].slot);
                args->selectedMons[i].species = GetMonData(partyMon, MON_DATA_SPECIES, NULL);
                args->selectedMons[i].personality = GetMonData(partyMon, MON_DATA_PERSONALITY, NULL);
                args->selectedMons[i].form = GetMonData(partyMon, MON_DATA_FORM, NULL);
                GetMonData(partyMon, MON_DATA_NICKNAME, &args->selectedMons[i].nickname);
                args->selectedMons[i].isShiny = MonIsShiny(partyMon);
                args->selectedMons[i].gender = GetMonData(partyMon, MON_DATA_GENDER, NULL);
                PokeathlonBox_GetPartyMonStats(SaveArray_Party_Get(args->courseArgs->saveData), data->selectedMons[i].slot, &args->selectedMons[i].stats);
            } else {
                PokeathlonBox_GetBoxMon(SaveArray_PCStorage_Get(args->courseArgs->saveData), data->selectedMons[i].boxno, data->selectedMons[i].slot, &boxMon);
                args->selectedMons[i].species = boxMon.species;
                args->selectedMons[i].personality = boxMon.personality;
                args->selectedMons[i].form = boxMon.form;
                CopyU16StringArrayN(args->selectedMons[i].nickname, boxMon.nickname, 11);
                args->selectedMons[i].isShiny = boxMon.isShiny;
                args->selectedMons[i].gender = boxMon.gender;
                PokeathlonBox_GetBoxMonStats(PCStorage_GetMonByIndexPair(SaveArray_PCStorage_Get(args->courseArgs->saveData), data->selectedMons[i].boxno, data->selectedMons[i].slot), &args->selectedMons[i].stats);
            }
        }
    }
}

static void PokeathlonBox_GetPartyMonStats(Party *party, u8 slot, PokeathlonBox_MonStats *stats) {
    PartyAprijuiceModifier aprijuiceModifier;
    PokeathlonPerformanceStars stars;

    Party_GetMonAprijuiceModifiers(party, &aprijuiceModifier, slot);
    CalcMonPokeathlonStars(&stars, Party_GetMonByIndex(party, slot), aprijuiceModifier.stats, HEAP_ID_POKEATHLON);
    stats->power = ((u32)(stars.stars << 0x1D) >> 0x1D);
    stats->stamina = ((u32)(stars.stars << 0x11) >> 0x1D);
    stats->jump = ((u32)(stars.stars << 0x14) >> 0x1D);
    stats->skill = ((u32)(stars.stars << 0x1A) >> 0x1D);
    stats->speed = ((u32)(stars.stars << 0x17) >> 0x1D);
    stats->starColors = (stats->starColors & ~7) | (7 & stars.color[PERFORMANCE_POWER]);
    stats->starColors = (stats->starColors & ~0x38) | ((u32)(stars.color[PERFORMANCE_STAMINA] << 0x1D) >> 0x1A);
    stats->starColors = (stats->starColors & 0xFFFFFE3F) | ((u32)(stars.color[PERFORMANCE_JUMP] << 0x1D) >> 0x17);
    stats->starColors = (stats->starColors & 0xFFFFF1FF) | ((u32)(stars.color[PERFORMANCE_SKILL] << 0x1D) >> 0x14);
    stats->starColors = (stats->starColors & 0xFFFF8FFF) | ((u32)(stars.color[PERFORMANCE_SPEED] << 0x1D) >> 0x11);
}

static void PokeathlonBox_GetBoxMonStats(BoxPokemon *boxMon, PokeathlonBox_MonStats *stats) {
    PokeathlonPerformanceStars stars;

    CalcBoxmonPokeathlonStars(&stars, boxMon, NULL, HEAP_ID_POKEATHLON);
    stats->power = ((u32)(stars.stars << 0x1D) >> 0x1D);
    stats->stamina = ((u32)(stars.stars << 0x11) >> 0x1D);
    stats->jump = ((u32)(stars.stars << 0x14) >> 0x1D);
    stats->skill = ((u32)(stars.stars << 0x1A) >> 0x1D);
    stats->speed = ((u32)(stars.stars << 0x17) >> 0x1D);
    stats->starColors = (stats->starColors & ~7) | (7 & stars.color[PERFORMANCE_POWER]);
    stats->starColors = (stats->starColors & ~0x38) | ((u32)(stars.color[PERFORMANCE_STAMINA] << 0x1D) >> 0x1A);
    stats->starColors = (stats->starColors & 0xFFFFFE3F) | ((u32)(stars.color[PERFORMANCE_JUMP] << 0x1D) >> 0x17);
    stats->starColors = (stats->starColors & 0xFFFFF1FF) | ((u32)(stars.color[PERFORMANCE_SKILL] << 0x1D) >> 0x14);
    stats->starColors = (stats->starColors & 0xFFFF8FFF) | ((u32)(stars.color[PERFORMANCE_SPEED] << 0x1D) >> 0x11);
}

static void PokeathlonBox_GetLightBoxMonStats(BoxPokemon *boxMon, PokeathlonBox_MonStats *stats) {
    PokeathlonTodayPerformance performance;

    CalcBoxMonPokeathlonPerformance(boxMon, &performance);
    stats->power = performance.stats[PERFORMANCE_POWER].hi; // from the "<< 0x1A) >> 0x1D" ones
    stats->stamina = performance.stats[PERFORMANCE_STAMINA].hi;
    stats->jump = performance.stats[PERFORMANCE_JUMP].hi;
    stats->skill = performance.stats[PERFORMANCE_SKILL].hi;
    stats->speed = performance.stats[PERFORMANCE_SPEED].hi;
}

static void PokeathlonBox_ShowMonInfo(int boxno, u8 slot, PokeathlonBox *data) {
    PokeathlonBox_MonInfo monInfo;
    u32 medalShifts[5];

    PokeathlonBox_MonStats partyMonStats;
    PokeathlonBox_MonStats partyLightMonStats;
    PartyAprijuiceModifier aprijuiceModifier;

    PokeathlonBox_MonStats boxMonStats;
    PokeathlonBox_MonStats boxLightMonStats;
    PokeathlonBox_BoxMon pokeathlonBoxMon;

    PartyAprijuiceModifier emptyAprijuiceModifier;

    medalShifts = sPokeathlonBoxMedalShifts;
    if (boxno != -1 && slot != -1) {
        monInfo.displayMode = 2;
        if (boxno == NUM_BOXES) {
            Party *party = SaveArray_Party_Get(data->saveData);
            Pokemon *partyMon = Party_GetMonByIndex(party, slot);
            monInfo.personality = GetMonData(partyMon, MON_DATA_PERSONALITY, NULL);
            monInfo.species = GetMonData(partyMon, MON_DATA_SPECIES, NULL);
            monInfo.form = GetMonData(partyMon, MON_DATA_FORM, NULL);
            GetMonData(partyMon, MON_DATA_NICKNAME, monInfo.nickname);
            monInfo.isShiny = MonIsShiny(partyMon);
            monInfo.gender = GetMonData(partyMon, MON_DATA_GENDER, NULL);
            BoxPokemon *boxMon = Mon_GetBoxMon(Party_GetMonByIndex(party, slot));
            PokeathlonBox_GetPartyMonStats(party, (u32)slot, &partyMonStats);
            PokeathlonBox_GetLightBoxMonStats(boxMon, &partyLightMonStats);
            Party_GetMonAprijuiceModifiers(party, &aprijuiceModifier, slot);
            PokeathlonBox_CalcStatChanges(&partyLightMonStats, &partyMonStats, &aprijuiceModifier, &monInfo);
            for (u8 i = 0; i < 5; i++) {
                if (PokeathlonSave_MonHasMedal(Save_Pokeathlon_Get(data->saveData), medalShifts[i], monInfo.species)) {
                    monInfo.medals[i] = 1;
                } else {
                    monInfo.medals[i] = 0;
                }
            }
            PokeathlonBox_DrawMonInfo(data->graphics, &monInfo);
            return;
        }

        PokeathlonBox_GetBoxMon(SaveArray_PCStorage_Get(data->saveData), boxno, slot, &pokeathlonBoxMon);
        monInfo.personality = pokeathlonBoxMon.personality;
        monInfo.species = pokeathlonBoxMon.species;
        monInfo.form = pokeathlonBoxMon.form;
        CopyU16StringArrayN(monInfo.nickname, pokeathlonBoxMon.nickname, 11);
        monInfo.isShiny = pokeathlonBoxMon.isShiny;
        monInfo.gender = pokeathlonBoxMon.gender;
        BoxPokemon *boxMon = PCStorage_GetMonByIndexPair(SaveArray_PCStorage_Get(data->saveData), boxno, slot);
        PokeathlonBox_GetBoxMonStats(boxMon, &boxMonStats);
        PokeathlonBox_GetLightBoxMonStats(boxMon, &boxLightMonStats);
        MI_CpuFill8(&emptyAprijuiceModifier, 0, 5);
        PokeathlonBox_CalcStatChanges(&boxLightMonStats, &boxMonStats, &emptyAprijuiceModifier, &monInfo);
        for (u8 i = 0; i < 5; i++) {
            if (PokeathlonSave_MonHasMedal(Save_Pokeathlon_Get(data->saveData), medalShifts[i], monInfo.species)) {
                monInfo.medals[i] = 1;
            } else {
                monInfo.medals[i] = 0;
            }
        }
        PokeathlonBox_DrawMonInfo(data->graphics, &monInfo);
        return;
    }

    monInfo.displayMode = 0;
    PokeathlonBox_DrawMonInfo(data->graphics, &monInfo);
}

static void PokeathlonBox_CalcStatChanges(PokeathlonBox_MonStats *stats1, PokeathlonBox_MonStats *stats2, PartyAprijuiceModifier *aprijuiceModifier, PokeathlonBox_MonInfo *mon) {
    mon->statChanges[0].direction = PokemonSummary_GetPokeathlonStarStates(mon->statChanges[0].stars, stats1->power, stats2->power, (u32)(stats2->starColors << 0x1D) >> 0x1D, aprijuiceModifier->stats[0]);
    mon->statChanges[1].direction = PokemonSummary_GetPokeathlonStarStates(mon->statChanges[1].stars, stats1->skill, stats2->skill, (u32)(stats2->starColors << 0x14) >> 0x1D, aprijuiceModifier->stats[1]);
    mon->statChanges[2].direction = PokemonSummary_GetPokeathlonStarStates(mon->statChanges[2].stars, stats1->speed, stats2->speed, (u32)(stats2->starColors << 0x11) >> 0x1D, aprijuiceModifier->stats[2]);
    mon->statChanges[3].direction = PokemonSummary_GetPokeathlonStarStates(mon->statChanges[3].stars, stats1->jump, stats2->jump, (u32)(stats2->starColors << 0x17) >> 0x1D, aprijuiceModifier->stats[3]);
    mon->statChanges[4].direction = PokemonSummary_GetPokeathlonStarStates(mon->statChanges[4].stars, stats1->stamina, stats2->stamina, (u32)(stats2->starColors << 0x1A) >> 0x1D, aprijuiceModifier->stats[4]);
}
