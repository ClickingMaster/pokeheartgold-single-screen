#include "global.h"

#include "constants/maps.h"

#include "assert.h"
#include "bg_window.h"
#include "field_system.h"
#include "filesystem.h"
#include "font.h"
#include "heap.h"
#include "igt.h"
#include "map_header.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_01.h"
#include "field/bottom_screen_app.h"
#include "player_avatar.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokedex.h"
#include "render_text.h"
#include "render_window.h"
#include "save.h"
#include "save_local_field_data.h"
#include "string_util.h"
#include "text.h"

// The player's progress shown before saving: location, name, badges, Pokedex, play time. Two layouts: the save
// prompt window (Field_SaveStatsPrinter_*) and the full-width one of the touch screen save app (*TouchSave).

struct SaveStatsPrinterStats {
    int dexOwned;
    int mapSec;
    PlayerProfile *profile;
    IGT *igt;
};

struct SaveStatsPrinter {
    FieldSystem *fieldSystem;
    enum HeapID heapId;
    u8 bgId;
    BgConfig *bgConfig;
    Window *window;
    MessageFormat *msgFmt;
    MsgData *msgData;
    struct SaveStatsPrinterStats stats;
    int width;
    int height;
};

// A line of the touch save layout.
struct SaveStatsRow {
    u32 msgId;
    u32 align; // 0: left, 1: right, 2: centered
    u32 row;
    u32 color; // index into sTextColors
};

// in asm/overlay_01_021F3F50_data_local.s (overlapping tables)
extern const u32 sTextColors[2];
extern const u32 sMsgIdsMain[4];
extern const u32 sMsgIdsSub[5];
extern const struct SaveStatsRow sStatRows[9];

void SaveStatsPrinter_GetStats(struct SaveStatsPrinterStats *stats, FieldSystem *fieldSystem);
void SaveStatsPrinter_BufferStats(MessageFormat *msgFmt, struct SaveStatsPrinterStats *stats);
int SaveStatsPrinter_GetHeight(struct SaveStatsPrinterStats *stats);
void SaveStatsPrinter_PrintStats(struct SaveStatsPrinter *printer);
int SaveStatsPrinter_GetAlignedX(Window *window, String *str, int align, int fontId);
void SaveStatsPrinter_PrintStatRows(struct SaveStatsPrinter *printer);
void FieldSystem_StorePlayerLocation(FieldSystem *fieldSystem);

void SaveStatsPrinter_GetStats(struct SaveStatsPrinterStats *stats, FieldSystem *fieldSystem) {
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

void SaveStatsPrinter_BufferStats(MessageFormat *msgFmt, struct SaveStatsPrinterStats *stats) {
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

int SaveStatsPrinter_GetHeight(struct SaveStatsPrinterStats *stats) {
    return 10;
}

// The save prompt layout: the location, then label / value lines (the Pokedex line only once it is enabled).
void SaveStatsPrinter_PrintStats(struct SaveStatsPrinter *printer) {
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

void Field_SaveStatsPrinter_Print(struct SaveStatsPrinter *printer) {
    printer->window = Heap_Alloc(printer->heapId, sizeof(Window));
    AddWindowParameterized(printer->bgConfig, printer->window, printer->bgId, 1, 1, printer->width, printer->height, 0xD, 0x189);
    LoadUserFrameGfx1(printer->bgConfig, (GFBgLayer)printer->bgId, 0x3D9, 0xB, 0, printer->heapId);
    FillWindowPixelBuffer(printer->window, GetFontAttribute(0, 6));
    SaveStatsPrinter_PrintStats(printer);
    DrawFrameAndWindow1(printer->window, FALSE, 0x3D9, 0xB);
}

void Field_SaveStatsPrinter_RemoveFromScreen(struct SaveStatsPrinter *printer) {
    EraseFrameAndWindow1(printer->window, FALSE);
    RemoveWindow(printer->window);
    Heap_Free(printer->window);
}

struct SaveStatsPrinter *Field_SaveStatsPrinter_New(FieldSystem *fieldSystem, enum HeapID heapId, int bgId) {
    struct SaveStatsPrinter *printer = Heap_Alloc(heapId, sizeof(struct SaveStatsPrinter));

    printer->fieldSystem = fieldSystem;
    printer->heapId = heapId;
    printer->bgId = bgId;
    printer->bgConfig = fieldSystem->bgConfig;
    printer->msgFmt = MessageFormat_New(heapId);
    printer->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1A7, heapId);
    SaveStatsPrinter_GetStats(&printer->stats, printer->fieldSystem);
    SaveStatsPrinter_BufferStats(printer->msgFmt, &printer->stats);
    printer->width = 0xD;
    printer->height = SaveStatsPrinter_GetHeight(&printer->stats);
    return printer;
}

void Field_SaveStatsPrinter_Delete(struct SaveStatsPrinter *printer) {
    DestroyMsgData(printer->msgData);
    MessageFormat_Delete(printer->msgFmt);
    Heap_Free(printer);
}

int SaveStatsPrinter_GetAlignedX(Window *window, String *str, int align, int fontId) {
    int x = 0;

    switch (align) {
    case 1:
        x = window->width * 8 - FontID_String_GetWidth(fontId, str, 0);
        break;
    case 2:
        x = (int)(window->width * 8 - FontID_String_GetWidth(fontId, str, 0)) / 2;
        break;
    }
    return x;
}

// The touch save layout (sStatRows; the Pokedex lines only once it is enabled).
void SaveStatsPrinter_PrintStatRows(struct SaveStatsPrinter *printer) {
    String *str;
    u32 i;
    int lineHeight;
    const struct SaveStatsRow *row;

    lineHeight = GetFontAttribute(0, 1) + GetFontAttribute(0, 3);
    row = sStatRows;
    for (i = 0; i < 9; i++, row++) {
        if (printer->stats.dexOwned == 0 && (row->msgId == 0xB || row->msgId == 7)) {
            continue;
        }
        str = ReadMsgData_ExpandPlaceholders(printer->msgFmt, printer->msgData, row->msgId, printer->heapId);
        AddTextPrinterParameterizedWithColor(printer->window, 0, str, SaveStatsPrinter_GetAlignedX(printer->window, str, row->align, 0), lineHeight * row->row, 0, sTextColors[row->color], NULL);
        String_Delete(str);
    }
}

void Field_SaveStatsPrinter_PrintTouchSave(struct SaveStatsPrinter *printer) {
    printer->window = Heap_Alloc(printer->heapId, sizeof(Window));
    AddWindowParameterized(printer->bgConfig, printer->window, printer->bgId, 7, 2, printer->width, printer->height, 4, 0x10B);
    FillWindowPixelBuffer(printer->window, 0);
    SaveStatsPrinter_PrintStatRows(printer);
    CopyWindowToVram(printer->window);
}

void Field_SaveStatsPrinter_RemoveTouchSave(struct SaveStatsPrinter *printer) {
    RemoveWindow(printer->window);
    Heap_Free(printer->window);
}

struct SaveStatsPrinter *Field_SaveStatsPrinter_NewTouchSave(FieldSystem *fieldSystem, enum HeapID heapID, u8 bgId) {
    struct SaveStatsPrinter *printer = Heap_Alloc(heapID, sizeof(struct SaveStatsPrinter));

    printer->fieldSystem = fieldSystem;
    printer->heapId = heapID;
    printer->bgId = bgId;
    printer->bgConfig = fieldSystem->bgConfig;
    printer->msgFmt = MessageFormat_New(heapID);
    printer->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1A7, heapID);
    TextFlags_SetCanABSpeedUpPrint(TRUE);
    TextFlags_SetAutoScrollParam(0);
    TextFlags_SetCanTouchSpeedUpPrint(TRUE);
    SaveStatsPrinter_GetStats(&printer->stats, printer->fieldSystem);
    SaveStatsPrinter_BufferStats(printer->msgFmt, &printer->stats);
    printer->width = 0x13;
    printer->height = SaveStatsPrinter_GetHeight(&printer->stats);
    return printer;
}

void Field_SaveStatsPrinter_DeleteTouchSave(struct SaveStatsPrinter *printer) {
    DestroyMsgData(printer->msgData);
    MessageFormat_Delete(printer->msgFmt);
    Heap_Free(printer);
}

int Field_SaveGameNormal(FieldSystem *fieldSystem) {
    FieldSystem_StorePlayerLocation(fieldSystem);
    if (SaveGameNormal(fieldSystem->saveData) == 2) {
        return TRUE;
    }
    return FALSE;
}

// Map objects and the player's position into the save data.
void FieldSystem_StorePlayerLocation(FieldSystem *fieldSystem) {
    FieldSystem_SyncMapObjectsToSave(fieldSystem);
    ov01_021F6830(fieldSystem, 4, 0);
    fieldSystem->location->x = PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
    fieldSystem->location->y = PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
    fieldSystem->location->warpId = -1;
    fieldSystem->location->direction = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
}

// Not in the Union Room or the Wi-Fi battle rooms.
void FieldSystem_TryStorePlayerLocation(FieldSystem *fieldSystem) {
    if (fieldSystem == NULL) {
        GF_ASSERT(FALSE);
        return;
    }
    if (fieldSystem->location->mapId != MAP_UNION
        && fieldSystem->location->mapId != MAP_WIFI_SINGLE_BATTLE_AREA
        && fieldSystem->location->mapId != MAP_WIFI_MULTI_BATTLE_AREA) {
        FieldSystem_StorePlayerLocation(fieldSystem);
    }
}
