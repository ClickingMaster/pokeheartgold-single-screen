#include "field_save_stats_printer_internal.h"

void ov01_021F3F50(struct SaveStatsPrinterStats *stats, FieldSystem *fieldSystem) {
    SaveData *saveData = fieldSystem->saveData;
    Location *position = LocalFieldData_GetCurrentPosition(Save_LocalFieldData_Get(saveData));
    Pokedex *pokedex = Save_Pokedex_Get(saveData);

    stats->mapSec = MapHeader_GetMapSec(position->mapId);
    if (Pokedex_IsEnabled(pokedex)) {
        stats->dexOwned = Pokedex_CountDexOwned(pokedex);
    } else {
        stats->dexOwned = 0;
    }
    stats->profile = Save_PlayerData_GetProfile(saveData);
    stats->igt = Save_PlayerData_GetIGTAddr(saveData);
}

void ov01_021F3F9C(MessageFormat *msgFmt, struct SaveStatsPrinterStats *stats) {
    int value;
    int numDigits;
    PrintingMode mode;

    BufferLandmarkName(msgFmt, 0, stats->mapSec);
    BufferPlayersName(msgFmt, 1, stats->profile);
    BufferIntegerAsString(msgFmt, 2, PlayerProfile_CountBadges(stats->profile), 2, PRINTING_MODE_LEFT_ALIGN, TRUE);

    value = stats->dexOwned;
    if (value >= 100) {
        numDigits = 3;
        mode = PRINTING_MODE_LEFT_ALIGN;
    } else if (value >= 10) {
        numDigits = 3;
        mode = PRINTING_MODE_RIGHT_ALIGN;
    } else {
        numDigits = 2;
        mode = PRINTING_MODE_RIGHT_ALIGN;
    }
    BufferIntegerAsString(msgFmt, 3, value, numDigits, mode, TRUE);

    value = GetIGTHours(stats->igt);
    if (value >= 100) {
        numDigits = 3;
        mode = PRINTING_MODE_LEFT_ALIGN;
    } else if (value >= 10) {
        numDigits = 3;
        mode = PRINTING_MODE_RIGHT_ALIGN;
    } else {
        numDigits = 2;
        mode = PRINTING_MODE_RIGHT_ALIGN;
    }
    BufferIntegerAsString(msgFmt, 4, value, numDigits, mode, TRUE);

    BufferIntegerAsString(msgFmt, 5, GetIGTMinutes(stats->igt), 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
}

int ov01_021F4044(struct SaveStatsPrinterStats *stats) {
    return 10;
}
void ov01_021F4048(struct SaveStatsPrinter *printer) {
    String *str;
    int y;
    int lineHeight;
    u32 i;

    lineHeight = GetFontAttribute(0, 1) + GetFontAttribute(0, 3);
    y = 0;

    str = ReadMsgData_ExpandPlaceholders(printer->msgFmt, printer->msgData, 0, printer->heapId);
    AddTextPrinterParameterized(printer->window, 0, str, 0, 0, 0xFF, NULL);
    String_Delete(str);

    for (i = 1; i < 5; i++) {
        if (sMsgIdsSub[i] == 3 && printer->stats.dexOwned == 0) {
            continue;
        }
        y += lineHeight;
        str = NewString_ReadMsgData(printer->msgData, sMsgIdsSub[i]);
        AddTextPrinterParameterized(printer->window, 0, str, 0, y, 0xFF, NULL);
        String_Delete(str);
        str = ReadMsgData_ExpandPlaceholders(printer->msgFmt, printer->msgData, sMsgIdsMain[i - 1], printer->heapId);
        {
            int x = 0x68 - FontID_String_GetWidth(0, str, GetFontAttribute(0, 2));
            AddTextPrinterParameterized(printer->window, 0, str, x, y, 0xFF, NULL);
        }
        String_Delete(str);
    }
}
