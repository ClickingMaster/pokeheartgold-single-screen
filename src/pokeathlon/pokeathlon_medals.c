#include "global.h"

#include "constants/species.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0314.h"
#include "pokeathlon/pokeathlon_record_screens.h"

#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "poke_overlay.h"
#include "pokemon.h"
#include "sound_playback.h"
#include "system.h"
#include "unk_02020A0C.h"

FS_EXTERN_OVERLAY(OVY_98);

// work->sprites[index], addressed as (work + index) first: with plain array indexing the compiler
// strength-reduces the sprite loops below, which the original code does not.
#define MEDALS_SPRITE(work, index) (((PokeathlonMedals *)((u8 *)(work) + (index) * sizeof(ManagedSprite *)))->sprites[0])

extern const s32 ov99_021E9F4C[];
extern const TouchscreenHitbox ov99_021E9F54[];
extern const int ov99_021E9F60[];
extern const int ov99_021E9F70[];
extern const GraphicsModes ov99_021E9F80;
extern const int ov99_021E9F90[];
extern const GridCallbacks ov99_021E9FA0;
extern const int ov99_021E9FB0[5];
extern const SpriteResourceCountsListUnion ov99_021E9FC4;
extern const WindowTemplate ov99_021E9FDC[12];
extern const TouchscreenHitbox ov99_021EA03C[];
extern const PokeathlonMedalsBgTemplates ov99_021EA0BC;
extern const ManagedSpriteTemplate ov99_021EA180[4];
extern const DpadMenuBox ov99_021EA250[];
extern const u8 ov99_021EA348[7];
extern const char _021EA5D8[]; // "seq error!"

BOOL ov99_021E714C(void) {
    return TRUE;
}

BOOL ov99_021E7150(PokeathlonMedals *work) {
    return work->args->unkC;
}

u8 PokeathlonMedals_GetPageSpeciesCount(PokeathlonMedals *work) {
    const PokeathlonMedalsFlags *flags = &work->flags;
    u8 count;

    if (flags->page == flags->maxPage) {
        count = flags->speciesCount % 30;
        if (count != 0) {
            return count;
        }
    }
    return 30;
}

u8 PokeathlonMedals_GetSpeciesMedals(PokeathlonMedals *work, int species) {
    return ((u8 *)work->args->unk4)[species - 1];
}

u32 PokeathlonMedals_CountMedalists(PokeathlonMedals *work) {
    return PokeathlonUtil_CountSpeciesWithAllMedals(work->args->unk4);
}

BOOL PokeathlonMedals_HasAllMedals(PokeathlonMedals *work, int species) {
    if (MATH_CountPopulation(PokeathlonMedals_GetSpeciesMedals(work, species)) == 5) {
        return TRUE;
    }
    return FALSE;
}

u16 PokeathlonMedals_GetSelectedSpecies(PokeathlonMedals *work) {
    const PokeathlonMedalsFlags *flags = &work->flags;
    int index = flags->cursor + flags->page * 30;

    if (index >= MAX_SPECIES) {
        return SPECIES_NONE;
    }
    return work->species[index];
}

Pokedex *PokeathlonMedals_GetPokedex(PokeathlonMedals *work) {
    return work->args->pokedex;
}

int PokeathlonMedals_GetSeenForm(PokeathlonMedals *work, u16 species) {
    if (species == SPECIES_NONE || species > MAX_SPECIES) {
        return 0;
    }
    return Pokedex_GetSeenFormByIdx(PokeathlonMedals_GetPokedex(work), species, 0);
}

int PokeathlonMedals_GetSeenGender(PokeathlonMedals *work, u16 species) {
    int gender;

    if (species == SPECIES_NONE || species > MAX_SPECIES) {
        return 0;
    }
    gender = Pokedex_SpeciesGetLastSeenGender(PokeathlonMedals_GetPokedex(work), species, 0);
    if (gender == -1) {
        gender = GetGenderBySpeciesAndPersonality(species, 0);
    }
    return gender;
}

u32 PokeathlonMedals_GetSpindaPersonality(PokeathlonMedals *work) {
    return Pokedex_GetSeenSpindaPersonality(PokeathlonMedals_GetPokedex(work), 0);
}

u8 PokeathlonMedals_CountTrustTrophies(PokeathlonMedals *work) {
    return ov98_0221EF80(PokeathlonMedals_CountMedalists(work));
}

void PokeathlonMedals_ShowSpeciesMedals(PokeathlonMedals *work, u16 species) {
    u8 i;
    u8 medals = PokeathlonMedals_GetSpeciesMedals(work, species);

    i = 0;
    do {
        ManagedSprite_SetDrawFlag(work->sprites[15 + i], medals & 1);
        medals = (u8)(medals >> 1);
        i++;
    } while (i < 5);
}

u16 *PokeathlonMedals_LoadSpeciesOrder(PokeathlonMedals *work, u32 *count) {
    u32 size;
    u16 *order = GfGfxLoader_LoadFromOpenNarc_GetSizeOut(work->zukanNarc, ov99_021E9F4C[ov99_021E714C()], FALSE, work->heapId, FALSE, &size);

    *count = size >> 1;
    return order;
}

#pragma force_active on

void PokeathlonMedals_BuildSpeciesList(PokeathlonMedals *work) {
    u32 count;
    u32 i;
    u16 species;

    work->speciesOrder = PokeathlonMedals_LoadSpeciesOrder(work, &count);
    for (i = 0; i < count; i++) {
        species = work->speciesOrder[i];
        GF_ASSERT(species != SPECIES_NONE && species <= MAX_SPECIES);
        if (PokeathlonMedals_GetSpeciesMedals(work, species)) {
            work->species[work->flags.speciesCount] = species;
            work->flags.speciesCount++;
        }
    }
    if (work->flags.speciesCount) {
        work->flags.maxPage = work->flags.speciesCount / 30;
        if (work->flags.speciesCount % 30 == 0) {
            work->flags.maxPage--;
        }
    }
}

void PokeathlonMedals_DrawMedalistTiles(PokeathlonMedals *work, s16 x, s16 y) {
    int ypos = y;
    int row;
    int col;
    int base = 0;
    u16 tile;

    for (row = 0; row < 2; row++) {
        for (col = 0; col < 2; col++) {
            tile = col + base + 1;
            LoadRectToBgTilemapRect(work->bgConfig, GF_BG_LYR_MAIN_3, &tile, x + col, ypos, 1, 1);
        }
        base += 2;
        ypos++;
    }
}

void PokeathlonMedals_DrawMedalistMark(PokeathlonMedals *work, int slot, int species) {
    if (PokeathlonMedals_HasAllMedals(work, species)) {
        int col = slot % 6;
        int row = slot / 6;
        PokeathlonMedals_DrawMedalistTiles(work, col * 4 + 6, row * 3 + 4);
        BgCommitTilemapBufferToVram(work->bgConfig, GF_BG_LYR_MAIN_3);
    }
}

#pragma force_active reset

void PokeathlonMedals_SetIconsVisible(PokeathlonMedals *work, BOOL visible) {
    int i = 0;

    do {
        ManagedSprite_SetDrawFlag(MEDALS_SPRITE(work, i + 20), visible);
        i++;
    } while (i < 30);
}

void PokeathlonMedals_ShowPage(PokeathlonMedals *work) {
    s32 count = PokeathlonMedals_GetPageSpeciesCount(work);
    PokeathlonRecordMon mon = { 0 };
    s32 i;

    BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
    PokeathlonMedals_SetIconsVisible(work, FALSE);
    for (i = 0; i < count; i++) {
        u16 species = work->species[work->flags.page * 30 + i];
        mon.info.species = species;
        mon.info.form = PokeathlonMedals_GetSeenForm(work, species);
        PokeathlonUtil_SetMonIconSprite(work->spriteContext, MEDALS_SPRITE(work, i + 20), &mon.info, TRUE, FALSE);
        ManagedSprite_SetDrawFlag(MEDALS_SPRITE(work, i + 20), TRUE);
        PokeathlonMedals_DrawMedalistMark(work, (u16)i, species);
    }
}

#pragma force_active on

void PokeathlonMedals_UpdatePageIndicator(PokeathlonMedals *work) {
    BOOL showUp = FALSE;
    BOOL showDown;

    if (work->flags.maxPage && work->flags.page) {
        showUp = TRUE;
    }
    if (work->flags.maxPage && work->flags.page < work->flags.maxPage) {
        showDown = TRUE;
    } else {
        showDown = FALSE;
    }
    ManagedSprite_SetDrawFlag(work->sprites[1], showUp);
    ManagedSprite_SetDrawFlag(work->sprites[2], showDown);
    PokeathlonUtil_BufferNumber(work->textContext, work->flags.page + 1, 2, 0);
    PokeathlonUtil_BufferNumber(work->textContext, work->flags.maxPage + 1, 2, 1);
    PokeathlonUtil_PrintMessageAt(work->textContext, 3, msg_0314_00129, 0, 0);
}

BOOL PokeathlonMedals_UpdatePageScroll(PokeathlonMedals *work) {
    int direction = work->flags.scrollBackward ? 1 : -1;
    int delta = direction << 6;
    int i;
    int input;
    s16 pos[2];

    if (work->scrollFrame == 0) {
        input = GridInputHandler_GetNextInput(work->grid);
        PokeathlonMedals_UpdatePageIndicator(work);
        if (input == 30) {
            PokeathlonMedals_HideSelectedSpecies(work);
        } else {
            PokeathlonMedals_ShowSelectedSpecies(work);
        }
    } else if (work->scrollFrame == 4) {
        PokeathlonMedals_ShowPage(work);
        for (i = 0; i < 30; i++) {
            int offset = work->scrollFrame * delta;
            ManagedSprite_GetPositionXY(work->sprites[20 + i], &pos[1], &pos[0]);
            ManagedSprite_SetPositionXY(work->sprites[20 + i], pos[1] - 2 * offset, pos[0]);
        }
    }
    if (work->scrollFrame < 8) {
        ScheduleSetBgPosText(work->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_ADD_X, -delta);
        ScheduleSetBgPosText(work->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_ADD_X, -delta);
        for (i = 0; i < 30; i++) {
            ManagedSprite_OffsetPositionXY(work->sprites[20 + i], delta, 0);
        }
    } else {
        ScheduleSetBgPosText(work->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(work->bgConfig, GF_BG_LYR_MAIN_3, BG_POS_OP_SET_X, 0);
        work->scrollFrame = 0;
        ManagedSprite_SetDrawFlag(work->sprites[0], TRUE);
        return TRUE;
    }
    work->scrollFrame++;
    return FALSE;
}

#pragma force_active reset

void PokeathlonMedals_LoadMonPic(PokeathlonMedals *work, u16 species) {
    PokeathlonRecordMon mon = { 0 };

    mon.info.species = species;
    mon.info.form = PokeathlonMedals_GetSeenForm(work, species);
    mon.info.gender = PokeathlonMedals_GetSeenGender(work, species);
    if (species == SPECIES_SPINDA) {
        mon.info.personality = PokeathlonMedals_GetSpindaPersonality(work);
    }
    PokeathlonUtil_LoadMonPicSprite(work->sprites[3], &mon.info, FALSE, FALSE, work->heapId);
}

void PokeathlonMedals_PrintSpeciesName(PokeathlonMedals *work, u16 species) {
    MessageFormat *msgFormat = PokeathlonUtil_GetMessageFormat(work->textContext);

    if (species != SPECIES_NONE) {
        BufferSpeciesName(msgFormat, 0, species);
        ov98_0221EBD8(work->textContext, 5, msg_0314_00124, FALSE);
    } else {
        PokeathlonUtil_ClearAndCopyWindow(work->textContext, 5);
    }
}

void PokeathlonMedals_ShowSelectedSpecies(PokeathlonMedals *work) {
    u16 species = PokeathlonMedals_GetSelectedSpecies(work);

    if (species > MAX_SPECIES) {
        species = SPECIES_NONE;
    }
    PokeathlonMedals_LoadMonPic(work, species);
    PokeathlonMedals_PrintSpeciesName(work, species);
    PokeathlonMedals_ShowSpeciesMedals(work, species);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, (u8)PokeathlonMedals_HasAllMedals(work, species));
}

void PokeathlonMedals_HideSelectedSpecies(PokeathlonMedals *work) {
    int i;

    ManagedSprite_SetDrawFlag(work->sprites[3], FALSE);
    PokeathlonMedals_PrintSpeciesName(work, SPECIES_NONE);
    for (i = 0; i < 5; i++) {
        ManagedSprite_SetDrawFlag(work->sprites[15 + i], FALSE);
    }
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

#pragma force_active on

BOOL PokeathlonMedals_Init(OverlayManager *man, int *state) {
    PokeathlonMedals *work;
    PokeathlonMedalsArgs *args;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_98), OVY_LOAD_ASYNC);
    args = OverlayManager_GetArgs(man);
    GfGfx_DisableEngineBPlanes();
    GfGfx_DisableEngineAPlanes();
    GF_ASSERT(args != NULL);
    Heap_Create(HEAP_ID_3, HEAP_ID_132, 0x30000);
    work = OverlayManager_CreateAndGetData(man, sizeof(PokeathlonMedals), HEAP_ID_132);
    MI_CpuFill8(work, 0, sizeof(PokeathlonMedals));
    work->heapId = HEAP_ID_132;
    work->args = args;
    PokeathlonUtil_ResetDisplay();
    work->narc = NARC_New(NARC_a_1_7_7, work->heapId);
    work->zukanNarc = NARC_New(NARC_application_zukanlist_zkn_data_zukan_data, work->heapId);
    PokeathlonMedals_BuildSpeciesList(work);
    PokeathlonMedals_InitBgs(work);
    PokeathlonMedals_LoadBgGraphics(work);
    work->textContext = PokeathlonUtil_CreateTextContext(work->heapId, work->bgConfig, 12, ov99_021E9FDC, NARC_msg_msg_0314_bin);
    PokeathlonMedals_CreateSprites(work);
    PokeathlonMedals_CreateGridInput(work);
    PokeathlonMedals_UpdatePageIndicator(work);
    PokeathlonMedals_ShowPage(work);
    PokeathlonMedals_ShowSelectedSpecies(work);
    PokeathlonMedals_PrintStaticText(work);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    Main_SetVBlankIntrCB((GFIntrCB)PokeathlonMedals_VBlankCallback, work);
    return TRUE;
}

#pragma force_active reset

BOOL PokeathlonMedals_Exit(OverlayManager *man, int *state) {
    PokeathlonMedals *work = OverlayManager_GetData(man);

    Heap_Free(work->speciesOrder);
    PokeathlonMedals_FreeGridInput(work);
    PokeathlonUtil_FreeSpriteContext(work->spriteContext, work->sprites, 50);
    PokeathlonUtil_FreeTextContext(work->textContext, 12);
    PokeathlonMedals_FreeBgs(work);
    PokeathlonUtil_DisableDisplay();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_132);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    return TRUE;
}

BOOL PokeathlonMedals_Main(OverlayManager *man, int *state) {
    PokeathlonMedals *work = OverlayManager_GetData(man);

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
        if (work->flags.refreshSpecies) {
            PokeathlonMedals_ShowSelectedSpecies(work);
            work->flags.refreshSpecies = FALSE;
        }
        if (work->flags.scrolling) {
            if (!PokeathlonMedals_UpdatePageScroll(work)) {
                break;
            }
            work->flags.scrolling = FALSE;
            break;
        }
        if (!PokeathlonMedals_HandleInput(work) && !PokeathlonUtil_CheckExitInput()) {
            break;
        }
        PlaySE(SEQ_SE_DP_SELECT);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, work->heapId);
        *state = 3;
        break;
    case 3:
        if (IsPaletteFadeFinished()) {
            *state = 4;
        }
        break;
    case 4:
        return TRUE;
    default:
        GF_ASSERT(!_021EA5D8);
        break;
    }
    PokeathlonUtil_DrawSprites(work->spriteContext);
    return FALSE;
}

void PokeathlonMedals_VBlankCallback(PokeathlonMedals *work) {
    vu8 *dtcm = (vu8 *)0x027E0000;

    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(work->bgConfig);
    *(vu32 *)(dtcm + 0x3FF8) |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK), DTCM address as a literal
}

void PokeathlonMedals_CreateGridInput(PokeathlonMedals *work) {
    work->grid = GridInputHandler_Create(ov99_021EA03C, ov99_021EA250, &ov99_021E9FA0, work, TRUE, work->flags.cursor, work->heapId);
}

void PokeathlonMedals_FreeGridInput(PokeathlonMedals *work) {
    if (work->grid != NULL) {
        GridInputHandler_Free(work->grid);
        work->grid = NULL;
    }
}

#pragma force_active on

void PokeathlonMedals_ChangePage(PokeathlonMedals *work, BOOL forward, BOOL touch) {
    u32 oldPage = work->flags.page;
    int input;

    if (forward) {
        work->flags.page = oldPage + 1;
    } else if (oldPage) {
        work->flags.page = oldPage - 1;
    }
    work->flags.page = work->flags.page > work->flags.maxPage ? work->flags.maxPage : work->flags.page;
    if (work->flags.page != oldPage) {
        PlaySE(SEQ_SE_DP_SELECT);
        work->flags.scrollBackward = !forward;
        work->flags.scrolling = TRUE;
        {
            u8 next = 0;
            input = GridInputHandler_GetNextInput(work->grid);
            if (touch) {
                if (input == 30) {
                    next = input;
                }
            } else {
                input = (u8)input;
                if (forward) {
                    next = input - 5;
                } else {
                    next = input + 5;
                }
            }
            GridInputHandler_SetNextInput(work->grid, next);
            PokeathlonMedals_MoveCursor(work, next);
            ManagedSprite_SetDrawFlag(work->sprites[0], FALSE);
        }
    }
}

BOOL PokeathlonMedals_HandleInput(PokeathlonMedals *work) {
    int input = GridInputHandler_HandleInput_AllowHold(work->grid);
    u32 next = GridInputHandler_GetNextInput(work->grid);
    int hit = TouchscreenHitbox_FindHitboxAtTouchNew(ov99_021E9F54);

    if (hit != -1) {
        if (hit == 0) {
            PokeathlonMedals_ChangePage(work, FALSE, TRUE);
        } else if (hit == 1) {
            PokeathlonMedals_ChangePage(work, TRUE, TRUE);
        }
    } else if (input == -1 && next < 30) {
        if (gSystem.newAndRepeatedKeys == PAD_KEY_LEFT) {
            PokeathlonMedals_ChangePage(work, FALSE, FALSE);
        } else if (gSystem.newAndRepeatedKeys == PAD_KEY_RIGHT) {
            PokeathlonMedals_ChangePage(work, TRUE, FALSE);
        }
    } else if (next == 30) {
        if (gSystem.newKeys & PAD_BUTTON_A) {
            return TRUE;
        }
    }
    return FALSE;
}

#pragma force_active reset

void PokeathlonMedals_MoveCursor(PokeathlonMedals *work, u32 target) {
    u8 x;
    u8 y;
    const DpadMenuBox *box = GridInputHandler_GetDpadBox(work->grid, target);

    DpadMenuBox_GetPosition(box, &x, &y);
    ManagedSprite_SetPositionXY(work->sprites[0], x, y);
    work->flags.cursor = target;
    PlaySE(SEQ_SE_DP_SELECT);
}

void PokeathlonMedals_OnButton(PokeathlonMedals *work, int newTarget, int prevTarget) {
    GridInputHandler_SetNextInput(work->grid, (u8)newTarget);
    PokeathlonMedals_MoveCursor(work, newTarget);
}

void PokeathlonMedals_OnSwitchToTouchMode(PokeathlonMedals *work, int newTarget, int prevTarget) {
}

void PokeathlonMedals_OnKeyMove(PokeathlonMedals *work, int newTarget, int prevTarget) {
    PokeathlonMedals_OnCursorMoved(work, newTarget, prevTarget, FALSE);
}

void PokeathlonMedals_OnTouch(PokeathlonMedals *work, int newTarget, int prevTarget) {
    PokeathlonMedals_OnCursorMoved(work, newTarget, prevTarget, TRUE);
}

void PokeathlonMedals_OnCursorMoved(PokeathlonMedals *work, u32 newTarget, int prevTarget, BOOL touch) {
    int anim;

    if (newTarget != prevTarget && newTarget < 32) {
        PokeathlonMedals_MoveCursor(work, newTarget);
        if ((int)newTarget < 30) {
            work->flags.refreshSpecies = TRUE;
        } else {
            PokeathlonMedals_HideSelectedSpecies(work);
        }
        if (newTarget == 30) {
            anim = 12;
        } else {
            anim = 14;
        }
        if (touch) {
            anim++;
        }
        ManagedSprite_SetAnim(work->sprites[0], anim);
    }
}

#pragma force_active on

void PokeathlonMedals_InitBgs(PokeathlonMedals *work) {
    GraphicsModes modes;
    PokeathlonMedalsBgTemplates templates;
    int i;

    work->bgConfig = BgConfig_Alloc(work->heapId);
    modes = ov99_021E9F80;
    SetBothScreensModesAndDisable(&modes);
    templates = ov99_021EA0BC;
    i = 0;
    do {
        InitBgFromTemplate(work->bgConfig, ov99_021EA348[i], &templates.templates[i], GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(work->bgConfig, ov99_021EA348[i]);
        BG_ClearCharDataRange(ov99_021EA348[i], 32, 0, work->heapId);
        i++;
    } while (i < 7);
}

#pragma force_active reset

void PokeathlonMedals_LoadBgGraphics(PokeathlonMedals *work) {
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 42, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x40, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 44, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 46, work->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 44, work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 47, work->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 48, work->bgConfig, GF_BG_LYR_MAIN_3, 1, 0, TRUE, work->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(work->narc, 43, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x40, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 45, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 49, work->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 45, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, work->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 50, work->bgConfig, GF_BG_LYR_SUB_2, 0, 0, TRUE, work->heapId);
}

void PokeathlonMedals_FreeBgs(PokeathlonMedals *work) {
    u8 i;

    NARC_Delete(work->zukanNarc);
    NARC_Delete(work->narc);
    for (i = 0; i < 7; i++) {
        FreeBgTilemapBuffer(work->bgConfig, ov99_021EA348[i]);
    }
    Heap_Free(work->bgConfig);
}

void PokeathlonMedals_CreateSprites(PokeathlonMedals *work) {
    SpriteResourceCountsListUnion counts = ov99_021E9FC4;

    work->spriteContext = PokeathlonUtil_CreateSpriteContext(work->heapId, &counts, 50);
    PokeathlonMedals_LoadSpriteResources(work);
    PokeathlonMedals_CreateUiSprites(work);
    PokeathlonMedals_CreateMedalSprites(work);
    PokeathlonMedals_CreateIconSprites(work);
    PokeathlonUtil_CreateSpriteRow(work->spriteContext, work->sprites, ov99_021E9F60, PokeathlonMedals_CountTrustTrophies(work), 5, 8, 152, TRUE);
}

void PokeathlonMedals_LoadMonPicResources(PokeathlonMedals *work) {
    SpriteSystem *spriteSystem = PokeathlonUtil_GetSpriteSystem(work->spriteContext);
    SpriteManager *spriteManager = PokeathlonUtil_GetSpriteManager(work->spriteContext);

    ov98_0221E784(spriteSystem, spriteManager, ov99_021E9F70, NNS_G2D_VRAM_TYPE_2DSUB);
}

void PokeathlonMedals_LoadSpriteResources(PokeathlonMedals *work) {
    SpriteSystem *spriteSystem = PokeathlonUtil_GetSpriteSystem(work->spriteContext);
    SpriteManager *spriteManager = PokeathlonUtil_GetSpriteManager(work->spriteContext);

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 1, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 1);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 0, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 2);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 2, TRUE, 2);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 3, TRUE, 2);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 4, FALSE, 5, NNS_G2D_VRAM_TYPE_2DSUB, 3);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 5, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 32);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 6, TRUE, 3);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 7, TRUE, 3);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 24, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 25, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 26, TRUE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, work->narc, 27, TRUE, 0);
    PokeathlonUtil_LoadMonIconResources(work->spriteContext, ov99_021E9F90, 30, 1, FALSE);
    PokeathlonMedals_LoadMonPicResources(work);
}

void PokeathlonMedals_CreateUiSprites(PokeathlonMedals *work) {
    const ManagedSpriteTemplate *spriteTemplate = ov99_021EA180;
    int i = 0;

    do {
        work->sprites[i] = PokeathlonUtil_NewSprite(work->spriteContext, spriteTemplate);
        ManagedSprite_SetAnimateFlag(work->sprites[i], TRUE);
        spriteTemplate++;
        i++;
    } while (i < 4);
}

#pragma force_active on

void PokeathlonMedals_CreateMedalSprites(PokeathlonMedals *work) {
    ManagedSpriteTemplate spriteTemplate = { 0 };
    const int *columns = ov99_021E9FB0;
    int y = 48;
    int i;

    for (i = 0; i < 5; i++, columns++, y += 32) {
        spriteTemplate.animation = i;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_CHAR] = 32;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_PLTT] = 3;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_CELL] = 3;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_ANIM] = 3;
        spriteTemplate.vram = NNS_G2D_VRAM_TYPE_2DSUB;
        spriteTemplate.pal = i;
        spriteTemplate.x = 104 + *columns * 8;
        spriteTemplate.y = y;
        MEDALS_SPRITE(work, i + 15) = PokeathlonUtil_NewSprite(work->spriteContext, &spriteTemplate);
        ManagedSprite_SetAnimateFlag(MEDALS_SPRITE(work, i + 15), TRUE);
        ManagedSprite_SetDrawFlag(MEDALS_SPRITE(work, i + 15), FALSE);
    }
}

void PokeathlonMedals_CreateIconSprites(PokeathlonMedals *work) {
    int ypos;
    ManagedSpriteTemplate spriteTemplate = { 0 };
    u32 row = 0;
    u32 base = 0;
    int y = 32;

    for (; row < 5; row++) {
        u32 col = 0;
        int x = 48;
        ypos = y;
        for (; col < 6; col++, x += 32) {
            u32 index = col + base;
            ManagedSprite *sprite;
            spriteTemplate.resIdList[GF_GFX_RES_TYPE_CHAR] = index + 2;
            spriteTemplate.resIdList[GF_GFX_RES_TYPE_PLTT] = 1;
            spriteTemplate.resIdList[GF_GFX_RES_TYPE_CELL] = 1;
            spriteTemplate.resIdList[GF_GFX_RES_TYPE_ANIM] = 1;
            spriteTemplate.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
            spriteTemplate.x = x;
            spriteTemplate.y = ypos;
            spriteTemplate.bgPriority = 1;
            sprite = PokeathlonUtil_NewSprite(work->spriteContext, &spriteTemplate);
            MEDALS_SPRITE(work, index + 20) = sprite;
        }
        base += 6;
        y += 24;
    }
}

#pragma force_active reset

void PokeathlonMedals_PrintStaticText(PokeathlonMedals *work) {
    MessageFormat *msgFormat = PokeathlonUtil_GetMessageFormat(work->textContext);
    int i;

    PokeathlonUtil_PrintMessageCentered(work->textContext, 11, msg_0314_00126, 3);
    ov98_0221EC08(work->textContext, 0, msg_0314_00127, 1, 0);
    ov98_0221EBEC(work->textContext, 4, msg_0314_00000, TRUE, 0, 4);
    for (i = 0; i < 5; i++) {
        BufferPokeathlonMedalName(msgFormat, 0, (u8)i);
        ov98_0221EBD8(work->textContext, i + 6, msg_0314_00004, FALSE);
    }
    BufferIntegerAsString(msgFormat, 0, PokeathlonMedals_CountMedalists(work), 3, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    ov98_0221EC08(work->textContext, 1, msg_0314_00128, 1, 0);
    if (ov99_021E7150(work)) {
        int trustTrophies = PokeathlonMedals_CountTrustTrophies(work);
        ov98_0221EC08(work->textContext, 2, trustTrophies + msg_0314_00113, 1, 0);
    }
}
