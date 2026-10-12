// Pokewalker connection app: the event log app (mode 8, sEventLogAppTemplate), which shows the 24 entries of the
// event log read from the Pokewalker one page at a time: the date and time, the Pokemon of the entry walking over the
// course background, and a message. Left / Right or the arrow buttons turn the page, B or the Quit button ends it.
// Before it: the picture loaders of the PC box selection's sub screen (pokewalker_app_modes.c) and the block table
// of the data sent to the Pokewalker (pokewalker_upload.c). After it: the event log records added by the link.

#include "application/pokewalker/pokewalker_event_log.h"

#include "global.h"

#include "constants/sndseq.h"

#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0275.h"

#include "assert.h"
#include "filesystem.h"
#include "follow_mon.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "player_data.h"
#include "pokemon.h"
#include "save.h"
#include "screen_fade.h"
#include "sound.h"
#include "sound_playback.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "char_tiles.h"

// Entry of NARC_fielddata_tsurepoke_tp_param (by SpeciesToOverworldModelIndexOffset).
typedef struct FollowMonModelParam {
    u8 unk_00;
    u8 isLarge; // 64x64 overworld sprite
    u8 unk_02;
    u8 unk_03;
} FollowMonModelParam;

typedef struct EventLogBgTemplates {
    BgTemplate templates[5];
} EventLogBgTemplates;

extern const u8 *const sPokewalkerUploadBlocks[16];
extern const u8 sPokewalkerUploadBlockSizes[16];
extern const u8 sEventLogTouchBoxes[]; // +3 previous page, +7 next page, +11 quit
extern const u16 sEventLogInvalidName[];
extern const GraphicsModes sEventLogGraphicsModes;
extern const OamCharTransferParam sEventLogOamTransferParam;
extern const SpriteResourceCountsListUnion sEventLogSpriteResourceCounts;
extern const OamManagerParam sEventLogOamManagerParam;
extern const WindowTemplate sEventLogWindowTemplates[4];
extern const GraphicsBanks sEventLogGraphicsBanks;
extern const ManagedSpriteTemplate sEventLogSpriteTemplate;
extern const ManagedSpriteTemplate sEventLogEffectSpriteTemplate;
extern const ManagedSpriteTemplate sEventLogMonSpriteTemplate;
extern const EventLogBgTemplates sEventLogBgTemplates;
extern const u8 sEventLogBgLayers[5];
extern const s8 sEventLogFleeHopHeights[]; // ends with 111
extern const u32 sEventLogCourseBgm[8];

void MATH_QSort(void *base, u32 count, u32 width, int (*compare)(const void *, const void *, void *), void *context);

void PokewalkerBoxDisplay_GetMonPictureLocation(PokewalkerBoxDisplaySprites *sprites) {
    SpriteResource *plttResource = sprites->resObjs[1][GF_GFX_RES_TYPE_PLTT];
    NNSG2dImageProxy *image = SpriteTransfer_GetCharProxy(sprites->resObjs[1][GF_GFX_RES_TYPE_CHAR]);
    NNSG2dImagePaletteProxy *palette = SpriteTransfer_GetPaletteProxy(plttResource, image);
    sprites->monCharLocation = NNS_G2dGetImageLocation(image, NNS_G2D_VRAM_TYPE_2DSUB);
    sprites->monPlttLocation = NNS_G2dGetImagePaletteLocation(palette, NNS_G2D_VRAM_TYPE_2DSUB);
}

void PokewalkerBoxDisplay_LoadMonPicture(PokewalkerBoxDisplaySprites *sprites, PokewalkerSelectMonInfo *info) {
    PokepicTemplate pokepic;
    NNSG2dPaletteData *palette;
    void *plttFile;
    GetMonSpriteCharAndPlttNarcIdsEx(&pokepic, info->species, info->gender, 2, info->isShiny, info->form, info->personality);
    CharTiles_LoadMonPicAsObjFromNarc((NarcId)pokepic.narcID, pokepic.charDataID, sprites->heapId, sprites->monCharData, info->personality, FALSE, 2, info->species);
    plttFile = AllocAndReadWholeNarcMemberByIdPair((NarcId)pokepic.narcID, pokepic.palDataID, sprites->heapId);
    if (NNS_G2dGetUnpackedPaletteData(plttFile, &palette) != TRUE) {
        GF_AssertFail();
    }
    MIi_CpuCopy16(palette->pRawData, sprites->monPalette, sizeof(sprites->monPalette));
    Heap_Free(plttFile);
}

void PokewalkerBoxDisplay_TransferMonPicture(PokewalkerBoxDisplaySprites *sprites) {
    DC_FlushRange(sprites->monCharData, 0xC80);
    GXS_LoadOBJ(sprites->monCharData, sprites->monCharLocation, 0xC80);
    DC_FlushRange(sprites->monPalette, sizeof(sprites->monPalette));
    GXS_LoadOBJPltt(sprites->monPalette, sprites->monPlttLocation, sizeof(sprites->monPalette));
}

#pragma force_active on
// The table is followed by the blocks themselves.
void PokewalkerApp_WriteUploadBlocks(PokewalkerUploadBlock *blocks) {
    u16 offset = 0;
    int i;
    u8 *data = (u8 *)blocks;
    data += 0x40;
    for (i = 0; i < 16; i++) {
        u8 sum;
        u32 j;
        const u8 *src;
        blocks[i].offset = offset;
        blocks[i].size = sPokewalkerUploadBlockSizes[i];
        sum = 0;
        src = sPokewalkerUploadBlocks[i];
        for (j = 0; j < blocks[i].size / 2; j++) {
            sum += src[0];
            sum += src[1];
            src += 2;
        }
        blocks[i].checksum = sum;
        MI_CpuCopy8(sPokewalkerUploadBlocks[i], data + offset, blocks[i].size);
        offset += blocks[i].size;
    }
}
#pragma force_active reset

void PokewalkerEventLog_ResetDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= ~REG_GX_DISPCNT_DISPLAY_MASK;
    reg_GXS_DB_DISPCNT &= ~REG_GXS_DB_DISPCNT_DISPLAY_MASK;
}

void PokewalkerEventLog_SetGxBanks(void) {
    GraphicsBanks banks = sEventLogGraphicsBanks;
    GfGfx_SetBanks(&banks);
}

// File of the species' overworld model in NARC_data_mmodel_mmodel.
int PokewalkerEventLog_GetMonModelFileId(int species, int form, int gender) {
    int fileId;
    if (species <= SPECIES_NONE || species > NATIONAL_DEX_COUNT) {
        fileId = 1;
    } else {
        fileId = SpeciesToOverworldModelIndexOffset(species) + 297;
        if (OverworldModelLookupHasFemaleForm(species)) {
            if (gender == MON_FEMALE) {
                fileId++;
            }
        } else {
            if (form > OverworldModelLookupFormCount(species)) {
                form = 0;
            }
            fileId += form;
        }
    }
    return fileId;
}

BOOL PokewalkerEventLog_CheckExit(PokewalkerEventLog *eventLog) {
    TouchscreenHitbox hitbox = *(const TouchscreenHitbox *)(sEventLogTouchBoxes + 11);
    if (TouchscreenHitbox_TouchNewIsIn(&hitbox) || (gSystem.newKeys & PAD_BUTTON_B)) {
        PlaySE(SEQ_SE_DP_SELECT);
        ManagedSprite_ResetSpriteAnimCtrlState(eventLog->sprites[EVENT_LOG_SPRITE_EXIT]);
        return TRUE;
    }
    return FALSE;
}

BOOL PokewalkerEventLog_CheckPrevPage(PokewalkerEventLog *eventLog) {
    TouchscreenHitbox hitbox;
    if (eventLog->page != 0) {
        hitbox = *(const TouchscreenHitbox *)(sEventLogTouchBoxes + 3);
        if (TouchscreenHitbox_TouchNewIsIn(&hitbox) || (gSystem.newKeys & PAD_KEY_LEFT)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL PokewalkerEventLog_CheckNextPage(PokewalkerEventLog *eventLog) {
    TouchscreenHitbox hitbox;
    if (eventLog->page < eventLog->count - 1) {
        hitbox = *(const TouchscreenHitbox *)(sEventLogTouchBoxes + 7);
        if (TouchscreenHitbox_TouchNewIsIn(&hitbox) || (gSystem.newKeys & PAD_KEY_RIGHT)) {
            return TRUE;
        }
    }
    return FALSE;
}

u8 PokewalkerEventLog_IsMonShiny(PokewalkerIrRecord *record, BOOL isWalkingMon) {
    if (isWalkingMon) {
        return record->isShiny;
    }
    return record->peerIsShiny;
}

u8 PokewalkerEventLog_GetCourse(PokewalkerIrRecord *record) {
    u32 course = record->unk_76;
    if (course >= 8) {
        course = 0;
    }
    return course;
}

int PokewalkerEventLog_CompareEntryTimes(PokewalkerEventLogSlot *a, PokewalkerEventLogSlot *b) {
    if (a->record->value == b->record->value) {
        return 0;
    }
    if (a->record->value > b->record->value) {
        return 1;
    }
    return -1;
}

#pragma force_active on
// Sorts the valid entries by time into entries[] and counts what the stroll end message rates. The stroll end record
// (the last one) gets the time after the previous entry.
void PokewalkerEventLog_BuildEntryList(PokewalkerEventLog *eventLog) {
    PokewalkerEventLogSlot slots[24] = { 0 };
    u32 earliest = -1;
    int i;
    PlayerProfile *profile;
    GF_ASSERT(eventLog != NULL);
    GF_ASSERT(eventLog->args != NULL);
    Save_Pokewalker_Get(eventLog->args->saveData);
    profile = Save_PlayerData_GetProfile(eventLog->args->saveData);
    eventLog->playerGender = PlayerProfile_GetTrainerGender(profile);
    eventLog->playerName = PlayerProfile_GetNamePtr(profile);
    for (i = 0; i < 24; i++) {
        PokewalkerIrRecord *record = &eventLog->args->records[i];
        int type;
        if ((u16)(record->unk_08 + 0xFFF9) <= 1 && (record->unk_0A > NATIONAL_DEX_COUNT || record->unk_0C > NATIONAL_DEX_COUNT)) { // HeartGold / SoulSilver
            continue;
        }
        switch (record->eventType) {
        case 12:
            record->eventType = POKEWALKER_EVENT_DOWSING_ITEM;
            break;
        case 14:
            record->eventType = POKEWALKER_EVENT_RADAR_CAUGHT;
            break;
        case POKEWALKER_EVENT_RAW_GIFT_ITEM:
            record->eventType = POKEWALKER_EVENT_GIFT_ITEM;
            break;
        case POKEWALKER_EVENT_RAW_GIFT_MON:
            record->eventType = POKEWALKER_EVENT_GIFT_MON;
            break;
        }
        type = record->eventType;
        if (type >= POKEWALKER_EVENT_RAW_GIFT_ITEM) {
            type = POKEWALKER_EVENT_NONE;
        } else if (type >= POKEWALKER_EVENT_PEER_FIRST && type <= POKEWALKER_EVENT_PEER_LAST) {
            eventLog->peerCount++;
        } else if (type == POKEWALKER_EVENT_GIFT_ITEM) {
            eventLog->hasGiftItem = TRUE;
        } else if (type == POKEWALKER_EVENT_GIFT_MON) {
            eventLog->hasGiftMon = TRUE;
        } else if (type == POKEWALKER_EVENT_DOWSING_ITEM) {
            if (eventLog->dowsingStreakPage == 0) {
                eventLog->dowsingCount++;
                if (eventLog->dowsingCount >= 4) {
                    eventLog->dowsingStreakPage = i;
                }
            }
        } else if (type == POKEWALKER_EVENT_RADAR_CAUGHT) {
            if (eventLog->radarStreakPage == 0) {
                eventLog->radarCount++;
                if (eventLog->radarCount >= 4) {
                    eventLog->radarStreakPage = i;
                }
            }
        }
        if (type != POKEWALKER_EVENT_NONE) {
            GF_ASSERT(record->eventType != POKEWALKER_EVENT_NONE);
            if (earliest > record->value) {
                earliest = record->value;
            }
            slots[eventLog->count].record = record;
            slots[eventLog->count].index = i;
            eventLog->count++;
        }
    }
    for (i = 0; i < eventLog->count; i++) {
        if (slots[i].record->eventType == POKEWALKER_EVENT_STROLL_START) {
            if (slots[i].record->value > earliest) {
                GF_ASSERT(earliest != 0);
                slots[i].record->value = earliest - 1;
            }
            break;
        }
    }
    MATH_QSort(slots, eventLog->count, sizeof(PokewalkerEventLogSlot), (int (*)(const void *, const void *, void *))PokewalkerEventLog_CompareEntryTimes, NULL);
    if (eventLog->count < 2) {
        slots[eventLog->count - 1].record->unk_76 = 0;
        slots[eventLog->count - 1].record->value = GF_RTC_DateTimeToSec();
    } else {
        PokewalkerIrRecord *previous = slots[eventLog->count - 2].record;
        slots[eventLog->count - 1].record->unk_76 = previous->unk_76;
        if (previous->eventType == POKEWALKER_EVENT_STEPS) {
            slots[eventLog->count - 1].record->value = previous->value + 3600;
        } else {
            slots[eventLog->count - 1].record->value = previous->value + 5;
        }
    }
    {
        // drop an hour with fewer than 300 steps after another one
        int previousType = POKEWALKER_EVENT_NONE;
        int previousSteps = 0;
        for (i = 0; i < eventLog->count; i++) {
            PokewalkerIrRecord *record = slots[i].record;
            int type = record->eventType;
            int steps = record->unk_78;
            if (type == POKEWALKER_EVENT_STEPS && previousType == POKEWALKER_EVENT_STEPS && steps < 300 && previousSteps < 300) {
                record->eventType = POKEWALKER_EVENT_NONE;
            }
            previousType = type;
            previousSteps = steps;
        }
    }
    {
        int count = 0;
        for (i = 0; i < eventLog->count; i++) {
            PokewalkerIrRecord *record = slots[i].record;
            if (record->eventType != POKEWALKER_EVENT_NONE) {
                eventLog->entries[count] = record;
                count++;
            }
        }
        eventLog->count = count;
    }
}
#pragma force_active reset

void PokewalkerEventLog_InitText(PokewalkerEventLog *eventLog) {
    const WindowTemplate *windowTemplate;
    Window *window;
    int i;
    GF_ASSERT(eventLog != NULL);
    GF_ASSERT(eventLog->bgConfig != NULL);
    eventLog->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0275_bin, eventLog->heapId);
    eventLog->speciesNames = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0237_bin, eventLog->heapId);
    eventLog->itemNames = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0222_bin, eventLog->heapId);
    eventLog->msgFormat = MessageFormat_New_Custom(13, 32, eventLog->heapId);
    windowTemplate = sEventLogWindowTemplates;
    i = 0;
    window = eventLog->windows;
    for (; i < 4; i++) {
        AddWindow(eventLog->bgConfig, window, windowTemplate);
        windowTemplate++;
        window++;
    }
    BG_FillCharDataRange(eventLog->bgConfig, GF_BG_LYR_MAIN_2, 0, 1, 0);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_14_OFFSET, eventLog->heapId);
    LoadFontPal1(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_14_OFFSET, eventLog->heapId);
    FontID_Alloc(4, eventLog->heapId);
}

void PokewalkerEventLog_FreeText(PokewalkerEventLog *eventLog) {
    int i;
    Window *window;
    i = 0;
    window = eventLog->windows;
    do {
        RemoveWindow(window);
        i++;
        window++;
    } while (i < 4);
    MessageFormat_Delete(eventLog->msgFormat);
    DestroyMsgData(eventLog->msgData);
    DestroyMsgData(eventLog->speciesNames);
    DestroyMsgData(eventLog->itemNames);
    FontID_Release(4);
}

#pragma force_active on
void PokewalkerEventLog_PrintToWindowWithFont(PokewalkerEventLog *eventLog, u32 windowId, u32 msgId, u16 x, int fontId) {
    String *string;
    GF_ASSERT(windowId < 4);
    FillWindowPixelBuffer(&eventLog->windows[windowId], 0);
    string = ReadMsgData_ExpandPlaceholders(eventLog->msgFormat, eventLog->msgData, msgId, eventLog->heapId);
    // called with an int font (FontID is u8 in font_types_def.h but a full word here)
    ((u8(*)(Window *, int, String *, u32, u32, u32, u32, PrinterCallback_t))AddTextPrinterParameterizedWithColor)(&eventLog->windows[windowId], fontId, string, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(string);
    CopyWindowToVram(&eventLog->windows[windowId]);
}
#pragma force_active reset

void PokewalkerEventLog_PrintToWindow(PokewalkerEventLog *eventLog, u32 windowId, u32 msgId, u16 x) {
    PokewalkerEventLog_PrintToWindowWithFont(eventLog, windowId, msgId, x, 1);
}

// Prints msgId, and msgId2 on the next line when it is not 0.
void PokewalkerEventLog_PrintMessage(PokewalkerEventLog *eventLog, int msgId, int msgId2) {
    String *string;
    String *string2;
    String *joined;
    Window *window = &eventLog->windows[2];

    FillWindowPixelBuffer(window, 0);
    string = ReadMsgData_ExpandPlaceholders(eventLog->msgFormat, eventLog->msgData, msgId, eventLog->heapId);
    if (msgId2 != 0) {
        string2 = ReadMsgData_ExpandPlaceholders(eventLog->msgFormat, eventLog->msgData, msgId2, eventLog->heapId);
        joined = String_New(String_GetLength(string) + String_GetLength(string2) + 2, eventLog->heapId);
        String_Copy(joined, string);
        String_AddChar(joined, CHAR_LF);
        String_Cat(joined, string2);
        AddTextPrinterParameterizedWithColor(window, 1, joined, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(string2);
        String_Delete(joined);
    } else {
        AddTextPrinterParameterizedWithColor(window, 1, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    }
    String_Delete(string);
    CopyWindowToVram(window);
}

#pragma force_active on
// A name without a terminator in its first `length` characters is shown as sEventLogInvalidName.
void PokewalkerEventLog_BufferName(PokewalkerEventLog *eventLog, const u16 *name, u8 field, int length) {
    BOOL invalid = TRUE;
    String *string = String_New(length, eventLog->heapId);
    int i;
    if (name[0] != 0) {
        for (i = 0; i < length; i++) {
            if (name[i] == EOS) {
                invalid = FALSE;
                break;
            }
        }
    }
    if (invalid) {
        CopyU16ArrayToString(string, sEventLogInvalidName);
    } else {
        CopyU16ArrayToString(string, name);
    }
    BufferString(eventLog->msgFormat, field, string, 2, 1, 2);
    String_Delete(string);
}
#pragma force_active reset

void PokewalkerEventLog_BufferItemName(PokewalkerEventLog *eventLog, u32 item) {
    String *string;
    u16 length;
    u16 name[20];
    if (item > ITEM_MAX) {
        item = ITEM_MAX;
    }
    string = NewString_ReadMsgData(eventLog->itemNames, item);
    length = String_GetLength(string) + 1;
    CopyStringToU16Array(string, name, 19);
    PokewalkerEventLog_BufferName(eventLog, name, 4, length);
    String_Delete(string);
}

void PokewalkerEventLog_BufferSpeciesName(PokewalkerEventLog *eventLog, u8 field, int species) {
    String *string = NewString_ReadMsgData(eventLog->speciesNames, species);
    u16 length = String_GetLength(string) + 1;
    u16 name[12];
    CopyStringToU16Array(string, name, length);
    PokewalkerEventLog_BufferName(eventLog, name, field, length);
    String_Delete(string);
}

// The species name when the nickname is not valid.
void PokewalkerEventLog_BufferNickname(PokewalkerEventLog *eventLog, const u16 *nickname, u8 field, int species) {
    int i;
    BOOL invalid = TRUE;
    if (nickname[0] != 0) {
        for (i = 0; i < POKEMON_NAME_LENGTH + 1; i++) {
            if (nickname[i] == EOS) {
                invalid = FALSE;
                break;
            }
        }
    }
    if (invalid) {
        PokewalkerEventLog_BufferSpeciesName(eventLog, field, species);
    } else {
        PokewalkerEventLog_BufferName(eventLog, nickname, field, POKEMON_NAME_LENGTH + 1);
    }
}

void PokewalkerEventLog_BufferNumber(PokewalkerEventLog *eventLog, int field, int value, int digits, PrintingMode mode) {
    BufferIntegerAsString(eventLog->msgFormat, field, value, digits, mode, TRUE);
}

void PokewalkerEventLog_ConvertEntryTime(PokewalkerEventLog *eventLog) {
    PokewalkerIrRecord *record = eventLog->entries[eventLog->page];
    RTCDate date = { 0 };
    RTCTime time = { 0 };
    RTC_ConvertSecondToDateTime(&eventLog->date, &eventLog->time, record->value);
}

// Fields: 0 player, 1 walking Pokemon, 2 other trainer, 3 and 5 other Pokemon, 4 item, 6 hour, 7 steps, 8 route,
// 9 month, 10 day. Also prints the date and the time of the entry.
void PokewalkerEventLog_PrintEntry(PokewalkerEventLog *eventLog, int msgId, int msgId2) {
    PokewalkerIrRecord *record = eventLog->entries[eventLog->page];
    int month = eventLog->date.month;
    int day = eventLog->date.day;
    int hour = eventLog->time.hour;
    int amPmMsg;
    String *string;
    int x;

    if (hour < 12) {
        amPmMsg = msg_0275_00087;
        if (hour == 0) {
            hour = 12;
        }
    } else {
        amPmMsg = msg_0275_00088;
        hour -= 12;
        if (hour == 0) {
            hour = 12;
        }
    }
    PokewalkerEventLog_BufferName(eventLog, eventLog->playerName, 0, PLAYER_NAME_LENGTH + 2);
    PokewalkerEventLog_BufferNickname(eventLog, record->nickname, 1, record->unk_0A);
    PokewalkerEventLog_BufferName(eventLog, record->peerTrainerName, 2, PLAYER_NAME_LENGTH + 2);
    PokewalkerEventLog_BufferNickname(eventLog, record->peerNickname, 3, record->unk_0C);
    PokewalkerEventLog_BufferItemName(eventLog, record->item);
    PokewalkerEventLog_BufferNickname(eventLog, record->peerNickname, 5, record->unk_0C);
    PokewalkerEventLog_BufferNumber(eventLog, 6, hour, 2, PRINTING_MODE_LEFT_ALIGN);
    PokewalkerEventLog_BufferNumber(eventLog, 7, record->unk_78, 5, PRINTING_MODE_LEFT_ALIGN);
    PokewalkerEventLog_BufferName(eventLog, record->routeName, 8, 21);
    PokewalkerEventLog_BufferNumber(eventLog, 9, month, 2, PRINTING_MODE_LEFT_ALIGN);
    PokewalkerEventLog_BufferNumber(eventLog, 10, day, 2, PRINTING_MODE_LEFT_ALIGN);
    PokewalkerEventLog_PrintMessage(eventLog, msgId, msgId2);
    PokewalkerEventLog_BufferNumber(eventLog, 9, month, 2, PRINTING_MODE_RIGHT_ALIGN);
    PokewalkerEventLog_BufferNumber(eventLog, 10, day, 2, PRINTING_MODE_RIGHT_ALIGN);
    PokewalkerEventLog_PrintToWindow(eventLog, 0, msg_0275_00086, 0);
    PokewalkerEventLog_BufferNumber(eventLog, 6, hour, 2, PRINTING_MODE_RIGHT_ALIGN);
    string = ReadMsgData_ExpandPlaceholders(eventLog->msgFormat, eventLog->msgData, amPmMsg, eventLog->heapId);
    x = FontID_String_GetCenterAlignmentX(1, string, 0, GetWindowWidth(&eventLog->windows[1]) * 8);
    String_Delete(string);
    PokewalkerEventLog_PrintToWindow(eventLog, 1, amPmMsg, (u8)x);
}

void PokewalkerEventLog_PrintQuit(PokewalkerEventLog *eventLog) {
    PokewalkerEventLog_PrintToWindowWithFont(eventLog, 3, msg_0275_00089, 4, 4);
}

#pragma force_active on
void PokewalkerEventLog_InitBgs(PokewalkerEventLog *eventLog) {
    GraphicsModes modes;
    EventLogBgTemplates templates;
    int i;
    const u8 *layer;
    BgTemplate *bgTemplate;
    eventLog->bgConfig = BgConfig_Alloc(eventLog->heapId);
    modes = sEventLogGraphicsModes;
    SetBothScreensModesAndDisable(&modes);
    templates = sEventLogBgTemplates;
    layer = sEventLogBgLayers;
    i = 0;
    bgTemplate = templates.templates;
    for (; i < 5; bgTemplate++, layer++, i++) {
        InitBgFromTemplate(eventLog->bgConfig, *layer, bgTemplate, GF_BG_TYPE_TEXT);
        BgClearTilemapBufferAndCommit(eventLog->bgConfig, *layer);
        BG_FillCharDataRange(eventLog->bgConfig, (GFBgLayer)*layer, 0, 1, 0);
    }
}
#pragma force_active reset

void PokewalkerEventLog_FreeBgs(PokewalkerEventLog *eventLog) {
    int i;
    for (i = 0; i < 5; i++) {
        FreeBgTilemapBuffer(eventLog->bgConfig, (GFBgLayer)sEventLogBgLayers[i]);
    }
    Heap_Free(eventLog->bgConfig);
}

void PokewalkerEventLog_LoadBgGraphics(PokewalkerEventLog *eventLog) {
    GF_ASSERT(eventLog->bgConfig != NULL);
    GfGfxLoader_GXLoadPalFromOpenNarc(eventLog->narc, 0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0, eventLog->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(eventLog->narc, 1, eventLog->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, eventLog->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(eventLog->narc, 2, eventLog->bgConfig, GF_BG_LYR_MAIN_0, 0, 0, TRUE, eventLog->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(eventLog->narc, 1, eventLog->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, eventLog->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(eventLog->narc, 3, eventLog->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, eventLog->heapId);
    GfGfxLoader_GXLoadPalFromOpenNarc(eventLog->narc, 9, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0, eventLog->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(eventLog->narc, 10, eventLog->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, eventLog->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(eventLog->narc, 11, eventLog->bgConfig, GF_BG_LYR_SUB_0, 0, 0, TRUE, eventLog->heapId);
}

// Sky of the course by the hour: day (6-15), evening (16-18) or night; course 5 has its own set.
void PokewalkerEventLog_LoadSky(PokewalkerEventLog *eventLog, u32 hour, int course) {
    int sky;
    if (hour >= 6 && hour < 16) {
        sky = 0;
    } else if (hour >= 16 && hour < 19) {
        sky = 1;
    } else {
        sky = 2;
    }
    if (course == 5) {
        sky = (u8)(sky + 3);
    }
    GfGfxLoader_LoadScrnDataFromOpenNarc(eventLog->narc, sky + 3, eventLog->bgConfig, GF_BG_LYR_MAIN_1, 0, 0, TRUE, eventLog->heapId);
}

void PokewalkerEventLog_PlayCourseBgm(PokewalkerEventLog *eventLog, u32 course) {
    GF_ASSERT(course < 8);
    PlayBGM(sEventLogCourseBgm[course]);
}

#pragma force_active on
void PokewalkerEventLog_InitSpriteSystem(PokewalkerEventLog *eventLog) {
    OamManagerParam oamManagerParam;
    OamCharTransferParam oamTransferParam;
    SpriteResourceCountsListUnion counts;
    eventLog->spriteSystem = SpriteSystem_Alloc(eventLog->heapId);
    eventLog->spriteManager = SpriteManager_New(eventLog->spriteSystem);
    oamManagerParam = sEventLogOamManagerParam;
    oamTransferParam = sEventLogOamTransferParam;
    oamTransferParam.maxTasks = 32;
    SpriteSystem_Init(eventLog->spriteSystem, &oamManagerParam, &oamTransferParam, 32);
    SpriteSystem_InitSprites(eventLog->spriteSystem, eventLog->spriteManager, 32);
    counts = sEventLogSpriteResourceCounts;
    SpriteSystem_InitManagerWithCapacities(eventLog->spriteSystem, eventLog->spriteManager, &counts);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(eventLog->spriteSystem), 0, FX32_CONST(524));
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}
#pragma force_active reset

void PokewalkerEventLog_FreeSprites(PokewalkerEventLog *eventLog) {
    SpriteSystem *spriteSystem = eventLog->spriteSystem;
    SpriteManager *spriteManager = eventLog->spriteManager;
    int i;
    PokewalkerEventLog_DeleteMonSprites(eventLog);
    for (i = 0; i < EVENT_LOG_SPRITE_MAX; i++) {
        GF_ASSERT(eventLog->sprites[i] != NULL);
        Sprite_DeleteAndFreeResources(eventLog->sprites[i]);
    }
    SpriteSystem_FreeResourcesAndManager(spriteSystem, spriteManager);
    SpriteSystem_Free(spriteSystem);
}

void PokewalkerEventLog_LoadSpriteResources(PokewalkerEventLog *eventLog) {
    SpriteSystem *spriteSystem = eventLog->spriteSystem;
    SpriteManager *spriteManager = eventLog->spriteManager;

    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 12, FALSE, 12, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 13, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 15, TRUE, 0);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 14, TRUE, 0);
    SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 16, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 1);
    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 17, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 1);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 19, TRUE, 1);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 18, TRUE, 1);
}

ManagedSprite *PokewalkerEventLog_CreateSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 animation, u8 drawPriority) {
    ManagedSpriteTemplate spriteTemplate = sEventLogSpriteTemplate;
    ManagedSprite *sprite;

    spriteTemplate.x = x;
    spriteTemplate.y = y;
    spriteTemplate.drawPriority = drawPriority;
    spriteTemplate.animation = animation;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &spriteTemplate, FX32_CONST(524));
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}

ManagedSprite *PokewalkerEventLog_CreateEffectSprite(SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 animation, u8 drawPriority) {
    ManagedSpriteTemplate spriteTemplate = sEventLogEffectSpriteTemplate;
    ManagedSprite *sprite;

    spriteTemplate.x = x;
    spriteTemplate.y = y;
    spriteTemplate.drawPriority = drawPriority;
    spriteTemplate.animation = animation;
    sprite = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &spriteTemplate, FX32_CONST(524));
    ManagedSprite_SetAnimateFlag(sprite, TRUE);
    return sprite;
}

void PokewalkerEventLog_CreateSprites(PokewalkerEventLog *eventLog) {
    eventLog->sprites[EVENT_LOG_SPRITE_PREV_PAGE] = PokewalkerEventLog_CreateSprite(eventLog->spriteSystem, eventLog->spriteManager, 32, 176, 8, 0);
    eventLog->sprites[EVENT_LOG_SPRITE_NEXT_PAGE] = PokewalkerEventLog_CreateSprite(eventLog->spriteSystem, eventLog->spriteManager, 80, 176, 9, 0);
    eventLog->sprites[EVENT_LOG_SPRITE_EXIT] = PokewalkerEventLog_CreateSprite(eventLog->spriteSystem, eventLog->spriteManager, 216, 176, 10, 0);
    eventLog->sprites[EVENT_LOG_SPRITE_COURSE] = PokewalkerEventLog_CreateSprite(eventLog->spriteSystem, eventLog->spriteManager, 128, 68, 0, 5);
    eventLog->sprites[EVENT_LOG_SPRITE_LEFT_ICON] = PokewalkerEventLog_CreateSprite(eventLog->spriteSystem, eventLog->spriteManager, 104, 56, 12, 2);
    eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON] = PokewalkerEventLog_CreateSprite(eventLog->spriteSystem, eventLog->spriteManager, 152, 56, 12, 1);
    eventLog->sprites[EVENT_LOG_SPRITE_MON_EFFECT] = PokewalkerEventLog_CreateSprite(eventLog->spriteSystem, eventLog->spriteManager, 104, 40, 26, 2);
    eventLog->sprites[EVENT_LOG_SPRITE_LEFT_EFFECT] = PokewalkerEventLog_CreateEffectSprite(eventLog->spriteSystem, eventLog->spriteManager, 104, 40, 8, 2);
    eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_EFFECT] = PokewalkerEventLog_CreateEffectSprite(eventLog->spriteSystem, eventLog->spriteManager, 152, 40, 8, 2);
    eventLog->sprites[EVENT_LOG_SPRITE_CENTER_EFFECT] = PokewalkerEventLog_CreateEffectSprite(eventLog->spriteSystem, eventLog->spriteManager, 128, 40, 8, 2);
    PokewalkerEventLog_CreateMonSprites(eventLog);
    PokewalkerEventLog_SetSceneVisible(eventLog, FALSE);
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_PREV_PAGE], FALSE);
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_NEXT_PAGE], eventLog->page < eventLog->count - 1);
}

void PokewalkerEventLog_SetSceneVisible(PokewalkerEventLog *eventLog, BOOL visible) {
    PokewalkerEventLog_SetMonSpriteVisible(&eventLog->monSprites[0], visible);
    PokewalkerEventLog_SetMonSpriteVisible(&eventLog->monSprites[1], visible);
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_LEFT_ICON], visible);
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], visible);
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_LEFT_EFFECT], visible);
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_MON_EFFECT], visible);
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_EFFECT], visible);
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_CENTER_EFFECT], visible);
}

// Plays the next animation of a visible sprite once its current one has ended.
void PokewalkerEventLog_AdvanceSpriteAnim(PokewalkerEventLog *eventLog, int spriteId) {
    if (ManagedSprite_GetDrawFlag(eventLog->sprites[spriteId]) && !ManagedSprite_IsAnimated(eventLog->sprites[spriteId])) {
        u16 animation = ManagedSprite_GetActiveAnim(eventLog->sprites[spriteId]) + 1;
        ManagedSprite_SetAnim(eventLog->sprites[spriteId], animation);
    }
}

void PokewalkerEventLog_SetCourseSprite(PokewalkerEventLog *eventLog, int course) {
    GF_ASSERT((u32)course < 8);
    ManagedSprite_SetAnimNoRestart(eventLog->sprites[EVENT_LOG_SPRITE_COURSE], course);
}

void PokewalkerEventLog_LoadSpriteCharData(Sprite *sprite, const void *data, u32 size, u32 offset) {
    NNS_G2D_VRAM_TYPE vramType = Sprite_GetVramType(sprite);
    u32 location = NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprite), vramType);
    DC_FlushRange(data, size);
    switch (vramType) {
    case NNS_G2D_VRAM_TYPE_2DMAIN:
        GX_LoadOBJ(data, location + offset, size);
        return;
    case NNS_G2D_VRAM_TYPE_2DSUB:
        GXS_LoadOBJ(data, location + offset, size);
        return;
    default:
        GF_AssertFail();
        return;
    }
}

#pragma force_active on
// Loads the resources of the walking Pokemon sprites over a few frames; TRUE when done.
BOOL PokewalkerEventLog_LoadMonSpriteResources(PokewalkerEventLog *eventLog) {
    SpriteSystem *spriteSystem = eventLog->spriteSystem;
    SpriteManager *spriteManager = eventLog->spriteManager;
    switch (eventLog->loadState) {
    case 0:
        SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 12, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 2);
        SpriteSystem_LoadPlttResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 12, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 3);
        eventLog->loadState++;
        break;
    case 1:
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 20, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 2);
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 20, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 3);
        SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 21, TRUE, 2);
        SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 22, TRUE, 2);
        eventLog->loadState++;
        break;
    case 2:
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 23, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 4);
        SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 23, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 5);
        SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 24, TRUE, 3);
        SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, eventLog->narc, 25, TRUE, 3);
        eventLog->loadState++;
        break;
    case 3:
        return TRUE;
    default:
        GF_AssertFail();
    }
    return FALSE;
}
#pragma force_active reset

// Diglett and Dugtrio have no shadow.
void PokewalkerEventLog_SetMonSpriteVisible(PokewalkerEventLogMonSprite *monSprite, BOOL visible) {
    if (visible == FALSE) {
        ManagedSprite_SetDrawFlag(monSprite->sprites[MON_SPRITE_LARGE], visible);
        ManagedSprite_SetDrawFlag(monSprite->sprites[MON_SPRITE_SMALL], visible);
    } else if (monSprite->isLarge) {
        ManagedSprite_SetDrawFlag(monSprite->sprites[MON_SPRITE_LARGE], visible);
    } else {
        ManagedSprite_SetDrawFlag(monSprite->sprites[MON_SPRITE_SMALL], visible);
    }
    if ((u32)(monSprite->species - SPECIES_DIGLETT) <= SPECIES_DUGTRIO - SPECIES_DIGLETT) {
        visible = FALSE;
    }
    ManagedSprite_SetDrawFlag(monSprite->shadowSprite, visible);
}

void PokewalkerEventLog_SetMonSpriteAnim(PokewalkerEventLogMonSprite *monSprite, int animation) {
    ManagedSprite_SetAnimNoRestart(monSprite->sprites[MON_SPRITE_LARGE], animation);
    ManagedSprite_SetAnimNoRestart(monSprite->sprites[MON_SPRITE_SMALL], animation);
}

void PokewalkerEventLog_SetMonSpritePosition(PokewalkerEventLogMonSprite *monSprite, s16 x, s16 y, BOOL keepShadow) {
    ManagedSprite_SetPositionXY(monSprite->sprites[MON_SPRITE_LARGE], x, y);
    ManagedSprite_SetPositionXY(monSprite->sprites[MON_SPRITE_SMALL], x, y);
    if (keepShadow == FALSE) {
        ManagedSprite_SetPositionXY(monSprite->shadowSprite, x, y);
    }
}

void PokewalkerEventLog_CreateMonSprites(PokewalkerEventLog *eventLog) {
    PokewalkerEventLog_CreateMonSprite(&eventLog->monSprites[0], eventLog->spriteSystem, eventLog->spriteManager, 104, 72, 0, 0);
    PokewalkerEventLog_SetMonSpriteAnim(&eventLog->monSprites[0], 3);
    PokewalkerEventLog_CreateMonSprite(&eventLog->monSprites[1], eventLog->spriteSystem, eventLog->spriteManager, 152, 72, 0, 1);
    PokewalkerEventLog_SetMonSpriteAnim(&eventLog->monSprites[1], 1);
}

void PokewalkerEventLog_DeleteMonSprites(PokewalkerEventLog *eventLog) {
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            Sprite_DeleteAndFreeResources(eventLog->monSprites[i].sprites[j]);
        }
        Sprite_DeleteAndFreeResources(eventLog->monSprites[i].shadowSprite);
    }
}

void PokewalkerEventLog_CreateMonSprite(PokewalkerEventLogMonSprite *monSprite, SpriteSystem *spriteSystem, SpriteManager *spriteManager, s16 x, s16 y, u8 animation, int side) {
    ManagedSpriteTemplate spriteTemplate = sEventLogMonSpriteTemplate;
    int i;
    for (i = 0; i < 2; i++) {
        spriteTemplate.x = x;
        spriteTemplate.y = y;
        spriteTemplate.drawPriority = 3;
        spriteTemplate.animation = animation;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_CHAR] = side + 2 + i * 2;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_PLTT] = side + 2;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_CELL] = i + 2;
        spriteTemplate.resIdList[GF_GFX_RES_TYPE_ANIM] = i + 2;
        monSprite->sprites[i] = SpriteSystem_NewSpriteWithYOffset(spriteSystem, spriteManager, &spriteTemplate, FX32_CONST(524));
        ManagedSprite_SetAnimateFlag(monSprite->sprites[i], TRUE);
        ManagedSprite_SetDrawFlag(monSprite->sprites[i], FALSE);
    }
    monSprite->shadowSprite = PokewalkerEventLog_CreateSprite(spriteSystem, spriteManager, x, y, 23, 4);
}

#pragma force_active on
// Copies the 8 frames of the species' overworld model texture into the small or the large sprite.
void PokewalkerEventLog_LoadMonSprite(PokewalkerEventLogMonSprite *monSprite, u16 species, u8 form, int gender, u8 isShiny, int heapId) {
    NARC *narc;
    void *modelFile;
    NNSG3dResTex *tex;
    Sprite *sprite;
    u8 *pixels;
    int dim;
    s16 x;
    s16 y;
    FollowMonModelParam param;
    int size;
    void *buf;
    int i;
    u32 offset;
    u8 *pltt;
    BOOL visible;
    ReadWholeNarcMemberByIdPair(&param, NARC_fielddata_tsurepoke_tp_param, SpeciesToOverworldModelIndexOffset(species));
    if (param.isLarge) {
        size = 0x800;
        sprite = monSprite->sprites[MON_SPRITE_LARGE]->sprite;
        dim = 8;
        monSprite->isLarge = TRUE;
        ManagedSprite_GetPositionXY(monSprite->sprites[MON_SPRITE_LARGE], &x, &y);
        ManagedSprite_SetPositionXY(monSprite->shadowSprite, x, y);
    } else {
        size = 0x200;
        sprite = monSprite->sprites[MON_SPRITE_SMALL]->sprite;
        dim = 4;
        monSprite->isLarge = FALSE;
        ManagedSprite_GetPositionXY(monSprite->sprites[MON_SPRITE_SMALL], &x, &y);
        ManagedSprite_SetPositionXY(monSprite->shadowSprite, x, y);
    }
    monSprite->species = species;
    narc = NARC_New(NARC_data_mmodel_mmodel, (enum HeapID)heapId);
    if (!SanitizeMonForm(species, form)) {
        form = 0;
    }
    modelFile = NARC_AllocAndReadWholeMember(narc, PokewalkerEventLog_GetMonModelFileId(species, form, gender), (enum HeapID)heapId);
    tex = NNS_G3dGetTex(modelFile);
    pixels = (u8 *)tex + tex->texInfo.ofsTex;
    buf = Heap_AllocAtEnd((enum HeapID)heapId, size);
    for (i = 0, offset = 0; i < 8; i++) {
        CharTiles_CopyRectFromBuffer(pixels + offset, dim, 0, 0, dim, dim, buf);
        PokewalkerEventLog_LoadSpriteCharData(sprite, buf, size, offset);
        offset += size;
    }
    Heap_Free(buf);
    Sprite_GetVramType(sprite);
    pltt = (u8 *)tex + tex->plttInfo.ofsPlttData;
    if (isShiny) {
        pltt += 0x20;
    }
    DC_FlushRange(pltt, 0x20);
    GX_LoadOBJPltt(pltt, NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(sprite), NNS_G2D_VRAM_TYPE_2DMAIN), 0x20);
    Heap_Free(modelFile);
    NARC_Delete(narc);
    thunk_Sprite_SetDrawFlag(sprite, TRUE);
    visible = TRUE;
    if (monSprite->species == SPECIES_DIGLETT || monSprite->species == SPECIES_DUGTRIO) {
        visible = FALSE;
    }
    ManagedSprite_SetDrawFlag(monSprite->shadowSprite, visible);
}
#pragma force_active reset

// Shows the walking Pokemon or the other Pokemon of the shown entry on the left (side 0, facing right) or the right.
void PokewalkerEventLog_ShowMon(PokewalkerEventLog *eventLog, int side, BOOL isWalkingMon) {
    PokewalkerIrRecord *record = eventLog->entries[eventLog->page];
    if (side == 0) {
        PokewalkerEventLog_SetMonSpritePosition(&eventLog->monSprites[0], 104, 72, FALSE);
        PokewalkerEventLog_SetMonSpriteAnim(&eventLog->monSprites[0], 3);
    } else {
        PokewalkerEventLog_SetMonSpritePosition(&eventLog->monSprites[1], 152, 72, FALSE);
        PokewalkerEventLog_SetMonSpriteAnim(&eventLog->monSprites[1], 1);
    }
    if (isWalkingMon) {
        PokewalkerEventLog_LoadMonSprite(&eventLog->monSprites[side], record->unk_0A, record->form, record->gender, PokewalkerEventLog_IsMonShiny(record, TRUE), eventLog->heapId);
        return;
    }
    PokewalkerEventLog_LoadMonSprite(&eventLog->monSprites[side], record->unk_0C, record->peerForm, record->peerGender, PokewalkerEventLog_IsMonShiny(record, FALSE), eventLog->heapId);
}

void PokewalkerEventLog_ShowTrainer(ManagedSprite *sprite, int gender, int pose) {
    pose += 15;
    if (gender == PLAYER_GENDER_FEMALE) {
        pose += 4;
    }
    ManagedSprite_SetDrawFlag(sprite, TRUE);
    ManagedSprite_SetAnim(sprite, pose);
}

void PokewalkerEventLog_ShowEffectAboveMon(PokewalkerEventLog *eventLog, int side, int spriteId, int animation) {
    s16 x, y;
    ManagedSprite_GetPositionXY(eventLog->sprites[spriteId], &x, &y);
    y = eventLog->monSprites[side].isLarge ? 24 : 40;
    ManagedSprite_SetPositionXY(eventLog->sprites[spriteId], x, y);
    ManagedSprite_SetAnim(eventLog->sprites[spriteId], animation);
    ManagedSprite_SetDrawFlag(eventLog->sprites[spriteId], TRUE);
}

void PokewalkerEventLog_ResetAnims(PokewalkerEventLog *eventLog) {
    PokewalkerEventLog_ResetFleeAnim(eventLog);
    PokewalkerEventLog_ResetCatchAnim(eventLog);
}

void PokewalkerEventLog_UpdateAnims(PokewalkerEventLog *eventLog) {
    PokewalkerEventLog_UpdateFleeAnim(eventLog);
    PokewalkerEventLog_UpdateCatchAnim(eventLog);
}

void PokewalkerEventLog_StartFleeAnim(PokewalkerEventLog *eventLog) {
    eventLog->fleeAnim.active = TRUE;
}

void PokewalkerEventLog_ResetFleeAnim(PokewalkerEventLog *eventLog) {
    eventLog->fleeAnim.active = FALSE;
    eventLog->fleeAnim.frame = 0;
    eventLog->fleeAnim.phase = 0;
    eventLog->fleeAnim.hops = 0;
}

#pragma force_active on
// Two hops in place, then the right-hand Pokemon runs off to the right, blinking.
void PokewalkerEventLog_UpdateFleeAnim(PokewalkerEventLog *eventLog) {
    if (eventLog->fleeAnim.active) {
        switch (eventLog->fleeAnim.phase) {
        case 0:
            PokewalkerEventLog_SetMonSpritePosition(&eventLog->monSprites[1], 152, (s16)(72 - sEventLogFleeHopHeights[eventLog->fleeAnim.frame]), TRUE);
            eventLog->fleeAnim.frame++;
            if (sEventLogFleeHopHeights[eventLog->fleeAnim.frame] == 111) {
                eventLog->fleeAnim.frame = 0;
                eventLog->fleeAnim.hops++;
                if (eventLog->fleeAnim.hops >= 2) {
                    PokewalkerEventLog_SetMonSpriteAnim(&eventLog->monSprites[1], 3);
                    eventLog->fleeAnim.hops = 0;
                    eventLog->fleeAnim.phase++;
                }
            }
            break;
        case 1: {
            s16 x = (s16)(152 + eventLog->fleeAnim.frame * 8);
            eventLog->fleeAnim.frame++;
            if (eventLog->fleeAnim.frame >= 5) {
                PokewalkerEventLog_SetMonSpriteVisible(&eventLog->monSprites[1], (eventLog->fleeAnim.frame & 1) == 1);
            }
            if (eventLog->fleeAnim.frame >= 7) {
                PokewalkerEventLog_SetMonSpriteVisible(&eventLog->monSprites[1], FALSE);
                eventLog->fleeAnim.frame = 0;
                eventLog->fleeAnim.phase++;
            } else {
                PokewalkerEventLog_SetMonSpritePosition(&eventLog->monSprites[1], x, 72, FALSE);
            }
            break;
        }
        case 2:
            break;
        default:
            GF_ASSERT(0);
        }
    }
}
#pragma force_active reset

void PokewalkerEventLog_StartCatchAnim(PokewalkerEventLog *eventLog) {
    eventLog->catchAnim.active = TRUE;
}

void PokewalkerEventLog_ResetCatchAnim(PokewalkerEventLog *eventLog) {
    eventLog->catchAnim.active = FALSE;
    eventLog->catchAnim.timer = 0;
    eventLog->catchAnim.state = 0;
}

#pragma force_active on
// The Poke Ball, then the right-hand Pokemon disappears after 25 frames.
void PokewalkerEventLog_UpdateCatchAnim(PokewalkerEventLog *eventLog) {
    if (eventLog->catchAnim.active) {
        switch (eventLog->catchAnim.state) {
        case 0:
            ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], TRUE);
            ManagedSprite_SetAnim(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], 24);
            eventLog->catchAnim.state++;
            break;
        case 1:
            if (eventLog->catchAnim.timer >= 25) {
                PokewalkerEventLog_SetMonSpriteVisible(&eventLog->monSprites[1], FALSE);
                eventLog->catchAnim.timer = 0;
            } else {
                eventLog->catchAnim.timer++;
            }
            break;
        case 2:
            break;
        default:
            GF_AssertFail();
            break;
        }
    }
}
#pragma force_active reset

// Events 1-10 with a HeartGold / SoulSilver Pokewalker: five kinds of meeting (5 messages each), the walking
// Pokemon wins on odd types; the second line depends on whether the step counts are 100 or more apart.
void PokewalkerEventLog_ShowPeerEvent(PokewalkerEventLog *eventLog) {
    PokewalkerIrRecord *record = eventLog->entries[eventLog->page];
    int kind = (record->eventType - 1) / 2;
    int msgBase = kind * 5;
    int stepsDiff;
    int msgOffset;
    PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
    PokewalkerEventLog_ShowMon(eventLog, 1, FALSE);
    stepsDiff = record->unk_78 - record->unk_7A;
    if (stepsDiff < 0) {
        stepsDiff = -stepsDiff;
    }
    if (record->eventType % 2 == 1) {
        msgOffset = stepsDiff >= 100 ? 1 : 2;
    } else {
        msgOffset = stepsDiff >= 100 ? 4 : 3;
    }
    PokewalkerEventLog_PrintEntry(eventLog, msgBase, msgBase + msgOffset);
}

void PokewalkerEventLog_ShowDowsingItem(PokewalkerEventLog *eventLog) {
    PokewalkerIrRecord *record = eventLog->entries[eventLog->page];
    int msgId;
    PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], TRUE);
    ManagedSprite_SetAnim(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], 11);
    if (eventLog->dowsingStreakPage != 0 && eventLog->dowsingStreakPage <= eventLog->page) {
        msgId = msg_0275_00027;
    } else if (record->unk_78 >= 4000) {
        msgId = msg_0275_00028;
    } else if (record->unk_78 >= 3000) {
        msgId = msg_0275_00029;
    } else if (record->unk_78 >= 2000) {
        msgId = msg_0275_00030;
    } else if (record->unk_78 >= 1000) {
        msgId = msg_0275_00031;
    } else {
        msgId = msg_0275_00032;
    }
    PokewalkerEventLog_PrintEntry(eventLog, msg_0275_00026, msgId);
}

void PokewalkerEventLog_ShowGiftItem(PokewalkerEventLog *eventLog) {
    PokewalkerIrRecord *record = eventLog->entries[eventLog->page];
    int msgId;
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], TRUE);
    ManagedSprite_SetAnim(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], 13);
    if (record->unk_0A != SPECIES_NONE) {
        msgId = msg_0275_00033;
        PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
    } else {
        msgId = msg_0275_00034;
        PokewalkerEventLog_ShowTrainer(eventLog->sprites[EVENT_LOG_SPRITE_LEFT_ICON], (u8)eventLog->playerGender, 3);
    }
    PokewalkerEventLog_PrintEntry(eventLog, msgId, 0);
}

void PokewalkerEventLog_ShowRadarCaught(PokewalkerEventLog *eventLog) {
    PokewalkerIrRecord *record = eventLog->entries[eventLog->page];
    int msgId;
    PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
    PokewalkerEventLog_ShowMon(eventLog, 1, FALSE);
    if (eventLog->radarStreakPage != 0 && eventLog->radarStreakPage <= eventLog->page) {
        msgId = msg_0275_00036;
    } else if (record->unk_78 >= 4000) {
        msgId = msg_0275_00037;
    } else if (record->unk_78 >= 3000) {
        msgId = msg_0275_00038;
    } else if (record->unk_78 >= 2000) {
        msgId = msg_0275_00039;
    } else if (record->unk_78 >= 1000) {
        msgId = msg_0275_00040;
    } else {
        msgId = msg_0275_00041;
    }
    PokewalkerEventLog_PrintEntry(eventLog, msg_0275_00035, msgId);
    PokewalkerEventLog_StartCatchAnim(eventLog);
}

void PokewalkerEventLog_ShowGiftMon(PokewalkerEventLog *eventLog) {
    PokewalkerIrRecord *record = eventLog->entries[eventLog->page];
    int msgId;
    if (record->unk_0A != SPECIES_NONE) {
        msgId = msg_0275_00042;
        PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
    } else {
        msgId = msg_0275_00043;
        PokewalkerEventLog_ShowTrainer(eventLog->sprites[EVENT_LOG_SPRITE_LEFT_ICON], (u8)eventLog->playerGender, 3);
    }
    PokewalkerEventLog_ShowMon(eventLog, 1, FALSE);
    PokewalkerEventLog_PrintEntry(eventLog, msgId, 0);
}

void PokewalkerEventLog_ShowRadarFled(PokewalkerEventLog *eventLog) {
    PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
    PokewalkerEventLog_ShowMon(eventLog, 1, FALSE);
    PokewalkerEventLog_StartFleeAnim(eventLog);
    PokewalkerEventLog_PrintEntry(eventLog, msg_0275_00044, 0);
}

void PokewalkerEventLog_ShowRadarLost(PokewalkerEventLog *eventLog) {
    PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
    PokewalkerEventLog_ShowMon(eventLog, 1, FALSE);
    PokewalkerEventLog_ShowEffectAboveMon(eventLog, 0, EVENT_LOG_SPRITE_MON_EFFECT, 26);
    PokewalkerEventLog_PrintEntry(eventLog, msg_0275_00045, 0);
}

// The second line depends on unk_77.
void PokewalkerEventLog_ShowMonFoundItem(PokewalkerEventLog *eventLog) {
    PokewalkerIrRecord *record = eventLog->entries[eventLog->page];
    int msgId2 = 0;
    PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], TRUE);
    ManagedSprite_SetAnim(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], 12);
    if (record->unk_77 >= 250) {
        msgId2 = msg_0275_00047;
    } else if (record->unk_77 >= 200) {
        msgId2 = msg_0275_00048;
    } else if (record->unk_77 >= 150) {
        msgId2 = msg_0275_00049;
    } else if (record->unk_77 >= 90) {
        msgId2 = msg_0275_00050;
    }
    PokewalkerEventLog_PrintEntry(eventLog, msg_0275_00046, msgId2);
}

// Events 18-22, three messages each by unk_77.
void PokewalkerEventLog_ShowHappy(PokewalkerEventLog *eventLog) {
    PokewalkerIrRecord *record = eventLog->entries[eventLog->page];
    int type = record->eventType;
    int value = record->unk_77;
    int msgId = (type - POKEWALKER_EVENT_HAPPY_FIRST) * 3 + msg_0275_00051;
    if (value < 200) {
        if (value >= 60) {
            msgId++;
        } else if (value >= 0) {
            msgId += 2;
        }
    }
    if (msgId == msg_0275_00053) { // empty
        msgId = msg_0275_00052;
    }
    PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
    PokewalkerEventLog_ShowTrainer(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], (u8)eventLog->playerGender, 1);
    PokewalkerEventLog_PrintEntry(eventLog, msgId, 0);
}

void PokewalkerEventLog_ShowMonJoined(PokewalkerEventLog *eventLog) {
    PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
    PokewalkerEventLog_ShowTrainer(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], (u8)eventLog->playerGender, 3);
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_EFFECT], TRUE);
    ManagedSprite_SetAnim(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_EFFECT], 8);
    PokewalkerEventLog_PrintEntry(eventLog, msg_0275_00066, 0);
}

void PokewalkerEventLog_ShowMonWentHome(PokewalkerEventLog *eventLog) {
    PokewalkerEventLog_ShowMon(eventLog, 1, TRUE);
    PokewalkerEventLog_SetMonSpriteAnim(&eventLog->monSprites[1], 3);
    PokewalkerEventLog_PrintEntry(eventLog, msg_0275_00067, 0);
}

void PokewalkerEventLog_ShowStrollStart(PokewalkerEventLog *eventLog) {
    PokewalkerIrRecord *record = eventLog->entries[eventLog->page];
    int msgId2;
    if (record->unk_0A == SPECIES_NONE) {
        msgId2 = msg_0275_00070;
    } else {
        PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
        msgId2 = msg_0275_00069;
    }
    PokewalkerEventLog_ShowTrainer(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], (u8)eventLog->playerGender, 3);
    PokewalkerEventLog_PrintEntry(eventLog, msg_0275_00068, msgId2);
}

#pragma force_active on
// "Returned at ..." and a rating of the Stroll: its steps, the Dowsing Machine items, the Poke Radar catches, the
// meetings with other Pokewalkers and the gifts.
void PokewalkerEventLog_ShowStrollEnd(PokewalkerEventLog *eventLog) {
    PokewalkerIrRecord *record = eventLog->entries[eventLog->page];
    int score = 0;
    int part;
    int msgId;
    int msgId2;
    int hasGiftMon;
    int hasGiftItem;
    u32 total;
    PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
    PokewalkerEventLog_ShowTrainer(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], (u8)eventLog->playerGender, 3);
    part = record->unk_7C / 100;
    if (part > 655) {
        part = 655;
    }
    score += part;
    part = 10 * eventLog->radarCount;
    if (part > 40) {
        part = 40;
    }
    score += part;
    part = 10 * eventLog->dowsingCount;
    if (part > 40) {
        part = 40;
    }
    score += part;
    part = 15 * eventLog->peerCount;
    if (part > 150) {
        part = 150;
    }
    hasGiftMon = eventLog->hasGiftMon;
    hasGiftItem = eventLog->hasGiftItem;
    score += part;
    total = score;
    total += hasGiftMon * 250;
    total += hasGiftItem * 250;
    if (total >= 550) {
        msgId2 = msg_0275_00072;
    } else if (total >= 300) {
        msgId2 = msg_0275_00073;
    } else if (total >= 200) {
        msgId2 = msg_0275_00074;
    } else if (total >= 150) {
        msgId2 = msg_0275_00075;
    } else if (total >= 100) {
        msgId2 = msg_0275_00076;
    } else if (total >= 30) {
        msgId2 = msg_0275_00077;
    } else {
        msgId2 = msg_0275_00078;
    }
    if ((int)eventLog->time.hour < 12) {
        msgId = msg_0275_00071;
    } else {
        msgId = msg_0275_00090;
    }
    PokewalkerEventLog_PrintEntry(eventLog, msgId, msgId2);
}

// Steps of an hour (300 or more), otherwise the Pokemon took a nap (by the hour of the day).
void PokewalkerEventLog_ShowSteps(PokewalkerEventLog *eventLog, u32 hour) {
    u16 steps = eventLog->entries[eventLog->page]->unk_78;
    int msgId;
    if (steps >= 300) {
        PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
        PokewalkerEventLog_ShowTrainer(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], (u8)eventLog->playerGender, 3);
        if (steps >= 4000) {
            msgId = msg_0275_00079;
        } else if (steps >= 2000) {
            msgId = msg_0275_00080;
        } else if (steps >= 1000) {
            msgId = msg_0275_00081;
        } else {
            msgId = msg_0275_00082;
        }
    } else {
        PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
        PokewalkerEventLog_SetMonSpritePosition(&eventLog->monSprites[0], 128, 72, FALSE);
        PokewalkerEventLog_SetMonSpriteAnim(&eventLog->monSprites[0], 0);
        PokewalkerEventLog_ShowEffectAboveMon(eventLog, 0, EVENT_LOG_SPRITE_CENTER_EFFECT, 10);
        if ((hour >= 20 && hour <= 24) || hour <= 7) {
            msgId = msg_0275_00083;
        } else if (hour >= 12 && hour <= 16) {
            msgId = msg_0275_00084;
        } else {
            msgId = msg_0275_00085;
        }
    }
    PokewalkerEventLog_PrintEntry(eventLog, msgId, 0);
}
#pragma force_active reset

// Events 1-10 with a Pokewalker of another game.
void PokewalkerEventLog_ShowDistantPeer(PokewalkerEventLog *eventLog) {
    PokewalkerEventLog_ShowMon(eventLog, 0, TRUE);
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], TRUE);
    ManagedSprite_SetAnim(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_ICON], 27);
    ManagedSprite_SetDrawFlag(eventLog->monSprites[1].shadowSprite, TRUE);
    ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_EFFECT], TRUE);
    ManagedSprite_SetAnim(eventLog->sprites[EVENT_LOG_SPRITE_RIGHT_EFFECT], 7);
    PokewalkerEventLog_PrintEntry(eventLog, msg_0275_00025, 0);
}

#pragma force_active on
void PokewalkerEventLog_ShowEntry(PokewalkerEventLog *eventLog) {
    PokewalkerIrRecord *record = eventLog->entries[eventLog->page];
    u32 hour;
    u8 type;
    PokewalkerEventLog_ConvertEntryTime(eventLog);
    hour = eventLog->time.hour;
    PokewalkerEventLog_LoadSky(eventLog, hour, PokewalkerEventLog_GetCourse(record));
    PokewalkerEventLog_SetCourseSprite(eventLog, PokewalkerEventLog_GetCourse(record));
    if (eventLog->course != record->unk_76) {
        PokewalkerEventLog_PlayCourseBgm(eventLog, PokewalkerEventLog_GetCourse(record));
    }
    eventLog->course = record->unk_76;
    type = record->eventType;
    if (type >= POKEWALKER_EVENT_PEER_FIRST && type <= POKEWALKER_EVENT_PEER_LAST) {
        if ((u16)(record->unk_08 + 0xFFF9) <= 1) { // HeartGold / SoulSilver
            PokewalkerEventLog_ShowPeerEvent(eventLog);
        } else {
            PokewalkerEventLog_ShowDistantPeer(eventLog);
        }
    } else if (type == POKEWALKER_EVENT_DOWSING_ITEM) {
        PokewalkerEventLog_ShowDowsingItem(eventLog);
    } else if (type == POKEWALKER_EVENT_GIFT_ITEM) {
        PokewalkerEventLog_ShowGiftItem(eventLog);
    } else if (type == POKEWALKER_EVENT_RADAR_CAUGHT) {
        PokewalkerEventLog_ShowRadarCaught(eventLog);
    } else if (type == POKEWALKER_EVENT_GIFT_MON) {
        PokewalkerEventLog_ShowGiftMon(eventLog);
    } else if (type == POKEWALKER_EVENT_RADAR_FLED) {
        PokewalkerEventLog_ShowRadarFled(eventLog);
    } else if (type == POKEWALKER_EVENT_RADAR_LOST) {
        PokewalkerEventLog_ShowRadarLost(eventLog);
    } else if (type == POKEWALKER_EVENT_MON_FOUND_ITEM) {
        PokewalkerEventLog_ShowMonFoundItem(eventLog);
    } else if (type >= POKEWALKER_EVENT_HAPPY_FIRST && type <= POKEWALKER_EVENT_HAPPY_LAST) {
        PokewalkerEventLog_ShowHappy(eventLog);
    } else if (type == POKEWALKER_EVENT_MON_JOINED) {
        PokewalkerEventLog_ShowMonJoined(eventLog);
    } else if (type == POKEWALKER_EVENT_MON_WENT_HOME) {
        PokewalkerEventLog_ShowMonWentHome(eventLog);
    } else if (type == POKEWALKER_EVENT_STROLL_START) {
        PokewalkerEventLog_ShowStrollStart(eventLog);
    } else if (type == POKEWALKER_EVENT_STROLL_END) {
        PokewalkerEventLog_ShowStrollEnd(eventLog);
    } else if (type == POKEWALKER_EVENT_STEPS) {
        PokewalkerEventLog_ShowSteps(eventLog, hour);
    }
}
#pragma force_active reset

void PokewalkerEventLog_StartPageTurn(PokewalkerEventLog *eventLog) {
    eventLog->pageTurnActive = TRUE;
}

// Fades both screens to white, shows the new page and fades back; TRUE when not turning a page.
BOOL PokewalkerEventLog_UpdatePageTurn(PokewalkerEventLog *eventLog) {
    if (eventLog->pageTurnActive == FALSE) {
        return TRUE;
    }
    switch (eventLog->pageTurnState) {
    case 0:
        BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_WHITE, 2, 1, eventLog->heapId);
        eventLog->pageTurnState++;
        break;
    case 1:
        if (IsPaletteFadeFinished()) {
            ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_PREV_PAGE], eventLog->page != 0);
            ManagedSprite_SetDrawFlag(eventLog->sprites[EVENT_LOG_SPRITE_NEXT_PAGE], eventLog->page < eventLog->count - 1);
            PokewalkerEventLog_SetSceneVisible(eventLog, FALSE);
            PokewalkerEventLog_ResetAnims(eventLog);
            PokewalkerEventLog_ShowEntry(eventLog);
            BeginNormalPaletteFade(FADE_MAIN_ONLY, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 2, 1, eventLog->heapId);
            eventLog->pageTurnState++;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            eventLog->pageTurnState = 0;
            eventLog->pageTurnActive = FALSE;
            return TRUE;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

void PokewalkerEventLog_Setup(OverlayManager *manager) {
    PokewalkerEventLog *eventLog;
    PokewalkerEventLogArgs *args = OverlayManager_GetArgs(manager);

    GF_ASSERT(args != NULL);
    Heap_Create(HEAP_ID_3, HEAP_ID_155, 0x60000);
    eventLog = OverlayManager_CreateAndGetData(manager, sizeof(PokewalkerEventLog), HEAP_ID_155);
    MI_CpuFill8(eventLog, 0, sizeof(PokewalkerEventLog));
    eventLog->heapId = HEAP_ID_155;
    eventLog->args = args;
    PokewalkerEventLog_BuildEntryList(eventLog);
    PokewalkerEventLog_ResetDisplay();
    PokewalkerEventLog_SetGxBanks();
    gSystem.screensFlipped = TRUE;
    GfGfx_SwapDisplay();
    eventLog->narc = NARC_New(NARC_a_2_4_9, eventLog->heapId);
    PokewalkerEventLog_InitBgs(eventLog);
    PokewalkerEventLog_InitText(eventLog);
    PokewalkerEventLog_InitSpriteSystem(eventLog);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    Main_SetVBlankIntrCB((GFIntrCB)PokewalkerEventLog_VBlankCB, eventLog);
}

void PokewalkerEventLog_Teardown(OverlayManager *manager) {
    PokewalkerEventLog *eventLog = OverlayManager_GetData(manager);

    PokewalkerEventLog_FreeText(eventLog);
    PokewalkerEventLog_FreeBgs(eventLog);
    PokewalkerEventLog_FreeSprites(eventLog);
    NARC_Delete(eventLog->narc);
    gSystem.screensFlipped = FALSE;
    GfGfx_SwapDisplay();
    PokewalkerEventLog_ResetDisplay();
    OverlayManager_FreeData(manager);
    Heap_Destroy(HEAP_ID_155);
}

void PokewalkerEventLog_VBlankCB(PokewalkerEventLog *eventLog) {
    GF_ASSERT(eventLog != NULL);
    GF_ASSERT(eventLog->spriteManager != NULL);
    GF_ASSERT(eventLog->bgConfig != NULL);
    SpriteSystem_DrawSprites(eventLog->spriteManager);
    SpriteSystem_TransferOam();
    DoScheduledBgGpuUpdates(eventLog->bgConfig);
    {
        vu32 *dtcm = (vu32 *)0x027E0000;
        dtcm[0x3FF8 / sizeof(u32)] |= OS_IE_V_BLANK; // OS_SetIrqCheckFlag(OS_IE_V_BLANK)
    }
}

#pragma force_active on
BOOL PokewalkerEventLog_Init(OverlayManager *manager, int *state) {
    PokewalkerEventLog *eventLog = OverlayManager_GetData(manager);
    switch (*state) {
    case 0:
        PokewalkerEventLog_Setup(manager);
        ++*state;
        break;
    case 1:
        PokewalkerEventLog_LoadBgGraphics(eventLog);
        PokewalkerEventLog_LoadSpriteResources(eventLog);
        ++*state;
        break;
    case 2:
        if (PokewalkerEventLog_LoadMonSpriteResources(eventLog)) {
            PokewalkerEventLog_CreateSprites(eventLog);
            PokewalkerEventLog_PrintQuit(eventLog);
            eventLog->course = 0xFF;
            PokewalkerEventLog_ShowEntry(eventLog);
            ++*state;
        }
        break;
    case 3:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_WHITE, 6, 1, eventLog->heapId);
        ++*state;
        break;
    case 4:
        if (IsPaletteFadeFinished()) {
            return TRUE;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}
#pragma force_active reset

BOOL PokewalkerEventLog_Exit(OverlayManager *manager, int *state) {
    PokewalkerEventLog *eventLog = OverlayManager_GetData(manager);

    switch (*state) {
    case 0:
        if (!ManagedSprite_IsAnimated(eventLog->sprites[EVENT_LOG_SPRITE_EXIT])) {
            (*state)++;
        }
        break;
    case 1:
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, eventLog->heapId);
        (*state)++;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            PokewalkerEventLog_Teardown(manager);
            return TRUE;
        }
        break;
    default:
        GF_AssertFail();
        break;
    }
    return FALSE;
}

BOOL PokewalkerEventLog_Main(OverlayManager *manager, int *state) {
    PokewalkerEventLog *eventLog = OverlayManager_GetData(manager);

    if (!PokewalkerEventLog_UpdatePageTurn(eventLog)) {
        return FALSE;
    }
    if (PokewalkerEventLog_CheckPrevPage(eventLog)) {
        if (eventLog->page != 0) {
            eventLog->page--;
        }
        PokewalkerEventLog_StartPageTurn(eventLog);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (PokewalkerEventLog_CheckNextPage(eventLog)) {
        eventLog->page = (eventLog->page + 1) % eventLog->count;
        PokewalkerEventLog_StartPageTurn(eventLog);
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (PokewalkerEventLog_CheckExit(eventLog)) {
        return TRUE;
    }
    PokewalkerEventLog_UpdateAnims(eventLog);
    PokewalkerEventLog_AdvanceSpriteAnim(eventLog, EVENT_LOG_SPRITE_RIGHT_ICON);
    PokewalkerEventLog_AdvanceSpriteAnim(eventLog, EVENT_LOG_SPRITE_LEFT_EFFECT);
    PokewalkerEventLog_AdvanceSpriteAnim(eventLog, EVENT_LOG_SPRITE_RIGHT_EFFECT);
    PokewalkerEventLog_AdvanceSpriteAnim(eventLog, EVENT_LOG_SPRITE_CENTER_EFFECT);
    return FALSE;
}

// The last event log entry, written by the link after a Stroll (its time is set by PokewalkerEventLog_BuildEntryList).
void PokewalkerEventLog_InitStrollEndRecord(PokewalkerIrRecord *record, PokewalkerEventLogParams *params) {
    MI_CpuFill8(record, 0, sizeof(PokewalkerIrRecord));
    MI_CpuCopy8(params->nickname, record->nickname, sizeof(record->nickname));
    record->unk_0A = params->mon.species;
    record->form = params->mon.form;
    record->gender = params->mon.gender;
    record->isShiny = params->mon.isShiny;
    record->unk_7C = params->steps;
    record->eventType = POKEWALKER_EVENT_STROLL_END;
    record->value = -1;
}

void PokewalkerEventLog_SetGiftMon(PokewalkerIrRecord *records, PokewalkerEventLogMon *mon) {
    int i;

    for (i = 0; i < 24; i++, records++) {
        if (records->eventType == POKEWALKER_EVENT_RAW_GIFT_MON) {
            records->unk_0C = mon->species;
            records->peerForm = mon->form;
            records->peerGender = mon->gender;
            records->peerIsShiny = mon->isShiny;
        }
    }
}

BOOL PokewalkerEventLog_HasGiftMon(PokewalkerIrRecord *records) {
    int i;
    for (i = 0; i < 24; i++, records++) {
        if (records->eventType == POKEWALKER_EVENT_RAW_GIFT_MON) {
            return TRUE;
        }
    }
    return FALSE;
}
