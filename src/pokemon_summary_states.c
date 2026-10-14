#include "global.h"

#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/sndseq.h"
#include "constants/species.h"

#include "msgdata/msg/msg_0302.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "item.h"
#include "message_format.h"
#include "move.h"
#include "msgdata.h"
#include "options.h"
#include "pokemon.h"
#include "pokemon_summary_internal.h"
#include "render_window.h"
#include "ribbon.h"
#include "screen_fade.h"
#include "sound_chatot.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_system.h"
#include "system.h"
#include "ui_util.h"
#include "pokemon_summary.h"

// Pokemon summary screen (ARM9 static 0x02088D48-0x02089E80): the move, ribbon and condition input states, the
// button flash, reading the shown Pokemon into PokemonSummary.mon, and switching pages. The force_active region keeps
// that function in the link as before.

// pokemon_summary_scene.c, declared with the type its call site was matched against.
void PokemonSummary_InitConditionChart(PokemonSummary *summary);

// Called without its unused last argument.
#define PokemonSummary_SwapMoveTypeSprites3(summary, slotA, slotB) ((void (*)(PokemonSummary *, u8, u8))PokemonSummary_SwapMoveTypeSprites)(summary, slotA, slotB)

// State 5: pick a move to move (A) or leave the move list (B).
int PokemonSummary_HandleMoveSelectInput(PokemonSummary *summary) {
    u32 keys;
    int index;

    keys = gSystem.newKeys;

    if (keys & PAD_KEY_UP) {
        if (PokemonSummary_MoveMoveCursor(summary, -1) == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            PokemonSummary_ShowSelectedMoveInfo(summary);
        }
        return 5;
    }
    if (keys & PAD_KEY_DOWN) {
        if (PokemonSummary_MoveMoveCursor(summary, 1) == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            PokemonSummary_ShowSelectedMoveInfo(summary);
        }
        return 5;
    }
    if ((keys & PAD_BUTTON_A) && summary->args->mode != 1) {
        PlaySE(SEQ_SE_DP_DECIDE);
        return PokemonSummary_FlashSwapButton(summary, 7);
    }
    if (keys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        PokemonSummary_DrawSwitchButton(summary, 0);
        return PokemonSummary_FlashCancelButton(summary, 4);
    }
    index = PokemonSummary_GetMoveSelectTouch(summary);
    if (index == 4) {
        if (summary->args->mode != 1) {
            PlaySE(SEQ_SE_DP_DECIDE);
            return PokemonSummary_FlashSwapButton(summary, 7);
        }
    } else if (index == 5) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        PokemonSummary_DrawSwitchButton(summary, 0);
        return PokemonSummary_FlashCancelButton(summary, 4);
    } else if (index != -1 && summary->mon.moves[index] != MOVE_NONE) {
        summary->moveSlot = index;
        PlaySE(SEQ_SE_DP_SELECT);
        PokemonSummary_ShowSelectedMoveInfo(summary);
        return 5;
    }
    return 5;
}

// State 7: show the swap cursor on the picked move.
int PokemonSummary_StartMoveSwap(PokemonSummary *summary) {
    PokemonSummary_DrawSwitchButton(summary, 0);
    PokemonSummary_ShowSwapCursorSprite(summary);
    summary->swapMoveSlot = summary->moveSlot;
    return 6;
}

// State 6: pick the move to swap with.
int PokemonSummary_HandleMoveSwapInput(PokemonSummary *summary) {
    int index;

    if (gSystem.newKeys & PAD_KEY_UP) {
        if (PokemonSummary_MoveMoveCursor(summary, -1) == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            PokemonSummary_ShowSelectedMoveInfo(summary);
        }
        return 6;
    }
    if (gSystem.newKeys & PAD_KEY_DOWN) {
        if (PokemonSummary_MoveMoveCursor(summary, 1) == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            PokemonSummary_ShowSelectedMoveInfo(summary);
        }
        return 6;
    }
    if (gSystem.newKeys & PAD_BUTTON_A) {
        PokemonSummary_DrawSwitchButton(summary, 1);
        Sprite_SetDrawFlag(summary->sprites[10], FALSE);
        if (summary->moveSlot != summary->swapMoveSlot) {
            PlaySE(SEQ_SE_DP_DECIDE);
            PokemonSummary_SwapMoves(summary);
            PokemonSummary_SwapMoveTypeSprites3(summary, summary->moveSlot, summary->swapMoveSlot);
            PokemonSummary_PrintSwappedMoves(summary);
            PokemonSummary_ShowSelectedMoveInfo(summary);
        } else {
            PlaySE(SEQ_SE_DP_SELECT);
        }
        return 5;
    }
    if (gSystem.newKeys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        PokemonSummary_DrawSwitchButton(summary, 1);
        Sprite_SetDrawFlag(summary->sprites[10], FALSE);
        return PokemonSummary_FlashCancelButton(summary, 5);
    }
    index = PokemonSummary_GetMoveSwapTouch(summary);
    if (index == 4) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        PokemonSummary_DrawSwitchButton(summary, 1);
        Sprite_SetDrawFlag(summary->sprites[10], FALSE);
        return PokemonSummary_FlashCancelButton(summary, 5);
    }
    if (index != -1 && summary->mon.moves[index] != MOVE_NONE) {
        PokemonSummary_DrawSwitchButton(summary, 1);
        Sprite_SetDrawFlag(summary->sprites[10], FALSE);
        if (index != summary->swapMoveSlot) {
            summary->moveSlot = index;
            PlaySE(SEQ_SE_DP_DECIDE);
            PokemonSummary_SwapMoves(summary);
            PokemonSummary_SwapMoveTypeSprites3(summary, summary->moveSlot, summary->swapMoveSlot);
            PokemonSummary_PrintSwappedMoves(summary);
            PokemonSummary_ShowSelectedMoveInfo(summary);
        } else {
            PlaySE(SEQ_SE_DP_SELECT);
        }
        return 5;
    }
    return 6;
}

// State 8 (learning a move): pick the move to forget; slot 4 is the move to learn.
int PokemonSummary_HandleForgetMoveInput(PokemonSummary *summary) {
    u32 keys;
    int index;

    keys = gSystem.newKeys;

    if (keys & PAD_KEY_UP) {
        if (PokemonSummary_MoveMoveCursor(summary, -1) == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            PokemonSummary_ShowSelectedMoveInfo(summary);
        }
        return 8;
    }
    if (keys & PAD_KEY_DOWN) {
        if (PokemonSummary_MoveMoveCursor(summary, 1) == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            PokemonSummary_ShowSelectedMoveInfo(summary);
        }
        return 8;
    }
    if (keys & PAD_BUTTON_A) {
        PlaySE(SEQ_SE_DP_DECIDE);
        index = summary->moveSlot;
        if (index == 4) {
            summary->args->selectedMoveSlot = 4;
            summary->args->exitedWithB = 0;
            return 21;
        }
        if (MoveIsHM(summary->mon.moves[index]) == TRUE && summary->args->moveToLearn != MOVE_NONE) {
            thunk_Sprite_SetDrawFlag(summary->sprites[18], FALSE);
            PokemonSummary_PrintCantForgetHMMessage(summary);
            return 8;
        }
        return PokemonSummary_ShowForgetConfirm(summary);
    }
    if (keys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        summary->args->selectedMoveSlot = 4;
        summary->args->exitedWithB = 1;
        return PokemonSummary_FlashCancelButton(summary, 21);
    }

    index = PokemonSummary_GetForgetMoveTouch(summary);
    switch (index) {
    case -1:
        break;
    case 4:
        if (summary->args->moveToLearn != MOVE_NONE) {
            PlaySE(SEQ_SE_DP_DECIDE);
            summary->moveSlot = 4;
            PokemonSummary_ShowSelectedMoveInfo(summary);
        }
        break;
    case 5:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        summary->moveSlot = 5;
        summary->args->selectedMoveSlot = 4;
        summary->args->exitedWithB = 0;
        return PokemonSummary_FlashCancelButton(summary, 21);
    default:
        if (MoveIsHM(summary->mon.moves[index]) == TRUE && summary->args->moveToLearn != MOVE_NONE) {
            PlaySE(SEQ_SE_DP_DECIDE);
            summary->moveSlot = index;
            thunk_Sprite_SetDrawFlag(summary->sprites[18], FALSE);
            PokemonSummary_UpdateMoveCursorSprite(summary);
            PokemonSummary_PrintCantForgetHMMessage(summary);
            return 8;
        }
        if (summary->mon.moves[index] != MOVE_NONE) {
            PlaySE(SEQ_SE_DP_DECIDE);
            summary->moveSlot = index;
            PokemonSummary_ShowSelectedMoveInfo(summary);
            return PokemonSummary_ShowForgetConfirm(summary);
        }
        break;
    }
    return 8;
}

// State 9: confirm forgetting the picked move.
int PokemonSummary_HandleForgetMoveConfirmInput(PokemonSummary *arg) {
    struct System *system = &gSystem;
    PokemonSummary *summary = arg;
    u32 keys;
    int input;

    keys = system->newKeys;

    if (keys & PAD_BUTTON_A) {
        PlaySE(SEQ_SE_DP_DECIDE);
        summary->args->selectedMoveSlot = summary->moveSlot;
        summary->args->exitedWithB = 0;
        return PokemonSummary_FlashConfirmButton(summary, 21);
    }
    if (keys & PAD_BUTTON_B) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        Sprite_SetAnimCtrlSeq(summary->sprites[9], 0);
        Sprite_SetDrawFlag(summary->sprites[10], FALSE);
        PokemonSummary_DrawForgetConfirm(summary, 0);
        return PokemonSummary_FlashCancelButton(summary, 8);
    }
    input = PokemonSummary_GetForgetConfirmTouch(summary);
    if (input != -1) {
        switch (input) {
        case 0:
            PlaySE(SEQ_SE_DP_DECIDE);
            summary->args->selectedMoveSlot = summary->moveSlot;
            summary->args->exitedWithB = 0;
            return PokemonSummary_FlashConfirmButton(summary, 21);
        case 1:
            PlaySE(SEQ_SE_GS_GEARCANCEL);
            Sprite_SetAnimCtrlSeq(summary->sprites[9], 0);
            Sprite_SetDrawFlag(summary->sprites[10], FALSE);
            PokemonSummary_DrawForgetConfirm(summary, 0);
            return PokemonSummary_FlashCancelButton(summary, 8);
        default:
            break;
        }
    }
    return 9;
}

// State 10: open the ribbon list.
int PokemonSummary_OpenRibbonList(PokemonSummary *summary) {
    if (PokemonSummary_SlideInRibbonList(summary) == 1) {
        return 12;
    }
    return 10;
}

// State 11: close the ribbon list.
int PokemonSummary_CloseRibbonList(PokemonSummary *summary) {
    if (PokemonSummary_SlideOutRibbonList(summary) == 1) {
        return 2;
    }
    return 11;
}

#pragma force_active on

// State 12: move the ribbon cursor; the arrows (sprites 64, 65) scroll by a page.
int PokemonSummary_HandleRibbonInput(PokemonSummary *summary) {
    int selection;

    if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        PokemonSummary_MoveRibbonCursor(summary, -1);
        return 12;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        PokemonSummary_MoveRibbonCursor(summary, 1);
        return 12;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        PokemonSummary_MoveRibbonCursor(summary, -3);
        return 12;
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        PokemonSummary_MoveRibbonCursor(summary, 3);
        return 12;
    }
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return PokemonSummary_FlashRibbonCancelButton(summary, 11);
    }
    selection = PokemonSummary_GetRibbonTouch(summary);
    switch (selection) {
    case -1:
        break;
    case 9:
        if (Sprite_GetDrawFlag(summary->sprites[64]) == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            Sprite_SetAnimCtrlSeq(summary->sprites[64], 2);
            return 13;
        }
        break;
    case 10:
        if (Sprite_GetDrawFlag(summary->sprites[65]) == TRUE) {
            PlaySE(SEQ_SE_DP_SELECT);
            Sprite_SetAnimCtrlSeq(summary->sprites[65], 3);
            return 14;
        }
        break;
    case 11:
        PlaySE(SEQ_SE_GS_GEARCANCEL);
        return PokemonSummary_FlashRibbonCancelButton(summary, 11);
    default:
        if (summary->ribbonCursor != selection) {
            summary->ribbonCursor = selection;
            PlaySE(SEQ_SE_DP_SELECT);
            PokemonSummary_MoveRibbonCursor(summary, 0);
        }
        return 12;
    }
    return 12;
}

#pragma force_active off

// State 13: the up arrow was touched; scroll up a page once its animation ends.
int PokemonSummary_WaitRibbonPageUp(PokemonSummary *summary) {
    if (Sprite_IsAnimated(summary->sprites[64]) == FALSE) {
        PokemonSummary_MoveRibbonCursor(summary, -9);
        return 12;
    }
    return 13;
}

// State 14: the same for the down arrow.
int PokemonSummary_WaitRibbonPageDown(PokemonSummary *summary) {
    if (Sprite_IsAnimated(summary->sprites[65]) == FALSE) {
        PokemonSummary_MoveRibbonCursor(summary, 9);
        return 12;
    }
    return 14;
}

// State 16: on A or B, read the condition again; unk7BE gets one bit per stat (cool..tough) that changed.
int PokemonSummary_UpdateCondition(PokemonSummary *summary) {
    Pokemon *mon;
    u8 old;

    if ((gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) == 0) {
        goto no_input;
    }
    if (summary->args->monDataType == 2) {
        BoxPokemon *boxMon = PokemonSummary_GetShownMon(summary);
        mon = AllocMonZeroed(HEAP_ID_19);
        CopyBoxPokemonToPokemon(boxMon, mon);
    } else {
        mon = PokemonSummary_GetShownMon(summary);
    }
    summary->unk7BE = 0;
    old = summary->mon.contestStats[0];
    summary->mon.contestStats[0] = GetMonData(mon, MON_DATA_COOL, NULL);
    if (old != summary->mon.contestStats[0]) {
        summary->unk7BE |= 1;
    }
    old = summary->mon.contestStats[1];
    summary->mon.contestStats[1] = GetMonData(mon, MON_DATA_BEAUTY, NULL);
    if (old != summary->mon.contestStats[1]) {
        summary->unk7BE |= 2;
    }
    old = summary->mon.contestStats[2];
    summary->mon.contestStats[2] = GetMonData(mon, MON_DATA_CUTE, NULL);
    if (old != summary->mon.contestStats[2]) {
        summary->unk7BE |= 4;
    }
    old = summary->mon.contestStats[3];
    summary->mon.contestStats[3] = GetMonData(mon, MON_DATA_SMART, NULL);
    if (old != summary->mon.contestStats[3]) {
        summary->unk7BE |= 8;
    }
    old = summary->mon.contestStats[4];
    summary->mon.contestStats[4] = GetMonData(mon, MON_DATA_TOUGH, NULL);
    if (old != summary->mon.contestStats[4]) {
        summary->unk7BE |= 0x10;
    }
    summary->mon.contestStats[5] = GetMonData(mon, MON_DATA_SHEEN, NULL);
    if (summary->args->monDataType == 2) {
        Heap_Free(mon);
    }
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, HEAP_ID_19);
    LoadUserFrameGfx2(summary->bgConfig, GF_BG_LYR_MAIN_1, 0x3E2, 13, Options_GetFrame(summary->args->options), HEAP_ID_19);
    if (summary->unk7BE == 0) {
        PokemonSummary_PrintConditionChangeMessage(summary, 0xFE);
        return 18;
    }
    PokemonSummary_InitConditionChart(summary);
    PokemonSummary_InitSheenSparkles(summary);
    PokemonSummary_ShowConditionChangeSprites(summary);
    return 17;
no_input:
    return 16;
}

// State 17: on A or B, show the next changed condition stat.
int PokemonSummary_ShowConditionChanges(PokemonSummary *summary) {
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        u8 changes = summary->unk7BE;
        u8 i = 0;
        do {
            u32 mask = 1 << i;
            if (changes & mask) {
                PokemonSummary_PrintConditionChangeMessage(summary, i);
                summary->unk7BE ^= mask;
                if (summary->unk7BE == 0) {
                    return 18;
                }
                break;
            }
            i = (u8)(i + 1);
        } while (i < 5);
    }
    return 17;
}

// State 18: leave on A or B.
int PokemonSummary_WaitConditionExit(PokemonSummary *summary) {
    if (gSystem.newKeys & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        return 21;
    }
    return 18;
}

// State 21.
int PokemonSummary_StartFadeOut(PokemonSummary *summary) {
    BeginAppScreenFade(1, HEAP_ID_19);
    return 22;
}

// State 22.
int PokemonSummary_WaitFadeOut(PokemonSummary *summary) {
    return (u8)(IsPaletteFadeFinished() == TRUE);
}

// State 15: flash a touched button (see PokemonSummary_StartButtonFlash), then go to flashNextState.
int PokemonSummary_FlashButton(PokemonSummary *summary) {
    switch (summary->unk7BE) {
    case 0:
        BgTilemapRectChangePalette(summary->bgConfig, summary->flashBgId, summary->flashX, summary->flashY, summary->flashWidth, summary->flashHeight, summary->flashPalette);
        ScheduleBgTilemapBufferTransfer(summary->bgConfig, summary->flashBgId);
        summary->unk7BE++;
        break;
    case 1:
        summary->flashTimer++;
        if (summary->flashTimer == 4) {
            BgTilemapRectChangePalette(summary->bgConfig, summary->flashBgId, summary->flashX, summary->flashY, summary->flashWidth, summary->flashHeight, summary->restorePalette);
            ScheduleBgTilemapBufferTransfer(summary->bgConfig, summary->flashBgId);
            summary->flashTimer = 0;
            summary->unk7BE++;
        }
        break;
    case 2:
        summary->flashTimer++;
        if (summary->flashTimer == 2) {
            summary->unk7BE = 0;
            return summary->flashNextState;
        }
        break;
    }
    return 15;
}

// State 20: one of the previous/next Pokemon buttons was touched.
int PokemonSummary_ChangeMonByButton(PokemonSummary *summary) {
    if (summary->flashY == 5) {
        PokemonSummary_ChangeMon(summary, -1);
    } else {
        PokemonSummary_ChangeMon(summary, 1);
    }
    PokemonSummary_DrawMonButtons(summary);
    return 19;
}

void PokemonSummary_ReadShownMon(PokemonSummary *summary) {
    void *mon = PokemonSummary_GetShownMon(summary);
    if (summary->args->monDataType == 2) {
        PokemonSummary_ReadBoxMon(summary, mon, &summary->mon);
    } else {
        PokemonSummary_ReadMon(summary, mon, &summary->mon);
    }
}

void PokemonSummary_ReadBoxMon(PokemonSummary *summary, BoxPokemon *boxMon, PokemonSummaryMon *out) {
    Pokemon *mon = AllocMonZeroed(HEAP_ID_19);
    CopyBoxPokemonToPokemon(boxMon, mon);
    PokemonSummary_ReadMon(summary, mon, out);
    Heap_Free(mon);
}

void PokemonSummary_ReadMon(PokemonSummary *summary, Pokemon *mon, PokemonSummaryMon *out) {
    BOOL decrypted;
    BoxPokemon *boxMon;
    u16 i;

    decrypted = AcquireMonLock(mon);
    out->species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    boxMon = Mon_GetBoxMon(mon);

    ReadMsgDataIntoString(summary->msgData, msg_0302_00011, summary->buffer);
    BufferBoxMonSpeciesName(summary->msgFormat, 0, boxMon);
    StringExpandPlaceholders(summary->msgFormat, summary->mon.speciesName, summary->buffer);

    ReadMsgDataIntoString(summary->msgData, msg_0302_00000, summary->buffer);
    BufferBoxMonNickname(summary->msgFormat, 0, boxMon);
    StringExpandPlaceholders(summary->msgFormat, summary->mon.nickname, summary->buffer);

    ReadMsgDataIntoString(summary->msgData, msg_0302_00014, summary->buffer);
    BufferBoxMonOTName(summary->msgFormat, 0, boxMon);
    StringExpandPlaceholders(summary->msgFormat, summary->mon.otName, summary->buffer);

    out->heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    out->level = GetMonData(mon, MON_DATA_LEVEL, NULL);
    out->isEgg = GetMonData(mon, MON_DATA_IS_EGG, NULL);

    if (GetMonData(mon, MON_DATA_NO_PRINT_GENDER, NULL) == TRUE && out->isEgg == FALSE) {
        out->noPrintGender = 0;
    } else {
        out->noPrintGender = 1;
    }

    out->gender = GetMonGender(mon);
    out->pokeball = GetMonData(mon, MON_DATA_POKEBALL, NULL);
    out->type1 = GetMonData(mon, MON_DATA_TYPE_1, NULL);
    out->type2 = GetMonData(mon, MON_DATA_TYPE_2, NULL);
    out->otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
    out->exp = GetMonData(mon, MON_DATA_EXPERIENCE, NULL);
    out->otGender = GetMonData(mon, MON_DATA_OT_GENDER, NULL);

    out->curLevelExp = GetMonExpBySpeciesAndLevel(out->species, out->level);
    if (out->level == 100) {
        out->nextLevelExp = out->curLevelExp;
    } else {
        out->nextLevelExp = GetMonExpBySpeciesAndLevel(out->species, out->level + 1);
    }

    out->hp = GetMonData(mon, MON_DATA_HP, NULL);
    out->maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
    out->attack = GetMonData(mon, MON_DATA_ATK, NULL);
    out->defense = GetMonData(mon, MON_DATA_DEF, NULL);
    out->spAttack = GetMonData(mon, MON_DATA_SP_ATK, NULL);
    out->spDefense = GetMonData(mon, MON_DATA_SP_DEF, NULL);
    out->speed = GetMonData(mon, MON_DATA_SPEED, NULL);
    out->ability = GetMonData(mon, MON_DATA_ABILITY, NULL);
    out->nature = GetMonNature(mon);

    for (i = 0; i < MAX_MON_MOVES; i++) {
        u8 ppUps;
        out->moves[i] = GetMonData(mon, MON_DATA_MOVE1 + i, NULL);
        out->pp[i] = GetMonData(mon, MON_DATA_MOVE1_PP + i, NULL);
        ppUps = GetMonData(mon, MON_DATA_MOVE1_PP_UPS + i, NULL);
        out->maxPP[i] = GetMoveMaxPP(out->moves[i], ppUps);
    }

    out->contestStats[0] = GetMonData(mon, MON_DATA_COOL, NULL);
    out->contestStats[1] = GetMonData(mon, MON_DATA_BEAUTY, NULL);
    out->contestStats[2] = GetMonData(mon, MON_DATA_CUTE, NULL);
    out->contestStats[3] = GetMonData(mon, MON_DATA_SMART, NULL);
    out->contestStats[4] = GetMonData(mon, MON_DATA_TOUGH, NULL);
    out->contestStats[5] = GetMonData(mon, MON_DATA_SHEEN, NULL);
    out->favoriteFlavor = 5;
    for (i = 0; i < 5; i++) {
        if (MonGetFlavorPreference(mon, i) == 1) {
            out->favoriteFlavor = i;
            break;
        }
    }

    out->markings = GetMonData(mon, MON_DATA_MARKINGS, NULL);
    out->form = GetMonData(mon, MON_DATA_FORM, NULL);
    out->status = Pokemon_GetStatusIconId(mon);
    if (Pokemon_IsImmuneToPokerus(mon) == TRUE) {
        out->pokerus = 2;
    } else if (Pokemon_HasPokerus(mon) == TRUE) {
        out->pokerus = 1;
        if (out->status == 7) {
            out->status = 0;
        }
    } else {
        out->pokerus = 0;
    }

    if (MonIsShiny(mon) == TRUE) {
        out->shiny = 1;
    } else {
        out->shiny = 0;
    }

    out->ribbons[0] = 0;
    out->ribbons[1] = 0;
    out->ribbons[2] = 0;
    out->ribbons[3] = 0;
    summary->ribbonCount = 0;
    for (i = 0; i < 80; i++) {
        if (GetMonData(mon, GetRibbonAttr(i, RIBBONDAT_MONDATNO), NULL) != 0) {
            out->ribbons[i / 32] |= 1 << (i & 31);
            summary->ribbonCount++;
        }
    }

    for (i = 0; i < 6; i++) {
        summary->mon.shinyLeaves[i] = GetMonData(mon, MON_DATA_SHINY_LEAF_A + i, NULL);
    }
    ReleaseMonLock(mon, decrypted);
}

void PokemonSummary_PlayCry(PokemonSummary *summary) {
    u8 delay;
    if (summary->mon.isEgg == FALSE) {
        NARC_ReadPokepicCryDelay(summary->pokepicAnimNarc, &delay, summary->mon.species, 1);
        if (summary->mon.species == SPECIES_CHATOT) {
            Chatot_PlayCryWithDelayedFallback((SOUND_CHATOT *)summary->args->chatot, 0, 100, 0, delay);
        } else {
            PlayCryDelayed(summary->mon.species, delay, summary->mon.form);
        }
    }
}

// Shows the first page: the move page when picking a move to forget (args->mode == 2), else the info page.
void PokemonSummary_ShowFirstPage(PokemonSummary *summary) {
    switch (summary->args->mode) {
    case 0:
    case 1:
        summary->page = 0;
        break;
    case 2:
        summary->page = 1;
        break;
    }
    PokemonSummary_HideSprites0To7And21And22(summary);
    PokemonSummary_UpdateTypeSprites(summary);
    PokemonSummary_ShowRibbonSprites(summary);
    PokemonSummary_UpdateMarkingSprites(summary);
    PokemonSummary_InitSheenSparkles(summary);
    PokemonSummary_AddPageWindows(summary);
    PokemonSummary_PrintPage(summary);
    PokemonSummary_LoadPageBgs(summary);
    PokemonSummary_UpdateShinyLeafSprites(summary);
    if (summary->args->mode == 2) {
        PokemonSummary_InitForgetMove(summary);
        return;
    }
    PokemonSummary_UpdatePartySlotSprite(summary);
    PokemonSummary_SetBoxMonIconPos(summary);
    PokemonSummary_AlternateBoxMonIcon(summary);
}

// Which pages can be shown (args->pageFlags, one bit per page): pages lists them, ended by 4.
void PokemonSummary_SetPages(PokemonSummaryArgs *args, const u8 *pages) {
    u8 i = 0;
    args->pageFlags = 0;
    do {
        u8 page = pages[i];
        if (page == 4) {
            break;
        }
        args->pageFlags |= 1 << page;
        i++;
    } while (i < 4);
}

void PokemonSummary_SetPage(PokemonSummary *summary, int page) {
    if (summary->page == page) {
        return;
    }
    PokemonSummary_RemovePageWindows(summary);
    summary->page = page;
    PokemonSummary_HideSprites0To7And21And22(summary);
    PokemonSummary_UpdateTypeSprites(summary);
    PokemonSummary_UpdatePokeathlonStars(summary);
    PokemonSummary_UpdateShinyLeafSprites(summary);
    PokemonSummary_ShowRibbonSprites(summary);
    PokemonSummary_InitSheenSparkles(summary);
    PokemonSummary_AddPageWindows(summary);
    FillBgTilemapRect(summary->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 3, 18, 20, 0);
    BgCommitTilemapBufferToVram(summary->bgConfig, GF_BG_LYR_MAIN_1);
    FillBgTilemapRect(summary->bgConfig, GF_BG_LYR_SUB_0, 0, 0, 3, 18, 20, 0);
    BgCommitTilemapBufferToVram(summary->bgConfig, GF_BG_LYR_SUB_0);
    PokemonSummary_PrintPage(summary);
    PokemonSummary_LoadPageBgs(summary);
    if (summary->args->mode == 2) {
        PokemonSummary_UpdateForgetMovePage(summary);
    }
}

void PokemonSummary_ChangePage(PokemonSummary *summary, int page) {
    PlaySE(SEQ_SE_DP_SELECT5);
    PokemonSummary_SetPage(summary, (u8)page);
}

// Left/right: the next shown page in that direction (eggs have only the first page).
void PokemonSummary_ScrollPage(PokemonSummary *summary, int direction) {
    s8 page = summary->page;
    if (summary->mon.isEgg) {
        return;
    }
    u8 pages = summary->args->pageFlags;
    do {
        page += direction;
        if (page < 0) {
            page = 2;
        } else if (page > 2) {
            page = 0;
        }
    } while ((pages & (1 << page)) == 0);
    if (page != summary->page) {
        PokemonSummary_ChangePage(summary, page);
    }
}
