#include "global.h"

#include "constants/sndseq.h"

#include "application/vs_recorder/vs_recorder_screens.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0013.h"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "orbit_cursor.h"
#include "palette.h"
#include "pm_string.h"
#include "pokedex.h"
#include "render_text.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "string_util.h"
#include "system.h"
#include "text.h"
#include "touch_hitbox_controller.h"
#include "touchscreen.h"

// VS Recorder video search (VS_RECORDER_SCREEN_SEARCH): set the search conditions (facility, species, the own
// location), request the search, then the result list with party icons, a found video's details and saving it.

typedef struct VsRecorderIconPos {
    s16 x;
    s16 y;
} VsRecorderIconPos;

typedef struct VsRecorderIconPositions {
    VsRecorderIconPos pos[5 * 6];
} VsRecorderIconPositions;

// party slots in icon order, by VsRecorder_HeaderUsesThreeMonLayout (0: singles, 1: the other side starts at the fourth icon)
typedef struct VsRecorderPartyIconOrder {
    int slots[2][6];
} VsRecorderPartyIconOrder;

typedef struct VsRecorderWindowTemplate {
    int x;
    int y;
    int width;
    int height;
} VsRecorderWindowTemplate;

typedef struct VsRecorderConditionWindows {
    VsRecorderWindowTemplate windows[6];
} VsRecorderConditionWindows;

typedef struct VsRecorderConditionMsgIds {
    u32 msgIds[6];
} VsRecorderConditionMsgIds;

typedef struct VsRecorderSearchButtonWindows {
    VsRecorderWindowTemplate windows[3];
} VsRecorderSearchButtonWindows;

typedef struct VsRecorderSearchButtonMsgIds {
    u32 msgIds[3];
} VsRecorderSearchButtonMsgIds;

// asm/vs_recorder_data.s
extern const TouchscreenHitbox sVsRecorderSearchResultOpenHitbox;
extern const TouchscreenHitbox sVsRecorderSearchBackHitbox;
extern const VsRecorderSearchButtonMsgIds sVsRecorderSearchButtonMsgIds;
extern const TouchscreenHitbox sVsRecorderSearchSaveSlotHitboxes[4];
extern const TouchscreenHitbox sVsRecorderSearchOverwriteHitboxes[2];
extern const TouchscreenHitbox sVsRecorderSearchDetailHitboxes[4];
extern const TouchscreenHitbox sVsRecorderSearchConditionHitboxes[5];
extern const VsRecorderConditionMsgIds sVsRecorderSearchConditionMsgIds;
extern const TouchscreenHitbox sVsRecorderSpeciesGroupHitboxes[9];
extern const VsRecorderListConfig sVsRecorderSearchLocationListConfig;
extern const VsRecorderListConfig sVsRecorderSearchFacilityListConfig;
extern const VsRecorderListConfig sVsRecorderSearchFacilityListConfigLong;
extern const VsRecorderListConfig sVsRecorderSearchResultListConfig;
extern const VsRecorderListConfig sVsRecorderSearchSpeciesListConfig;
extern const VsRecorderPartyIconOrder sVsRecorderSearchListIconOrder;
extern const VsRecorderPartyIconOrder sVsRecorderSearchScrollIconOrder;
extern const VsRecorderSearchButtonWindows sVsRecorderSearchButtonWindows;
extern const VsRecorderConditionWindows sVsRecorderSearchConditionWindows;
extern const VsRecorderIconPositions sVsRecorderSearchResultIconPositions;
extern const u16 sVsRecorderSearchSpeciesGroupStarts[];

// the result list entries are video headers; the icons read their party part
#define VIDEO_PARTY(work, i) ((VsRecorderVideoParty *)(work)->net.videoList.headers[i])

// sVsRecorderVideoSearchSubStates
#define SEARCH_SHOW_CONDITIONS      2
#define SEARCH_CONDITIONS_MAIN      3
#define SEARCH_EXIT                 4
#define SEARCH_OPEN_CONDITION       5
#define SEARCH_SELECT_FACILITY      6
#define SEARCH_CLEAR_SPECIES        7
#define SEARCH_SELECT_CATEGORY      8
#define SEARCH_SELECT_SPECIES       9
#define SEARCH_SELECT_LOCATION      10
#define SEARCH_RETURN_TO_CONDITIONS 11
#define SEARCH_REQUEST              12
#define SEARCH_LEAVE_CONDITIONS     13
#define SEARCH_SHOW_RESULTS         14
#define SEARCH_RESULTS_MAIN         15
#define SEARCH_RESULTS_BACK         16
#define SEARCH_SHOW_DETAILS         17
#define SEARCH_DETAILS_MAIN         18
#define SEARCH_SLOTS_MAIN           23
#define SEARCH_OVERWRITE_MAIN       26

#define SEARCH_ANY_SPECIES  0xFFFF
#define SEARCH_ANY          0xFF
#define SEARCH_ANY_FACILITY 0xFE // sent instead of SEARCH_ANY when VsRecorderApp_HasVisitedBattleFrontier is FALSE

void VsRecorder_PlaceSearchResultIcons(VsRecorderWork *work) {
    VsRecorderIconPositions positions = sVsRecorderSearchResultIconPositions;
    int i, j;
    VsRecorderVideoSearch *search = work->screenWork;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 6; j++) {
            if (search->icons[i][j].sprite != NULL) {
                ManagedSprite_SetPositionXY(search->icons[i][j].sprite, positions.pos[i * 6 + j].x, positions.pos[i * 6 + j].y);
            }
        }
    }
}

#pragma force_active on

// party icons of the visible result rows
void VsRecorder_CreateSearchResultIcons(VsRecorderWork *work) {
    int i;
    VsRecorderVideoSearch *search = work->screenWork;
    VsRecorderPartyIconOrder order = sVsRecorderSearchListIconOrder;
    int species[6] = { 0 };
    int forms[6] = { 0 };
    int j;
    int count;
    int layout;
    int scroll;
    int k;

    scroll = work->listWindow.scroll;
    for (i = 0; i < 5; i++) {
        if (VIDEO_PARTY(work, scroll + i) == NULL) {
            continue;
        }
        if (VsRecorder_HeaderUsesThreeMonLayout(VIDEO_PARTY(work, scroll + i)->species)) {
            layout = 1;
        } else {
            layout = 0;
        }
        count = 0;
        for (j = 0; j < 6; j++) {
            species[j] = 0;
            forms[j] = 0;
        }
        for (j = 0; j < 3; j++) {
            if (VIDEO_PARTY(work, scroll + i)->species[order.slots[layout][j]] != SPECIES_NONE) {
                species[count] = VIDEO_PARTY(work, scroll + i)->species[order.slots[layout][j]];
                forms[count] = VIDEO_PARTY(work, scroll + i)->forms[order.slots[layout][j]];
                count++;
            }
        }
        if (VsRecorder_HeaderUsesThreeMonLayout(VIDEO_PARTY(work, scroll + i)->species)) {
            count = 3;
        }
        for (j = 3; j < 6; j++) {
            if (VIDEO_PARTY(work, scroll + i)->species[order.slots[layout][j]] != SPECIES_NONE) {
                species[count] = VIDEO_PARTY(work, scroll + i)->species[order.slots[layout][j]];
                forms[count] = VIDEO_PARTY(work, scroll + i)->forms[order.slots[layout][j]];
                count++;
            }
        }
        for (k = 0; k < 6; k++) {
            int iconSpecies = species[k];
            int iconForm = forms[k];

            search->icons[i][k].resId = search->iconResId;
            search->icons[i][k].sprite = VsRecorder_NewMonIcon(work, search->monIconNarc, search->icons[i][k].resId, iconSpecies, iconForm, FALSE);
            if (search->icons[i][k].sprite != NULL) {
                ((void (*)(ManagedSprite *, int))ManagedSprite_SetDrawPriority)(search->icons[i][k].sprite, 6 - k); // the priority is passed as int
            }
            search->iconResId++;
        }
    }
    VsRecorder_PlaceSearchResultIcons(work);
}

#pragma force_active reset

void VsRecorder_SetSearchResultIconsDrawFlag(VsRecorderWork *work, BOOL draw) {
    int i;
    int j;
    VsRecorderVideoSearch *row; // walks by one row of icons

    for (j = 0, row = work->screenWork; j < 5; row = (VsRecorderVideoSearch *)((u8 *)row + sizeof(row->icons[0])), j++) {
        VsRecorderVideoSearch *icon = row; // walks by one icon

        for (i = 0; i < 6; i++, icon = (VsRecorderVideoSearch *)((u8 *)icon + sizeof(VsRecorderVideoListIcon))) {
            if (icon->icons[0][0].sprite != NULL) {
                ManagedSprite_SetDrawFlag(icon->icons[0][0].sprite, draw);
            }
        }
    }
}

void VsRecorder_FreeSearchResultIcons(VsRecorderWork *work) {
    int i;
    int j;
    VsRecorderVideoSearch *row = work->screenWork; // walks by one row of icons

    VsRecorder_SetVBlankPaused(work, TRUE);
    for (j = 0; j < 5; row = (VsRecorderVideoSearch *)((u8 *)row + sizeof(row->icons[0])), j++) {
        VsRecorderVideoSearch *icon = row; // walks by one icon

        for (i = 0; i < 6; i++, icon = (VsRecorderVideoSearch *)((u8 *)icon + sizeof(VsRecorderVideoListIcon))) {
            if (icon->icons[0][0].sprite != NULL) {
                VsRecorder_DeleteMonIcon(work, icon->icons[0][0].resId, icon->icons[0][0].sprite);
                icon->icons[0][0].sprite = NULL;
            }
        }
    }
    VsRecorder_SetVBlankPaused(work, FALSE);
}

#pragma force_active on

// keep the icons in step with the list scroll: drop the row that left, shift, make the row that came in
void VsRecorder_ScrollSearchResultIcons(VsRecorderWork *work) {
    int row = 0;
    int freed[6] = { 0 };
    VsRecorderVideoSearch *search = work->screenWork;
    int i;
    int j;
    int index;
    int count;
    int layout;

    if (search->iconScroll == work->listWindow.scroll) {
        return;
    }
    VsRecorder_SetVBlankPaused(work, TRUE);
    if (search->iconScroll > work->listWindow.scroll) {
        row = 4;
    }
    for (j = 0; j < 6; j++) {
        if (search->icons[row][j].sprite != NULL) {
            VsRecorder_DeleteSprite(work, (void *)search->icons[row][j].resId, search->icons[row][j].sprite);
            freed[j] = search->icons[row][j].resId;
            search->icons[row][j].sprite = NULL;
        }
    }
    if (row != 0) {
        for (j = 4; j >= 1; j--) {
            for (layout = 0; layout < 6; layout++) {
                search->icons[j][layout].sprite = search->icons[j - 1][layout].sprite;
                search->icons[j][layout].resId = search->icons[j - 1][layout].resId;
            }
        }
        row = 0;
    } else {
        for (i = 1; i < 5; i++) {
            for (j = 0; j < 6; j++) {
                search->icons[i - 1][j].sprite = search->icons[i][j].sprite;
                search->icons[i - 1][j].resId = search->icons[i][j].resId;
            }
        }
        row = 4;
    }
    index = work->listWindow.scroll + row;
    {
        int n;
        VsRecorderPartyIconOrder order = sVsRecorderSearchScrollIconOrder;
        int species[6] = { 0 };
        int forms[6] = { 0 };

        layout = 0;
        if (VsRecorder_HeaderUsesThreeMonLayout(VIDEO_PARTY(work, index)->species)) {
            layout = 1;
        }
        count = 0;
        for (n = 0; n < 6; n++) {
            forms[n] = species[n] = 0;
        }
        for (j = 0; j < 3; j++) {
            if (VIDEO_PARTY(work, index)->species[order.slots[layout][j]] != SPECIES_NONE) {
                species[count] = VIDEO_PARTY(work, index)->species[order.slots[layout][j]];
                forms[count] = VIDEO_PARTY(work, index)->forms[order.slots[layout][j]];
                count++;
            }
        }
        if (VsRecorder_HeaderUsesThreeMonLayout(VIDEO_PARTY(work, index)->species)) {
            count = 3;
        }
        for (j = 3; j < 6; j++) {
            if (VIDEO_PARTY(work, index)->species[order.slots[layout][j]] != SPECIES_NONE) {
                species[count] = VIDEO_PARTY(work, index)->species[order.slots[layout][j]];
                forms[count] = VIDEO_PARTY(work, index)->forms[order.slots[layout][j]];
                count++;
            }
        }
        for (j = 0; j < 6; j++) {
            int iconSpecies = species[j];
            int iconForm = forms[j];

            search->icons[row][j].resId = search->iconResId;
            search->icons[row][j].sprite = VsRecorder_NewMonIcon(work, search->monIconNarc, search->icons[row][j].resId, iconSpecies, iconForm, FALSE);
            if (search->icons[row][j].sprite != NULL) {
                ((void (*)(ManagedSprite *, int))ManagedSprite_SetDrawPriority)(search->icons[row][j].sprite, 6 - j); // the priority is passed as int
            }
            search->iconResId++;
        }
    }
    VsRecorder_PlaceSearchResultIcons(work);
    search->iconScroll = work->listWindow.scroll;
    for (j = 0; j < 6; j++) {
        if (freed[j] != 0) {
            SpriteManager_UnloadCharObjById(work->spriteManager, freed[j] + 100000);
        }
    }
    VsRecorder_SetVBlankPaused(work, FALSE);
}

#pragma force_active reset

// at least one condition is set
BOOL VsRecorder_HasSearchConditions(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    if (search->species != SEARCH_ANY_SPECIES) {
        return TRUE;
    }
    if (search->facility != SEARCH_ANY) {
        return TRUE;
    }
    if (search->country != SEARCH_ANY || search->region != SEARCH_ANY) {
        return TRUE;
    }
    return FALSE;
}

#pragma force_active on

// the condition labels and value windows on the main screen
void VsRecorder_CreateSearchConditionWindows(VsRecorderWork *work, int mode) {
    VsRecorderVideoSearch *search = work->screenWork;
    VsRecorderConditionWindows templates = sVsRecorderSearchConditionWindows;
    VsRecorderConditionMsgIds msgIds = sVsRecorderSearchConditionMsgIds;
    const VsRecorderWindowTemplate *template;
    Window *window;
    int i;
    const u32 *msgId;
    String *str;
    int baseTile;
    BOOL center;

    switch (mode) {
    case 0:
        search->windowCount = 6;
        template = templates.windows;
        msgId = msgIds.msgIds;
        center = FALSE;
        break;
    case 1:
        break;
    }
    baseTile = 256;
    for (i = 0; i < search->windowCount; i++) {
        window = &search->windows[i];
        InitWindow(window);
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, template[i].x, template[i].y, template[i].width, template[i].height, 14, baseTile);
        FillWindowPixelBuffer(window, 0);
        str = NewString_ReadMsgData(work->msgData, *msgId);
        baseTile += template[i].width * template[i].height;
        if (center) {
            VsRecorder_GetCenteredTextX(window, str);
        }
        AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(str);
        msgId++;
    }
}

#pragma force_active reset

void VsRecorder_PrintSearchFacility(VsRecorderWork *work, u32 facility) {
    VsRecorderVideoSearch *search = work->screenWork;
    Window *window = &search->windows[3];
    String *str;

    FillWindowPixelBuffer(window, 0);
    if (facility == SEARCH_ANY) {
        str = NewString_ReadMsgData(work->msgData, msg_0013_00125);
    } else if (facility - 0xFA <= 3) {
        str = NewString_ReadMsgData(work->msgData, facility - 0x55);
    } else {
        str = NewString_ReadMsgData(work->msgData, facility + 0x84);
    }
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
}

void VsRecorder_PrintSearchSpecies(VsRecorderWork *work, u16 species) {
    VsRecorderVideoSearch *search = work->screenWork;
    Window *window = &search->windows[4];
    u16 name[0x100];
    String *str;

    FillWindowPixelBuffer(window, 0);
    if (species == SEARCH_ANY_SPECIES) {
        str = NewString_ReadMsgData(work->msgData, msg_0013_00125);
    } else {
        str = String_New(255, HEAP_ID_109);
        GetSpeciesNameIntoArray(species, HEAP_ID_109, name);
        CopyU16ArrayToString(str, name);
    }
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
}

void VsRecorder_PrintSearchLocation(VsRecorderWork *work, u32 country, u32 region) {
    VsRecorderVideoSearch *search = work->screenWork;
    String *str;
    MessageFormat *msgFormat;
    String *template;

    FillWindowPixelBuffer(&search->windows[5], 0);
    if (country == SEARCH_ANY && region == SEARCH_ANY) {
        str = NewString_ReadMsgData(work->msgData, msg_0013_00125);
    } else {
        msgFormat = VsRecorder_NewMessageFormat(HEAP_ID_109);
        if (region != 0) {
            str = String_New(255, HEAP_ID_109);
            template = NewString_ReadMsgData(work->msgData, msg_0013_00023);
            BufferCityName(msgFormat, 0, country, region);
            StringExpandPlaceholders(msgFormat, str, template);
        } else {
            str = String_New(255, HEAP_ID_109);
            template = NewString_ReadMsgData(work->msgData, msg_0013_00022);
            BufferCountryName(msgFormat, 0, country);
            StringExpandPlaceholders(msgFormat, str, template);
        }
        String_Delete(template);
        MessageFormat_Delete(msgFormat);
    }
    AddTextPrinterParameterizedWithColor(&search->windows[5], 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(&search->windows[5]);
    String_Delete(str);
}

// "SEARCH YOUR LOCATION" / "Ignore Location"
void VsRecorder_PrintLocationButtonLabel(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;
    Window *window = &search->buttonWindows[2];
    String *str;
    u32 msgId;

    if (search->country == SEARCH_ANY) {
        msgId = 0x32;
    } else {
        msgId = 0x14D;
    }
    str = NewString_ReadMsgData(work->msgData, msgId);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
}

// the condition buttons on the touch screen: facility, Pokemon, location
void VsRecorder_CreateSearchButtonWindows(VsRecorderWork *work, int mode) {
    VsRecorderSearchButtonWindows templates;
    VsRecorderSearchButtonMsgIds msgIds;
    VsRecorderVideoSearch *search;
    int i;
    int j;
    int baseTile;
    Window *window;
    Window *labelWindow;
    u32 *msgId;

    search = work->screenWork;
    templates = sVsRecorderSearchButtonWindows;
    msgIds = sVsRecorderSearchButtonMsgIds;
    if (mode != 0) {
        GF_AssertFail();
    }
    search->buttonWindowCount = 3;
    i = 0;
    msgId = msgIds.msgIds;
    baseTile = 0x100;
    window = search->buttonWindows;
    for (; i < 3; i++) {
        InitWindow(window);
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, (u8)templates.windows[i].x, (u8)templates.windows[i].y, (u8)templates.windows[i].width, (u8)templates.windows[i].height, 14, (u16)baseTile);
        FillWindowPixelBuffer(window, 0);
        baseTile += templates.windows[i].width * templates.windows[i].height;
        window++;
    }
    labelWindow = search->buttonWindows;
    for (j = 0; j < 2; j++) {
        String *str = NewString_ReadMsgData(work->msgData, *msgId);

        AddTextPrinterParameterizedWithColor(labelWindow, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        ScheduleWindowCopyToVram(labelWindow);
        String_Delete(str);
        labelWindow++;
        msgId++;
    }
    VsRecorder_PrintLocationButtonLabel(work);
}

void VsRecorder_RemoveSearchWindows(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;
    int i;

    for (i = 0; i < search->windowCount; i++) {
        Window *window = &search->windows[i];

        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
    search->windowCount = 0;
}

void VsRecorder_RemoveSearchButtonWindows(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;
    int i;

    for (i = 0; i < search->buttonWindowCount; i++) {
        Window *window = &search->buttonWindows[i];

        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
    search->buttonWindowCount = 0;
}

// result list title and the label under the touch area
void VsRecorder_CreateSearchResultWindows(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;
    Window *window = &search->windows[0];
    String *str;
    u32 x;

    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, 3, 3, 20, 4, 14, 0x20);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, msg_0013_00126);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
    window = &search->buttonWindows[0];
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 11, 6, 10, 4, 14, 0x100);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, msg_0013_00095);
    x = (80 - FontID_String_GetWidthMultiline(0, str, 0)) / 2;
    AddTextPrinterParameterizedWithColor(window, 0, str, x, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
    search->buttonWindowCount = 1;
    search->windowCount = 1;
}

BOOL VsRecorder_VideoSearchShowConditions(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_ResetBgPriorities();
        VsRecorder_InitBgBlend(&search->eva, &search->evb, 0);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
        GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
        VsRecorder_LoadScreen(work, 0x38, GF_BG_LYR_MAIN_3);
        VsRecorder_LoadScreen(work, 0x3B, GF_BG_LYR_SUB_3);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        VsRecorder_CreateSearchButtons(work);
        VsRecorder_SetSearchButtonsMode(work, FALSE);
        search->iconResId = 500;
        search->hitboxes = TouchHitboxController_Create(sVsRecorderSearchConditionHitboxes, 5, VsRecorder_OnSearchConditionTouch, work, HEAP_ID_109);
        search->slotHitboxes = TouchHitboxController_Create(sVsRecorderSpeciesGroupHitboxes, 9, VsRecorder_OnSpeciesGroupTouch, work, HEAP_ID_109);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 0)) {
            VsRecorder_ShowTopMessage(work, msg_0013_00124);
            search->species = SEARCH_ANY_SPECIES;
            search->facility = SEARCH_ANY;
            search->country = SEARCH_ANY;
            search->region = SEARCH_ANY;
            VsRecorder_CreateSearchConditionWindows(work, 0);
            VsRecorder_CreateSearchButtonWindows(work, 0);
            VsRecorder_PrintSearchSpecies(work, search->species);
            VsRecorder_PrintSearchFacility(work, search->facility);
            VsRecorder_PrintSearchLocation(work, search->country, search->region);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, SEARCH_CONDITIONS_MAIN);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchConditionsMain(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    TouchHitboxController_IsTriggered(search->hitboxes);
    return FALSE;
}

// leave the search for the menu
BOOL VsRecorder_VideoSearchExit(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_RemoveSearchWindows(work);
        VsRecorder_RemoveSearchButtonWindows(work);
        VsRecorder_HideTopMessage(work);
        TouchHitboxController_Destroy(search->hitboxes);
        TouchHitboxController_Destroy(search->slotHitboxes);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 0)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_FreeSearchButtons(work);
        VsRecorder_RebuildMenuRowsParent(work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_StartFlipScreens(work, 1);
        work->step++;
        break;
    default:
        if (VsRecorder_IsFlipScreensDone(work)) {
            if (VsRecorder_StepFade(&search->fade, 0)) {
                VsRecorder_ClearPaletteBlend(work);
                VsRecorder_ResetFade(&search->fade);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, 16, (u16)work->blendColor);
                VsRecorder_SetScreenIfDone(work, VS_RECORDER_SCREEN_MENU, TRUE, work->screenPtr);
                VsRecorder_SetSubState(work, 5);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
                Heap_Free(search);
            } else {
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 2, (u8)search->fade, (u16)work->blendColor);
                PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
            }
        }
        break;
    }
    return FALSE;
}

// a condition button was touched: open its choice
BOOL VsRecorder_VideoSearchOpenCondition(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_RemoveSearchButtonWindows(work);
        work->step++;
        // fallthrough
    case 1:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 2)) {
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        switch (search->condition) {
        case VS_RECORDER_SEARCH_FACILITY:
            VsRecorder_SetSubState(work, SEARCH_SELECT_FACILITY);
            break;
        case VS_RECORDER_SEARCH_SPECIES:
            VsRecorder_SetSubState(work, SEARCH_CLEAR_SPECIES);
            break;
        case VS_RECORDER_SEARCH_LOCATION:
            VsRecorder_SetSubState(work, SEARCH_SELECT_LOCATION);
            break;
        }
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchSelectFacility(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_SetMessageText(work, msg_0013_00113);
        search->facility = SEARCH_ANY;
        VsRecorder_PrintSearchFacility(work, search->facility);
        VsRecorder_SetSearchButtonsMode(work, TRUE);
        VsRecorder_LoadScreen(work, 0x3D, GF_BG_LYR_SUB_3);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 2)) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_CreateTouchScrollSprites(&work->touchScroll, work);
            VsRecorder_ClearListWindow(&work->listWindow);
            if (VsRecorderApp_HasVisitedBattleFrontier(work) == TRUE) {
                VsRecorder_InitMessageList(&work->listWindow, work, (VsRecorderListConfig *)&sVsRecorderSearchFacilityListConfigLong);
            } else {
                VsRecorder_InitMessageList(&work->listWindow, work, (VsRecorderListConfig *)&sVsRecorderSearchFacilityListConfig);
            }
            VsRecorder_TouchScrollSetEnabled(&work->touchScroll, &work->listWindow);
            VsRecorder_CreateListArrows(&work->listWindow, work, NNS_G2D_VRAM_TYPE_2DSUB);
            VsRecorder_SetVBlankPaused(work, FALSE);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    case 2: {
        int facility;

        VsRecorder_UpdateTouchScroll(&work->touchScroll);
        VsRecorder_ListScroll(&work->listWindow, work->touchScroll.direction);
        facility = VsRecorder_UpdateMessageList(&work->listWindow, work);
        if (facility) {
            search->facility = facility;
            VsRecorder_PrintSearchFacility(work, facility);
            work->step++;
        }
        if (TouchscreenHitbox_TouchNewIsIn(&sVsRecorderSearchBackHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            work->step++;
        }
        break;
    }
    case 3:
        VsRecorder_FreeTouchScroll(&work->touchScroll);
        VsRecorder_RemoveListWindow(&work->listWindow);
        VsRecorder_FreeListArrows(&work->listWindow, work);
        VsRecorder_InitTouchScroll(&work->touchScroll);
        VsRecorder_ClearListWindow(&work->listWindow);
        work->step++;
        // fallthrough
    case 4:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 2)) {
            VsRecorder_SetSearchButtonsMode(work, FALSE);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, SEARCH_RETURN_TO_CONDITIONS);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchClearSpecies(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    if (work->step == 0) {
        VsRecorder_SetMessageText(work, msg_0013_00114);
        search->species = SEARCH_ANY_SPECIES;
        VsRecorder_PrintSearchSpecies(work, search->species);
        VsRecorder_SetSearchButtonsMode(work, TRUE);
        work->step++;
    }
    VsRecorder_SetSubState(work, SEARCH_SELECT_CATEGORY);
    return FALSE;
}

// one window per species group, grayed out if no species of it was seen
void VsRecorder_CreateSearchCategoryWindows(VsRecorderWork *work) {
    String *str;
    int baseTile;
    u8 x;
    u8 y;
    u8 width;
    int textY;
    int textX;
    u32 i;
    VsRecorderVideoSearch *search;
    Window *window;
    const TouchscreenHitbox *hitbox;
    u8 height;

    baseTile = 1;
    search = work->screenWork;
    i = 0;
    window = search->buttonWindows;
    hitbox = sVsRecorderSpeciesGroupHitboxes;
    for (; i < 9; i++) {
        x = (u8)(hitbox->rect.left >> 3);
        y = (u8)(hitbox->rect.top >> 3);
        width = (u8)(((u32)hitbox->rect.right >> 3) - x);
        height = (u8)(((u32)hitbox->rect.bottom >> 3) - y);
        InitWindow(window);
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, x, y, width, height, 14, baseTile);
        FillWindowPixelBuffer(window, 0);
        str = NewString_ReadMsgData(work->msgData, i + 68);
        textX = VsRecorder_GetCenteredTextX(window, str);
        textY = (height * 8 - 16) / 2;
        if (VsRecorder_SearchCategoryHasSeen(work, i) == TRUE) {
            AddTextPrinterParameterizedWithColor(window, 0, str, textX, textY, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        } else {
            AddTextPrinterParameterizedWithColor(window, 0, str, textX, textY, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(12, 11, 0), NULL);
        }
        ScheduleWindowCopyToVram(window);
        String_Delete(str);
        baseTile += width * height;
        window++;
        hitbox++;
    }
}

void VsRecorder_RemoveSearchCategoryWindows(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;
    u32 i;
    Window *window;

    for (i = 0, window = search->buttonWindows; i < 9; i++, window++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
    }
}

BOOL VsRecorder_VideoSearchSelectCategory(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_LoadScreen(work, 0x3C, GF_BG_LYR_SUB_3);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 2)) {
            VsRecorder_CreateSearchCategoryWindows(work);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    case 2:
        TouchHitboxController_IsTriggered(search->slotHitboxes);
        if (TouchscreenHitbox_TouchNewIsIn(&sVsRecorderSearchBackHitbox) || search->categoryChosen == TRUE) {
            VsRecorder_OnTouchConfirm(work);
            work->step++;
        }
        break;
    case 3:
        VsRecorder_RemoveSearchCategoryWindows(work);
        work->step++;
        // fallthrough
    case 4:
        if (search->categoryChosen == TRUE) {
            if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 2)) {
                work->step++;
            }
        } else {
            VsRecorder_StepFade(&search->fade, 1);
            if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 2)) {
                VsRecorder_SetSearchButtonsMode(work, FALSE);
                work->step++;
            }
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        }
        break;
    default:
        if (search->categoryChosen == TRUE) {
            VsRecorder_SetSubState(work, SEARCH_SELECT_SPECIES);
        } else {
            VsRecorder_SetSubState(work, SEARCH_RETURN_TO_CONDITIONS);
        }
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchSelectSpecies(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_SetMessageText(work, msg_0013_00114);
        VsRecorder_SetSearchButtonsMode(work, TRUE);
        VsRecorder_LoadScreen(work, 0x3A, GF_BG_LYR_SUB_3);
        work->step++;
        break;
    case 1:
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 2)) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_CreateTouchScrollSprites(&work->touchScroll, work);
            search->listConfig = sVsRecorderSearchSpeciesListConfig;
            search->listConfig.entries = search->entries;
            search->listConfig.count = search->speciesCount;
            VsRecorder_ClearListWindow(&work->listWindow);
            VsRecorder_InitList(&work->listWindow, work, search->speciesNames, &search->listConfig);
            work->listWindow.unk_48 = 0;
            VsRecorder_TouchScrollSetEnabled(&work->touchScroll, &work->listWindow);
            VsRecorder_CreateListArrows(&work->listWindow, work, NNS_G2D_VRAM_TYPE_2DSUB);
            VsRecorder_SetVBlankPaused(work, FALSE);
            work->step++;
        }
        break;
    case 2: {
        u16 species;

        VsRecorder_UpdateTouchScroll(&work->touchScroll);
        VsRecorder_ListScroll(&work->listWindow, work->touchScroll.direction);
        species = ((u16(*)(VsRecorderListWindow *, VsRecorderWork *))VsRecorder_UpdateMessageList)(&work->listWindow, work); // read as u16
        if (species) {
            search->species = species;
            VsRecorder_PrintSearchSpecies(work, species);
            work->step++;
        }
        if (TouchscreenHitbox_TouchNewIsIn(&sVsRecorderSearchBackHitbox)) {
            search->species = SEARCH_ANY_SPECIES;
            VsRecorder_OnTouchConfirm(work);
            work->step++;
        }
        break;
    }
    case 3:
        VsRecorder_FreeTouchScroll(&work->touchScroll);
        VsRecorder_RemoveListWindow(&work->listWindow);
        VsRecorder_FreeListArrows(&work->listWindow, work);
        VsRecorder_InitTouchScroll(&work->touchScroll);
        VsRecorder_ClearListWindow(&work->listWindow);
        work->step++;
        // fallthrough
    case 4:
        if (search->species != SEARCH_ANY_SPECIES) {
            VsRecorder_StepFade(&search->fade, 1);
            if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 2)) {
                VsRecorder_SetSearchButtonsMode(work, FALSE);
                work->step++;
            }
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        } else {
            if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 2)) {
                work->step++;
            }
        }
        break;
    default:
        VsRecorder_FreeSearchSpeciesList(work);
        if (search->species != SEARCH_ANY_SPECIES) {
            VsRecorder_SetSubState(work, SEARCH_RETURN_TO_CONDITIONS);
        } else {
            VsRecorder_SetSubState(work, SEARCH_SELECT_CATEGORY);
        }
        break;
    }
    return FALSE;
}

BOOL VsRecorder_SearchCategoryHasSeen(VsRecorderWork *work, int category) {
    int count;
    BOOL found;
    u16 *list;
    Pokedex *pokedex;
    int end;
    int start;
    int i;
    u32 next;

    found = FALSE;
    next = category + 1;
    end = sVsRecorderSearchSpeciesGroupStarts[next];
    start = sVsRecorderSearchSpeciesGroupStarts[category];
    list = VsRecorder_LoadSpeciesList(HEAP_ID_109, found, &count);
    pokedex = Save_Pokedex_Get(work->saveData);
    for (i = start; i < end; i++) {
        if (!Pokedex_CheckMonSeenFlag(pokedex, list[i])) {
            list[i] = 0xFFFF;
        }
    }
    for (i = start; i < end; i++) {
        if (list[i] != 0xFFFF) {
            found = TRUE;
            break;
        }
    }
    Heap_Free(list);
    return found;
}

// the seen species of the group first, then the rest of the list; entries for the list window
void VsRecorder_BuildSearchSpeciesList(VsRecorderWork *work, int category) {
    VsRecorderVideoSearch *search = work->screenWork;
    u16 *tmp;
    Pokedex *pokedex;
    int start;
    int end;
    int i;
    u32 next;

    next = category + 1;
    end = sVsRecorderSearchSpeciesGroupStarts[next];
    start = sVsRecorderSearchSpeciesGroupStarts[category];
    search->speciesList = VsRecorder_LoadSpeciesList(HEAP_ID_109, 0, &search->listSize);
    tmp = VsRecorder_LoadSpeciesList(HEAP_ID_109, 0, &search->listSize);
    pokedex = Save_Pokedex_Get(work->saveData);
    search->speciesCount = 0;
    for (i = start; i < end; i++) {
        if (!Pokedex_CheckMonSeenFlag(pokedex, tmp[i])) {
            tmp[i] = 0xFFFF;
        } else {
            search->speciesCount++;
        }
    }
    i = 0;
    for (; start < end; start++) {
        if (tmp[start] != 0xFFFF) {
            search->speciesList[i] = tmp[start];
            i++;
        }
    }
    for (; i < search->listSize; i++) {
        search->speciesList[i] = tmp[i];
    }
    Heap_Free(tmp);
    search->entries = Heap_Alloc(HEAP_ID_109, search->speciesCount * sizeof(VsRecorderListEntry));
    MI_CpuFill8(search->entries, 0, search->speciesCount * sizeof(VsRecorderListEntry));
    for (i = 0; i < search->speciesCount; i++) {
        search->entries[i].msgId = search->speciesList[i];
        search->entries[i].value = search->speciesList[i];
    }
    search->speciesNames = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0237_bin, HEAP_ID_109);
    search->categoryChosen = TRUE;
}

void VsRecorder_FreeSearchSpeciesList(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    DestroyMsgData(search->speciesNames);
    Heap_Free(search->entries);
    Heap_Free(search->speciesList);
    search->categoryChosen = FALSE;
}

BOOL VsRecorder_VideoSearchSelectLocation(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_SetMessageText(work, msg_0013_00115);
        search->country = SEARCH_ANY;
        search->region = SEARCH_ANY;
        VsRecorder_PrintSearchLocation(work, search->country, search->region);
        VsRecorder_SetSearchButtonsMode(work, TRUE);
        VsRecorder_LoadScreen(work, 0x39, GF_BG_LYR_SUB_3);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 2)) {
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_CreateTouchScrollSprites(&work->touchScroll, work);
            VsRecorder_InitMessageList(&work->listWindow, work, (VsRecorderListConfig *)&sVsRecorderSearchLocationListConfig);
            VsRecorder_TouchScrollSetEnabled(&work->touchScroll, &work->listWindow);
            VsRecorder_CreateListArrows(&work->listWindow, work, NNS_G2D_VRAM_TYPE_2DSUB);
            VsRecorder_SetVBlankPaused(work, FALSE);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    case 2:
        VsRecorder_UpdateTouchScroll(&work->touchScroll);
        VsRecorder_ListScroll(&work->listWindow, work->touchScroll.direction);
        if (VsRecorder_UpdateMessageList(&work->listWindow, work)) {
            search->country = FrontierCard_GetCountry(work->videoOwners[0]);
            search->region = FrontierCard_GetRegion(work->videoOwners[0]);
            VsRecorder_PrintSearchLocation(work, search->country, search->region);
            work->step++;
        }
        if (TouchscreenHitbox_TouchNewIsIn(&sVsRecorderSearchBackHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            work->step++;
        }
        break;
    case 3:
        VsRecorder_FreeTouchScroll(&work->touchScroll);
        VsRecorder_RemoveListWindow(&work->listWindow);
        VsRecorder_FreeListArrows(&work->listWindow, work);
        work->step++;
        // fallthrough
    case 4:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 2)) {
            VsRecorder_SetSearchButtonsMode(work, FALSE);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, SEARCH_RETURN_TO_CONDITIONS);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchReturnToConditions(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_LoadScreen(work, 0x38, GF_BG_LYR_MAIN_3);
        VsRecorder_LoadScreen(work, 0x3B, GF_BG_LYR_SUB_3);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 2)) {
            VsRecorder_SetMessageText(work, msg_0013_00124);
            VsRecorder_CreateSearchButtonWindows(work, 0);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, SEARCH_CONDITIONS_MAIN);
        break;
    }
    return FALSE;
}

// send the search; no result: "There is no matching Battle Video..." and back to the conditions
BOOL VsRecorder_VideoSearchRequest(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    if (!VsRecorder_IsNetIdle(work)) {
        return FALSE;
    }
    switch (work->step) {
    case 0:
        VsRecorder_SetMessageText(work, msg_0013_00117);
        PlaySE(SEQ_SE_PL_BREC80);
        work->step++;
        break;
    case 1: {
        u32 facility = search->facility;

        if (facility == SEARCH_ANY) {
            if (VsRecorderApp_HasVisitedBattleFrontier(work) == TRUE) {
                facility = SEARCH_ANY;
            } else {
                facility = SEARCH_ANY_FACILITY;
            }
        }
        if (GlobalTerminalNet_RequestVideoSearch(VsRecorder_GetNet(work), search->species, facility, search->country, search->region) == TRUE) {
            work->step++;
        }
        break;
    }
    case 2: {
        GlobalTerminalNetError *error;

        if (GlobalTerminalNet_GetError(VsRecorder_GetNet(work), &error) == TRUE) {
            StopSE(SEQ_SE_PL_BREC80, 0);
            VsRecorder_SetupMessageScreenMsg(work, 7, error->code, error->type);
            work->net.videoList.count = 0;
            work->step++;
        } else {
            StopSE(SEQ_SE_PL_BREC80, 0);
            work->messageScreen.msgId = 0x76;
            work->step++;
        }
        break;
    }
    case 3:
        if (work->net.videoList.count == 0) {
            VsRecorder_SetMessageText(work, work->messageScreen.msgId);
            PlaySE(SEQ_SE_PL_BREC58);
        } else {
            PlaySE(SEQ_SE_PL_BREC12);
        }
        work->step++;
        break;
    default:
        VsRecorder_ClearListWindow(&work->listWindow);
        if (work->net.videoList.count == 0) {
            VsRecorder_SetSubState(work, SEARCH_CONDITIONS_MAIN);
        } else {
            VsRecorder_SetSubState(work, SEARCH_LEAVE_CONDITIONS);
        }
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchLeaveConditions(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_RemoveSearchWindows(work);
        VsRecorder_RemoveSearchButtonWindows(work);
        VsRecorder_HideTopMessage(work);
        TouchHitboxController_Destroy(search->hitboxes);
        TouchHitboxController_Destroy(search->slotHitboxes);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 0)) {
            VsRecorder_SetSearchButtonsMode(work, TRUE);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, SEARCH_SHOW_RESULTS);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchShowResults(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        if (BattleRecord_Exists()) {
            BattleRecord_Delete();
        }
        search->listConfig = sVsRecorderSearchResultListConfig;
        search->listConfig.entries = NULL;
        search->listConfig.count = work->net.videoList.count;
        VsRecorder_LoadMonIconGfx(work);
        search->monIconNarc = ((NARC * (*)(enum HeapID)) VsRecorder_OpenMonIconNarc)(HEAP_ID_109);
        VsRecorder_LoadScreen(work, 4, GF_BG_LYR_MAIN_3);
        VsRecorder_LoadScreen(work, 7, GF_BG_LYR_SUB_3);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        VsRecorder_CreateSearchResultWindows(work);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateTouchScrollSprites(&work->touchScroll, work);
        VsRecorder_InitList(&work->listWindow, work, NULL, &search->listConfig);
        work->listWindow.unk_48 = 1;
        VsRecorder_TouchScrollSetEnabled(&work->touchScroll, &work->listWindow);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateListArrows(&work->listWindow, work, NNS_G2D_VRAM_TYPE_2DMAIN);
        VsRecorder_SetListArrowsY(&work->listWindow, 64, 184);
        VsRecorder_RedrawNameList(&work->listWindow, work);
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateSearchResultIcons(work);
        VsRecorder_SetSearchResultIconsDrawFlag(work, FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 0)) {
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            VsRecorder_SetSearchResultIconsDrawFlag(work, TRUE);
            VsRecorder_PlaceOrbitCursor(work, work->orbitCursors[0], 16, work->listWindow.cursorRow * 24 + 76);
            OrbitCursor_SetVisible(work->orbitCursors[0], TRUE);
            OrbitCursor_SetRadius(work->orbitCursors[0], 12, 12);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, SEARCH_RESULTS_MAIN);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchResultsMain(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_UpdateTouchScroll(&work->touchScroll);
        VsRecorder_ListMoveCursor(&work->listWindow, work->touchScroll.direction);
        VsRecorder_RedrawNameList(&work->listWindow, work);
        OrbitCursor_MoveTarget(work->orbitCursors[0], 16, (s16)(work->listWindow.cursorRow * 24 + 76));
        VsRecorder_ScrollSearchResultIcons(work);
        if (TouchscreenHitbox_TouchNewIsIn(&sVsRecorderSearchResultOpenHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            search->nextSubState = SEARCH_SHOW_DETAILS;
            work->step++;
        }
        if (TouchscreenHitbox_TouchNewIsIn(&sVsRecorderSearchBackHitbox)) {
            VsRecorder_OnTouchConfirm(work);
            search->nextSubState = SEARCH_RESULTS_BACK;
            work->step++;
        }
        break;
    case 1:
        VsRecorder_FreeTouchScroll(&work->touchScroll);
        VsRecorder_RemoveListWindow(&work->listWindow);
        VsRecorder_FreeListArrows(&work->listWindow, work);
        VsRecorder_CloseMonIconNarc(search->monIconNarc);
        VsRecorder_FreeSearchResultIcons(work);
        VsRecorder_UnloadMonIconGfx(work);
        VsRecorder_RemoveSearchWindows(work);
        VsRecorder_RemoveSearchButtonWindows(work);
        OrbitCursor_SetVisible(work->orbitCursors[0], FALSE);
        OrbitCursor_SetRadius(work->orbitCursors[0], 0, 0);
        if (search->nextSubState == SEARCH_RESULTS_BACK) {
            VsRecorder_InitTouchScroll(&work->touchScroll);
            VsRecorder_ClearListWindow(&work->listWindow);
            search->iconScroll = 0;
        }
        work->step++;
        // fallthrough
    case 2:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 0)) {
            VsRecorder_SetSearchButtonsMode(work, FALSE);
            if (search->nextSubState == SEARCH_SHOW_DETAILS) {
                VsRecorder_SetTextSpriteMsg(&search->buttons[0], (VsRecorderGfx *)&work->narc, 3);
                VsRecorder_SetTextSpriteMsg(&search->buttons[1], (VsRecorderGfx *)&work->narc, 0x5E);
                ManagedSprite_SetAnim(search->buttons[0].sprite, 0);
                ManagedSprite_SetAnim(search->buttons[1].sprite, 3);
            }
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, search->nextSubState);
        break;
    }
    return FALSE;
}

// back from the result list to the conditions (kept)
BOOL VsRecorder_VideoSearchResultsBack(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_LoadScreen(work, 0x38, GF_BG_LYR_MAIN_3);
        VsRecorder_LoadScreen(work, 0x3B, GF_BG_LYR_SUB_3);
        VsRecorder_SetSearchButtonsMode(work, FALSE);
        search->hitboxes = TouchHitboxController_Create(sVsRecorderSearchConditionHitboxes, 5, VsRecorder_OnSearchConditionTouch, work, HEAP_ID_109);
        search->slotHitboxes = TouchHitboxController_Create(sVsRecorderSpeciesGroupHitboxes, 9, VsRecorder_OnSpeciesGroupTouch, work, HEAP_ID_109);
        VsRecorder_CreateSearchButtonWindows(work, 0);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 0)) {
            VsRecorder_ShowTopMessage(work, msg_0013_00124);
            VsRecorder_CreateSearchConditionWindows(work, 0);
            VsRecorder_PrintSearchSpecies(work, search->species);
            VsRecorder_PrintSearchFacility(work, search->facility);
            VsRecorder_PrintSearchLocation(work, search->country, search->region);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, SEARCH_CONDITIONS_MAIN);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchShowDetails(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        VsRecorder_LoadScreen(work, 6, GF_BG_LYR_SUB_3);
        work->step++;
        break;
    case 1:
        VsRecorder_LoadSearchResultDetails(work);
        search->hitboxes = TouchHitboxController_Create(sVsRecorderSearchDetailHitboxes, 4, VsRecorder_OnSearchDetailsTouch, work, HEAP_ID_109);
        search->slotHitboxes = TouchHitboxController_Create(sVsRecorderSearchSaveSlotHitboxes, 4, VsRecorder_OnSearchSlotTouch, work, HEAP_ID_109);
        search->overwriteHitboxes = TouchHitboxController_Create(sVsRecorderSearchOverwriteHitboxes, 2, VsRecorder_OnSearchOverwriteTouch, work, HEAP_ID_109);
        work->step++;
        break;
    case 2:
        search->showProfile = 0;
        VsRecorder_SetVBlankPaused(work, TRUE);
        VsRecorder_CreateVideoInfo(&search->info, work);
        VsRecorder_LoadScreen(work, VsRecorderVideoInfo_GetScreenId(&search->info), GF_BG_LYR_MAIN_3);
        VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, FALSE);
        VsRecorder_SetVBlankPaused(work, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG3, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        work->step++;
        break;
    case 3:
        VsRecorder_StepFade(&search->fade, 0);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 0)) {
            VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, TRUE);
            VsRecorder_CreateSearchDetailsButtonWindow(work);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, SEARCH_DETAILS_MAIN);
        break;
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchDetailsMain(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    TouchHitboxController_IsTriggered(search->hitboxes);
    return FALSE;
}

BOOL VsRecorder_VideoSearchToggleProfile(VsRecorderWork *work) {
    if (VsRecorder_FoundVideoToggleProfile(work)) {
        VsRecorder_SetSubState(work, SEARCH_DETAILS_MAIN);
    }
    return FALSE;
}

// ends the screen (the battle is played by the launcher)
BOOL VsRecorder_VideoSearchPlay(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    if (VsRecorder_FoundVideoPlay(work)) {
        Heap_Free(search);
        return TRUE;
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchBackToResults(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    switch (work->step) {
    case 0:
        TouchHitboxController_Destroy(search->hitboxes);
        TouchHitboxController_Destroy(search->slotHitboxes);
        TouchHitboxController_Destroy(search->overwriteHitboxes);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        VsRecorder_FreeFoundVideoDetails(work);
        VsRecorder_SetVBlankPaused(work, TRUE);
        if (search->showProfile == 0) {
            VsRecorder_FreeVideoInfo(&search->info, work);
        } else {
            VsRecorder_FreeProfile(&search->profile, work);
        }
        VsRecorder_SetVBlankPaused(work, FALSE);
        VsRecorder_RemoveSearchDetailsButtonWindow(work);
        work->step++;
        break;
    case 1:
        VsRecorder_StepFade(&search->fade, 1);
        if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 0)) {
            VsRecorder_SetTextSpriteMsg(&search->buttons[0], (VsRecorderGfx *)&work->narc, 3);
            VsRecorder_SetTextSpriteMsg(&search->buttons[1], (VsRecorderGfx *)&work->narc, 0x6F);
            ManagedSprite_SetAnim(search->buttons[0].sprite, 0);
            ManagedSprite_SetAnim(search->buttons[1].sprite, 1);
            VsRecorder_SetSearchButtonsMode(work, TRUE);
            work->step++;
        }
        PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
        break;
    default:
        VsRecorder_SetSubState(work, SEARCH_SHOW_RESULTS);
        break;
    }
    return FALSE;
}

// a video already saved can't be saved again: say so and redraw the details; else open the save slots
BOOL VsRecorder_VideoSearchShowSlots(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    if (VsRecorder_IsVideoSaved(work, BattleRecordHeader_GetField(BattleRecord_GetHeader(), 4, 0))) {
        switch (work->step) {
        case 0:
            VsRecorder_SetVBlankPaused(work, TRUE);
            if (search->showProfile == 0) {
                VsRecorder_FreeVideoInfo(&search->info, work);
            } else {
                VsRecorder_FreeProfile(&search->profile, work);
            }
            VsRecorder_RemoveSearchDetailsButtonWindow(work);
            VsRecorder_SetVBlankPaused(work, FALSE);
            GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
            work->step++;
            break;
        case 1:
            VsRecorder_StepFade(&search->fade, 1);
            if (VsRecorder_StepBlend(&search->eva, &search->evb, 1, 0)) {
                VsRecorder_SetVBlankPaused(work, TRUE);
                VsRecorder_FreeSearchButtons(work);
                VsRecorder_CreateSearchSlotButtons(work);
                VsRecorder_SetVBlankPaused(work, FALSE);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_3);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_3);
                work->step++;
            }
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
            break;
        case 2:
            VsRecorder_ShowTopMessage(work, msg_0013_00277);
            work->step++;
            break;
        case 3:
            if (System_GetTouchNew()) {
                VsRecorder_FreeSearchSlotButtons(work);
                VsRecorder_HideTopMessage(work);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
                BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
                GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
                work->step++;
            }
            break;
        case 4:
            VsRecorder_SetVBlankPaused(work, TRUE);
            VsRecorder_CreateSearchButtons(work);
            if (work->selection == 210) {
                VsRecorder_SetTextSpriteMsg(&search->buttons[0], (VsRecorderGfx *)&work->narc, 3);
                VsRecorder_SetTextSpriteMsg(&search->buttons[1], (VsRecorderGfx *)&work->narc, 0x5E);
                ManagedSprite_SetAnim(search->buttons[0].sprite, 0);
                ManagedSprite_SetAnim(search->buttons[1].sprite, 3);
            }
            VsRecorder_SetVBlankPaused(work, FALSE);
            VsRecorder_CreateSearchDetailsButtonWindow(work);
            GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
            GfGfxLoader_LoadCharDataFromOpenNarc(work->narc, 0x3E, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
            VsRecorder_SetVBlankPaused(work, TRUE);
            if (search->showProfile == 0) {
                VsRecorder_CreateVideoInfo(&search->info, work);
                VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, FALSE);
                VsRecorder_LoadScreen(work, VsRecorderVideoInfo_GetScreenId(&search->info), GF_BG_LYR_MAIN_3);
            } else {
                VsRecorder_CreateProfile(&search->profile, work);
                VsRecorder_SetProfileDrawFlag(&search->profile, FALSE);
                GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 0x50, work->bgConfig, GF_BG_LYR_MAIN_3, 0, 0, FALSE, HEAP_ID_109);
            }
            VsRecorder_SetVBlankPaused(work, FALSE);
            GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, 6, work->bgConfig, GF_BG_LYR_SUB_3, 0, 0, FALSE, HEAP_ID_109);
            work->step++;
            break;
        case 5:
            VsRecorder_StepFade(&search->fade, 0);
            if (VsRecorder_StepBlend(&search->eva, &search->evb, 0, 0)) {
                if (search->showProfile == 0) {
                    VsRecorder_SetVideoPartyIconsDrawFlag(&search->info, TRUE);
                } else {
                    VsRecorder_SetProfileDrawFlag(&search->profile, TRUE);
                }
                GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
                GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
                work->step++;
            }
            PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xC, (u8)search->fade, (u16)work->blendColor);
            break;
        default: {
            int i;

            for (i = 0; i < work->net.videoList.count; i++) {
                work->net.videoList.headers[i] = &work->net.videoList.videos[i];
            }
            VsRecorder_SetSubState(work, SEARCH_DETAILS_MAIN);
            break;
        }
        }
    } else {
        if (VsRecorder_FoundVideoShowSlots(work)) {
            VsRecorder_SetSubState(work, SEARCH_SLOTS_MAIN);
        }
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchSlotsMain(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    TouchHitboxController_IsTriggered(search->slotHitboxes);
    return FALSE;
}

BOOL VsRecorder_VideoSearchSlotsCancel(VsRecorderWork *work) {
    int i;

    if (VsRecorder_FoundVideoSlotsCancel(work)) {
        for (i = 0; i < work->net.videoList.count; i++) {
            work->net.videoList.headers[i] = &work->net.videoList.videos[i];
        }
        VsRecorder_SetSubState(work, SEARCH_DETAILS_MAIN);
    }
    return FALSE;
}

// after saving
BOOL VsRecorder_VideoSearchReturnToDetails(VsRecorderWork *work) {
    int i;

    if (VsRecorder_FoundVideoReturnToDetails(work)) {
        for (i = 0; i < work->net.videoList.count; i++) {
            work->net.videoList.headers[i] = &work->net.videoList.videos[i];
        }
        VsRecorder_SetSubState(work, SEARCH_DETAILS_MAIN);
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchConfirmOverwrite(VsRecorderWork *work) {
    if (VsRecorder_FoundVideoConfirmOverwrite(work)) {
        VsRecorder_SetSubState(work, SEARCH_OVERWRITE_MAIN);
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchOverwriteMain(VsRecorderWork *work) {
    VsRecorderVideoSearch *search = work->screenWork;

    TouchHitboxController_IsTriggered(search->overwriteHitboxes);
    return FALSE;
}

BOOL VsRecorder_VideoSearchOverwriteNo(VsRecorderWork *work) {
    if (VsRecorder_FoundVideoOverwriteNo(work)) {
        VsRecorder_SetSubState(work, SEARCH_SLOTS_MAIN);
    }
    return FALSE;
}

BOOL VsRecorder_VideoSearchSave(VsRecorderWork *work) {
    VsRecorder_FoundVideoSave(work);
    return FALSE;
}
