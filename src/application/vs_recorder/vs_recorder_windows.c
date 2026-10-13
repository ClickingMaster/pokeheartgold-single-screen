#include <nitro/gx/g2.h>

#include "global.h"

#include "application/vs_recorder/vs_recorder_internal.h"

#include "assert.h"
#include "bg_window.h"
#include "gf_gfx_planes.h"
#include "mail_message.h"
#include "message_format.h"
#include "msgdata.h"
#include "pm_string.h"
#include "pokemon_icon_idx.h"
#include "render_text.h"
#include "sprite_system.h"
#include "text.h"

// VS Recorder message windows (a notice box on the touch screen, a message on the top screen) and the trainer profile
// panel (frontier card: labels, name, birthday month, country / region, greeting, Pokemon icon, trainer sprite).

typedef struct VsRecorderWindowCoords {
    s16 x;
    s16 y;
    s16 width;
    s16 height;
} VsRecorderWindowCoords;

typedef struct VsRecorderWindowCoordsTable {
    VsRecorderWindowCoords coords[10];
} VsRecorderWindowCoordsTable;

typedef struct VsRecorderMsgIdTable {
    u32 ids[12];
} VsRecorderMsgIdTable;

typedef struct VsRecorderMailBuf {
    u32 v[2];
} VsRecorderMailBuf;

extern const VsRecorderWindowCoordsTable ov40_02244F40; // profile label windows
extern const VsRecorderMsgIdTable ov40_02244F10;        // profile label messages
extern const u32 ov40_02244FF0[][4];                    // trainer avatar files

// touch screen notice (BG 6 window)
void VsRecorder_ShowNotice(VsRecorderWork *work, int msgId) {
    Window *window = &work->messageWindow;
    String *str;

    if (work->messageShown != TRUE) {
        work->messageShown = TRUE;
        ov40_0222C6C8(work, GF_BG_LYR_SUB_2, FALSE);
        reg_G2S_DB_BLDCNT = 0;
        SetBgPriority(GF_BG_LYR_SUB_2, 0);
        InitWindow(window);
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 1, 0x13, 0x1E, 4, 0xE, 0x20);
        str = NewString_ReadMsgData(work->msgData, msgId);
        FillWindowPixelBuffer(window, 0xCC);
        AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 12), NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(str);
        OS_WaitVBlankIntr();
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
    }
}

void VsRecorder_HideNotice(VsRecorderWork *work) {
    if (work->messageShown != FALSE) {
        work->messageShown = FALSE;
        ClearWindowTilemapAndCopyToVram(&work->messageWindow);
        RemoveWindow(&work->messageWindow);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_SUB_2);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
        SetBgPriority(GF_BG_LYR_SUB_2, 2);
        G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, 7, 8);
        G2x_SetBlendAlpha_((u32)&reg_G2S_DB_BLDCNT, GX_BLEND_PLANEMASK_BG2, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, 7, 8);
    }
}

void VsRecorder_RemoveTopMessage(VsRecorderWork *work) {
    ClearWindowTilemapAndCopyToVram(&work->messageWindow);
    RemoveWindow(&work->messageWindow);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

// top screen message (BG 2 window)
void VsRecorder_ShowTopMessage(VsRecorderWork *work, int msgId) {
    Window *window = &work->messageWindow;
    String *str;

    if (work->messageShown != TRUE) {
        work->messageShown = TRUE;
        ov40_0222C6C8(work, GF_BG_LYR_MAIN_2, FALSE);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        InitWindow(window);
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, 1, 0x13, 0x1E, 4, 0xE, 0x20);
        str = NewString_ReadMsgData(work->msgData, msgId);
        FillWindowPixelBuffer(window, 0xCC);
        AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 12), NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(str);
    }
}

void VsRecorder_SetMessageText(VsRecorderWork *work, int msgId) {
    Window *window = &work->messageWindow;
    String *str;

    if (work->messageShown != FALSE) {
        str = NewString_ReadMsgData(work->msgData, msgId);
        FillWindowPixelBuffer(window, 0xCC);
        AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 12), NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(str);
    }
}

void VsRecorder_HideTopMessage(VsRecorderWork *work) {
    if (work->messageShown != FALSE) {
        work->messageShown = FALSE;
        ClearWindowTilemapAndCopyToVram(&work->messageWindow);
        RemoveWindow(&work->messageWindow);
        BgClearTilemapBufferAndCommit(work->bgConfig, GF_BG_LYR_MAIN_2);
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
    }
}

// touch screen notice raised from the menu; VsRecorder_OnMenuTouch hides it on the next touch
void VsRecorder_ShowMenuNotice(VsRecorderWork *work, int msgId) {
    Window *window = &work->messageWindow;
    String *str;

    if (work->messageShown != TRUE && work->menuNoticeShown != TRUE) {
        work->messageShown = TRUE;
        work->menuNoticeShown = TRUE;
        reg_G2S_DB_BLDCNT = 0;
        SetBgPriority(GF_BG_LYR_SUB_2, 0);
        ov40_0222C6C8(work, GF_BG_LYR_SUB_2, FALSE);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        InitWindow(window);
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_SUB_2, 1, 0x13, 0x1E, 4, 0xE, 0x20);
        str = NewString_ReadMsgData(work->msgData, msgId);
        FillWindowPixelBuffer(window, 0xCC);
        AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 12), NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(str);
    }
}

void VsRecorder_PrintProfile(VsRecorderProfile *profile, VsRecorderWork *work) {
    VsRecorderWindowCoordsTable coords = ov40_02244F40;
    VsRecorderMsgIdTable msgIds = ov40_02244F10;
    VsRecorderMailBuf mail;
    String *cityTemplate;
    String *countryTemplate;
    String *countryStr;
    u32 city;
    String *dateStr;
    String *monthTemplate;
    u32 month;
    String *nameTemplate;
    String *nameStr;
    int i;
    int baseTile;
    FrontierCard *card;
    const u32 *idp;
    Window *window2;
    Window *window;
    const VsRecorderWindowCoords *cp;
    MessageFormat *fmt;
    String *str;
    u32 country;

    baseTile = 1;
    i = 0;
    window = profile->windows;
    idp = msgIds.ids;
    cp = coords.coords;
    for (; i < 8; i++) {
        str = NewString_ReadMsgData(work->msgData, *idp);
        InitWindow(window);
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, cp->x, cp->y, cp->width, cp->height, 0x0E, baseTile);
        FillWindowPixelBuffer(window, 0);
        AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        ScheduleWindowCopyToVram(window);
        baseTile += cp->width * cp->height;
        String_Delete(str);
        window++;
        idp++;
        cp++;
    }

    card = profile->card;
    fmt = VsRecorder_NewMessageFormat(HEAP_ID_109);
    window = &profile->windows[0];
    nameStr = FrontierCard_NewNameString((const u16 *)card, HEAP_ID_109);
    VsRecorder_SanitizeName(work, nameStr);
    nameTemplate = NewString_ReadMsgData(work->msgData, 13);
    str = String_New(0xFF, HEAP_ID_109);
    BufferString(fmt, 0, nameStr, 0, 1, 2);
    StringExpandPlaceholders(fmt, str, nameTemplate);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(nameStr);
    String_Delete(nameTemplate);
    String_Delete(str);
    MessageFormat_ResetBuffers(fmt);

    month = FrontierCard_GetBirthdayMonth(card);
    window = &profile->windows[2];
    monthTemplate = NewString_ReadMsgData(work->msgData, 15);
    dateStr = FrontierCard_NewNameString((const u16 *)card, HEAP_ID_109);
    VsRecorder_SanitizeName(work, dateStr);
    str = String_New(0xFF, HEAP_ID_109);
    BufferMonthNameAbbr(fmt, 0, month);
    StringExpandPlaceholders(fmt, str, monthTemplate);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(monthTemplate);
    String_Delete(dateStr);
    String_Delete(str);
    MessageFormat_ResetBuffers(fmt);

    window = &profile->windows[3];
    str = NewString_ReadMsgData(work->msgData, 16);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);

    country = FrontierCard_GetCountry(card);
    city = FrontierCard_GetRegion(card);
    window = &profile->windows[4];
    FillWindowPixelBuffer(window, 0);
    if (country == 0) {
        str = NewString_ReadMsgData(work->msgData, 21);
        AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(str);
    } else {
        countryStr = String_New(0xFF, HEAP_ID_109);
        countryTemplate = NewString_ReadMsgData(work->msgData, 22);
        BufferCountryName(fmt, 0, country);
        StringExpandPlaceholders(fmt, countryStr, countryTemplate);
        AddTextPrinterParameterizedWithColor(window, 0, countryStr, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(countryStr);
        String_Delete(countryTemplate);
        if (city != 0) {
            window2 = &profile->windows[5];
            FillWindowPixelBuffer(window2, 0);
            str = String_New(0xFF, HEAP_ID_109);
            cityTemplate = NewString_ReadMsgData(work->msgData, 23);
            BufferCityName(fmt, 0, country, city);
            StringExpandPlaceholders(fmt, str, cityTemplate);
            AddTextPrinterParameterizedWithColor(window2, 0, str, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            ScheduleWindowCopyToVram(window2);
            String_Delete(str);
            String_Delete(cityTemplate);
        }
    }

    MessageFormat_ResetBuffers(fmt);
    window = &profile->windows[6];
    str = NewString_ReadMsgData(work->msgData, 17);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);

    window = &profile->windows[7];
    str = FrontierCard_GetGreeting(card, (MailMessage *)&mail, HEAP_ID_109);
    if (str == NULL) {
        str = MailMsg_GetExpandedString((MailMessage *)&mail, HEAP_ID_109);
    }
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
    MessageFormat_Delete(fmt);
}

void VsRecorder_RemoveProfileWindows(VsRecorderProfile *profile) {
    int i;
    Window *window;

    i = 0;
    window = profile->windows;
    for (; i < 8; i++) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        window++;
    }
}

void VsRecorder_SetProfileMonIconDrawFlag(VsRecorderProfile *profile, BOOL draw) {
    ManagedSprite *sprite = profile->monIcon;
    if (sprite != NULL) {
        ManagedSprite_SetDrawFlag(sprite, draw);
    }
}

#pragma force_active on

void VsRecorder_FreeProfileMonIcon(VsRecorderProfile *profile, VsRecorderWork *work) {
    SpriteManager_UnloadCharObjById(work->spriteManager, 100000);
    SpriteManager_UnloadPlttObjById(work->spriteManager, 100000);
    SpriteManager_UnloadCellObjById(work->spriteManager, 100000);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 100000);
    if (profile->monIcon) {
        Sprite_DeleteAndFreeResources(profile->monIcon);
        profile->monIcon = NULL;
    }
}

// icon of the card's Pokemon (top screen)
void VsRecorder_CreateProfileMonIcon(VsRecorderProfile *profile, VsRecorderWork *work) {
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;
    PaletteData *plttData = work->plttData;
    int slot, species, form, isEgg;
    ManagedSpriteTemplate template;

    slot = SpriteSystem_LoadPaletteBuffer(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 100000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconCell64KNaix(), FALSE, 100000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconAnim64KNaix(), FALSE, 100000);
    species = FrontierCard_GetSpecies(profile->card);
    form = FrontierCard_GetSanitizedForm(profile->card);
    isEgg = FrontierCard_IsEgg(profile->card);
    if (species) {
        SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconNaixEx(species, isEgg, form), FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 100000);
        template.x = 76;
        template.y = 60;
        template.z = 0;
        template.animation = 0;
        template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
        template.resIdList[0] = 100000;
        template.resIdList[1] = 100000;
        template.resIdList[2] = 100000;
        template.resIdList[3] = 100000;
        template.resIdList[4] = -1;
        template.resIdList[5] = -1;
        template.drawPriority = 0;
        template.pal = 0;
        template.bgPriority = 0;
        template.vramTransfer = 0;
        profile->monIcon = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
        // called through an int prototype, as compiled (no u8 truncation of the offset)
        ((void (*)(ManagedSprite *, int))ManagedSprite_SetPaletteOverrideOffset)(profile->monIcon, slot + GetMonIconPaletteEx(species, form, isEgg));
        ManagedSprite_SetAnim(profile->monIcon, 1);
    }
}

#pragma force_active reset

void VsRecorder_SetProfileAvatarDrawFlag(VsRecorderProfile *profile, BOOL draw) {
    ManagedSprite_SetDrawFlag(profile->avatar, draw);
}

void VsRecorder_FreeProfileAvatar(VsRecorderProfile *profile, VsRecorderWork *work) {
    SpriteManager_UnloadCharObjById(work->spriteManager, 165535);
    SpriteManager_UnloadPlttObjById(work->spriteManager, 165535);
    SpriteManager_UnloadCellObjById(work->spriteManager, 165535);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 165535);
    Sprite_DeleteAndFreeResources(profile->avatar);
}

// NARC_a_1_7_9 file of a trainer avatar: kind 2 palette, 3 char, 4 (value + 0x13C)
u32 VsRecorder_GetAvatarFileId(int avatar, int kind) {
    if (avatar > 16 || avatar < 0) {
        GF_AssertFail();
        avatar = 0;
    }
    if (kind == 4) {
        return avatar + 0x13C;
    }
    if (kind >= 5) {
        return 0;
    }
    return ov40_02244FF0[avatar][kind];
}

// trainer sprite of the card (top screen): the player's own gender sprite unless a video is selected
void VsRecorder_CreateProfileAvatar(VsRecorderProfile *profile, VsRecorderWork *work) {
    ManagedSpriteTemplate template;
    PaletteData *plttData;
    int charFile = 14;
    int palFile = 13;
    int gender = FrontierCard_GetGender(profile->card);
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;
    plttData = work->plttData;
    if (work->selection == 0) {
        if (gender == 1) {
            palFile = 15;
            charFile = 16;
        }
    } else {
        int avatar = FrontierCard_GetAvatar(profile->card);
        if (avatar >= 16) {
            avatar = 0;
        }
        palFile = VsRecorder_GetAvatarFileId(avatar, 2);
        charFile = VsRecorder_GetAvatarFileId(avatar, 3);
    }
    SpriteSystem_LoadPaletteBuffer(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_a_1_7_9, palFile, FALSE, 1, NNS_G2D_VRAM_TYPE_2DMAIN, 165535);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_a_1_7_9, 9, FALSE, 165535);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_a_1_7_9, 10, FALSE, 165535);
    SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(spriteSystem, spriteManager, NARC_a_1_7_9, charFile, FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 165535);
    template.x = 40;
    template.y = 60;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 0;
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.resIdList[0] = 165535;
    template.resIdList[1] = 165535;
    template.resIdList[2] = 165535;
    template.resIdList[3] = 165535;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    template.bgPriority = 0;
    template.vramTransfer = 0;
    profile->avatar = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
    ManagedSprite_SetAnim(profile->avatar, 1);
    ManagedSprite_TickFrame(profile->avatar);
}

void VsRecorder_CreateProfile(VsRecorderProfile *profile, VsRecorderWork *work) {
    VsRecorder_PrintProfile(profile, work);
    VsRecorder_CreateProfileAvatar(profile, work);
    VsRecorder_CreateProfileMonIcon(profile, work);
}

#pragma force_active on

void VsRecorder_FreeProfile(VsRecorderProfile *profile, VsRecorderWork *work) {
    VsRecorder_RemoveProfileWindows(profile);
    VsRecorder_SetProfileDrawFlag(profile, FALSE);
    VsRecorder_FreeProfileAvatar(profile, work);
    VsRecorder_FreeProfileMonIcon(profile, work);
}

#pragma force_active reset

void VsRecorder_SetProfileDrawFlag(VsRecorderProfile *profile, BOOL draw) {
    VsRecorder_SetProfileMonIconDrawFlag(profile, draw);
    VsRecorder_SetProfileAvatarDrawFlag(profile, draw);
}
