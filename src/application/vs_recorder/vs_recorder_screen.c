#include "global.h"

#include "application/vs_recorder/vs_recorder_internal.h"

#include "assert.h"
#include "bg_window.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "msgdata.h"
#include "orbit_cursor.h"
#include "pm_string.h"
#include "pokemon_icon_idx.h"
#include "render_text.h"
#include "save_misc_data.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"

// VS Recorder screen helpers: the battle video info panel (owner, mode, number, party icons), touch indicator, labelled
// sprites, the message screen (18) and the "TOUCH HERE!" window.

typedef struct VsRecorderWindowRect {
    s16 x;
    s16 y;
    s16 width;
    s16 height;
} VsRecorderWindowRect;

typedef struct VsRecorderWindowRectSet {
    VsRecorderWindowRect rects[5];
} VsRecorderWindowRectSet;

typedef struct VsRecorderMsgIdSet {
    u32 msgIds[6];
} VsRecorderMsgIdSet;

typedef struct VsRecorderIconPos {
    s16 x;
    s16 y;
} VsRecorderIconPos;

typedef struct VsRecorderIconLayout {
    VsRecorderIconPos pos[2][12];
} VsRecorderIconLayout;

extern const VsRecorderWindowRectSet ov40_02244EE8; // video info label windows
extern const VsRecorderMsgIdSet ov40_02244E28;      // video info labels
extern const VsRecorderIconLayout ov40_02244F90;    // party icon positions: [0] two trainers, [1] four

#pragma force_active on

void VsRecorder_PrintVideoInfo(VsRecorderVideoInfo *info, VsRecorderWork *work) {
    VsRecorderWindowRectSet templates = ov40_02244EE8;
    VsRecorderMsgIdSet msgIds = ov40_02244E28;
    String *unitStr;
    String *numberStr;
    u32 value;
    String *bodyStr;
    String *nameStr;
    int i;
    int baseTile;
    String *digitsLo;
    String *digitsMid;
    String *digitsHi;
    u32 lo;
    u32 mid;
    u32 hi;
    const u32 *msgId = msgIds.msgIds;
    const VsRecorderWindowRect *tmpl;
    Window *window;
    MessageFormat *fmt;
    String *str;
    String *out;
    u64 total;
    u64 rest;
    const u16 *ownerName;
    int mode;
    baseTile = 1;
    for (i = 0, tmpl = templates.rects, window = info->windows; i < 5; i++) {
        if (tmpl->x == 0xFF) {
            break;
        }
        str = NewString_ReadMsgData(work->msgData, *msgId);
        InitWindow(window);
        AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, tmpl->x, tmpl->y, tmpl->width, tmpl->height, 0xE, baseTile);
        FillWindowPixelBuffer(window, 0);
        AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        ScheduleWindowCopyToVram(window);
        baseTile += tmpl->width * tmpl->height;
        String_Delete(str);
        msgId++;
        tmpl++;
        window++;
    }
    ownerName = info->ownerName;
    fmt = VsRecorder_NewMessageFormat(HEAP_ID_109);
    window = &info->windows[0];
    nameStr = FrontierCard_NewNameString(ownerName, HEAP_ID_109);
    VsRecorder_SanitizeName(work, nameStr);
    bodyStr = NewString_ReadMsgData(work->msgData, 7);
    out = String_New(0xFF, HEAP_ID_109);
    BufferString(fmt, 0, nameStr, 0, 1, 2);
    StringExpandPlaceholders(fmt, out, bodyStr);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, out, VsRecorder_GetCenteredTextX(window, out), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(nameStr);
    String_Delete(bodyStr);
    String_Delete(out);
    MessageFormat_ResetBuffers(fmt);
    mode = (int)BattleRecordHeader_GetField(info->header, 3, 0);
    window = &info->windows[1];
    str = NewString_ReadMsgData(work->msgData, mode + 0x84);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
    MessageFormat_ResetBuffers(fmt);
    value = (u32)BattleRecordHeader_GetField(info->header, 2, 0);
    if (value != 0) {
        window = &info->windows[2];
        numberStr = String_New(0xFF, HEAP_ID_109);
        unitStr = NewString_ReadMsgData(work->msgData, 9);
        out = String_New(0xFF, HEAP_ID_109);
        String16_FormatInteger(numberStr, value, 4, PRINTING_MODE_LEFT_ALIGN, TRUE);
        BufferString(fmt, 0, numberStr, 0, 1, 2);
        StringExpandPlaceholders(fmt, out, unitStr);
        FillWindowPixelBuffer(window, 0);
        AddTextPrinterParameterizedWithColor(window, 0, out, VsRecorder_GetCenteredTextX(window, out), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(numberStr);
        String_Delete(unitStr);
        String_Delete(out);
        MessageFormat_ResetBuffers(fmt);
    } else {
        window = &info->windows[3];
        FillWindowPixelBuffer(window, 0);
        ScheduleWindowCopyToVram(window);
    }
    // video number, printed as xx-xxxxx-xxxxx
    total = BattleRecordHeader_GetField(info->header, 4, 0);
    rest = total;
    digitsLo = String_New(0xFF, HEAP_ID_109);
    digitsMid = String_New(0xFF, HEAP_ID_109);
    digitsHi = String_New(0xFF, HEAP_ID_109);
    window = &info->windows[4];
    lo = (u32)(rest % 100000);
    rest /= 100000;
    mid = (u32)(rest % 100000);
    rest /= 100000;
    hi = (u32)rest;
    if (hi / 10 % 10 == 0 && total != 0) {
        str = NewString_ReadMsgData(work->msgData, 12);
    } else {
        str = NewString_ReadMsgData(work->msgData, 11);
    }
    out = String_New(0xFF, HEAP_ID_109);
    String16_FormatInteger(digitsLo, lo, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    String16_FormatInteger(digitsMid, mid, 5, PRINTING_MODE_LEADING_ZEROS, TRUE);
    String16_FormatInteger(digitsHi, hi, 2, PRINTING_MODE_LEADING_ZEROS, TRUE);
    BufferString(fmt, 2, digitsLo, 0, 1, 2);
    BufferString(fmt, 1, digitsMid, 0, 1, 2);
    BufferString(fmt, 0, digitsHi, 0, 1, 2);
    StringExpandPlaceholders(fmt, out, str);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, out, VsRecorder_GetCenteredTextX(window, out), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(digitsLo);
    String_Delete(digitsMid);
    String_Delete(digitsHi);
    String_Delete(str);
    String_Delete(out);
    MessageFormat_ResetBuffers(fmt);
    MessageFormat_Delete(fmt);
}

#pragma force_active reset

void VsRecorder_RemoveVideoInfoWindows(VsRecorderVideoInfo *info) {
    int i;
    Window *window = (Window *)info;
    i = 0;
    window = info->windows;
    do {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        i++;
        window++;
    } while (i < 5);
}

BOOL ov40_022303B8(void *header) {
    // called without a prototype in the original (int result)
    switch (((int (*)(void *, int, int))BattleRecordHeader_GetField)(header, 3, 0)) {
    case 0xE:
    case 0x11:
    case 0x14:
    case 0x17:
    case 0x1A:
    case 0x1D:
    case 0x20:
        return TRUE;
    default:
        return FALSE;
    }
}

BOOL ov40_02230404(VsRecorderVideoInfo *info) {
    return ov40_022303B8(info->header);
}

int ov40_02230410(VsRecorderVideoInfo *info) {
    return ov40_02230404(info) == TRUE ? 0x51 : 0x52;
}

#pragma force_active on

void VsRecorder_CreateVideoPartyIcons(VsRecorderVideoInfo *info, VsRecorderWork *work) {
    int mon;
    int count = 0;
    VsRecorderIconLayout layout = ov40_02244F90;
    ManagedSpriteTemplate template;
    int species[12];
    int forms[12];
    int sel;
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;
    PaletteData *plttData = work->plttData;
    int step;
    int start;
    int end;
    int fm;
    int sp;
    int n;
    int i;
    int form;
    SpriteSystem_LoadPaletteBuffer(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 100000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconCell64KNaix(), FALSE, 100000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconAnim64KNaix(), FALSE, 100000);
    step = 6;
    end = 6;
    n = 0;
    start = 0;
    sel = 0;
    if (ov40_02230404(info) == TRUE) {
        step = 3;
        end = 3;
        sel = 1;
    }
    do {
        for (i = start; i < end; i++) {
            species[n] = 0xFF;
            mon = BattleRecordHeader_GetField(info->header, 0, i);
            form = BattleRecordHeader_GetField(info->header, 1, i);
            if (mon != 0) {
                species[n] = mon;
                forms[n] = form;
                n++;
            }
        }
        for (i = n; i < end; i++) {
            species[n] = 0;
            forms[n] = 0;
            n++;
        }
        start += step;
        end += step;
    } while (end <= 12);
    for (i = 0; i < 12; i++) {
        info->monIcons[i] = NULL;
        if (species[i] != 0) {
            sp = species[i];
            fm = forms[i];
            SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconNaixEx(sp, FALSE, fm), FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, 100000 + count);
            template.x = layout.pos[sel][i].x + 8;
            template.y = layout.pos[sel][i].y - 12;
            template.z = 0;
            template.animation = 0;
            template.drawPriority = 0;
            template.pal = 0;
            template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
            template.bgPriority = 0;
            template.vramTransfer = 0;
            template.resIdList[0] = 100000 + count;
            template.resIdList[1] = 100000;
            template.resIdList[2] = 100000;
            template.resIdList[3] = 100000;
            template.resIdList[4] = -1;
            template.resIdList[5] = -1;
            info->monIcons[count] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
            ((void (*)(ManagedSprite *, int))ManagedSprite_SetPaletteOverrideOffset)(info->monIcons[count], GetMonIconPaletteEx(sp, fm, 0) + 4);
            ManagedSprite_SetAnim(info->monIcons[count], 1);
            ManagedSprite_TickFrame(info->monIcons[count]);
            ((void (*)(ManagedSprite *, int))ManagedSprite_SetDrawPriority)(info->monIcons[count], 12 - i);
            count++;
        }
    }
}

#pragma force_active reset

void VsRecorder_CreateVideoInfo(VsRecorderVideoInfo *info, VsRecorderWork *work) {
    VsRecorder_PrintVideoInfo(info, work);
    VsRecorder_CreateVideoPartyIcons(info, work);
}

#pragma force_active on

void VsRecorder_FreeVideoInfo(VsRecorderVideoInfo *info, VsRecorderWork *work) {
    int i;
    VsRecorderVideoInfo *icon;
    SpriteManager_UnloadPlttObjById(work->spriteManager, 100000);
    SpriteManager_UnloadCellObjById(work->spriteManager, 100000);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 100000);
    i = 0;
    icon = info; // monIcons stride over the info
    do {
        if (icon->monIcons[0]) {
            SpriteManager_UnloadCharObjById(work->spriteManager, i + 100000);
            Sprite_DeleteAndFreeResources(icon->monIcons[0]);
            icon->monIcons[0] = NULL;
        }
        i++;
        icon = (VsRecorderVideoInfo *)((u8 *)icon + sizeof(ManagedSprite *));
    } while (i < 12);
    // the original call passes the work as a second argument
    ((void (*)(VsRecorderVideoInfo *, VsRecorderWork *))VsRecorder_RemoveVideoInfoWindows)(info, work);
}

#pragma force_active reset

void VsRecorder_SetVideoPartyIconsDrawFlag(VsRecorderVideoInfo *info, BOOL draw) {
    int i;
    for (i = 0; i < 12; i++, info = (VsRecorderVideoInfo *)((u8 *)info + sizeof(ManagedSprite *))) {
        ManagedSprite *sprite = info->monIcons[0];
        if (sprite != NULL) {
            ManagedSprite_SetDrawFlag(sprite, draw);
        }
    }
}

#pragma force_active on

int VsRecorder_GetCenteredTextX(Window *window, String *str) {
    int width = FontID_String_GetWidth(0, str, 0);
    return (GetWindowWidth(window) * 8 - width) / 2;
}

#pragma force_active reset

void VsRecorder_ResetRecordSave(VsRecorderWork *work) {
    work->recordWriteStep = 0;
    work->recordWriteState = 0;
}

BOOL VsRecorder_ClearSavedVideoKeys(VsRecorderWork *work) {
    u32 a;
    u32 b;
    u8 c;
    SAVE_MISC_DATA *misc = Save_Misc_Get(work->saveData);
    SaveMisc_GetExtraChunkKeys(misc, work->selection + 2, &a, &b, &c);
    SaveMisc_SetExtraChunkKeys(misc, work->selection + 2, -1, -1, c);
    return TRUE;
}

void VsRecorder_ResetBgPriorities(void) {
    SetBgPriority(GF_BG_LYR_MAIN_0, 1);
    SetBgPriority(GF_BG_LYR_MAIN_1, 3);
    SetBgPriority(GF_BG_LYR_MAIN_2, 0);
    SetBgPriority(GF_BG_LYR_MAIN_3, 1);
    SetBgPriority(GF_BG_LYR_SUB_0, 1);
    SetBgPriority(GF_BG_LYR_SUB_1, 3);
    SetBgPriority(GF_BG_LYR_SUB_2, 0);
    SetBgPriority(GF_BG_LYR_SUB_3, 1);
}

void VsRecorder_PlaceOrbitCursor(VsRecorderWork *work, OrbitCursor *cursor, s16 x, s16 y) {
    if (work->cursorHidden == 0) {
        OrbitCursor_SetVisible(cursor, TRUE);
    }
    OrbitCursor_SetActive(cursor, TRUE);
    OrbitCursor_SetPosition(cursor, x, y);
    OrbitCursor_SetTarget(cursor, x, y);
}

int VsRecorder_CountDigits(u64 value) {
    int digits = 1;
    while (value > 1) {
        value /= 10;
        digits++;
    }
    return digits;
}

void VsRecorder_LoadScreen(VsRecorderWork *work, int fileId, GFBgLayer layer) {
    GfGfxLoader_LoadScrnDataFromOpenNarc(work->narc, fileId, work->bgConfig, layer, 0, 0, FALSE, HEAP_ID_109);
}

// expanding ring at the touched point, 7 frames
void VsRecorder_TouchIndicatorTask(SysTask *task, void *data) {
    VsRecorderWork *work = data;
    s16 radius;
    work->touchIndicator.counter++;
    if (work->touchIndicator.counter >= 7) {
        SysTask_Destroy(task);
        work->touchIndicator.counter = 0;
        work->touchIndicator.active = FALSE;
        sub_02087A54(work->touchIndicator.cursor);
        OrbitCursor_SetRadius(work->touchIndicator.cursor, 0, 0);
        OrbitCursor_SetActive(work->touchIndicator.cursor, FALSE);
        OrbitCursor_SetVisible(work->touchIndicator.cursor, FALSE);
    } else {
        radius = (s16)(work->touchIndicator.counter * 2 + 6);
        OrbitCursor_SetRadius(work->touchIndicator.cursor, radius, radius);
    }
}

void VsRecorder_ShowTouchIndicator(VsRecorderWork *work) {
    u32 x;
    u32 y;
    if (work->touchIndicator.active == TRUE) {
        if (work->touchIndicator.task != NULL) {
            SysTask_Destroy(work->touchIndicator.task);
        }
        work->touchIndicator.task = NULL;
    }
    if (gSystem.screensFlipped == 0) {
        work->touchIndicator.cursor = work->orbitCursors[1];
    } else {
        work->touchIndicator.cursor = work->orbitCursors[0];
    }
    work->touchIndicator.active = TRUE;
    work->touchIndicator.counter = 0;
    System_GetTouchNewCoords(&x, &y);
    sub_02087A30(work->touchIndicator.cursor);
    OrbitCursor_SetActive(work->touchIndicator.cursor, TRUE);
    OrbitCursor_SetVisible(work->touchIndicator.cursor, TRUE);
    OrbitCursor_SetTarget(work->touchIndicator.cursor, (s16)x, (s16)y);
    OrbitCursor_SetPosition(work->touchIndicator.cursor, (s16)x, (s16)y);
    work->touchIndicator.task = SysTask_CreateOnMainQueue(VsRecorder_TouchIndicatorTask, work, 0x1000);
    if (work->touchIndicator.task != NULL) {
        VsRecorder_TouchIndicatorTask(work->touchIndicator.task, work);
    } else {
        work->touchIndicator.active = FALSE;
        OrbitCursor_SetVisible(work->touchIndicator.cursor, FALSE);
        GF_AssertFail();
    }
}

void VsRecorder_OnTouchConfirm(VsRecorderWork *work) {
    VsRecorder_ShowTouchIndicator(work);
    PlaySE(SEQ_SE_PL_BREC57);
}

u32 VsRecorder_IsVBlankPaused(VsRecorderWork *work) {
    return work->vblankPaused;
}

void VsRecorder_SetVBlankPaused(VsRecorderWork *work, u32 paused) {
    work->vblankPaused = paused;
}

#pragma force_active on

void VsRecorder_CreateLabeledSprite(VsRecorderWork *work, VsRecorderTextSprite *textSprite, NNS_G2D_VRAM_TYPE vram, int msgId, s16 x, s16 y, s16 textX, s16 textY, int anim, BOOL draw) {
    textSprite->sprite = VsRecorder_NewButtonSprite(work, vram);
    VsRecorder_CreateTextSprite(textSprite, (VsRecorderGfx *)&work->narc, vram);
    VsRecorder_SetTextSpriteMsg(textSprite, (VsRecorderGfx *)&work->narc, msgId);
    ManagedSprite_SetPositionXY(textSprite->sprite, x, y);
    TextOBJ_SetPosition(textSprite->text, textX, textY);
    TextOBJ_SetSpritesDrawFlag(textSprite->text, draw);
    ManagedSprite_SetAnim(textSprite->sprite, anim);
}

#pragma force_active reset

void VsRecorder_FreeLabeledSprite(VsRecorderTextSprite *textSprite) {
    VsRecorder_FreeTextSprite(textSprite);
    Sprite_DeleteAndFreeResources(textSprite->sprite);
}

#pragma force_active on

// message of the message screen: type 0 / 1 by index, type 2 by group and index
u32 VsRecorder_SetupMessageScreenMsg(VsRecorderWork *work, u32 group, u32 index, u32 type) {
    u32 result;
    work->messageScreen.savedSubState = work->subState;
    work->messageScreen.savedStep = work->step;
    work->messageScreen.returnScreen = *work->screenPtr;
    work->messageScreen.active = TRUE;
    result = 194;
    switch (type) {
    case 0:
        switch (index) {
        case 0:
            result = 244;
            break;
        case 1:
            result = 245;
            break;
        case 2:
            result = 246;
            break;
        case 3:
            result = 247;
            break;
        case 4:
            result = 248;
            break;
        case 5:
            result = 249;
            break;
        case 6:
            result = 250;
            break;
        case 7:
            result = 251;
            break;
        case 8:
            result = 252;
            break;
        case 9:
            result = 253;
            break;
        case 10:
            result = 254;
            break;
        case 11:
            result = 255;
            break;
        case 12:
            result = 256;
            break;
        case 13:
            result = 257;
            break;
        case 14:
            result = 258;
            break;
        default:
            GF_AssertFail();
            break;
        }
        break;
    case 1:
        switch (index) {
        case 0:
            result = 259;
            break;
        case 1:
            result = 260;
            break;
        case 2:
            result = 261;
            break;
        case 3:
            result = 262;
            break;
        case 4:
            result = 263;
            break;
        case 5:
            result = 264;
            break;
        case 6:
            result = 265;
            break;
        case 7:
            result = 266;
            break;
        case 8:
            result = 267;
            break;
        case 9:
            result = 268;
            break;
        default:
            GF_AssertFail();
            break;
        }
        break;
    case 2:
        switch (group) {
        case 0:
            switch (index) {
            case 0:
                result = 194;
                break;
            case 1:
                result = 195;
                break;
            case 2:
                result = 196;
                break;
            case 3:
                result = 197;
                break;
            case 4:
                result = 198;
                break;
            case 5:
                result = 199;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case 1:
            switch (index) {
            case 0:
                result = 200;
                break;
            case 1:
                result = 201;
                break;
            case 2:
                result = 202;
                break;
            case 3:
                result = 203;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case 2:
            switch (index) {
            case 0:
                result = 204;
                break;
            case 1:
                result = 205;
                break;
            case 2:
                result = 206;
                break;
            case 3:
                result = 207;
                break;
            case 4:
                result = 208;
                break;
            case 5:
                result = 209;
                break;
            case 6:
                result = 210;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case 3:
            switch (index) {
            case 0:
                result = 211;
                break;
            case 1:
                result = 212;
                break;
            case 2:
                result = 213;
                break;
            case 3:
                result = 214;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case 4:
            switch (index) {
            case 0:
                result = 215;
                break;
            case 1:
                result = 216;
                break;
            case 2:
                result = 217;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case 5:
            switch (index) {
            case 0:
                result = 218;
                break;
            case 1:
                result = 219;
                break;
            case 2:
                result = 220;
                break;
            case 3:
                result = 221;
                break;
            case 4:
                result = 222;
                break;
            case 5:
                result = 223;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case 6:
            switch (index) {
            case 0:
                result = 224;
                break;
            case 1:
                result = 225;
                break;
            case 2:
                result = 226;
                break;
            case 3:
                result = 227;
                break;
            case 4:
                result = 228;
                break;
            case 5:
                result = 229;
                break;
            case 6:
                result = 230;
                break;
            case 7:
                result = 231;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case 7:
            switch (index) {
            case 0:
                result = 232;
                break;
            case 1:
                result = 233;
                break;
            case 2:
                result = 234;
                break;
            case 3:
                result = 235;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case 8:
            switch (index) {
            case 0:
                result = 236;
                break;
            case 1:
                result = 237;
                break;
            case 2:
                result = 238;
                break;
            case 3:
                result = 239;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        default:
            GF_AssertFail();
            break;
        }
        break;
    }
    work->messageScreen.msgId = result;
    return work->messageScreen.msgId;
}

#pragma force_active reset

// switch to the message screen; it returns to the current screen after a touch
void VsRecorder_ShowMessageScreen(VsRecorderWork *work, int group, u32 index, u32 type) {
    work->messageScreen.savedSubState = work->subState;
    work->messageScreen.savedStep = work->step;
    work->messageScreen.returnScreen = *work->screenPtr;
    work->messageScreen.step = 0;
    work->messageScreen.timer = 0;
    work->messageScreen.msgId = VsRecorder_SetupMessageScreenMsg(work, group, index, type);
    work->messageScreen.active = TRUE;
    *work->screenPtr = VS_RECORDER_SCREEN_MESSAGE;
}

void VsRecorder_MessageScreen(VsRecorderWork *work) {
    switch (work->messageScreen.step) {
    case 0:
        if (work->messageScreen.timer < 8) {
            work->messageScreen.timer = work->messageScreen.timer + 1;
        } else {
            VsRecorder_ShowTopMessage(work, work->messageScreen.msgId);
            work->messageScreen.step = work->messageScreen.step + 1;
        }
        break;
    case 1:
        if (System_GetTouchNew()) {
            VsRecorder_HideTopMessage(work);
            work->messageScreen.step = work->messageScreen.step + 1;
        }
        break;
    default:
        if (work->messageScreen.timer > 0) {
            work->messageScreen.timer = work->messageScreen.timer - 1;
        } else {
            *work->screenPtr = work->messageScreen.returnScreen;
        }
        break;
    }
}

// TRUE if one of the saved videos 1-3 has this video number
BOOL VsRecorder_IsVideoSaved(VsRecorderWork *work, u64 number) {
    int i = 1;
    u8 *slot;
    slot = (u8 *)work + sizeof(void *); // savedVideos stride over the work
    do {
        void *header = ((VsRecorderWork *)slot)->savedVideos[0];
        if (header != NULL && BattleRecordHeader_GetField(header, 4, 0) == number) {
            return TRUE;
        }
        i++;
        slot += sizeof(void *);
    } while (i < 4);
    return FALSE;
}

// replace a name the font cannot show by the placeholder name
void VsRecorder_SanitizeName(VsRecorderWork *work, String *str) {
    GlobalTerminalNet *net = VsRecorder_GetNet(work);
    String *tmp = String_New(0x40, net->heapID);
    if (!FontID_String_AllCharsValid(0, str, tmp)) {
        VsRecorder_SetPlaceholderName(str, net->heapID);
    }
    String_Delete(tmp);
}

void VsRecorder_SetPlaceholderName(String *str, enum HeapID heapID) {
    MsgData *msgData;
    String_SetEmpty(str);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0xD, heapID);
    ReadMsgDataIntoString(msgData, 0x14C, str);
    DestroyMsgData(msgData);
}

void VsRecorder_ShowTouchHere(VsRecorderWork *work) {
    Window *window = &work->messageWindow;
    String *str;
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, 8, 0x10, 0x10, 2, 0xE, 0x12C);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, 0xC0);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 14, 0), NULL);
    String_Delete(str);
    ScheduleWindowCopyToVram(window);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
}

void VsRecorder_HideTouchHere(VsRecorderWork *work) {
    ClearWindowTilemapAndCopyToVram(&work->messageWindow);
    RemoveWindow(&work->messageWindow);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}
