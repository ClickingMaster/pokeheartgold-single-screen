#include "global.h"

#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0302.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "message_printer.h"
#include "move.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokedex_util.h"
#include "pokemon.h"
#include "pokemon_summary_internal.h"
#include "render_window.h"
#include "ribbon.h"
#include "string_util.h"
#include "text.h"
#include "trainer_memo.h"
#include "pokemon_summary.h"

// Pokemon summary screen text (ARM9 static 0x0208C3E4-0x0208DE40): the screen's windows and the text of each page.
// Window templates: asm/pokemon_summary_text_data.s.

extern const WindowTemplate gPokemonSummaryRibbonWindowTemplates[];
extern const WindowTemplate gPokemonSummaryInfoWindowTemplates[];
extern const WindowTemplate gPokemonSummaryStatsWindowTemplates[];
extern const WindowTemplate gPokemonSummaryWindowTemplates[34];
extern const s8 gNatureStatMods[][5];

// PokemonSummary_PrintTrainerMemo calls it with three arguments.
#define sub_0208E600_3(mon, isMine, heapID) ((TrainerMemo * (*)(void *, int, enum HeapID)) TrainerMemo_New)(mon, isMine, heapID)

void PokemonSummary_AddWindows(PokemonSummary *summary) {
    u16 i;
    for (i = 0; i < 34; i++) {
        AddWindow(summary->bgConfig, &summary->windows[i], &gPokemonSummaryWindowTemplates[i]);
        FillWindowPixelBuffer(&summary->windows[i], 0);
    }
    PokemonSummary_PrintLabels(summary);
    PokemonSummary_PrintNickname(summary);
    PokemonSummary_PrintLevel(summary);
    PokemonSummary_PrintHeldItem(summary);
}

void PokemonSummary_AddPageWindows(PokemonSummary *summary) {
    const WindowTemplate *templates;
    u32 i;

    switch (summary->page) {
    case 0:
        templates = gPokemonSummaryInfoWindowTemplates;
        summary->numPageWindows = 8;
        break;
    case 1:
        templates = gPokemonSummaryStatsWindowTemplates;
        summary->numPageWindows = 18;
        break;
    case 2:
        templates = gPokemonSummaryRibbonWindowTemplates;
        summary->numPageWindows = 8;
        break;
    case 3:
        if (summary->args->mode != 3 && summary->args->mode != 4) {
            return;
        }
        summary->numPageWindows = 3;
        break;
    default:
        return;
    }

    summary->pageWindows = AllocWindows(HEAP_ID_19, (u8)summary->numPageWindows);
    for (i = 0; i < summary->numPageWindows; i++) {
        AddWindow(summary->bgConfig, &summary->pageWindows[i], &templates[i]);
    }
}

void PokemonSummary_RemovePageWindows(PokemonSummary *summary) {
    u32 i;
    switch (summary->page) {
    case 0:
    case 1:
        for (i = 0; i < summary->numPageWindows; i++) {
            ClearWindowTilemapAndCopyToVram(&summary->pageWindows[i]);
        }
        break;
    case 2:
        ClearWindowTilemapAndCopyToVram(summary->pageWindows);
        break;
    case 3:
        if (summary->args->mode != 3 && summary->args->mode != 4) {
            return;
        }
        break;
    default:
        return;
    }
    WindowArray_Delete(summary->pageWindows, (u8)summary->numPageWindows);
}

void PokemonSummary_RemoveWindows(PokemonSummary *summary) {
    u32 i;
    Window *window;
    PokemonSummary_RemovePageWindows(summary);
    for (i = 0, window = summary->windows; i < 34; i++, window++) {
        RemoveWindow(window);
    }
}

void PokemonSummary_PrintNickname(PokemonSummary *summary) {
    Window *window = &summary->windows[33];
    u32 value;
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, summary->mon.nickname, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 15, 0), NULL);
    value = summary->mon.noPrintGender;
    if (value == 0) {
        value = summary->mon.gender;
        if (value == MON_MALE) {
            ReadMsgDataIntoString(summary->msgData, msg_0302_00001, summary->buffer);
            PokemonSummary_PrintBuffer(summary, window, MAKE_TEXT_COLOR(3, 4, 0), 1);
        } else if (value == MON_FEMALE) {
            ReadMsgDataIntoString(summary->msgData, msg_0302_00002, summary->buffer);
            PokemonSummary_PrintBuffer(summary, window, MAKE_TEXT_COLOR(5, 6, 0), 1);
        }
    }
    ScheduleWindowCopyToVram(window);
}

void PokemonSummary_PrintLevel(PokemonSummary *summary) {
    Window *window = &summary->windows[32];
    String *template;
    u32 value;
    FillWindowPixelBuffer(window, 0);
    value = summary->mon.isEgg;
    if (value == 0) {
        MessagePrinter_PrintGlyph(summary->numberPrinter, 1, window, 0, 5);
        template = NewString_ReadMsgData(summary->msgData, msg_0302_00003);
        value = summary->mon.level;
        BufferIntegerAsString(summary->msgFormat, 0, value, 3, PRINTING_MODE_LEFT_ALIGN, TRUE);
        StringExpandPlaceholders(summary->msgFormat, summary->buffer, template);
        String_Delete(template);
        AddTextPrinterParameterizedWithColor(window, 0, summary->buffer, 16, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }
    ScheduleWindowCopyToVram(window);
}

void PokemonSummary_PrintHeldItem(PokemonSummary *summary) {
    String *template;
    FillWindowPixelBuffer(&summary->windows[31], 0);
    if (summary->mon.heldItem != ITEM_NONE) {
        BufferItemName(summary->msgFormat, 0, summary->mon.heldItem);
        template = NewString_ReadMsgData(summary->msgData, msg_0302_00005);
        StringExpandPlaceholders(summary->msgFormat, summary->buffer, template);
        String_Delete(template);
    } else {
        ReadMsgDataIntoString(summary->msgData, msg_0302_00006, summary->buffer);
    }
    PokemonSummary_PrintBuffer(summary, &summary->windows[31], MAKE_TEXT_COLOR(1, 2, 0), 0);
    ScheduleWindowCopyToVram(&summary->windows[6]);
    ScheduleWindowCopyToVram(&summary->windows[31]);
}

int PokemonSummary_IsPlayersMon(PokemonSummary *summary) {
    PokemonSummaryArgs *args = summary->args;
    if (summary->mon.otId == args->playerId && summary->mon.otGender == args->playerGender && String_Compare(summary->mon.otName, summary->playerName) == 0) {
        return TRUE;
    }
    return FALSE;
}

// alignment: 0 left, 1 right, 2 centered, otherwise the x position.
void PokemonSummary_PrintBuffer(PokemonSummary *summary, Window *window, u32 color, int alignment) {
    int x;
    switch (alignment) {
    case 0:
        x = 0;
        break;
    case 1: {
        u8 width = FontID_String_GetWidth(0, summary->buffer, 0);
        x = (u8)((u8)(GetWindowWidth(window) * 8) - width);
        break;
    }
    case 2: {
        u8 width = FontID_String_GetWidth(0, summary->buffer, 0);
        x = (u8)(((u8)(GetWindowWidth(window) * 8) - width) / 2);
        break;
    }
    default:
        x = alignment;
        break;
    }
    AddTextPrinterParameterizedWithColor(window, 0, summary->buffer, x, 0, TEXT_SPEED_NOTRANSFER, color, NULL);
}

// The stat's label, coloured by the nature's effect on it.
void PokemonSummary_PrintStatLabel(PokemonSummary *summary, int window, int msgNo, int stat, int alignment) {
    u32 color = MAKE_TEXT_COLOR(14, 15, 0);
    int mod = gNatureStatMods[summary->mon.nature][stat];
    if (mod < 0) {
        color = MAKE_TEXT_COLOR(14, 8, 0);
    } else if (mod > 0) {
        color = MAKE_TEXT_COLOR(14, 7, 0);
    }
    ReadMsgDataIntoString(summary->msgData, msgNo, summary->buffer);
    {
        Window *windows = summary->windows;
        PokemonSummary_PrintBuffer(summary, &windows[window], color, alignment);
    }
}

void PokemonSummary_PrintMessage(PokemonSummary *summary, int window, int msgNo, u32 color, int alignment) {
    Window *windows;
    ReadMsgDataIntoString(summary->msgData, msgNo, summary->buffer);
    windows = summary->windows;
    PokemonSummary_PrintBuffer(summary, &windows[window], color, alignment);
}

void PokemonSummary_FormatNumber(PokemonSummary *summary, int msgNo, int value, int digits, u8 mode) {
    String *template = NewString_ReadMsgData(summary->msgData, msgNo);
    BufferIntegerAsString(summary->msgFormat, 0, value, digits, (PrintingMode)mode, TRUE);
    StringExpandPlaceholders(summary->msgFormat, summary->buffer, template);
    String_Delete(template);
}

// "first / second" around a label centered on centerX.
void PokemonSummary_PrintValuePair(PokemonSummary *summary, int window, int labelMsg, int firstMsg, int secondMsg, u16 firstValue, u16 secondValue, volatile u8 digits, u8 centerX, u8 y) {
    Window *target = &summary->pageWindows[window];
    u32 width;
    u16 x;
    u16 end;

    ReadMsgDataIntoString(summary->msgData, labelMsg, summary->buffer);
    width = FontID_String_GetWidth(0, summary->buffer, 0);
    x = centerX - width / 2;
    end = x + width;
    AddTextPrinterParameterizedWithColor(target, 0, summary->buffer, x, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    PokemonSummary_FormatNumber(summary, firstMsg, firstValue, digits, PRINTING_MODE_LEFT_ALIGN);
    width = FontID_String_GetWidth(0, summary->buffer, 0);
    AddTextPrinterParameterizedWithColor(target, 0, summary->buffer, x - width, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    PokemonSummary_FormatNumber(summary, secondMsg, secondValue, digits, PRINTING_MODE_LEFT_ALIGN);
    AddTextPrinterParameterizedWithColor(target, 0, summary->buffer, end, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
}

void PokemonSummary_PrintLabels(PokemonSummary *summary) {
    PokemonSummary_PrintMessage(summary, 0, msg_0302_00007, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 1, msg_0302_00023, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 2, msg_0302_00109, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 3, msg_0302_00126, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 4, msg_0302_00128, MAKE_TEXT_COLOR(14, 15, 0), 0);
    if (summary->args->isFlag982Set) {
        PokemonSummary_PrintMessage(summary, 5, msg_0302_00157, MAKE_TEXT_COLOR(14, 15, 0), 0);
    } else {
        FillWindowPixelBuffer(&summary->windows[5], 0);
    }
    PokemonSummary_PrintMessage(summary, 6, msg_0302_00004, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 7, msg_0302_00008, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 8, msg_0302_00010, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 9, msg_0302_00012, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 10, msg_0302_00013, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 11, msg_0302_00015, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 12, msg_0302_00017, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 13, msg_0302_00019, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 15, msg_0302_00110, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintStatLabel(summary, 16, msg_0302_00111, 0, 0);
    PokemonSummary_PrintStatLabel(summary, 17, msg_0302_00112, 1, 0);
    PokemonSummary_PrintStatLabel(summary, 18, msg_0302_00113, 3, 0);
    PokemonSummary_PrintStatLabel(summary, 19, msg_0302_00114, 4, 0);
    PokemonSummary_PrintStatLabel(summary, 20, msg_0302_00115, 2, 0);
    ReadMsgDataIntoString(summary->msgData, msg_0302_00116, summary->buffer);
    AddTextPrinterParameterizedWithColor(&summary->windows[21], 0, summary->buffer, 3, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 15, 0), NULL);
    ReadMsgDataIntoString(summary->msgData, msg_0302_00146, summary->buffer);
    AddTextPrinterParameterizedWithColor(&summary->windows[23], 4, summary->buffer, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 15, 0), NULL);
    PokemonSummary_PrintMessage(summary, 24, msg_0302_00149, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 25, msg_0302_00147, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 26, msg_0302_00148, MAKE_TEXT_COLOR(14, 15, 0), 0);
    PokemonSummary_PrintMessage(summary, 27, msg_0302_00162, MAKE_TEXT_COLOR(14, 15, 0), 2);
    PokemonSummary_PrintMessage(summary, 28, msg_0302_00160, MAKE_TEXT_COLOR(1, 2, 0), 2);
    PokemonSummary_PrintMessage(summary, 29, msg_0302_00182, MAKE_TEXT_COLOR(1, 2, 0), 0);
    PokemonSummary_PrintMessage(summary, 30, msg_0302_00179, MAKE_TEXT_COLOR(14, 15, 0), 0);
}

void PokemonSummary_PrintPage(PokemonSummary *summary) {
    FillBgTilemapRect(summary->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 0, 32, 2, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(summary->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, 2, 18, 22, TILEMAP_FILL_KEEP_PAL);
    FillBgTilemapRect(summary->bgConfig, GF_BG_LYR_SUB_0, 0, 0, 1, 18, 23, TILEMAP_FILL_KEEP_PAL);
    switch (summary->page) {
    case 0:
        PokemonSummary_PrintInfo(summary);
        PokemonSummary_PrintMemo(summary);
        break;
    case 1:
        PokemonSummary_PrintStats(summary);
        PokemonSummary_PrintMoves(summary);
        break;
    case 2:
        PokemonSummary_PrintPokeathlonStatLabels(summary);
        PokemonSummary_PrintRibbonCount(summary);
        break;
    case 3:
        PokemonSummary_PrintFlavorPreference(summary);
        break;
    }
}

void PokemonSummary_PrintInfo(PokemonSummary *summary) {
    ScheduleWindowCopyToVram(&summary->windows[23]);
    if (summary->mon.isEgg) {
        ClearWindowTilemapAndScheduleTransfer(&summary->windows[0]);
        ClearWindowTilemapAndScheduleTransfer(&summary->windows[7]);
        ClearWindowTilemapAndScheduleTransfer(&summary->windows[8]);
        ClearWindowTilemapAndScheduleTransfer(&summary->windows[9]);
        ClearWindowTilemapAndScheduleTransfer(&summary->windows[10]);
        ClearWindowTilemapAndScheduleTransfer(&summary->windows[11]);
        ClearWindowTilemapAndScheduleTransfer(&summary->windows[12]);
        ClearWindowTilemapAndScheduleTransfer(&summary->windows[13]);
        ClearWindowTilemapAndScheduleTransfer(&summary->windows[14]);
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[0]);
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[1]);
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[2]);
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[3]);
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[4]);
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[5]);
        return;
    }

    ScheduleWindowCopyToVram(&summary->windows[0]);
    ScheduleWindowCopyToVram(&summary->windows[7]);
    ScheduleWindowCopyToVram(&summary->windows[8]);
    ScheduleWindowCopyToVram(&summary->windows[9]);
    ScheduleWindowCopyToVram(&summary->windows[10]);
    ScheduleWindowCopyToVram(&summary->windows[11]);
    ScheduleWindowCopyToVram(&summary->windows[12]);
    ScheduleWindowCopyToVram(&summary->windows[13]);
    ScheduleWindowCopyToVram(&summary->windows[14]);
    FillWindowPixelBuffer(&summary->pageWindows[0], 0);
    FillWindowPixelBuffer(&summary->pageWindows[1], 0);
    FillWindowPixelBuffer(&summary->pageWindows[2], 0);
    FillWindowPixelBuffer(&summary->pageWindows[3], 0);
    FillWindowPixelBuffer(&summary->pageWindows[4], 0);
    FillWindowPixelBuffer(&summary->pageWindows[5], 0);

    {
        int dexNo = Pokedex_ConvertToCurrentDexNo(summary->args->natDexEnabled, summary->mon.species);
        if (dexNo != 0) {
            PokemonSummary_FormatNumber(summary, msg_0302_00009, dexNo, 3, PRINTING_MODE_LEADING_ZEROS);
        } else {
            ReadMsgDataIntoString(summary->msgData, msg_0302_00022, summary->buffer);
        }
    }
    if (summary->mon.shiny == FALSE) {
        PokemonSummary_PrintBuffer(summary, summary->pageWindows, MAKE_TEXT_COLOR(1, 2, 0), 2);
    } else {
        PokemonSummary_PrintBuffer(summary, summary->pageWindows, MAKE_TEXT_COLOR(5, 6, 0), 2);
    }
    {
        int x = FontID_String_GetCenterAlignmentX(0, summary->mon.speciesName, 0, 72);
        AddTextPrinterParameterizedWithColor(&summary->pageWindows[1], 0, summary->mon.speciesName, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }
    {
        if (summary->mon.otGender == 0) {
            int x = FontID_String_GetCenterAlignmentX(0, summary->mon.otName, 0, 72);
            AddTextPrinterParameterizedWithColor(&summary->pageWindows[2], 0, summary->mon.otName, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 0), NULL);
        } else {
            int x = FontID_String_GetCenterAlignmentX(0, summary->mon.otName, 0, 72);
            AddTextPrinterParameterizedWithColor(&summary->pageWindows[2], 0, summary->mon.otName, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(5, 6, 0), NULL);
        }
    }
    PokemonSummary_FormatNumber(summary, msg_0302_00016, (u16)summary->mon.otId, 5, PRINTING_MODE_LEADING_ZEROS);
    PokemonSummary_PrintBuffer(summary, &summary->pageWindows[3], MAKE_TEXT_COLOR(1, 2, 0), 2);
    PokemonSummary_FormatNumber(summary, msg_0302_00018, summary->mon.exp, 7, PRINTING_MODE_LEFT_ALIGN);
    PokemonSummary_PrintBuffer(summary, &summary->pageWindows[4], MAKE_TEXT_COLOR(1, 2, 0), 1);
    if (summary->mon.level < MAX_LEVEL) {
        PokemonSummary_FormatNumber(summary, msg_0302_00021, summary->mon.nextLevelExp - summary->mon.exp, 7, PRINTING_MODE_LEFT_ALIGN);
    } else {
        PokemonSummary_FormatNumber(summary, msg_0302_00021, 0, 7, PRINTING_MODE_LEFT_ALIGN);
    }
    PokemonSummary_PrintBuffer(summary, &summary->pageWindows[5], MAKE_TEXT_COLOR(1, 2, 0), 1);
    ScheduleWindowCopyToVram(&summary->pageWindows[0]);
    ScheduleWindowCopyToVram(&summary->pageWindows[1]);
    ScheduleWindowCopyToVram(&summary->pageWindows[2]);
    ScheduleWindowCopyToVram(&summary->pageWindows[3]);
    ScheduleWindowCopyToVram(&summary->pageWindows[4]);
    ScheduleWindowCopyToVram(&summary->pageWindows[5]);
}

// The trainer memo: nature, where and when it was met, characteristic, egg watch.
void PokemonSummary_PrintTrainerMemo(Window *window, void *mon, int isMine) {
    TrainerMemo *memo = sub_0208E600_3(mon, isMine, HEAP_ID_19);
    if (memo->notepad.nature != NULL) {
        AddTextPrinterParameterizedWithColor(window, 0, memo->notepad.nature, 6, (memo->notepad.natureLine - 1) * 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }
    if (memo->notepad.dateLocationMet != NULL) {
        AddTextPrinterParameterizedWithColor(window, 0, memo->notepad.dateLocationMet, 6, (memo->notepad.dateLocationMetLine - 1) * 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }
    if (memo->notepad.characteristic != NULL) {
        AddTextPrinterParameterizedWithColor(window, 0, memo->notepad.characteristic, 6, (memo->notepad.characteristicLine - 1) * 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }
    if (memo->notepad.eggWatch != NULL) {
        AddTextPrinterParameterizedWithColor(window, 0, memo->notepad.eggWatch, 6, (memo->notepad.eggWatchLine - 1) * 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }
    FillBgTilemapRect(window->bgConfig, GF_BG_LYR_MAIN_1, 0, 16, 5, 2, 18, 0);
    TrainerMemo_Delete(memo);
}

int PokemonSummary_CountShinyLeaves(PokemonSummary *summary) {
    return summary->mon.shinyLeaves[0] + summary->mon.shinyLeaves[1] + summary->mon.shinyLeaves[2] + summary->mon.shinyLeaves[3] + summary->mon.shinyLeaves[4] + summary->mon.shinyLeaves[5];
}

void PokemonSummary_PrintMemo(PokemonSummary *summary) {
    BoxPokemon *boxMon;
    int isMine;
    Pokemon *mon;
    String *string;

    ScheduleWindowCopyToVram(&summary->windows[1]);
    FillWindowPixelBuffer(&summary->pageWindows[6], 0);
    boxMon = PokemonSummary_GetShownMon(summary);
    isMine = PokemonSummary_IsPlayersMon(summary);
    if (summary->args->monDataType == 2) {
        mon = AllocMonZeroed(HEAP_ID_19);
        CopyBoxPokemonToPokemon(boxMon, mon);
        PokemonSummary_PrintTrainerMemo(&summary->pageWindows[6], mon, isMine);
        Heap_Free(mon);
    } else {
        PokemonSummary_PrintTrainerMemo(&summary->pageWindows[6], boxMon, isMine);
    }
    ScheduleWindowCopyToVram(&summary->pageWindows[6]);
    if (PokemonSummary_CountShinyLeaves(summary) != 0) {
        FillWindowPixelBuffer(&summary->pageWindows[7], 0);
        string = NewString_ReadMsgData(summary->msgData, msg_0302_00192);
        AddTextPrinterParameterizedWithColor(&summary->pageWindows[7], 0, string, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ScheduleWindowCopyToVram(&summary->pageWindows[7]);
        String_Delete(string);
    }
}

#pragma force_active on

void PokemonSummary_PrintStats(PokemonSummary *summary) {
    String *template;
    MsgData *abilityDescriptions;

    PokemonSummary_PrintStatLabel(summary, 16, msg_0302_00111, 0, 0);
    PokemonSummary_PrintStatLabel(summary, 17, msg_0302_00112, 1, 0);
    PokemonSummary_PrintStatLabel(summary, 18, msg_0302_00113, 3, 0);
    PokemonSummary_PrintStatLabel(summary, 19, msg_0302_00114, 4, 0);
    PokemonSummary_PrintStatLabel(summary, 20, msg_0302_00115, 2, 0);
    ScheduleWindowCopyToVram(&summary->windows[2]);
    ScheduleWindowCopyToVram(&summary->windows[15]);
    ScheduleWindowCopyToVram(&summary->windows[16]);
    ScheduleWindowCopyToVram(&summary->windows[17]);
    ScheduleWindowCopyToVram(&summary->windows[18]);
    ScheduleWindowCopyToVram(&summary->windows[19]);
    ScheduleWindowCopyToVram(&summary->windows[20]);
    ScheduleWindowCopyToVram(&summary->windows[21]);
    FillWindowPixelBuffer(&summary->pageWindows[0], 0);
    FillWindowPixelBuffer(&summary->pageWindows[1], 0);
    FillWindowPixelBuffer(&summary->pageWindows[2], 0);
    FillWindowPixelBuffer(&summary->pageWindows[3], 0);
    FillWindowPixelBuffer(&summary->pageWindows[4], 0);
    FillWindowPixelBuffer(&summary->pageWindows[5], 0);
    FillWindowPixelBuffer(&summary->pageWindows[6], 0);
    FillWindowPixelBuffer(&summary->pageWindows[7], 0);
    PokemonSummary_PrintValuePair(summary, 0, 117, 119, 118, summary->mon.hp, summary->mon.maxHp, 3, (u8)(GetWindowWidth(summary->pageWindows) * 4), 0);
    PokemonSummary_FormatNumber(summary, msg_0302_00120, summary->mon.attack, 3, PRINTING_MODE_LEFT_ALIGN);
    PokemonSummary_PrintBuffer(summary, &summary->pageWindows[1], MAKE_TEXT_COLOR(1, 2, 0), 1);
    PokemonSummary_FormatNumber(summary, msg_0302_00121, summary->mon.defense, 3, PRINTING_MODE_LEFT_ALIGN);
    PokemonSummary_PrintBuffer(summary, &summary->pageWindows[2], MAKE_TEXT_COLOR(1, 2, 0), 1);
    PokemonSummary_FormatNumber(summary, msg_0302_00122, summary->mon.spAttack, 3, PRINTING_MODE_LEFT_ALIGN);
    PokemonSummary_PrintBuffer(summary, &summary->pageWindows[3], MAKE_TEXT_COLOR(1, 2, 0), 1);
    PokemonSummary_FormatNumber(summary, msg_0302_00123, summary->mon.spDefense, 3, PRINTING_MODE_LEFT_ALIGN);
    PokemonSummary_PrintBuffer(summary, &summary->pageWindows[4], MAKE_TEXT_COLOR(1, 2, 0), 1);
    PokemonSummary_FormatNumber(summary, msg_0302_00124, summary->mon.speed, 3, PRINTING_MODE_LEFT_ALIGN);
    PokemonSummary_PrintBuffer(summary, &summary->pageWindows[5], MAKE_TEXT_COLOR(1, 2, 0), 1);
    BufferAbilityName(summary->msgFormat, 0, summary->mon.ability);
    template = NewString_ReadMsgData(summary->msgData, msg_0302_00125);
    StringExpandPlaceholders(summary->msgFormat, summary->buffer, template);
    String_Delete(template);
    PokemonSummary_PrintBuffer(summary, &summary->pageWindows[6], MAKE_TEXT_COLOR(1, 2, 0), 0);
    abilityDescriptions = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0722_bin, HEAP_ID_19);
    ReadMsgDataIntoString(abilityDescriptions, summary->mon.ability, summary->buffer);
    DestroyMsgData(abilityDescriptions);
    AddTextPrinterParameterizedWithColor(&summary->pageWindows[7], 0, summary->buffer, 3, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    ScheduleWindowCopyToVram(&summary->pageWindows[0]);
    ScheduleWindowCopyToVram(&summary->pageWindows[1]);
    ScheduleWindowCopyToVram(&summary->pageWindows[2]);
    ScheduleWindowCopyToVram(&summary->pageWindows[3]);
    ScheduleWindowCopyToVram(&summary->pageWindows[4]);
    ScheduleWindowCopyToVram(&summary->pageWindows[5]);
    ScheduleWindowCopyToVram(&summary->pageWindows[6]);
    ScheduleWindowCopyToVram(&summary->pageWindows[7]);
}

#pragma force_active off

void PokemonSummary_PrintMoves(PokemonSummary *summary) {
    if (summary->args->mode != 2) {
        ScheduleWindowCopyToVram(&summary->windows[4]);
    }
    FillWindowPixelBuffer(&summary->pageWindows[8], 0);
    FillWindowPixelBuffer(&summary->pageWindows[9], 0);
    FillWindowPixelBuffer(&summary->pageWindows[10], 0);
    FillWindowPixelBuffer(&summary->pageWindows[11], 0);
    PokemonSummary_PrintMoveSlot(summary, 0);
    PokemonSummary_PrintMoveSlot(summary, 1);
    PokemonSummary_PrintMoveSlot(summary, 2);
    PokemonSummary_PrintMoveSlot(summary, 3);
    ScheduleWindowCopyToVram(&summary->pageWindows[8]);
    ScheduleWindowCopyToVram(&summary->pageWindows[9]);
    ScheduleWindowCopyToVram(&summary->pageWindows[10]);
    ScheduleWindowCopyToVram(&summary->pageWindows[11]);
    ScheduleWindowCopyToVram(&summary->windows[23]);
}

#pragma force_active on

void PokemonSummary_PrintPokeathlonStatLabels(PokemonSummary *summary) {
    ScheduleWindowCopyToVram(&summary->windows[5]);
    if (summary->args->isFlag982Set) {
        FillWindowPixelBuffer(&summary->pageWindows[3], 0);
        FillWindowPixelBuffer(&summary->pageWindows[4], 0);
        FillWindowPixelBuffer(&summary->pageWindows[5], 0);
        FillWindowPixelBuffer(&summary->pageWindows[6], 0);
        FillWindowPixelBuffer(&summary->pageWindows[7], 0);
        ReadMsgDataIntoString(summary->msgData, msg_0302_00187, summary->buffer);
        AddTextPrinterParameterizedWithColor(&summary->pageWindows[3], 0, summary->buffer, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ReadMsgDataIntoString(summary->msgData, msg_0302_00188, summary->buffer);
        AddTextPrinterParameterizedWithColor(&summary->pageWindows[4], 0, summary->buffer, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ReadMsgDataIntoString(summary->msgData, msg_0302_00189, summary->buffer);
        AddTextPrinterParameterizedWithColor(&summary->pageWindows[5], 0, summary->buffer, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ReadMsgDataIntoString(summary->msgData, msg_0302_00190, summary->buffer);
        AddTextPrinterParameterizedWithColor(&summary->pageWindows[6], 0, summary->buffer, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ReadMsgDataIntoString(summary->msgData, msg_0302_00191, summary->buffer);
        AddTextPrinterParameterizedWithColor(&summary->pageWindows[7], 0, summary->buffer, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ScheduleWindowCopyToVram(&summary->pageWindows[3]);
        ScheduleWindowCopyToVram(&summary->pageWindows[4]);
        ScheduleWindowCopyToVram(&summary->pageWindows[5]);
        ScheduleWindowCopyToVram(&summary->pageWindows[6]);
        ScheduleWindowCopyToVram(&summary->pageWindows[7]);
    }
}

#pragma force_active off

void PokemonSummary_PrintRibbonCount(PokemonSummary *summary) {
    ScheduleWindowCopyToVram(&summary->windows[30]);
    ScheduleWindowCopyToVram(&summary->windows[29]);
    ScheduleWindowCopyToVram(&summary->windows[23]);
    FillWindowPixelBuffer(summary->pageWindows, 0);
    PokemonSummary_FormatNumber(summary, msg_0302_00183, summary->ribbonCount, 3, PRINTING_MODE_LEFT_ALIGN);
    PokemonSummary_PrintBuffer(summary, summary->pageWindows, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ScheduleWindowCopyToVram(summary->pageWindows);
    BgCommitTilemapBufferToVram(summary->bgConfig, GF_BG_LYR_SUB_0);
}

void PokemonSummary_PrintFlavorPreference(PokemonSummary *summary) {
    u8 mode = summary->args->mode;
    if (mode == 3) {
    } else if (mode != 4) {
        return;
    }
    {
        FillWindowPixelBuffer(&summary->pageWindows[1], 0);
        ReadMsgDataIntoString(summary->msgData, msg_0302_00165, summary->buffer);
        PokemonSummary_PrintBuffer(summary, &summary->pageWindows[1], MAKE_TEXT_COLOR(14, 15, 0), 0);
        ReadMsgDataIntoString(summary->msgData, summary->mon.favoriteFlavor + 166, summary->buffer);
        AddTextPrinterParameterizedWithColor(&summary->pageWindows[1], 0, summary->buffer, 0, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        ScheduleWindowCopyToVram(&summary->pageWindows[1]);
    }
}

void PokemonSummary_PrintRibbonInfo(PokemonSummary *summary) {
    FillWindowPixelBuffer(&summary->pageWindows[1], 0);
    FillWindowPixelBuffer(&summary->pageWindows[2], 0);
    if (summary->ribbonCursor + summary->ribbonRow * 9 < summary->ribbonCount) {
        int msgNo = GetRibbonAttr(summary->ribbonId, RIBBONDAT_NAMEGMM);
        ReadMsgDataIntoString(summary->ribbonMsgData, msgNo, summary->buffer);
        PokemonSummary_PrintBuffer(summary, &summary->pageWindows[1], MAKE_TEXT_COLOR(14, 15, 0), 0);
        msgNo = GetRibbonDescGmm((const u8 *)summary->args->ribbons, summary->ribbonId);
        ReadMsgDataIntoString(summary->ribbonMsgData, msgNo, summary->buffer);
        PokemonSummary_PrintBuffer(summary, &summary->pageWindows[2], MAKE_TEXT_COLOR(1, 2, 0), 0);
    }
    ScheduleWindowCopyToVram(&summary->pageWindows[1]);
    ScheduleWindowCopyToVram(&summary->pageWindows[2]);
}

#pragma force_active on

// slot 4: the move to learn.
void PokemonSummary_PrintMoveSlot(PokemonSummary *summary, int slot) {
    int offset;
    u8 *windows;
    u32 move;
    u8 pp;
    u8 maxPP;

    windows = (u8 *)summary->pageWindows;
    offset = (slot + 8) * sizeof(Window);
    if (slot != 4) {
        move = summary->mon.moves[slot];
        pp = summary->mon.pp[slot];
        maxPP = summary->mon.maxPP[slot];
    } else {
        move = summary->args->moveToLearn;
        maxPP = pp = GetMoveMaxPP(move, 0);
    }
    ReadMsgDataIntoString(summary->moveNames, move, summary->buffer);
    AddTextPrinterParameterizedWithColor((Window *)(windows + offset), 0, summary->buffer, 1, 2, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    if (move != MOVE_NONE) {
        ReadMsgDataIntoString(summary->msgData, msg_0302_00135, summary->buffer);
        AddTextPrinterParameterizedWithColor((Window *)(windows + offset), 0, summary->buffer, 16, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        PokemonSummary_PrintValuePair(summary, slot + 8, 117, slot + 136, slot + 141, pp, maxPP, 2, 60, 16);
    } else {
        u32 width;
        ReadMsgDataIntoString(summary->msgData, msg_0302_00153, summary->buffer);
        width = FontID_String_GetWidth(0, summary->buffer, 0);
        AddTextPrinterParameterizedWithColor((Window *)(windows + offset), 0, summary->buffer, 60 - (width >> 1), 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }
}

#pragma force_active off

// Power, accuracy and description of the move.
void PokemonSummary_PrintMoveInfo(PokemonSummary *summary, u16 move) {
    MsgData *moveDescriptions;
    u32 attr;

    ScheduleWindowCopyToVram(&summary->windows[24]);
    ScheduleWindowCopyToVram(&summary->windows[25]);
    ScheduleWindowCopyToVram(&summary->windows[26]);
    FillWindowPixelBuffer(&summary->pageWindows[13], 0);
    FillWindowPixelBuffer(&summary->pageWindows[14], 0);
    FillWindowPixelBuffer(&summary->pageWindows[15], 0);
    attr = GetMoveAttr(move, MOVEATTR_POWER);
    if (attr <= 1) {
        ReadMsgDataIntoString(summary->msgData, msg_0302_00154, summary->buffer);
    } else {
        PokemonSummary_FormatNumber(summary, msg_0302_00150, attr, 3, PRINTING_MODE_LEFT_ALIGN);
    }
    PokemonSummary_PrintBuffer(summary, &summary->pageWindows[13], MAKE_TEXT_COLOR(1, 2, 0), 1);
    attr = GetMoveAttr(move, MOVEATTR_ACCURACY);
    if (attr == 0) {
        ReadMsgDataIntoString(summary->msgData, msg_0302_00154, summary->buffer);
    } else {
        PokemonSummary_FormatNumber(summary, msg_0302_00151, attr, 3, PRINTING_MODE_LEFT_ALIGN);
    }
    PokemonSummary_PrintBuffer(summary, &summary->pageWindows[14], MAKE_TEXT_COLOR(1, 2, 0), 1);
    moveDescriptions = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0749_bin, HEAP_ID_19);
    ReadMsgDataIntoString(moveDescriptions, move, summary->buffer);
    PokemonSummary_PrintBuffer(summary, &summary->pageWindows[15], MAKE_TEXT_COLOR(1, 2, 0), 0);
    DestroyMsgData(moveDescriptions);
    ScheduleWindowCopyToVram(&summary->pageWindows[13]);
    ScheduleWindowCopyToVram(&summary->pageWindows[14]);
    ScheduleWindowCopyToVram(&summary->pageWindows[15]);
}

void PokemonSummary_ClearMoveInfo(PokemonSummary *summary) {
    ClearWindowTilemapAndScheduleTransfer(&summary->windows[24]);
    ClearWindowTilemapAndScheduleTransfer(&summary->windows[25]);
    ClearWindowTilemapAndScheduleTransfer(&summary->windows[26]);
    ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[13]);
    ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[14]);
    ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[15]);
}

void PokemonSummary_PrintMoveToLearn(PokemonSummary *summary) {
    if (summary->args->moveToLearn != MOVE_NONE) {
        FillWindowPixelBuffer(&summary->pageWindows[12], 0);
        PokemonSummary_PrintMoveSlot(summary, 4);
        ScheduleWindowCopyToVram(&summary->pageWindows[12]);
    } else {
        ScheduleWindowCopyToVram(&summary->windows[23]);
    }
}

void PokemonSummary_ScheduleMainBg1Transfer(PokemonSummary *summary) {
    ScheduleBgTilemapBufferTransfer(summary->bgConfig, GF_BG_LYR_MAIN_1);
}

void PokemonSummary_PrintSwappedMoves(PokemonSummary *summary) {
    FillWindowPixelBuffer(&summary->pageWindows[8 + summary->moveSlot], 0);
    FillWindowPixelBuffer(&summary->pageWindows[8 + summary->swapMoveSlot], 0);
    PokemonSummary_PrintMoveSlot(summary, summary->moveSlot);
    PokemonSummary_PrintMoveSlot(summary, summary->swapMoveSlot);
    ScheduleWindowCopyToVram(&summary->pageWindows[8 + summary->moveSlot]);
    ScheduleWindowCopyToVram(&summary->pageWindows[8 + summary->swapMoveSlot]);
}

void PokemonSummary_PrintCantForgetHMMessage(PokemonSummary *summary) {
    Window *window;
    if (summary->page == 1) {
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[13]);
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[14]);
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[15]);
        window = &summary->pageWindows[15];
    } else {
        window = &summary->pageWindows[5];
    }
    FillWindowPixelBuffer(window, 0);
    ReadMsgDataIntoString(summary->msgData, msg_0302_00156, summary->buffer);
    PokemonSummary_PrintBuffer(summary, window, MAKE_TEXT_COLOR(1, 2, 0), 0);
    ScheduleWindowCopyToVram(window);
}

void PokemonSummary_ShowForgetLabel(PokemonSummary *summary, int show) {
    if (show == TRUE) {
        FillWindowPixelBuffer(&summary->pageWindows[16], 0);
        ReadMsgDataIntoString(summary->msgData, msg_0302_00193, summary->buffer);
        AddTextPrinterParameterizedWithColor(&summary->pageWindows[16], 4, summary->buffer, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 15, 0), NULL);
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[12]);
        ScheduleWindowCopyToVram(&summary->pageWindows[16]);
    } else {
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[16]);
        if (summary->args->moveToLearn != MOVE_NONE) {
            FillWindowPixelBuffer(&summary->pageWindows[12], 0);
            PokemonSummary_PrintMoveSlot(summary, 4);
            ScheduleWindowCopyToVram(&summary->pageWindows[12]);
        }
    }
}

void PokemonSummary_ShowSwitchLabel(PokemonSummary *summary, int show) {
    if (show == TRUE) {
        FillWindowPixelBuffer(&summary->pageWindows[17], 0);
        ReadMsgDataIntoString(summary->msgData, msg_0302_00194, summary->buffer);
        AddTextPrinterParameterizedWithColor(&summary->pageWindows[17], 4, summary->buffer, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 15, 0), NULL);
        ScheduleWindowCopyToVram(&summary->pageWindows[17]);
    } else {
        ClearWindowTilemapAndScheduleTransfer(&summary->pageWindows[17]);
    }
}

void PokemonSummary_PrintConditionChangeMessage(PokemonSummary *summary, int result) {
    int msgNo;
    Window *window;
    switch (result) {
    case 0:
        msgNo = msg_0302_00172;
        break;
    case 1:
        msgNo = msg_0302_00174;
        break;
    case 2:
        msgNo = msg_0302_00175;
        break;
    case 3:
        msgNo = msg_0302_00176;
        break;
    case 4:
        msgNo = msg_0302_00173;
        break;
    case 0xFE:
        msgNo = msg_0302_00177;
        break;
    default:
        msgNo = msg_0302_00178;
        break;
    }
    window = &summary->pageWindows[2];
    DrawFrameAndWindow2(window, TRUE, 0x3E2, 13);
    FillWindowPixelBuffer(window, 15);
    ReadMsgDataIntoString(summary->msgData, msgNo, summary->buffer);
    AddTextPrinterParameterizedWithColor(window, 1, summary->buffer, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 15), NULL);
    ScheduleWindowCopyToVram(window);
}
