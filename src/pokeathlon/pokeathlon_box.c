#include "pokeathlon/pokeathlon_box.h"

#include "msgdata/msg.naix"

#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "nnsys.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "pokemon.h"
#include "save_arrays.h"
#include "screen_fade.h"
#include "sprite_transfer.h"
#include "system.h"
#include "text.h"
#include "oam_manager.h"
#include "char_tiles.h"
#include "wireless_status_icon.h"

// functions in asm:
int BoxMonSelector_Run(int);
void BoxMonSelector_RenderSprites(u32);
void BoxMonSelector_Delete(u32);
u32 BoxMonSelector_New(u32, u32, u32);
void BoxMonSelector_Init(u32, BgConfig *, PCStorage *, Party *, u32, u32, BOOL, u32, void *, void *, void *, void *, PokeathlonBox *);
void BoxMonSelector_VBlank(u32);
s8 PokemonSummary_GetPokeathlonStarStates(void *, u8, u8, u32, s8);

#define pokeathlonBoxObjCharTransferTemplate gPokeathlonBoxObjCharTransferTemplate
#define pokeathlonBoxGraphicsModes           gPokeathlonBoxGraphicsModes
#define pokeathlonBoxGraphicsBanks           gPokeathlonBoxGraphicsBanks
#define pokeathlonBoxBgTemplate3             gPokeathlonBoxBgTemplate3
#define pokeathlonBoxBgTemplate1             gPokeathlonBoxBgTemplate1
#define pokeathlonBoxBgTemplate2             gPokeathlonBoxBgTemplate2
#define pokeathlonBoxBgTemplate4             gPokeathlonBoxBgTemplate4

extern const GraphicsBanks pokeathlonBoxGraphicsBanks;
extern const GraphicsModes pokeathlonBoxGraphicsModes;
extern const ObjCharTransferTemplate pokeathlonBoxObjCharTransferTemplate;
extern const u32 gPokeathlonBoxMedalShifts[5];
extern const BgTemplate pokeathlonBoxBgTemplate3;
extern const BgTemplate pokeathlonBoxBgTemplate1;
extern const BgTemplate pokeathlonBoxBgTemplate2;
extern const BgTemplate pokeathlonBoxBgTemplate4;
extern const u8 gPokeathlonBoxSpriteRows[5];
extern const u8 gPokeathlonBoxMonSpriteRows[5];

BOOL PokeathlonBox_Init(OverlayManager *manager) {
    if (PokeathlonBox_RunSetup(manager)) {
        return TRUE;
    }
    return FALSE;
}

BOOL PokeathlonBox_Main(OverlayManager *manager) {
    PokeathlonBox *data = OverlayManager_GetData(manager);

    switch (data->state) {
    case 0:
        data->state = 1;
        break;

    case 1:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_POKEATHLON);
        data->state = 2;
        break;

    case 2:
        if (IsPaletteFadeFinished()) {
            data->state = 3;
        }
        break;

    case 3:
        int res = BoxMonSelector_Run(data->monSelector);
        if (res == 2) {
            data->cancelled = FALSE;
            data->state = 4;
        } else if (res == 3) {
            data->cancelled = TRUE;
            data->state = 4;
        }
        break;

    case 4:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_POKEATHLON);
        data->state = 5;
        break;

    case 5:
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

BOOL PokeathlonBox_Exit(OverlayManager *manager) {
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

BOOL PokeathlonBox_RunSetup(OverlayManager *manager) {
    PokeathlonBoxArgs *args = OverlayManager_GetArgs(manager);

    do {
        if (PokeathlonBox_Setup(manager)) {
            return TRUE;
        }
    } while (args->courseArgs->mode == 0);

    return FALSE;
}

BOOL PokeathlonBox_Setup(OverlayManager *manager) {
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
    data->state = 0;

    ResetAllTextPrinters();
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, HEAP_ID_POKEATHLON);
    FontID_Alloc(2, HEAP_ID_POKEATHLON);

    return TRUE;
}

void PokeathlonBox_SetGraphicsBanks() {
    GraphicsBanks banks = pokeathlonBoxGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

void PokeathlonBox_SetGraphicsModes(BgConfig *bgConfig) {
    GraphicsModes modes = pokeathlonBoxGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
}

void PokeathlonBox_SetObjCharTransferTemplate() {
    ObjCharTransferTemplate template = pokeathlonBoxObjCharTransferTemplate;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(0x14, HEAP_ID_POKEATHLON);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

void PokeathlonBox_CreateOamManager(PokeathlonBox *data) {
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 1, 126, 0, 32, HEAP_ID_POKEATHLON);
}

void PokeathlonBox_VBlankCallback(void *data) {
    BoxMonSelector_VBlank(((PokeathlonBox *)data)->monSelector);
}

BOOL PokeathlonBox_GetBoxMon(PCStorage *storage, u32 boxno, u32 slotno, PokeathlonBox_BoxMon *ptr) {
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
    ptr->species = 0;
    ptr->personality = 0;
    ptr->isEgg = 0;
    ptr->form = 0;
    ptr->unkC = 0;
    ptr->unkE = 0;
    ptr->isShiny = 0;
    ptr->gender = 0;
    return FALSE;
}

BOOL PokeathlonBox_GetLightBoxMon(PCStorage *storage, u32 boxno, u32 slotno, PokeathlonBox_BoxMon *ptr) {
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
    ptr->species = 0;
    ptr->personality = 0;
    ptr->isEgg = 0;
    ptr->form = 0;
    ptr->unkC = 0;
    ptr->unkE = 0;
    ptr->isShiny = 0;
    ptr->gender = 0;
    return FALSE;
}

void PokeathlonBox_GetBoxName(String *dest, PCStorage *storage, u32 boxno) {
    PCStorage_GetBoxName(storage, boxno, dest);
}

void PokeathlonBox_CopySelectedMons(PokeathlonBox *data, PokeathlonBoxArgs *args) {
    PokeathlonBox_BoxMon boxMon;

    args->selectedMons[0].unk0 = data->cancelled;
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

void PokeathlonBox_GetPartyMonStats(Party *party, u8 slot, PokeathlonBox_MonStats *stats) {
    PartyAprijuiceModifier aprijuiceModifier;
    PokeathlonPerformanceStars stars;

    Party_GetMonAprijuiceModifiers(party, &aprijuiceModifier, slot);
    CalcMonPokeathlonStars(&stars, Party_GetMonByIndex(party, slot), aprijuiceModifier.stats, HEAP_ID_POKEATHLON);
    stats->power = ((u32)(stars.stars << 0x1D) >> 0x1D);
    stats->stamina = ((u32)(stars.stars << 0x11) >> 0x1D);
    stats->jump = ((u32)(stars.stars << 0x14) >> 0x1D);
    stats->skill = ((u32)(stars.stars << 0x1A) >> 0x1D);
    stats->speed = ((u32)(stars.stars << 0x17) >> 0x1D);
    stats->stars = (stats->stars & ~7) | (7 & stars.color[PERFORMANCE_POWER]);
    stats->stars = (stats->stars & ~0x38) | ((u32)(stars.color[PERFORMANCE_STAMINA] << 0x1D) >> 0x1A);
    stats->stars = (stats->stars & 0xFFFFFE3F) | ((u32)(stars.color[PERFORMANCE_JUMP] << 0x1D) >> 0x17);
    stats->stars = (stats->stars & 0xFFFFF1FF) | ((u32)(stars.color[PERFORMANCE_SKILL] << 0x1D) >> 0x14);
    stats->stars = (stats->stars & 0xFFFF8FFF) | ((u32)(stars.color[PERFORMANCE_SPEED] << 0x1D) >> 0x11);
}

void PokeathlonBox_GetBoxMonStats(BoxPokemon *boxMon, PokeathlonBox_MonStats *stats) {
    PokeathlonPerformanceStars stars;

    CalcBoxmonPokeathlonStars(&stars, boxMon, NULL, HEAP_ID_POKEATHLON);
    stats->power = ((u32)(stars.stars << 0x1D) >> 0x1D);
    stats->stamina = ((u32)(stars.stars << 0x11) >> 0x1D);
    stats->jump = ((u32)(stars.stars << 0x14) >> 0x1D);
    stats->skill = ((u32)(stars.stars << 0x1A) >> 0x1D);
    stats->speed = ((u32)(stars.stars << 0x17) >> 0x1D);
    stats->stars = (stats->stars & ~7) | (7 & stars.color[PERFORMANCE_POWER]);
    stats->stars = (stats->stars & ~0x38) | ((u32)(stars.color[PERFORMANCE_STAMINA] << 0x1D) >> 0x1A);
    stats->stars = (stats->stars & 0xFFFFFE3F) | ((u32)(stars.color[PERFORMANCE_JUMP] << 0x1D) >> 0x17);
    stats->stars = (stats->stars & 0xFFFFF1FF) | ((u32)(stars.color[PERFORMANCE_SKILL] << 0x1D) >> 0x14);
    stats->stars = (stats->stars & 0xFFFF8FFF) | ((u32)(stars.color[PERFORMANCE_SPEED] << 0x1D) >> 0x11);
}

void PokeathlonBox_GetLightBoxMonStats(BoxPokemon *boxMon, PokeathlonBox_MonStats *stats) {
    PokeathlonTodayPerformance performance;

    CalcBoxMonPokeathlonPerformance(boxMon, &performance);
    stats->power = performance.stats[PERFORMANCE_POWER].hi; // from the "<< 0x1A) >> 0x1D" ones
    stats->stamina = performance.stats[PERFORMANCE_STAMINA].hi;
    stats->jump = performance.stats[PERFORMANCE_JUMP].hi;
    stats->skill = performance.stats[PERFORMANCE_SKILL].hi;
    stats->speed = performance.stats[PERFORMANCE_SPEED].hi;
}

void PokeathlonBox_ShowMonInfo(int boxno, u8 slot, PokeathlonBox *data) {
    PokeathlonBox_MonInfo monInfo;
    u32 medalShifts[5];

    PokeathlonBox_MonStats partyMonStats;
    PokeathlonBox_MonStats partyLightMonStats;
    PartyAprijuiceModifier aprijuiceModifier;

    PokeathlonBox_MonStats boxMonStats;
    PokeathlonBox_MonStats boxLightMonStats;
    PokeathlonBox_BoxMon pokeathlonBoxMon;

    PartyAprijuiceModifier emptyAprijuiceModifier;

    medalShifts = gPokeathlonBoxMedalShifts;
    if (boxno != -1 && slot != -1) {
        monInfo.unk9 = 2;
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

    monInfo.unk9 = 0;
    PokeathlonBox_DrawMonInfo(data->graphics, &monInfo);
}

void PokeathlonBox_CalcStatChanges(PokeathlonBox_MonStats *stats1, PokeathlonBox_MonStats *stats2, PartyAprijuiceModifier *aprijuiceModifier, PokeathlonBox_MonInfo *mon) {
    mon->statChanges[0].direction = PokemonSummary_GetPokeathlonStarStates(mon->statChanges[0].stars, stats1->power, stats2->power, (u32)(stats2->stars << 0x1D) >> 0x1D, aprijuiceModifier->stats[0]);
    mon->statChanges[1].direction = PokemonSummary_GetPokeathlonStarStates(mon->statChanges[1].stars, stats1->skill, stats2->skill, (u32)(stats2->stars << 0x14) >> 0x1D, aprijuiceModifier->stats[1]);
    mon->statChanges[2].direction = PokemonSummary_GetPokeathlonStarStates(mon->statChanges[2].stars, stats1->speed, stats2->speed, (u32)(stats2->stars << 0x11) >> 0x1D, aprijuiceModifier->stats[2]);
    mon->statChanges[3].direction = PokemonSummary_GetPokeathlonStarStates(mon->statChanges[3].stars, stats1->jump, stats2->jump, (u32)(stats2->stars << 0x17) >> 0x1D, aprijuiceModifier->stats[3]);
    mon->statChanges[4].direction = PokemonSummary_GetPokeathlonStarStates(mon->statChanges[4].stars, stats1->stamina, stats2->stamina, (u32)(stats2->stars << 0x1A) >> 0x1D, aprijuiceModifier->stats[4]);
}

PokeathlonBox_Graphics *PokeathlonBox_InitGraphics(enum HeapID heapID) {
    PokeathlonBox_Graphics *ptr = Heap_Alloc(heapID, sizeof(PokeathlonBox_Graphics));
    MI_CpuFill8(ptr, 0, sizeof(PokeathlonBox_Graphics));
    ptr->heapID = heapID;
    ptr->sub.heapID = heapID;
    return ptr;
}

void PokeathlonBox_SetupGraphics(PokeathlonBox_Graphics *graphics, BgConfig *bgConfig, u8 courseId, u32 isSupreme) {
    graphics->bgConfig = bgConfig;
    PokeathlonBox_LoadBgGraphics(bgConfig, graphics->heapID);
    AddWindowParameterized(graphics->bgConfig, &graphics->window1, GF_BG_LYR_SUB_0, 1, 1, 17, 2, 15, 0x01);
    AddWindowParameterized(graphics->bgConfig, &graphics->window2, GF_BG_LYR_SUB_0, 22, 1, 9, 2, 15, 0x23);
    AddWindowParameterized(graphics->bgConfig, &graphics->window3, GF_BG_LYR_SUB_1, 1, 15, 8, 2, 15, 0x35);
    AddWindowParameterized(graphics->bgConfig, &graphics->window4, GF_BG_LYR_SUB_1, 9, 15, 3, 2, 15, 0x45);
    AddWindowParameterized(graphics->bgConfig, &graphics->window5, GF_BG_LYR_SUB_1, 13, 5, 10, 10, 15, 0x4B);
    AddWindowParameterized(graphics->bgConfig, &graphics->window6, GF_BG_LYR_SUB_1, 1, 19, 12, 4, 15, 0xAF);
    graphics->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0278_bin, graphics->heapID);
    graphics->msgFormat = MessageFormat_New(graphics->heapID);
    graphics->nickname = String_New(11, graphics->heapID);
    graphics->courseId = courseId;
    graphics->isSupreme = isSupreme;
    PokeathlonBox_LoadSpriteResources(&graphics->sub);
    PokeathlonBox_CreateSprites(&graphics->sub);
    PokeathlonBox_PrintStaticTexts(graphics);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

void PokeathlonBox_DrawMonInfo(PokeathlonBox_Graphics *graphics, PokeathlonBox_MonInfo *mon) {
    if (mon->unk9 == 2) {
        PokeathlonBox_ShowMonSprites(graphics, mon);
        PokeathlonBox_PrintMonTexts(graphics, mon);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        return;
    }
    PokeathlonBox_HideMonSprites(); // the argument is passed on in r0 from the caller (graphics)
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

void PokeathlonBox_RenderAndAnimateSprites(PokeathlonBox_Graphics *graphics) {
    if (graphics->sub.spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(graphics->sub.spriteList);
    }
}

void PokeathlonBox_ReleaseGraphics(PokeathlonBox_Graphics *graphics) {
    u8 i;

    DestroyMsgData(graphics->msgData);
    MessageFormat_Delete(graphics->msgFormat);
    String_Delete(graphics->nickname);
    RemoveWindow(&graphics->window1);
    RemoveWindow(&graphics->window2);
    RemoveWindow(&graphics->window3);
    RemoveWindow(&graphics->window4);
    RemoveWindow(&graphics->window5);
    RemoveWindow(&graphics->window6);
    FreeBgTilemapBuffer(graphics->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(graphics->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(graphics->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(graphics->bgConfig, GF_BG_LYR_SUB_3);

    PokeathlonBox_SubGraphics *sub = &graphics->sub;
    PokeathlonBox_DeleteSprites(sub);
    for (i = 0; i < 3; i++) {
        SpriteTransfer_DeleteCharTransferTask(sub->resources[i].charResObj);
        SpriteTransfer_DeletePlttTransferTask(sub->resources[i].plttResObj);
    }
    for (i = 0; i < 6; i++) {
        Destroy2DGfxResObjMan(sub->mgrs[i]);
    }
    SpriteList_Delete(sub->spriteList);
    Heap_Free(graphics);
}

void PokeathlonBox_DeleteSprites(PokeathlonBox_SubGraphics *subGraphics) {
    int i, j;

    for (i = 0; i < 5; i++) {
        Sprite_Delete(subGraphics->sprites[i]);
    }
    for (i = 0; i < 5; i++) {
        Sprite_Delete(subGraphics->statRows[i].sprite);
        for (j = 0; j < 5; j++) {
            Sprite_Delete(subGraphics->statRows[i].sprites[j]);
        }
    }

    Sprite_Delete(subGraphics->monPicSprite);
    Heap_Free(subGraphics->monPicChars);
}

void PokeathlonBox_LoadBgGraphics(BgConfig *bgConfig, enum HeapID heapID) {
    PokeathlonBox_InitBgFromTemplates(bgConfig);
    GfGfxLoader_GXLoadPal(NARC_a_2_4_1 /*243*/, 0, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0, 0, heapID);
    GfGfxLoader_LoadCharData(NARC_a_2_4_1 /*243*/, 1, bgConfig, GF_BG_LYR_SUB_3, 0, 0, 1, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_1 /*243*/, 2, bgConfig, GF_BG_LYR_SUB_3, 0, 0x600, 1, heapID);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_SUB_3);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_1 /*243*/, 3, bgConfig, GF_BG_LYR_SUB_2, 0, 0x600, 1, heapID);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_SUB_2);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, heapID);
}

void PokeathlonBox_InitBgFromTemplates(BgConfig *bgConfig) {
    BgTemplate bgTemplate1 = pokeathlonBoxBgTemplate1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bgTemplate1, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate bgTemplate2 = pokeathlonBoxBgTemplate2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &bgTemplate2, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);

    BgTemplate bgTemplate3 = pokeathlonBoxBgTemplate3;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &bgTemplate3, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);

    BgTemplate bgTemplate4 = pokeathlonBoxBgTemplate4;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &bgTemplate4, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
}

void PokeathlonBox_PrintStaticTexts(PokeathlonBox_Graphics *graphics) {
    BufferPokeathlonCourseName(graphics->msgFormat, 0, (u8)graphics->courseId);
    String *str = ReadMsgData_ExpandPlaceholders(graphics->msgFormat, graphics->msgData, 0, graphics->heapID);
    FillWindowPixelBuffer(&graphics->window1, 0);
    AddTextPrinterParameterizedWithColor(&graphics->window1, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    CopyWindowToVram(&graphics->window1);
    String_Delete(str);
    FillWindowPixelBuffer(&graphics->window2, 0);
    if (graphics->isSupreme != 0) {
        str = NewString_ReadMsgData(graphics->msgData, 1);
        AddTextPrinterParameterizedWithColor(&graphics->window2, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(str);
    }
    CopyWindowToVram(&graphics->window2);
    FillWindowPixelBuffer(&graphics->window5, 0);
    str = NewString_ReadMsgData(graphics->msgData, 8);
    AddTextPrinterParameterizedWithColor(&graphics->window5, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(str);
    str = NewString_ReadMsgData(graphics->msgData, 4);
    AddTextPrinterParameterizedWithColor(&graphics->window5, 0, str, 0, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(str);
    str = NewString_ReadMsgData(graphics->msgData, 6);
    AddTextPrinterParameterizedWithColor(&graphics->window5, 0, str, 0, 32, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(str);
    str = NewString_ReadMsgData(graphics->msgData, 5);
    AddTextPrinterParameterizedWithColor(&graphics->window5, 0, str, 0, 48, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(str);
    str = NewString_ReadMsgData(graphics->msgData, 7);
    AddTextPrinterParameterizedWithColor(&graphics->window5, 0, str, 0, 64, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(str);
    CopyWindowToVram(&graphics->window5);
}

void PokeathlonBox_PrintMonTexts(PokeathlonBox_Graphics *graphics, PokeathlonBox_MonInfo *mon) {
    String *str;

    CopyU16ArrayToString(graphics->nickname, mon->nickname);
    FillWindowPixelBuffer(&graphics->window3, 0);
    AddTextPrinterParameterizedWithColor(&graphics->window3, 0, graphics->nickname, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    CopyWindowToVram(&graphics->window3);
    if (mon->gender == 0) {
        str = NewString_ReadMsgData(graphics->msgData, 2);
        FillWindowPixelBuffer(&graphics->window4, 0);
        AddTextPrinterParameterizedWithColor(&graphics->window4, 0, str, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(7, 8, 0), 0);
        CopyWindowToVram(&graphics->window4);
        String_Delete(str);
    } else if (mon->gender == 1) {
        str = NewString_ReadMsgData(graphics->msgData, 3);
        FillWindowPixelBuffer(&graphics->window4, 0);
        AddTextPrinterParameterizedWithColor(&graphics->window4, 0, str, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 0), 0);
        CopyWindowToVram(&graphics->window4);
        String_Delete(str);
    } else {
        FillWindowPixelBuffer(&graphics->window4, 0);
        CopyWindowToVram(&graphics->window4);
    }
    str = GetSpeciesName(mon->species, graphics->heapID);
    BufferString(graphics->msgFormat, 0, str, 2, 1, 2);
    String_Delete(str);
    str = ReadMsgData_ExpandPlaceholders(graphics->msgFormat, graphics->msgData, 9, graphics->heapID);
    FillWindowPixelBuffer(&graphics->window6, 0);
    AddTextPrinterParameterizedWithColor(&graphics->window6, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    CopyWindowToVram(&graphics->window6);
    String_Delete(str);
}

void PokeathlonBox_ShowMonSprites(PokeathlonBox_Graphics *graphics, PokeathlonBox_MonInfo *mon) {
    u8 i;
    PokeathlonBox_SubGraphics *sub = &graphics->sub;
    u8 subroutine_arg0[5];

    PokeathlonBox_LoadMonPic(&graphics->sub); // mon is still in r1
    PokeathlonBox_TransferMonPic(&graphics->sub);
    Sprite_SetDrawFlag(graphics->sub.monPicSprite, 1);
    PokeathlonBox_ShowStatStars(&graphics->sub, mon);
    subroutine_arg0 = gPokeathlonBoxMonSpriteRows;
    for (i = 0; i < 5; i++) {
        if (mon->medals[subroutine_arg0[i]] != 0) {
            Sprite_SetDrawFlag(sub->sprites[subroutine_arg0[i]], 1);
        } else {
            Sprite_SetDrawFlag(sub->sprites[subroutine_arg0[i]], 0);
        }
    }
}

// The direction sprite of stat row i, addressed the way the original code did (base + i * row size).
#define STAT_ROW_SPRITE(subGraphics, i) (((PokeathlonBox_SubGraphics *)((u8 *)(subGraphics) + (i) * sizeof(PokeathlonBox_StatRow)))->statRows[0].sprite)

void PokeathlonBox_ShowStatStars(PokeathlonBox_SubGraphics *subGraphics, PokeathlonBox_MonInfo *mon) {
    u8 i;
    u8 j;

    for (i = 0; i < 5; i++) {
        // mon shifted by i stat blocks, so that statChanges[0] is stat i
        const PokeathlonBox_MonInfo *shifted = (const PokeathlonBox_MonInfo *)((const u8 *)mon + i * sizeof(PokeathlonBox_StatChange));
        s8 direction = shifted->statChanges[0].direction;
        if (direction > 0) {
            Sprite_SetDrawFlag(STAT_ROW_SPRITE(subGraphics, i), 1);
            Sprite_SetAnimCtrlSeq(STAT_ROW_SPRITE(subGraphics, i), 4);
        } else if (direction < 0) {
            Sprite_SetDrawFlag(STAT_ROW_SPRITE(subGraphics, i), 1);
            Sprite_SetAnimCtrlSeq(STAT_ROW_SPRITE(subGraphics, i), 5);
        } else {
            Sprite_SetDrawFlag(STAT_ROW_SPRITE(subGraphics, i), 0);
        }
        for (j = 0; j < 5; j++) {
            switch (shifted->statChanges[0].stars[j]) {
            case 0:
                Sprite_SetDrawFlag(subGraphics->statRows[i].sprites[j], 0);
                break;
            case 1:
                Sprite_SetDrawFlag(subGraphics->statRows[i].sprites[j], 1);
                Sprite_SetAnimCtrlSeq(subGraphics->statRows[i].sprites[j], 3);
                break;
            case 2:
                Sprite_SetDrawFlag(subGraphics->statRows[i].sprites[j], 1);
                Sprite_SetAnimCtrlSeq(subGraphics->statRows[i].sprites[j], 0);
                break;
            case 3:
                Sprite_SetDrawFlag(subGraphics->statRows[i].sprites[j], 1);
                Sprite_SetAnimCtrlSeq(subGraphics->statRows[i].sprites[j], 1);
                break;
            case 4:
                Sprite_SetDrawFlag(subGraphics->statRows[i].sprites[j], 1);
                Sprite_SetAnimCtrlSeq(subGraphics->statRows[i].sprites[j], 2);
                break;
            default:
                GF_AssertFail();
                break;
            }
        }
    }
}

#pragma force_active on

void PokeathlonBox_HideMonSprites(PokeathlonBox_Graphics *graphics) {
    PokeathlonBox_SubGraphics *sub = &graphics->sub;
    u8 i;
    u8 j;

    for (i = 0; i < 5; i++) {
        Sprite_SetDrawFlag(sub->sprites[i], FALSE);
        Sprite_SetDrawFlag(sub->unk1C8[i], FALSE);
    }
    for (i = 0; i < 5; i++) {
        Sprite_SetDrawFlag(sub->statRows[i].sprite, FALSE);
        for (j = 0; j < 5; j++) {
            Sprite_SetDrawFlag(sub->statRows[i].sprites[j], FALSE);
        }
    }
    Sprite_SetDrawFlag(sub->monPicSprite, FALSE);
}

void PokeathlonBox_LoadSpriteResources(PokeathlonBox_SubGraphics *sub) {
    int i;

    sub->spriteList = G2dRenderer_Init(66, &sub->renderer, sub->heapID);
    G2dRenderer_SetSubSurfaceCoords(&sub->renderer, 0, FX32_CONST(256));
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    for (i = 0; i < 6; i++) {
        sub->mgrs[i] = Create2DGfxResObjMan(3, (GfGfxResType)i, sub->heapID);
    }
    sub->resources[0].charResObj = AddCharResObjFromNarc(sub->mgrs[0], NARC_a_2_4_1, 7, TRUE, 1, NNS_G2D_VRAM_TYPE_2DSUB, sub->heapID);
    sub->resources[0].plttResObj = AddPlttResObjFromNarc(sub->mgrs[1], NARC_a_2_4_1, 4, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 4, sub->heapID);
    sub->resources[0].cellResObj = AddCellOrAnimResObjFromNarc(sub->mgrs[2], NARC_a_2_4_1, 6, TRUE, 1, GF_GFX_RES_TYPE_CELL, sub->heapID);
    sub->resources[0].animResObj = AddCellOrAnimResObjFromNarc(sub->mgrs[3], NARC_a_2_4_1, 5, TRUE, 1, GF_GFX_RES_TYPE_ANIM, sub->heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(sub->resources[0].charResObj);
    SpriteTransfer_CreateExtPlttTransferTask(sub->resources[0].plttResObj);
    sub->resources[1].charResObj = AddCharResObjFromNarc(sub->mgrs[0], NARC_a_1_6_2, 66, FALSE, 2, NNS_G2D_VRAM_TYPE_2DSUB, sub->heapID);
    sub->resources[1].plttResObj = AddPlttResObjFromNarc(sub->mgrs[1], NARC_a_1_6_2, 65, FALSE, 2, NNS_G2D_VRAM_TYPE_2DSUB, 4, sub->heapID);
    sub->resources[1].cellResObj = AddCellOrAnimResObjFromNarc(sub->mgrs[2], NARC_a_1_6_2, 67, FALSE, 2, GF_GFX_RES_TYPE_CELL, sub->heapID);
    sub->resources[1].animResObj = AddCellOrAnimResObjFromNarc(sub->mgrs[3], NARC_a_1_6_2, 68, FALSE, 2, GF_GFX_RES_TYPE_ANIM, sub->heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(sub->resources[1].charResObj);
    SpriteTransfer_CreateExtPlttTransferTask(sub->resources[1].plttResObj);
    sub->resources[2].charResObj = AddCharResObjFromNarc(sub->mgrs[0], NARC_application_choose_starter_choose_starter_sub_res, 9, FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, sub->heapID);
    sub->resources[2].plttResObj = AddPlttResObjFromNarc(sub->mgrs[1], NARC_application_choose_starter_choose_starter_sub_res, 6, FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 4, sub->heapID);
    sub->resources[2].cellResObj = AddCellOrAnimResObjFromNarc(sub->mgrs[2], NARC_application_choose_starter_choose_starter_sub_res, 10, FALSE, 3, GF_GFX_RES_TYPE_CELL, sub->heapID);
    sub->resources[2].animResObj = AddCellOrAnimResObjFromNarc(sub->mgrs[3], NARC_application_choose_starter_choose_starter_sub_res, 10, FALSE, 3, GF_GFX_RES_TYPE_ANIM, sub->heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(sub->resources[2].charResObj);
    SpriteTransfer_CreateExtPlttTransferTask(sub->resources[2].plttResObj);
}

#pragma force_active reset

void PokeathlonBox_CreateSprites(PokeathlonBox_SubGraphics *sub) {
    u8 i;
    u8 j;
    int x;
    int y;

    for (i = 0; i < 5; i++) {
        x = i * 32 + 104;
        sub->sprites[i] = PokeathlonBox_CreateSprite(sub, 1, 2, 0, x, 168, i, FALSE);
        sub->unk1C8[i] = PokeathlonBox_CreateSprite(sub, 1, 1, 0, x, 168, 5, FALSE);
    }
    for (i = 0; i < 5; i++) {
        y = gPokeathlonBoxSpriteRows[i] * 16 + 48;
        sub->statRows[i].sprite = PokeathlonBox_CreateSprite(sub, 2, 1, 0, 96, y, 4, FALSE);
        for (j = 0; j < 5; j++) {
            sub->statRows[i].sprites[j] = PokeathlonBox_CreateSprite(sub, 2, 2, 0, j * 16 + 168, y, 3, FALSE);
        }
    }
    sub->monPicSprite = PokeathlonBox_CreateSprite(sub, 3, 0, 0, 48, 72, 0, FALSE);
    sub->monPicChars = Heap_Alloc(sub->heapID, 0xC80);
    PokeathlonBox_GetMonPicVramOffsets(sub);
}

Sprite *PokeathlonBox_CreateSprite(PokeathlonBox_SubGraphics *sub, int resourceId, int drawPriority, int priority, int x, int y, int anim, BOOL draw) {
    SpriteTemplate template;
    SpriteResourcesHeader header;
    Sprite *sprite;

    CreateSpriteResourcesHeader(&header, resourceId, resourceId, resourceId, resourceId, -1, -1, FALSE, priority, sub->mgrs[0], sub->mgrs[1], sub->mgrs[2], sub->mgrs[3], NULL, NULL);
    PokeathlonBox_InitSpriteTemplate(&template, sub->spriteList, &header, NNS_G2D_VRAM_TYPE_2DSUB, sub->heapID);
    template.position.x = x << FX32_SHIFT;
    template.position.y = (y << FX32_SHIFT) + FX32_CONST(256);
    template.drawPriority = drawPriority;
    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, anim);
    Sprite_SetPriority(sprite, 1);
    Sprite_SetDrawFlag(sprite, draw);
    return sprite;
}

void PokeathlonBox_InitSpriteTemplate(SpriteTemplate *template, SpriteList *spriteList, SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen, enum HeapID heapID) {
    template->spriteList = spriteList;
    template->header = header;
    template->position.z = 0;
    template->scale.x = FX32_ONE;
    template->scale.y = FX32_ONE;
    template->scale.z = FX32_ONE;
    template->rotation = 0;
    template->drawPriority = 0;
    template->whichScreen = whichScreen;
    template->heapID = heapID;
}

void PokeathlonBox_GetMonPicVramOffsets(PokeathlonBox_SubGraphics *sub) {
    SpriteResource *plttResObj = sub->resources[2].plttResObj;
    NNSG2dImageProxy *charProxy = SpriteTransfer_GetCharProxy(sub->resources[2].charResObj);
    NNSG2dImagePaletteProxy *plttProxy = SpriteTransfer_GetPaletteProxy(plttResObj, charProxy);
    sub->monPicCharOffset = NNS_G2dGetImageLocation(charProxy, NNS_G2D_VRAM_TYPE_2DSUB);
    sub->monPicPlttOffset = NNS_G2dGetImagePaletteLocation(plttProxy, NNS_G2D_VRAM_TYPE_2DSUB);
}

// Loads the front sprite chars and palette of `mon` into the buffers drawn by PokeathlonBox_TransferMonPic.
#pragma force_active on

void PokeathlonBox_LoadMonPic(PokeathlonBox_SubGraphics *sub, PokeathlonBox_MonInfo *mon) {
    PokepicTemplate pokepic;
    NNSG2dPaletteData *plttData;
    void *file;

    GetMonSpriteCharAndPlttNarcIdsEx(&pokepic, mon->species, mon->gender, MON_PIC_FACING_FRONT, mon->isShiny, mon->form, mon->personality);
    CharTiles_LoadMonPicAsObjFromNarc((NarcId)pokepic.narcID, pokepic.charDataID, sub->heapID, sub->monPicChars, mon->personality, FALSE, MON_PIC_FACING_FRONT, mon->species);
    file = AllocAndReadWholeNarcMemberByIdPair((NarcId)pokepic.narcID, pokepic.palDataID, sub->heapID);
    if (NNS_G2dGetUnpackedPaletteData(file, &plttData) != TRUE) {
        GF_AssertFail();
    }
    MIi_CpuCopy16(plttData->pRawData, (u16 *)sub->monPicPalette, 32);
    Heap_Free(file);
}

#pragma force_active reset

void PokeathlonBox_TransferMonPic(PokeathlonBox_SubGraphics *sub) {
    DC_FlushRange(sub->monPicChars, 0xc80);
    GXS_LoadOBJ(sub->monPicChars, sub->monPicCharOffset, 0xc80);
    DC_FlushRange(sub->monPicPalette, 0x20);
    GXS_LoadOBJPltt(sub->monPicPalette, sub->monPicPlttOffset, 0x20);
}
