#include "global.h"

#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"

#include "bg_cell_manager.h"
#include "bg_window.h"
#include "filesystem.h"
#include "heap.h"
#include "menu_input_state.h"
#include "party.h"
#include "player_data.h"
#include "pokemon.h"
#include "pokemon_summary_internal.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "ui_util.h"
#include "pokemon_summary.h"

// Pokemon summary screen (ARM9 static 0x02089E80-0x0208B1AC): touch input on the pages, the page backgrounds and
// HP/EXP bars, switching to another Pokemon, the move info and ribbon list slide-ins, the ribbon cursor, the touch
// hitboxes and the touched-button flashes. Tables: asm/pokemon_summary_data.s. The force_active regions keep those
// functions in the link as before.

extern const TouchscreenHitbox gPokemonSummaryCancelButtonHitbox[];
extern const TouchscreenHitbox gPokemonSummarySwapButtonHitbox[];
extern const TouchscreenHitbox gPokemonSummaryConfirmButtonHitbox[];
extern const TouchscreenHitbox gPokemonSummaryMonButtonHitboxes[];
extern const TouchscreenHitbox gPokemonSummaryMoveHitboxes[];
extern const u32 gPokemonSummaryPageScreens[][3];
extern const TouchscreenHitbox gPokemonSummaryRibbonHitboxes[];

// The EXP bar passes the tile as a u32.
#define FillBgTilemapRect_U32(bgConfig, bgId, fillValue, x, y, width, height, mode) ((void (*)(BgConfig *, u8, u32, u8, u8, u8, u8, u8))FillBgTilemapRect)(bgConfig, bgId, fillValue, x, y, width, height, mode)

// pokemon_summary_scene.c, declared with the types their call sites were matched against.
u8 PokemonSummary_CheckTouchedButton(PokemonSummary *summary);
void PokemonSummary_ReloadPokepic(PokemonSummary *summary);

BOOL PokemonSummary_HasPage(PokemonSummary *summary, int page) {
    return (summary->args->pageFlags & (1 << page)) != 0;
}

// State 2's touch input: the page tabs (0-2), the cancel button (3) and the party icons (4-9).
int PokemonSummary_HandleTouchInput(PokemonSummary *arg) {
    PokemonSummary *summary = arg;
    u8 button = PokemonSummary_CheckTouchedButton(summary);
    if (button == 0xFF) {
        return 2;
    }
    switch (button) {
    case 0:
        if (PokemonSummary_HasPage(summary, button)) {
            PokemonSummary_ChangePage(summary, (s8)button);
        }
        break;
    case 1:
    case 2:
        if (!summary->mon.isEgg && PokemonSummary_HasPage(summary, button)) {
            PokemonSummary_ChangePage(summary, (s8)button);
        }
        break;
    case 3:
        PokemonSummary_SetMenuInputState(summary, 1);
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return PokemonSummary_FlashCancelButton(summary, 21);
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9: {
        int slot = button - 4;
        if (summary->args->partySlot != slot && PokemonSummary_CanShowMonAt(summary, slot)) {
            PokemonSummary_ShowMon(summary, (u8)slot);
            return 19;
        }
    } break;
    }
    return 2;
}

void PokemonSummary_LoadBgScreen(PokemonSummary *summary, BgConfig *bgConfig, int member, int bgId) {
    NNSG2dScreenData *screen;
    void *file = AllocAndReadWholeNarcMemberByIdPair(NARC_a_1_6_2, member, HEAP_ID_19);
    NNS_G2dGetUnpackedScreenData(file, &screen);
    LoadRectToBgTilemapRect(summary->bgConfig, (u8)bgId, screen->rawData, 0, 0, 32, 32);
    ScheduleBgTilemapBufferTransfer(bgConfig, (u8)bgId);
    Heap_Free(file);
}

// The current page's backgrounds (gPokemonSummaryPageScreens[page]: main, sub, sub when picking a move to forget
// or for an egg), the previous/next Pokemon buttons and the HP or EXP bar.
void PokemonSummary_LoadPageBgs(PokemonSummary *summary) {
    if (summary->args->isFlag982Set == FALSE && summary->page == 2) {
        PokemonSummary_LoadBgScreen(summary, summary->bgConfig, 11, GF_BG_LYR_MAIN_3);
    } else {
        PokemonSummary_LoadBgScreen(summary, summary->bgConfig, gPokemonSummaryPageScreens[summary->page][0], GF_BG_LYR_MAIN_3);
    }
    if (summary->args->mode == 2) {
        PokemonSummary_LoadBgScreen(summary, summary->bgConfig, gPokemonSummaryPageScreens[summary->page][2], GF_BG_LYR_SUB_2);
        ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, 128);
        ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, 0);
        if (summary->args->moveToLearn == MOVE_NONE) {
            PokemonSummary_ClearForgetButton(summary);
        }
    } else if (summary->mon.isEgg) {
        PokemonSummary_LoadBgScreen(summary, summary->bgConfig, gPokemonSummaryPageScreens[summary->page][2], GF_BG_LYR_SUB_2);
    } else if (PokemonSummary_HasPage(summary, 2)) {
        PokemonSummary_LoadBgScreen(summary, summary->bgConfig, gPokemonSummaryPageScreens[summary->page][1], GF_BG_LYR_SUB_2);
    } else if (summary->page == 0) {
        PokemonSummary_LoadBgScreen(summary, summary->bgConfig, 77, GF_BG_LYR_SUB_2);
    } else {
        PokemonSummary_LoadBgScreen(summary, summary->bgConfig, 78, GF_BG_LYR_SUB_2);
    }
    if (summary->args->monDataType == 2) {
        BgCellManager_DrawCellAt(summary->cellManager, 4, 20, 4);
        PokemonSummary_DrawMonButtons(summary);
    }
    if (!summary->mon.isEgg) {
        if (summary->page == 0) {
            PokemonSummary_DrawExpBar(summary);
        } else if (summary->page == 1) {
            PokemonSummary_DrawHpBar(summary);
        }
    }
}

void PokemonSummary_DrawHpBar(PokemonSummary *summary) {
    u16 tileBase;
    u32 pixels;
    u8 i;
    u16 fullTile;

    switch (CalculateHpBarColor(summary->mon.hp, summary->mon.maxHp, 48)) {
    case 0:
    case 3:
    case 4:
        tileBase = 0xF097;
        break;
    case 2:
        tileBase = 0xF0B7;
        break;
    case 1:
        tileBase = 0xF0D7;
        break;
    }
    pixels = CalculateHpBarPixelsLength(summary->mon.hp, summary->mon.maxHp, 48);
    i = 0;
    fullTile = (u16)(tileBase + 8);
    do {
        u16 tile = pixels >= 8 ? fullTile : (u16)(tileBase + pixels);
        FillBgTilemapRect(summary->bgConfig, GF_BG_LYR_MAIN_3, tile, (u8)(i + 10), 5, 1, 1, 17);
        if (pixels < 8) {
            pixels = 0;
        } else {
            pixels = (u8)(pixels - 8);
        }
        i = (u8)(i + 1);
    } while (i < 6);
    ScheduleBgTilemapBufferTransfer(summary->bgConfig, GF_BG_LYR_MAIN_3);
}

void PokemonSummary_DrawExpBar(PokemonSummary *summary) {
    u32 exp;
    u32 levelExp;
    u32 pixels;
    u8 i;

    if (summary->mon.level < 100) {
        u32 start = summary->mon.curLevelExp;
        levelExp = summary->mon.nextLevelExp - start;
        exp = summary->mon.exp - start;
    } else {
        levelExp = 0;
        exp = 0;
    }
    pixels = CalculateHpBarPixelsLength(exp, levelExp, 56);
    i = 0;
    do {
        u32 tile = pixels >= 8 ? 0xE03F : (u16)(0xE037 + pixels);
        FillBgTilemapRect_U32(summary->bgConfig, GF_BG_LYR_SUB_2, tile, (u8)(i + 9), 19, 1, 1, 17);
        if (pixels < 8) {
            pixels = 0;
        } else {
            pixels = (u8)(pixels - 8);
        }
        i = (u8)(i + 1);
    } while (i < 7);
    ScheduleBgTilemapBufferTransfer(summary->bgConfig, GF_BG_LYR_SUB_2);
}

void PokemonSummary_ShowMon(PokemonSummary *summary, u8 slot) {
    summary->args->partySlot = slot;
    PokemonSummary_ReadShownMon(summary);
    PokemonSummary_PlayCry(summary);
    PokemonSummary_PrintNickname(summary);
    PokemonSummary_PrintLevel(summary);
    PokemonSummary_PrintHeldItem(summary);
    PokemonSummary_LoadPageBgs(summary);
    PokemonSummary_ReloadPokepic(summary);
    PokemonSummary_HideSprites0To7And21And22(summary);
    PokemonSummary_SetMonTypeSprites(summary);
    PokemonSummary_UpdateTypeSprites(summary);
    PokemonSummary_UpdatePokeathlonStars(summary);
    PokemonSummary_UpdateShinyLeafSprites(summary);
    PokemonSummary_ShowRibbonSprites(summary);
    PokemonSummary_LoadShownMonIcon(summary);
    PokemonSummary_LoadBallSprite(summary);
    PokemonSummary_UpdateStatusSprite(summary);
    PokemonSummary_InitSheenSparkles(summary);
    PokemonSummary_UpdateMarkingSprites(summary);
    PokemonSummary_PrintPage(summary);
    PokemonSummary_UpdatePartySlotSprite(summary);
    PokemonSummary_AlternateBoxMonIcon(summary);
}

void PokemonSummary_ChangeMon(PokemonSummary *summary, int direction) {
    int slot = PokemonSummary_FindNextMon(summary, direction);
    if (slot != -1) {
        PokemonSummary_ShowMon(summary, (u8)slot);
    }
}

// The next Pokemon to show in that direction, or -1. args->party holds a Pokemon array (monDataType 0), a Party (1) or a
// BoxPokemon array (2).
int PokemonSummary_FindNextMon(PokemonSummary *summary, int direction) {
    switch (summary->args->monDataType) {
    case 0:
        return PokemonSummary_FindNextMonInArray(summary, direction);
    case 1:
        return PokemonSummary_FindNextPartyMon(summary, direction);
    case 2:
        return PokemonSummary_FindNextBoxMon(summary, direction);
    default:
        return -1;
    }
}

BOOL PokemonSummary_CanShowMonAt(PokemonSummary *summary, int slot) {
    int size;
    int offset;
    u8 *mons;
    Pokemon *mon;

    switch (summary->args->monDataType) {
    case 0:
        size = Pokemon_sizeof();
        mons = (u8 *)summary->args->party;
        offset = slot * size;
        if (GetMonData((Pokemon *)(mons + offset), MON_DATA_SPECIES, NULL) != SPECIES_NONE) {
            if (GetMonData((Pokemon *)(mons + offset), MON_DATA_IS_EGG, NULL) != 0) {
                return TRUE;
            }
        }
        break;
    case 1:
        if (slot < summary->args->partyCount) {
            mon = Party_GetMonByIndex(summary->args->party, slot);
            if (GetMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE) {
                if (GetMonData(mon, MON_DATA_IS_EGG, NULL) != 0) {
                    if (PokemonSummary_CanShowEgg(summary) != TRUE) {
                        break;
                    }
                    return TRUE;
                }
                return TRUE;
            }
        }
        break;
    case 2:
        size = BoxPokemon_sizeof();
        mons = (u8 *)summary->args->party;
        offset = slot * size;
        if (GetBoxMonData((BoxPokemon *)(mons + offset), MON_DATA_SPECIES, NULL) != SPECIES_NONE) {
            if (GetBoxMonData((BoxPokemon *)(mons + offset), MON_DATA_IS_EGG, NULL) != 0) {
                return TRUE;
            }
        }
        break;
    }
    return FALSE;
}

BOOL PokemonSummary_CanShowEgg(PokemonSummary *summary) {
    s8 page;
    if (summary->args->mode == 3) {
        return FALSE;
    }
    page = summary->page;
    if (page != 0 && page != 3) {
        return FALSE;
    }
    return TRUE;
}

int PokemonSummary_FindNextMonInArray(PokemonSummary *summary, int direction) {
    s8 slot = summary->args->partySlot;
    int size;
    u8 *mons;
    do {
        slot = (s8)(slot + direction);
        if (slot < 0 || slot >= summary->args->partyCount) {
            return -1;
        }
        size = Pokemon_sizeof();
        mons = (u8 *)summary->args->party;
        if (GetMonData((Pokemon *)(mons + slot * size), MON_DATA_SPECIES, NULL) != SPECIES_NONE) {
            if (GetMonData((Pokemon *)(mons + slot * size), MON_DATA_IS_EGG, NULL) == 0 || PokemonSummary_CanShowEgg(summary) == TRUE) {
                return slot;
            }
        }
    } while (1);
}

int PokemonSummary_FindNextPartyMon(PokemonSummary *summary, int direction) {
    s8 slot = summary->args->partySlot;
    Pokemon *mon;
    do {
        slot = (s8)(slot + direction);
        if (slot < 0 || slot >= summary->args->partyCount) {
            return -1;
        }
        mon = Party_GetMonByIndex(summary->args->party, slot);
        if (GetMonData(mon, MON_DATA_SPECIES, NULL) != SPECIES_NONE) {
            if (GetMonData(mon, MON_DATA_IS_EGG, NULL) == 0 || PokemonSummary_CanShowEgg(summary) == TRUE) {
                return slot;
            }
        }
    } while (1);
}

int PokemonSummary_FindNextBoxMon(PokemonSummary *summary, int direction) {
    s8 slot = summary->args->partySlot;
    int size;
    u8 *mons;
    do {
        slot = (s8)(slot + direction);
        if (slot < 0 || slot >= summary->args->partyCount) {
            return -1;
        }
        size = BoxPokemon_sizeof();
        mons = (u8 *)summary->args->party;
        if (GetBoxMonData((BoxPokemon *)(mons + slot * size), MON_DATA_SPECIES, NULL) != SPECIES_NONE) {
            if (GetBoxMonData((BoxPokemon *)(mons + slot * size), MON_DATA_IS_EGG, NULL) == 0 || PokemonSummary_CanShowEgg(summary) == TRUE) {
                return slot;
            }
        }
    } while (1);
}

void *PokemonSummary_GetShownMon(PokemonSummary *summary) {
    PokemonSummaryArgs *args = summary->args;
    switch (args->monDataType) {
    case 0:
        return (u8 *)args->party + args->partySlot * Pokemon_sizeof();
    case 1:
        return Party_GetMonByIndex(args->party, args->partySlot);
    case 2:
        return (u8 *)args->party + args->partySlot * BoxPokemon_sizeof();
    default:
        return NULL;
    }
}

// Slides the move info panel (sub BG 1) in; TRUE when done.
BOOL PokemonSummary_SlideInMoveInfo(PokemonSummary *summary) {
    switch (summary->unk7BE) {
    case 0:
        Sprite_SetDrawFlag(summary->sprites[9], FALSE);
        Sprite_SetAnimCtrlSeq(summary->sprites[9], 0);
        ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, 0);
        ClearWindowTilemapAndScheduleTransfer(&summary->windows[4]);
        summary->unk7BE = 1;
        break;
    case 1:
        if (Bg_GetXpos(summary->bgConfig, GF_BG_LYR_SUB_1) >= 128) {
            ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, 128);
            summary->unk7BE = 2;
        } else {
            ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_ADD_X, 64);
        }
        break;
    case 2:
        PokemonSummary_PrintMoveToLearn(summary);
        PokemonSummary_ShowSelectedMoveInfo(summary);
        thunk_Sprite_SetDrawFlag(summary->sprites[11], TRUE);
        PokemonSummary_ShowMonIconSprite(summary);
        if (summary->mon.type1 != summary->mon.type2) {
            thunk_Sprite_SetDrawFlag(summary->sprites[12], TRUE);
        }
        Sprite_SetDrawFlag(summary->sprites[9], TRUE);
        summary->unk7BE = 0;
        return TRUE;
    }
    return FALSE;
}

// Slides it out again; TRUE when done.
BOOL PokemonSummary_SlideOutMoveInfo(PokemonSummary *summary) {
    switch (summary->unk7BE) {
    case 0:
        thunk_Sprite_SetDrawFlag(summary->sprites[11], FALSE);
        thunk_Sprite_SetDrawFlag(summary->sprites[12], FALSE);
        thunk_Sprite_SetDrawFlag(summary->sprites[18], FALSE);
        thunk_Sprite_SetDrawFlag(summary->sprites[19], FALSE);
        Sprite_SetDrawFlag(summary->sprites[9], FALSE);
        PokemonSummary_ScheduleMainBg1Transfer(summary);
        PokemonSummary_ClearMoveInfo(summary);
        summary->unk7BE = 1;
        break;
    case 1:
        if (Bg_GetXpos(summary->bgConfig, GF_BG_LYR_SUB_1) <= 128) {
            ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, 0);
            summary->unk7BE = 2;
        } else {
            ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SUB_X, 64);
        }
        break;
    case 2:
        ScheduleWindowCopyToVram(&summary->windows[32]);
        ScheduleWindowCopyToVram(&summary->windows[6]);
        ScheduleWindowCopyToVram(&summary->windows[31]);
        ScheduleWindowCopyToVram(&summary->windows[4]);
        Sprite_SetDrawFlag(summary->sprites[9], TRUE);
        PokemonSummary_UpdatePartySlotSprite(summary);
        summary->unk7BE = 0;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

// Moves the move cursor up or down, skipping empty slots; slot 4 (the move to learn) only when picking a move to
// forget. TRUE if it moved.
BOOL PokemonSummary_MoveMoveCursor(PokemonSummary *summary, int direction) {
    u8 old = summary->moveSlot;
    s8 slot = old;
    s8 max;
    if (summary->args->mode == 2) {
        if (summary->args->moveToLearn) {
            max = 4;
        } else {
            max = 3;
        }
    } else {
        max = 3;
    }
    do {
        slot += direction;
        if (slot < 0) {
            slot = max;
        } else if (slot > max) {
            slot = 0;
        }
        if (slot == 4) {
            break;
        }
        if (summary->mon.moves[slot] != MOVE_NONE) {
            break;
        }
    } while (slot != old);
    if (slot != old) {
        summary->moveSlot = slot;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active off

void PokemonSummary_ShowSelectedMoveInfo(PokemonSummary *summary) {
    u32 slot;
    PokemonSummary_UpdateMoveCursorSprite(summary);
    slot = summary->moveSlot;
    if (slot == 4) {
        u16 move = summary->args->moveToLearn;
        if (move != MOVE_NONE) {
            PokemonSummary_ShowMoveInfo(summary, move);
        } else {
            PokemonSummary_ShowMoveInfo(summary, -1);
        }
    } else if (slot == 5) {
        PokemonSummary_ShowMoveInfo(summary, -1);
    } else {
        PokemonSummary_ShowMoveInfo(summary, summary->mon.moves[slot]);
    }
}

#pragma force_active on

// Called with the move as an int (and ClearMoveInfo with it too).
#define PokemonSummary_ClearMoveInfo_Int(summary, move)         ((void (*)(PokemonSummary *, int))PokemonSummary_ClearMoveInfo)(summary, move)
#define PokemonSummary_PrintMoveInfo_Int(summary, move)         ((void (*)(PokemonSummary *, int))PokemonSummary_PrintMoveInfo)(summary, move)
#define PokemonSummary_SetMoveCategorySprite_Int(summary, move) ((void (*)(PokemonSummary *, int))PokemonSummary_SetMoveCategorySprite)(summary, move)

// -1: clear it.
void PokemonSummary_ShowMoveInfo(PokemonSummary *summary, int move) {
    if (move == -1) {
        PokemonSummary_ClearMoveInfo_Int(summary, move);
        thunk_Sprite_SetDrawFlag(summary->sprites[18], FALSE);
        return;
    }
    PokemonSummary_PrintMoveInfo_Int(summary, move);
    PokemonSummary_SetMoveCategorySprite_Int(summary, move);
    thunk_Sprite_SetDrawFlag(summary->sprites[18], TRUE);
}

void PokemonSummary_SwapMoves(PokemonSummary *summary) {
    void *mon = PokemonSummary_GetShownMon(summary);
    u16 move;
    u8 value;
    if (summary->args->monDataType == 2) {
        BoxMonSwapMoves(mon, summary->moveSlot, summary->swapMoveSlot);
    } else {
        MonSwapMoves(mon, summary->moveSlot, summary->swapMoveSlot);
    }
    move = summary->mon.moves[summary->moveSlot];
    summary->mon.moves[summary->moveSlot] = summary->mon.moves[summary->swapMoveSlot];
    summary->mon.moves[summary->swapMoveSlot] = move;
    value = summary->mon.pp[summary->moveSlot];
    summary->mon.pp[summary->moveSlot] = summary->mon.pp[summary->swapMoveSlot];
    summary->mon.pp[summary->swapMoveSlot] = value;
    value = summary->mon.maxPP[summary->moveSlot];
    summary->mon.maxPP[summary->moveSlot] = summary->mon.maxPP[summary->swapMoveSlot];
    summary->mon.maxPP[summary->swapMoveSlot] = value;
    *(u32 *)summary->args->monModified = 1;
}

#pragma force_active off

// Picking a move to forget: the move page with the move to learn.
void PokemonSummary_InitForgetMove(PokemonSummary *summary) {
    ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_X, 0);
    PokemonSummary_PrintMoveToLearn(summary);
    PokemonSummary_ShowSelectedMoveInfo(summary);
    thunk_Sprite_SetDrawFlag(summary->sprites[11], TRUE);
    PokemonSummary_ShowMonIconSprite(summary);
    if (summary->mon.type1 != summary->mon.type2) {
        thunk_Sprite_SetDrawFlag(summary->sprites[12], TRUE);
    }
    Sprite_SetDrawFlag(summary->sprites[9], TRUE);
}

void PokemonSummary_UpdateForgetMovePage(PokemonSummary *summary) {
    PokemonSummary_PrintMoveToLearn(summary);
    PokemonSummary_ShowSelectedMoveInfo(summary);
    PokemonSummary_ShowMonIconSprite(summary);
    if (summary->page == 1) {
        PokemonSummary_HideConditionSprites(summary);
        thunk_Sprite_SetDrawFlag(summary->sprites[11], TRUE);
        if (summary->mon.type1 != summary->mon.type2) {
            thunk_Sprite_SetDrawFlag(summary->sprites[12], TRUE);
        }
        ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 0);
    } else {
        PokemonSummary_ShowConditionSprites(summary);
        ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_MAIN_2, BG_POS_OP_SET_Y, 256);
    }
}

// Slides the ribbon list (sub BG 1) in; TRUE when done.
BOOL PokemonSummary_SlideInRibbonList(PokemonSummary *summary) {
    switch (summary->unk7BE) {
    case 0:
        ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_X, 0);
        ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, 0);
        Sprite_SetPriority(summary->sprites[9], 3);
        thunk_Sprite_SetPaletteOverride(summary->sprites[9], 0);
        ClearWindowTilemapAndScheduleTransfer(&summary->windows[29]);
        ClearWindowTilemapAndScheduleTransfer(&summary->windows[23]);
        ClearWindowTilemapAndScheduleTransfer(summary->pageWindows);
        summary->ribbonRow = 0;
        summary->unk7BE = 1;
        break;
    case 1:
        if (Bg_GetYpos(summary->bgConfig, GF_BG_LYR_SUB_1) >= 72) {
            ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, 72);
            summary->unk7BE = 2;
        } else {
            ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_ADD_Y, 36);
        }
        break;
    case 2:
        Sprite_SetDrawFlag(summary->sprites[63], TRUE);
        Sprite_SetDrawFlag(summary->sprites[66], TRUE);
        PokemonSummary_MoveRibbonCursor(summary, 0);
        summary->unk7BE = 0;
        return TRUE;
    }
    return FALSE;
}

// Slides it out again; TRUE when done.
BOOL PokemonSummary_SlideOutRibbonList(PokemonSummary *summary) {
    switch (summary->unk7BE) {
    case 0:
        summary->ribbonCursor = 0;
        summary->ribbonRow = 0;
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[1]);
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[2]);
        PokemonSummary_ShowRibbonSprites(summary);
        summary->unk7BE = 1;
        break;
    case 1:
        if (Bg_GetYpos(summary->bgConfig, GF_BG_LYR_SUB_1) <= 0) {
            ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SET_Y, 0);
            summary->unk7BE = 2;
        } else {
            ScheduleSetBgPosText(summary->bgConfig, GF_BG_LYR_SUB_1, BG_POS_OP_SUB_Y, 36);
        }
        break;
    case 2:
        ScheduleWindowCopyToVram(&summary->windows[29]);
        ScheduleWindowCopyToVram(&summary->windows[23]);
        ScheduleWindowCopyToVram(summary->pageWindows);
        Sprite_SetPriority(summary->sprites[9], 0);
        thunk_Sprite_SetPaletteOverride(summary->sprites[9], 2);
        summary->unk7BE = 0;
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

// Moves the ribbon cursor in the 3x3 grid: +-1 left/right, +-3 up/down (turning the page at the edges), +-9 a page.
void PokemonSummary_MoveRibbonCursor(PokemonSummary *summary, int direction) {
    u8 cursor = summary->ribbonCursor, row = summary->ribbonRow;
    if (direction == 1) {
        if (cursor % 3 == 2) {
            return;
        }
        summary->ribbonCursor++;
    } else if (direction == -1) {
        if (cursor % 3 == 0) {
            return;
        }
        summary->ribbonCursor--;
    } else if (direction == 3) {
        if (cursor >= 6) {
            if ((row + 1) * 9 < summary->ribbonCount) {
                summary->ribbonRow++;
                summary->ribbonCursor = summary->ribbonCursor % 3;
            }
        } else {
            summary->ribbonCursor = cursor + 3;
        }
    } else if (direction == -3) {
        if (cursor <= 2) {
            if (row != 0) {
                summary->ribbonRow--;
                summary->ribbonCursor = summary->ribbonCursor % 3 + 6;
            }
        } else {
            summary->ribbonCursor = cursor - 3;
        }
    } else if (direction == 9) {
        if ((row + 1) * 9 < summary->ribbonCount) {
            summary->ribbonRow++;
        }
    } else if (direction == -9) {
        if (row != 0) {
            summary->ribbonRow--;
        }
    }
    if ((cursor != summary->ribbonCursor || row != summary->ribbonRow) && direction != 9 && direction != -9) {
        PlaySE(SEQ_SE_DP_SELECT);
    }
    summary->ribbonId = PokemonSummary_GetRibbonAt(summary, summary->ribbonCursor);
    PokemonSummary_UpdateRibbonCursorSprite(summary);
    if (row != summary->ribbonRow) {
        PokemonSummary_UpdateRibbonSprites(summary);
    }
    PokemonSummary_PrintRibbonInfo(summary);
    if (summary->ribbonRow != 0) {
        Sprite_SetDrawFlag(summary->sprites[64], TRUE);
    } else {
        Sprite_SetDrawFlag(summary->sprites[64], FALSE);
    }
    if ((summary->ribbonRow + 1) * 9 < summary->ribbonCount) {
        Sprite_SetDrawFlag(summary->sprites[65], TRUE);
    } else {
        Sprite_SetDrawFlag(summary->sprites[65], FALSE);
    }
}

#pragma force_active off

// The ribbon in that grid cell of the current page: the cell-th ribbon the Pokemon has, counted from the page start.
u8 PokemonSummary_GetRibbonAt(PokemonSummary *summary, u8 cell) {
    u16 i;
    u8 target;
    u8 found;
    found = 0;
    i = found;
    target = summary->ribbonRow * 9 + cell;
    for (; i < 80; i++) {
        if ((summary->mon.ribbons[(u32)i >> 5] & (1 << (i & 31))) != 0) {
            if (target == found) {
                return (u8)i;
            }
            found++;
        }
    }
    return 0;
}

void PokemonSummaryArgs_SetPlayer(PokemonSummaryArgs *args, PlayerProfile *profile) {
    args->playerName = PlayerProfile_GetNamePtr(profile);
    args->playerId = PlayerProfile_GetTrainerID(profile);
    args->playerGender = PlayerProfile_GetTrainerGender(profile);
}

int StatusIcon_GetCharNaix(void) {
    return 64;
}

int StatusIcon_GetPlttNaix(void) {
    return 65;
}

int StatusIcon_GetCellNaix(void) {
    return 63;
}

int StatusIcon_GetAnimNaix(void) {
    return 62;
}

u32 Pokemon_GetStatusIconId(Pokemon *mon) {
    u32 status = GetMonData(mon, MON_DATA_STATUS, NULL);
    if (GetMonData(mon, MON_DATA_HP, NULL) == 0) {
        return 6;
    }
    if (status & 0x88) { // poison, bad poison
        return 4;
    }
    if (status & 7) { // sleep
        return 3;
    }
    if (status & 0x10) { // burn
        return 5;
    }
    if (status & 0x20) { // freeze
        return 2;
    }
    if (status & 0x40) { // paralysis
        return 1;
    }
    return 7;
}

void PokemonSummary_SetMenuInputState(PokemonSummary *summary, int state) {
    PokemonSummaryArgs *args = summary->args;
    MenuInputStateMgr *mgr = args->menuInputStatePtr;
    if (mgr != NULL) {
        MenuInputStateMgr_SetState(mgr, (MenuInputState)state);
    }
}

// The touched move (0-3) or -1.
int PokemonSummary_GetTouchedMove(PokemonSummary *summary) {
    return TouchscreenHitbox_FindRectAtTouchNew(gPokemonSummaryMoveHitboxes);
}

// Picking the move to swap with: a move (0-3), cancel (4) or -1.
int PokemonSummary_GetMoveSwapTouch(PokemonSummary *summary) {
    int ret = TouchscreenHitbox_FindRectAtTouchNew(gPokemonSummaryMoveHitboxes);
    if (ret != -1) {
        return ret;
    }
    if (TouchscreenHitbox_TouchNewIsIn(gPokemonSummaryCancelButtonHitbox) == TRUE) {
        return 4;
    }
    return -1;
}

// The move list: a move (0-3), swap (4), cancel (5) or -1.
int PokemonSummary_GetMoveSelectTouch(PokemonSummary *summary) {
    int ret = TouchscreenHitbox_FindRectAtTouchNew(gPokemonSummaryMoveHitboxes);
    if (ret != -1) {
        return ret;
    }
    if (TouchscreenHitbox_TouchNewIsIn(gPokemonSummarySwapButtonHitbox) == TRUE) {
        return 4;
    }
    if (TouchscreenHitbox_TouchNewIsIn(gPokemonSummaryCancelButtonHitbox) == TRUE) {
        return 5;
    }
    return -1;
}

// Picking a move to forget: a move (0-3), the move to learn (4), cancel (5) or -1.
int PokemonSummary_GetForgetMoveTouch(PokemonSummary *summary) {
    int ret = TouchscreenHitbox_FindRectAtTouchNew(gPokemonSummaryMoveHitboxes);
    if (ret != -1) {
        return ret;
    }
    if (TouchscreenHitbox_TouchNewIsIn(gPokemonSummaryConfirmButtonHitbox) == TRUE) {
        return 4;
    }
    if (TouchscreenHitbox_TouchNewIsIn(gPokemonSummaryCancelButtonHitbox) == TRUE) {
        return 5;
    }
    return -1;
}

// Confirming the move to forget: yes (0), cancel (1) or -1.
int PokemonSummary_GetForgetConfirmTouch(PokemonSummary *summary) {
    if (TouchscreenHitbox_TouchNewIsIn(gPokemonSummaryConfirmButtonHitbox) == TRUE) {
        return 0;
    }
    if (TouchscreenHitbox_TouchNewIsIn(gPokemonSummaryCancelButtonHitbox) == TRUE) {
        return 1;
    }
    return -1;
}

// The previous (0) or next (1) Pokemon button, or -1.
int PokemonSummary_GetMonButtonTouch(PokemonSummary *summary) {
    return TouchscreenHitbox_FindRectAtTouchNew(gPokemonSummaryMonButtonHitboxes);
}

// The ribbon list: a ribbon cell (0-8), page up (9), page down (10), cancel (11) or -1.
int PokemonSummary_GetRibbonTouch(PokemonSummary *summary) {
    return TouchscreenHitbox_FindRectAtTouchNew(gPokemonSummaryRibbonHitboxes);
}

// State 8: the move to forget was picked; ask to confirm.
int PokemonSummary_ShowForgetConfirm(PokemonSummary *summary) {
    Sprite_SetAnimCtrlSeq(summary->sprites[9], 1);
    Sprite_SetDrawFlag(summary->sprites[10], TRUE);
    PokemonSummary_ResetSwapCursorSprite(summary);
    PokemonSummary_DrawForgetConfirm(summary, 1);
    return 9;
}

void PokemonSummary_DrawForgetConfirm(PokemonSummary *summary, int show) {
    if (show == 1) {
        BgCellManager_DrawCellAt(summary->cellManager, 0, 0, 18);
        Sprite_SetDrawFlag(summary->sprites[17], FALSE);
        PokemonSummary_ShowForgetLabel(summary, 1);
        return;
    }
    if (summary->args->moveToLearn != MOVE_NONE) {
        BgCellManager_DrawCellAt(summary->cellManager, 1, 0, 18);
        Sprite_SetDrawFlag(summary->sprites[17], TRUE);
    } else {
        PokemonSummary_ClearForgetButton(summary);
    }
    PokemonSummary_ShowForgetLabel(summary, 0);
}

void PokemonSummary_ClearForgetButton(PokemonSummary *summary) {
    FillBgTilemapRect(summary->bgConfig, GF_BG_LYR_SUB_2, 0x3006, 0, 18, 17, 6, 17);
    ScheduleBgTilemapBufferTransfer(summary->bgConfig, GF_BG_LYR_SUB_2);
}

void PokemonSummary_DrawSwitchButton(PokemonSummary *summary, int value) {
    if (summary->args->mode == 1) {
        return;
    }
    if (value == 1) {
        BgCellManager_DrawCellAt(summary->cellManager, 2, 1, 16);
        PokemonSummary_ShowSwitchLabel(summary, 1);
        return;
    }
    BgCellManager_DrawCellAt(summary->cellManager, 3, 1, 16);
    PokemonSummary_ShowSwitchLabel(summary, 0);
}

#pragma force_active on

// Starts flashing a touched button (a tilemap rect of bgId, palette then restorePalette) for PokemonSummary_FlashButton,
// which then goes to nextState. Returns that state (15).
int PokemonSummary_StartButtonFlash(PokemonSummary *summary, u8 bgId, u8 x, u8 y, u8 width, u8 height, u8 palette, u8 restorePalette, u8 nextState) {
    summary->flashX = x;
    summary->flashY = y;
    summary->flashWidth = width;
    summary->flashHeight = height;
    summary->flashPalette = palette;
    summary->restorePalette = restorePalette;
    summary->flashBgId = bgId;
    summary->flashTimer = 0;
    summary->flashNextState = nextState;
    return 15;
}

#pragma force_active off

int PokemonSummary_FlashCancelButton(PokemonSummary *summary, u8 nextState) {
    return PokemonSummary_StartButtonFlash(summary, GF_BG_LYR_SUB_2, 23, 20, 9, 4, 1, 0, nextState);
}

int PokemonSummary_FlashConfirmButton(PokemonSummary *summary, u8 nextState) {
    return PokemonSummary_StartButtonFlash(summary, GF_BG_LYR_SUB_2, 1, 19, 15, 4, 1, 0, nextState);
}

int PokemonSummary_FlashSwapButton(PokemonSummary *summary, u8 nextState) {
    return PokemonSummary_StartButtonFlash(summary, GF_BG_LYR_SUB_2, 1, 17, 10, 2, 1, 0, nextState);
}

// 0: the previous Pokemon button, else the next one.
int PokemonSummary_FlashMonButton(PokemonSummary *summary, int which, u8 nextState) {
    if (which == 0) {
        return PokemonSummary_StartButtonFlash(summary, GF_BG_LYR_SUB_2, 24, 5, 6, 3, 1, 0, nextState);
    } else {
        return PokemonSummary_StartButtonFlash(summary, GF_BG_LYR_SUB_2, 24, 13, 6, 3, 1, 0, nextState);
    }
}

int PokemonSummary_FlashRibbonCancelButton(PokemonSummary *summary, u8 nextState) {
    return PokemonSummary_StartButtonFlash(summary, GF_BG_LYR_SUB_1, 26, 29, 6, 4, 1, 0, nextState);
}

// The previous/next Pokemon buttons, blanked when there is no Pokemon that way.
void PokemonSummary_DrawMonButtons(PokemonSummary *summary) {
    if (PokemonSummary_FindNextMon(summary, -1) != -1) {
        BgCellManager_DrawCellAt(summary->cellManager, 5, 24, 5);
    } else {
        FillBgTilemapRect(summary->bgConfig, GF_BG_LYR_SUB_2, 1, 24, 5, 6, 3, 16);
        ScheduleBgTilemapBufferTransfer(summary->bgConfig, GF_BG_LYR_SUB_2);
    }

    if (PokemonSummary_FindNextMon(summary, 1) != -1) {
        BgCellManager_DrawCellAt(summary->cellManager, 6, 24, 13);
    } else {
        FillBgTilemapRect(summary->bgConfig, GF_BG_LYR_SUB_2, 1, 24, 13, 6, 3, 16);
        ScheduleBgTilemapBufferTransfer(summary->bgConfig, GF_BG_LYR_SUB_2);
    }
}
