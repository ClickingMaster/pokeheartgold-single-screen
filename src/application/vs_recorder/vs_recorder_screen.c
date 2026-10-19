#include <nitro/gx/g2.h>

#include "global.h"

#include "constants/species.h"

#include "application/vs_recorder/vs_recorder_internal.h"
#include "global_terminal/global_terminal_net.h"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0013.h"

#include "assert.h"
#include "battle_recording.h"
#include "bg_window.h"
#include "font.h"
#include "font_sprite.h"
#include "game_stats.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "mail_message.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "orbit_cursor.h"
#include "palette.h"
#include "pm_string.h"
#include "pokemon_icon_idx.h"
#include "render_text.h"
#include "save_misc_data.h"
#include "sound_playback.h"
#include "sprite_system.h"
#include "string.h"
#include "sys_task_api.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "wireless_status_icon.h"

// VS Recorder screen helpers: menu row labels, fades, message and list windows, the trainer profile and battle video
// info panels, the touch indicator and the message screens.

typedef struct VsRecorderSchemeTable {
    u32 values[7];
} VsRecorderSchemeTable;

typedef struct VsRecorderWindowRect {
    s16 x;
    s16 y;
    s16 width;
    s16 height;
} VsRecorderWindowRect;

typedef struct VsRecorderWindowCoordsTable {
    VsRecorderWindowRect coords[10];
} VsRecorderWindowCoordsTable;

typedef struct VsRecorderMsgIdTable {
    u32 ids[12];
} VsRecorderMsgIdTable;

typedef struct VsRecorderMailBuf {
    u32 v[2];
} VsRecorderMailBuf;

static void VsRecorder_PrintProfile(VsRecorderProfile *profile, VsRecorderWork *work);
static void VsRecorder_RemoveProfileWindows(VsRecorderProfile *profile);
static void VsRecorder_SetProfileMonIconDrawFlag(VsRecorderProfile *profile, BOOL draw);
static void VsRecorder_FreeProfileMonIcon(VsRecorderProfile *profile, VsRecorderWork *work);
static void VsRecorder_CreateProfileMonIcon(VsRecorderProfile *profile, VsRecorderWork *work);
static void VsRecorder_SetProfileAvatarDrawFlag(VsRecorderProfile *profile, BOOL draw);
static void VsRecorder_FreeProfileAvatar(VsRecorderProfile *profile, VsRecorderWork *work);
static void VsRecorder_CreateProfileAvatar(VsRecorderProfile *profile, VsRecorderWork *work);

typedef struct VsRecorderRankLimits {
    int v[3];
} VsRecorderRankLimits;

#define GetSpeciesNameIntoArrayInt ((void (*)(int, enum HeapID, u16 *))GetSpeciesNameIntoArray)

static void VsRecorder_PrintListLocation(VsRecorderListWindow *list, VsRecorderWork *work);

static ManagedSprite *VsRecorder_NewScrollSprite(void *unused, VsRecorderWork *work, NNS_G2D_VRAM_TYPE vram, int anim);
static void VsRecorder_DeleteTouchScrollSprites(VsRecorderTouchScroll *scroll);
static void VsRecorder_TouchScrollTrack(VsRecorderTouchScroll *scroll, int y);
static void VsRecorder_FlipScreensTask(SysTask *task, void *data);
static void VsRecorder_VBlankTask(SysTask *task, void *data);

typedef struct VsRecorderWindowRectSet {
    VsRecorderWindowRect rects[5];
} VsRecorderWindowRectSet;

typedef struct VsRecorderMsgIdSet {
    u32 msgIds[6];
} VsRecorderMsgIdSet;

typedef struct VsRecorderIconLayout {
    VsRecorderIconPos pos[2][12];
} VsRecorderIconLayout;

static void VsRecorder_PrintVideoInfo(VsRecorderVideoInfo *info, VsRecorderWork *work);
static void VsRecorder_RemoveVideoInfoWindows(VsRecorderVideoInfo *info, VsRecorderWork *work);
static BOOL VsRecorderVideoInfo_UsesThreeMonLayout(VsRecorderVideoInfo *info);
static void VsRecorder_CreateVideoPartyIcons(VsRecorderVideoInfo *info, VsRecorderWork *work);
static void VsRecorder_TouchIndicatorTask(SysTask *task, void *data);
static void VsRecorder_ShowTouchIndicator(VsRecorderWork *work);
static void VsRecorder_SetPlaceholderName(String *str, enum HeapID heapID);

static const TouchscreenHitbox sVsRecorderTouchScrollRightHitbox[1] = {
    { 8, 128, 224, 248 }
};

static const TouchscreenHitbox sVsRecorderTouchScrollLeftHitbox[1] = {
    { 8, 128, 8, 32 }
};

static const VsRecorderRankLimits sVsRecorderRankLimitsRedraw = {
    .v = { 16, 12, 20 },
};

static const VsRecorderRankLimits sVsRecorderRankLimits = {
    .v = { 16, 12, 20 },
};

static const VsRecorderMsgIdSet sVsRecorderVideoInfoLabelMsgIds = {
    .msgIds = { 20, 20, 20, 10, 20, 20 },
};

// per colour scheme (VsRecorderWork.colorScheme)
static const u32 sVsRecorderSchemeBgFramePaletteFiles[7] = { 102, 103, 104, 105, 106, 129, 134 };

static const u32 sVsRecorderSchemeBgPaletteFiles[7] = { 112, 113, 114, 115, 116, 131, 136 };

static const u32 sVsRecorderSchemeMenuBlendColors[7] = { 0x1642, 0x357F, 0x3DEF, 0x31F, 188, 0x131, 0x23F };

static const u32 sVsRecorderSchemeObjPaletteFiles[7] = { 117, 118, 119, 120, 121, 132, 137 };

static const u32 sVsRecorderSchemeFileIds1[7] = { 107, 108, 109, 110, 111, 130, 135 };

static const u32 sVsRecorderSchemeBlendColors[7] = { 0x73FA, 0x771F, 0x6F7B, 0x6FFF, 0x4EBF, 0x575E, 0x5B9F };

static const VsRecorderWindowRectSet sVsRecorderVideoInfoLabelWindows = {
    .rects = {
              { 4, 4, 24, 2 },
              { 4, 6, 24, 4 },
              { 4, 0xB, 5, 2 },
              { 4, 0xD, 5, 2 },
              { 4, 0x15, 24, 2 },
              },
};

static const VsRecorderMsgIdTable sVsRecorderProfileLabelMsgIds = {
    .ids = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
};

static const VsRecorderWindowCoordsTable sVsRecorderProfileLabelWindows = {
    .coords = {
               { 4, 4, 24, 2 },
               { 0xF, 8, 3, 2 },
               { 0x10, 8, 13, 2 },
               { 4, 0xB, 11, 2 },
               { 4, 0xD, 24, 2 },
               { 4, 0xF, 26, 2 },
               { 4, 0x11, 11, 2 },
               { 4, 0x13, 24, 4 },
               { 0xFF, 0, 0, 0 },
               { 4, 0x15, 24, 2 },
               },
};

static const VsRecorderIconLayout sVsRecorderVideoPartyIconLayout = {
    .pos = {
            {
            { 0x58, 0x68 },
            { 0x70, 0x68 },
            { 0x88, 0x68 },
            { 0xA0, 0x68 },
            { 0xB8, 0x68 },
            { 0xD0, 0x68 },
            { 0x18, 0x98 },
            { 0x30, 0x98 },
            { 0x48, 0x98 },
            { 0x60, 0x98 },
            { 0x78, 0x98 },
            { 0x90, 0x98 },
        },
            {
            { 0x18, 0x88 },
            { 0x30, 0x88 },
            { 0x48, 0x88 },
            { 0x28, 0xA0 },
            { 0x40, 0xA0 },
            { 0x58, 0xA0 },
            { 0x90, 0x88 },
            { 0xA8, 0x88 },
            { 0xC0, 0x88 },
            { 0xA0, 0xA0 },
            { 0xB8, 0xA0 },
            { 0xD0, 0xA0 },
        },
            },
};

static const u32 sVsRecorderAvatarFileIds[16][4] = {
    { 3,  60, 17, 18 },
    { 5,  6,  19, 20 },
    { 11, 24, 21, 22 },
    { 31, 57, 23, 24 },
    { 50, 48, 25, 26 },
    { 51, 14, 27, 28 },
    { 62, 32, 29, 30 },
    { 70, 49, 31, 32 },
    { 6,  3,  33, 34 },
    { 7,  10, 35, 36 },
    { 13, 36, 37, 38 },
    { 14, 25, 39, 40 },
    { 35, 85, 41, 42 },
    { 37, 35, 43, 44 },
    { 42, 18, 45, 46 },
    { 63, 33, 47, 48 }
};

// label of a menu row; "save slot" entries (msg 8 / 0x29) show the owner of the saved video
void VsRecorder_SetMenuRowText(VsRecorderWork *work, VsRecorderMenuRow *row, int msgId, const VsRecorderMenuEntry *entry) {
    Window window;
    String *str;
    MsgData *msgData = work->msgData;

    if (msgId == 8) {
        void *owner = work->videoOwners[entry->selection];
        if (owner == NULL) {
            str = NewString_ReadMsgData(msgData, msgId);
        } else {
            MessageFormat *format = VsRecorder_NewMessageFormat(HEAP_ID_109);
            String *template = NewString_ReadMsgData(work->msgData, msg_0013_00007);
            String *name = FrontierCard_NewNameString((const u16 *)work->videoOwners[entry->selection], HEAP_ID_109);
            VsRecorder_SanitizeName(work, name);
            str = String_New(0xFF, HEAP_ID_109);
            BufferString(format, 0, name, 0, 1, 2);
            StringExpandPlaceholders(format, str, template);
            String_Delete(template);
            String_Delete(name);
            MessageFormat_Delete(format);
        }
    } else if (msgId == 0x29) {
        void *owner = work->videoOwners[entry->selection];
        if (owner == NULL) {
            str = NewString_ReadMsgData(msgData, msgId);
        } else {
            MessageFormat *format = VsRecorder_NewMessageFormat(HEAP_ID_109);
            String *template = NewString_ReadMsgData(work->msgData, msg_0013_00007);
            String *name = FrontierCard_NewNameString((const u16 *)work->videoOwners[entry->selection], HEAP_ID_109);
            VsRecorder_SanitizeName(work, name);
            str = String_New(0xFF, HEAP_ID_109);
            BufferString(format, 0, name, 0, 1, 2);
            StringExpandPlaceholders(format, str, template);
            String_Delete(template);
            String_Delete(name);
            MessageFormat_Delete(format);
        }
    } else {
        str = NewString_ReadMsgData(msgData, msgId);
    }

    InitWindow(&window);
    AddTextWindowTopLeftCorner(work->bgConfig, &window, 0x14, 2, 0, 0);
    AddTextPrinterParameterizedWithColor(&window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 13, 0), NULL);
    TextOBJ_CopyFromBGWindow(row->text, row->textLayout, &window, HEAP_ID_109);
    String_Delete(str);
    RemoveWindow(&window);
}

void VsRecorder_FreeMenuRowText(VsRecorderWork *work) {
    int i;
    int j;
    VsRecorderMenuRow *rows[2];

    for (i = 0; i < 5; i++) {
        rows[0] = &work->choiceRows[i];
        rows[1] = &work->pathRows[i];
        for (j = 0; j < 2; j++) {
            TextOBJLayout_Delete(rows[j]->textLayout);
            TextOBJ_Destroy(rows[j]->text);
            ObjCharTransfer_FreeCharSpace(&rows[j]->charAlloc);
        }
    }
}

// label for a sprite of the given screen (vram); palette 9999 main / 10000 sub
void VsRecorder_CreateTextSprite(VsRecorderTextSprite *textSprite, VsRecorderGfx *gfx, NNS_G2D_VRAM_TYPE vram) {
    s16 position[2]; // y, x
    TextOBJTemplate template;
    Window window;
    int plttResId = 9999;

    if (vram == NNS_G2D_VRAM_TYPE_2DSUB) {
        plttResId++;
    }
    InitWindow(&window);
    AddTextWindowTopLeftCorner(gfx->bgConfig, &window, 0x14, 2, 0, 0);
    textSprite->textLayout = TextOBJLayout_New(&window, HEAP_ID_109);
    ObjCharTransfer_ReserveCharSpace(FontOAM_GetCharSize(&window, vram, HEAP_ID_109), TRUE, vram, &textSprite->charAlloc);
    ManagedSprite_GetPositionXY(textSprite->sprite, &position[1], &position[0]);
    template.fontSystem = gfx->fontSystem;
    template.window = &window;
    template.spriteList = SpriteManager_GetSpriteList(gfx->spriteManager);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(gfx->spriteManager, plttResId);
    template.sprite = textSprite->sprite->sprite;
    template.offset = textSprite->charAlloc.offset;
    template.x = position[1] + 0x24;
    template.y = position[0] - 8;
    template.bgPriority = 3;
    template.drawPriority = 0;
    template.vram = vram;
    template.heapID = HEAP_ID_109;
    textSprite->text = TextOBJ_Create(&template, textSprite->textLayout);
    TextOBJ_SetPalOffsetRespectVramOffset(textSprite->text, 1);
    RemoveWindow(&window);
    TextOBJ_SetSpritesDrawFlag(textSprite->text, FALSE);
}

void VsRecorder_SetTextSpriteMsg(VsRecorderTextSprite *textSprite, VsRecorderGfx *gfx, int msgId) {
    Window window;
    String *string;

    string = NewString_ReadMsgData(gfx->msgData, msgId);
    InitWindow(&window);
    AddTextWindowTopLeftCorner(gfx->bgConfig, &window, 0x14, 2, 0, 0);
    AddTextPrinterParameterizedWithColor(&window, 0, string, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(14, 13, 0), NULL);
    TextOBJ_CopyFromBGWindow(textSprite->text, textSprite->textLayout, &window, HEAP_ID_109);
    String_Delete(string);
    RemoveWindow(&window);
}

void VsRecorder_FreeTextSprite(VsRecorderTextSprite *textSprite) {
    TextOBJLayout_Delete(textSprite->textLayout);
    TextOBJ_Destroy(textSprite->text);
    ObjCharTransfer_FreeCharSpace(&textSprite->charAlloc);
}

void VsRecorder_LoadButtonGfx31(VsRecorderWork *work, int vram) {
    NARC *narc = work->narc;
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x31, FALSE, vram, 11924);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x1A, FALSE, 11924);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x1B, FALSE, 11924);
}

void VsRecorder_LoadButtonGfx(VsRecorderWork *work, int vram) {
    NARC *narc = work->narc;
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x7A, FALSE, vram, 11924);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x7B, FALSE, 11924);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x7C, FALSE, 11924);
}

void VsRecorder_LoadSlotButtonGfx(VsRecorderWork *work, int vram) {
    NARC *narc = work->narc;
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x1E, FALSE, vram, 11924);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x20, FALSE, 11924);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x1F, FALSE, 11924);
}

void VsRecorder_FreeButtonGfx(VsRecorderWork *work) {
    SpriteManager_UnloadCharObjById(work->spriteManager, 11924);
    SpriteManager_UnloadCellObjById(work->spriteManager, 11924);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 11924);
}

ManagedSprite *VsRecorder_NewButtonSprite(VsRecorderWork *work, NNS_G2D_VRAM_TYPE vram) {
    ManagedSpriteTemplate template;
    ManagedSprite *sprite;

    template.x = 0x80;
    template.y = 0x60;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 1;
    template.bgPriority = 3;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    template.pal = 0;
    template.vram = vram;
    template.vramTransfer = 0;
    template.resIdList[0] = 11924;
    template.resIdList[2] = 11924;
    template.resIdList[3] = 11924;
    if (vram == NNS_G2D_VRAM_TYPE_2DMAIN) {
        template.resIdList[1] = 9999;
    } else {
        template.resIdList[1] = 10000;
    }
    sprite = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);

    ManagedSprite_SetPaletteOverride(sprite, 2);
    ManagedSprite_SetAnim(sprite, 0);
    ManagedSprite_TickFrame(sprite);
    return sprite;
}

void VsRecorder_FreeMenuRows(VsRecorderWork *work) {
    VsRecorder_SetMenuRowsDrawFlag(work, FALSE);
    VsRecorder_DeleteMenuRowSprites(work);
    VsRecorder_UnloadMenuRowChars(work);
}

// rebuild the menu rows one level up (after a sub-screen freed them)
void VsRecorder_RebuildMenuRowsParent(VsRecorderWork *work) {
    VsRecorder_LoadMenuRowGfx(work);
    VsRecorder_MenuPop(work);
    VsRecorderApp_CountMenuChoices(work);
    VsRecorder_CreateMenuRowSprites(work);
    VsRecorder_SetMenuRowsDrawFlag(work, TRUE);
    VsRecorder_RefreshChoiceRows(work);
    VsRecorder_AttachMenuRowText(work);
    VsRecorder_LayoutMenuRows(work);
    VsRecorder_RefreshPathRows(work);
}

// rebuild the menu rows at the current level
void VsRecorder_RebuildMenuRows(VsRecorderWork *work) {
    VsRecorder_LoadMenuRowGfx(work);
    VsRecorder_MenuPop(work);
    VsRecorderApp_CountMenuChoices(work);
    VsRecorder_CreateMenuRowSprites(work);
    VsRecorder_SetMenuRowsDrawFlag(work, TRUE);
    VsRecorder_RefreshChoiceRows(work);
    VsRecorder_AttachMenuRowText(work);
    VsRecorder_LayoutMenuRows(work);
    VsRecorder_RefreshPathRows(work);
    work->menuDepth++;
}

// mode 0: both screens, 1: main only, 2: sub only
void VsRecorder_InitBlend(int *eva, int *evb, int mainPlane1, int mainPlane2, int subPlane1, int subPlane2, int mode) {
    *eva = 0;
    *evb = 31;
    if (mode == 0) {
        G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, mainPlane1, mainPlane2, *eva, *evb);
        G2x_SetBlendAlpha_((u32)&reg_G2S_DB_BLDCNT, subPlane1, subPlane2, *eva, *evb);
    }
    if (mode == 1) {
        G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, mainPlane1, mainPlane2, *eva, *evb);
    }
    if (mode == 2) {
        G2x_SetBlendAlpha_((u32)&reg_G2S_DB_BLDCNT, subPlane1, subPlane2, *eva, *evb);
    }
}

void VsRecorder_ApplyBlend(int *eva, int *evb, int mainPlane1, int mainPlane2, int subPlane1, int subPlane2, int mode) {
    if (mode == 0) {
        G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, mainPlane1, mainPlane2, *eva, *evb);
        G2x_SetBlendAlpha_((u32)&reg_G2S_DB_BLDCNT, subPlane1, subPlane2, *eva, *evb);
    }
    if (mode == 1) {
        G2x_SetBlendAlpha_((u32)&reg_G2_BLDCNT, mainPlane1, mainPlane2, *eva, *evb);
    }
    if (mode == 2) {
        G2x_SetBlendAlpha_((u32)&reg_G2S_DB_BLDCNT, subPlane1, subPlane2, *eva, *evb);
    }
}

void VsRecorder_InitBgBlend(int *eva, int *evb, int mode) {
    VsRecorder_InitBlend(eva, evb, GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, GX_BLEND_PLANEMASK_BG3, GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_OBJ, mode);
}

// one step of the alpha fade in (or out when reverse); TRUE when done
BOOL VsRecorder_StepBlend(int *eva, int *evb, BOOL reverse, int mode) {
    BOOL done = TRUE;
    if (reverse == FALSE) {
        if (*eva < 8) {
            ++*eva;
            done = FALSE;
        }
        if (*evb > 7) {
            *evb -= 3;
            done = FALSE;
        }
    } else {
        if (*eva > 0) {
            --*eva;
            done = FALSE;
        }
        if (*evb < 31) {
            *evb += 3;
            done = FALSE;
        }
    }
    if (mode == 0) {
        reg_G2_BLDALPHA = *eva | (*evb << 8);
        reg_G2S_DB_BLDALPHA = *eva | (*evb << 8);
    }
    if (mode == 1) {
        reg_G2_BLDALPHA = *eva | (*evb << 8);
    }
    if (mode == 2) {
        reg_G2S_DB_BLDALPHA = *eva | (*evb << 8);
    }
    return done;
}

// palette fade value 0..16 in steps of 2; TRUE when done
BOOL VsRecorder_StepFade(int *fade, int direction) {
    BOOL done = TRUE;
    if (direction == 1) {
        if (*fade < 16) {
            *fade += 2;
            done = FALSE;
        }
    } else {
        if (*fade > 0) {
            *fade -= 2;
            done = FALSE;
        }
    }
    return done;
}

void VsRecorder_ResetFade(int *fade) {
    *fade = 16;
}

MessageFormat *VsRecorder_NewMessageFormat(enum HeapID heapID) {
    return MessageFormat_New_Custom(4, 0x40, heapID);
}

u32 VsRecorder_GetBlendColor(VsRecorderWork *work) {
    VsRecorderSchemeTable table = *(const VsRecorderSchemeTable *)sVsRecorderSchemeBlendColors;
    return table.values[work->colorScheme];
}

u32 VsRecorder_GetMenuBlendColor(VsRecorderWork *work) {
    VsRecorderSchemeTable table = *(const VsRecorderSchemeTable *)sVsRecorderSchemeMenuBlendColors;
    if (work->mode != 0) {
        return 0x7E05;
    }
    return table.values[work->colorScheme];
}

// NARC_a_1_9_1 palette file of the colour scheme: 0 BG frame, 2 OBJ, 3 BG
u32 VsRecorder_GetSchemeFileId(VsRecorderWork *work, u32 kind) {
    VsRecorderSchemeTable bgFrame = *(const VsRecorderSchemeTable *)sVsRecorderSchemeBgFramePaletteFiles;
    VsRecorderSchemeTable b = *(const VsRecorderSchemeTable *)sVsRecorderSchemeFileIds1;
    VsRecorderSchemeTable obj = *(const VsRecorderSchemeTable *)sVsRecorderSchemeObjPaletteFiles;
    VsRecorderSchemeTable bg = *(const VsRecorderSchemeTable *)sVsRecorderSchemeBgPaletteFiles;
    u32 index = work->colorScheme;

    switch (kind) {
    case 0:
        return bgFrame.values[index];
    case 1:
        return b.values[index];
    case 2:
        return obj.values[index];
    case 3:
        return bg.values[index];
    default:
        GF_AssertFail();
        return bgFrame.values[0];
    }
}

void VsRecorder_ReloadSchemePalettes(VsRecorderWork *work, u32 unused) {
    PaletteData *plttData = work->plttData;
    SpriteManager *spriteManager = work->spriteManager;
    NARC *narc = work->narc;
    SpriteSystem *spriteSystem = work->spriteSystem;

    PaletteData_LoadNarc(plttData, NARC_a_1_9_1, VsRecorder_GetSchemeFileId(work, 3), HEAP_ID_109, PLTTBUF_MAIN_BG, 0x120, 0);
    PaletteData_LoadNarc(plttData, NARC_a_1_9_1, VsRecorder_GetSchemeFileId(work, 3), HEAP_ID_109, PLTTBUF_SUB_BG, 0x120, 0);
    PaletteData_LoadNarc(plttData, NARC_a_1_9_1, VsRecorder_GetSchemeFileId(work, 0), HEAP_ID_109, PLTTBUF_MAIN_BG, 0x20, 0xE0);
    PaletteData_LoadNarc(plttData, NARC_a_1_9_1, VsRecorder_GetSchemeFileId(work, 0), HEAP_ID_109, PLTTBUF_SUB_BG, 0x20, 0xE0);

    OrbitCursor_ReloadPalette(work->orbitCursors[0], work->colorScheme);
    OrbitCursor_ReloadPalette(work->orbitCursors[0], work->colorScheme);
    SpriteManager_UnloadPlttObjById(spriteManager, 9999);
    SpriteManager_UnloadPlttObjById(spriteManager, 10000);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, narc, VsRecorder_GetSchemeFileId(work, 2), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 9999);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(plttData, PLTTBUF_SUB_OBJ, spriteSystem, spriteManager, narc, VsRecorder_GetSchemeFileId(work, 2), FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 10000);
}

void VsRecorder_ClearPaletteBlend(VsRecorderWork *work) {
    PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_OBJ, 0xFFFF, 0, work->blendColor);
    PaletteData_BlendPalettes(work->plttData, PLTTBUF_MAIN_BG, 0xFFFF, 0, work->blendColor);
    PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_OBJ, 0xFFFF, 0, work->blendColor);
    PaletteData_BlendPalettes(work->plttData, PLTTBUF_SUB_BG, 0xFFFF, 0, work->blendColor);
}

u16 *VsRecorder_LoadSpeciesList(enum HeapID heapID, int unused, int *count) {
    u32 size;
    u16 *data = GfGfxLoader_LoadFromNarc_GetSizeOut(NARC_application_zukanlist_zkn_data_zukan_data, 13, FALSE, heapID, FALSE, &size);
    *count = size >> 1;
    return data;
}

void VsRecorder_SetBoxUploaded(VsRecorderWork *work, u32 value) {
    work->boxUploaded = value;
}

u32 VsRecorder_IsBoxUploaded(VsRecorderWork *work) {
    return work->boxUploaded;
}

// touch screen notice (BG 6 window)
void VsRecorder_ShowNotice(VsRecorderWork *work, int msgId) {
    Window *window = &work->messageWindow;
    String *str;

    if (work->messageShown != TRUE) {
        work->messageShown = TRUE;
        VsRecorder_LoadMessageBg(work, GF_BG_LYR_SUB_2, FALSE);
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
        VsRecorder_LoadMessageBg(work, GF_BG_LYR_MAIN_2, FALSE);
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
        VsRecorder_LoadMessageBg(work, GF_BG_LYR_SUB_2, FALSE);
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

static void VsRecorder_PrintProfile(VsRecorderProfile *profile, VsRecorderWork *work) {
    VsRecorderWindowCoordsTable coords = sVsRecorderProfileLabelWindows;
    VsRecorderMsgIdTable msgIds = sVsRecorderProfileLabelMsgIds;
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
    const VsRecorderWindowRect *cp;
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
    nameTemplate = NewString_ReadMsgData(work->msgData, msg_0013_00013);
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
    monthTemplate = NewString_ReadMsgData(work->msgData, msg_0013_00015);
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
    str = NewString_ReadMsgData(work->msgData, msg_0013_00016);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, 0, str, VsRecorder_GetCenteredTextX(window, str), 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);

    country = FrontierCard_GetCountry(card);
    city = FrontierCard_GetRegion(card);
    window = &profile->windows[4];
    FillWindowPixelBuffer(window, 0);
    if (country == 0) {
        str = NewString_ReadMsgData(work->msgData, msg_0013_00021);
        AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        ScheduleWindowCopyToVram(window);
        String_Delete(str);
    } else {
        countryStr = String_New(0xFF, HEAP_ID_109);
        countryTemplate = NewString_ReadMsgData(work->msgData, msg_0013_00022);
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
            cityTemplate = NewString_ReadMsgData(work->msgData, msg_0013_00023);
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
    str = NewString_ReadMsgData(work->msgData, msg_0013_00017);
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

static void VsRecorder_RemoveProfileWindows(VsRecorderProfile *profile) {
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

static void VsRecorder_SetProfileMonIconDrawFlag(VsRecorderProfile *profile, BOOL draw) {
    ManagedSprite *sprite = profile->monIcon;
    if (sprite != NULL) {
        ManagedSprite_SetDrawFlag(sprite, draw);
    }
}

static void VsRecorder_FreeProfileMonIcon(VsRecorderProfile *profile, VsRecorderWork *work) {
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
static void VsRecorder_CreateProfileMonIcon(VsRecorderProfile *profile, VsRecorderWork *work) {
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
        ((void (*)(ManagedSprite *, int))ManagedSprite_SetPaletteOverrideOffset)(profile->monIcon, slot + GetMonIconPaletteEx(species, form, isEgg));
        ManagedSprite_SetAnim(profile->monIcon, 1);
    }
}

static void VsRecorder_SetProfileAvatarDrawFlag(VsRecorderProfile *profile, BOOL draw) {
    ManagedSprite_SetDrawFlag(profile->avatar, draw);
}

static void VsRecorder_FreeProfileAvatar(VsRecorderProfile *profile, VsRecorderWork *work) {
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
    return sVsRecorderAvatarFileIds[avatar][kind];
}

// trainer sprite of the card (top screen): the player's own gender sprite unless a video is selected
static void VsRecorder_CreateProfileAvatar(VsRecorderProfile *profile, VsRecorderWork *work) {
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

void VsRecorder_FreeProfile(VsRecorderProfile *profile, VsRecorderWork *work) {
    VsRecorder_RemoveProfileWindows(profile);
    VsRecorder_SetProfileDrawFlag(profile, FALSE);
    VsRecorder_FreeProfileAvatar(profile, work);
    VsRecorder_FreeProfileMonIcon(profile, work);
}

void VsRecorder_SetProfileDrawFlag(VsRecorderProfile *profile, BOOL draw) {
    VsRecorder_SetProfileMonIconDrawFlag(profile, draw);
    VsRecorder_SetProfileAvatarDrawFlag(profile, draw);
}

// "country / region" of the shown trainer in the list's second row
static void VsRecorder_PrintListLocation(VsRecorderListWindow *list, VsRecorderWork *work) {
    MessageFormat *fmt;
    String *template;
    String *str;
    u32 country = FrontierCard_GetCountry(work->videoOwners[0]);
    u32 city = FrontierCard_GetRegion(work->videoOwners[0]);
    Window *window = &list->window;

    if (country == 0 && city == 0) {
        str = NewString_ReadMsgData(work->msgData, msg_0013_00125);
    } else {
        fmt = VsRecorder_NewMessageFormat(HEAP_ID_109);
        if (city != 0) {
            str = String_New(0xFF, HEAP_ID_109);
            template = NewString_ReadMsgData(work->msgData, msg_0013_00023);
            BufferCityName(fmt, 0, country, city);
            StringExpandPlaceholders(fmt, str, template);
        } else {
            str = String_New(0xFF, HEAP_ID_109);
            template = NewString_ReadMsgData(work->msgData, msg_0013_00022);
            BufferCountryName(fmt, 0, country);
            StringExpandPlaceholders(fmt, str, template);
        }
        String_Delete(template);
        MessageFormat_Delete(fmt);
    }

    AddTextPrinterParameterizedWithColor(window, 0, str, 0, 0x10, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    ScheduleWindowCopyToVram(window);
    String_Delete(str);
}

// one message per entry; entry msg 0x10 is followed by the trainer's location
void VsRecorder_InitMessageList(VsRecorderListWindow *list, VsRecorderWork *work, VsRecorderListConfig *config) {
    Window *window = &list->window;
    int i;
    int count;
    list->count = config->count;
    list->state = 0;
    list->drawnPos = 0;
    list->rows = config->rows;
    list->config = config;
    list->msgData = work->msgData;
    list->pages = list->count / list->rows + 1;
    count = list->count;
    if (count < list->rows) {
        list->rows = count;
        list->topMargin = count - 1;
        list->bottomMargin = list->rows - 1;
    }
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, (u8)config->bgId, (u8)config->x, (u8)config->y, (u8)config->width, (u8)config->height, 14, (u16)config->baseTile);
    FillWindowPixelBuffer(window, 0);
    for (i = 0; i < list->rows; i++) {
        String *str = NewString_ReadMsgData(list->msgData, config->entries[i].msgId);
        AddTextPrinterParameterizedWithColor(window, 0, str, 0, config->lineHeight * 16 * i, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        String_Delete(str);
        if (config->entries[i].msgId == 0x10) {
            VsRecorder_PrintListLocation(list, work);
        }
    }
    ScheduleWindowCopyToVram(window);
}

// message list (msgData, or the app's msg_0013 if NULL); without entries: numbered names of work->net.videoList.headers
void VsRecorder_InitList(VsRecorderListWindow *list, VsRecorderWork *work, MsgData *msgData, VsRecorderListConfig *config) {
    Window *window = &list->window;
    int i;
    int count;

    list->count = config->count;
    list->state = 0;
    list->drawnPos = 0;
    list->rows = config->rows;
    list->config = config;
    if (msgData != NULL) {
        list->msgData = msgData;
    } else {
        list->msgData = work->msgData;
    }
    list->pages = list->count / list->rows + 1;
    list->topMargin = list->rows / 2;
    list->bottomMargin = list->rows / 2;
    count = list->count;
    if (count < list->rows) {
        list->rows = count;
        list->topMargin = count - 1;
        list->bottomMargin = list->rows - 1;
    }

    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, (u8)config->bgId, (u8)config->x, (u8)config->y, (u8)config->width, (u8)config->height, 14, (u16)config->baseTile);
    FillWindowPixelBuffer(window, 0);

    if (config->entries != NULL) {
        for (i = 0; i < list->rows; i++) {
            String *str = NewString_ReadMsgData(list->msgData, config->entries[i].msgId);
            AddTextPrinterParameterizedWithColor(window, 0, str, 0, config->lineHeight * 16 * i, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            String_Delete(str);
        }
    } else {
        i = 0;
        if (list->rows > 0) {
            int y = 4;
            do {
                String *name = String_New(0xFF, HEAP_ID_109);
                String *number = String_New(0xFF, HEAP_ID_109);
                String *template = NewString_ReadMsgData(work->msgData, msg_0013_00099);
                u32 width;

                String16_FormatInteger(number, i + 1, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
                CopyU16ArrayToString(name, (const u16 *)work->net.videoList.headers[i]);
                VsRecorder_SanitizeName(work, name);
                width = FontID_String_GetWidth(0, number, 0);
                AddTextPrinterParameterizedWithColor(window, 0, number, 16 - width, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
                AddTextPrinterParameterizedWithColor(window, 0, template, 16, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
                AddTextPrinterParameterizedWithColor(window, 0, name, 22, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
                String_Delete(name);
                String_Delete(number);
                String_Delete(template);
                y += 24;
                i++;
            } while (i < list->rows);
        }
    }
    ScheduleWindowCopyToVram(window);
}

// ranking list (type 0 message value, 1 month, 2 species); equal times share a rank
void VsRecorder_InitRankingList(VsRecorderListWindow *list, VsRecorderWork *work, MsgData *msgData, VsRecorderListConfig *config, int unused, int type, String *valueTemplate) {
    Window *win;
    String *label;
    String *number;
    String *fieldString;
    int lastGroup = 0;
    u64 previous;
    String *valueString;
    int month;
    int species;
    u64 value;
    int *grp;
    int msgId;
    int i;
    MessageFormat *fmt;
    String *expanded;
    int groups[30] = { 0 };
    u16 speciesName[256];
    VsRecorderRankLimits limits;

    win = &list->window;
    list->count = config->count;
    list->state = 0;
    list->drawnPos = 0;
    list->rows = config->rows;
    list->config = config;
    if (msgData) {
        list->msgData = msgData;
    } else {
        list->msgData = work->msgData;
    }
    list->pages = list->count / list->rows + 1;
    {
        int count = list->count;
        if (count < list->rows) {
            list->rows = count;
            list->topMargin = count - 1;
            list->bottomMargin = list->rows - 1;
        }
    }
    list->topMargin = list->rows / 2;
    list->bottomMargin = list->rows / 2;
    InitWindow(win);
    AddWindowParameterized(work->bgConfig, win, (u8)config->bgId, (u8)config->x, (u8)config->y, (u8)config->width, (u8)config->height, 14, (u16)config->baseTile);
    FillWindowPixelBuffer(win, 0);
    fmt = VsRecorder_NewMessageFormat(HEAP_ID_109);
    limits = sVsRecorderRankLimits;
    previous = list->config->entries[0].time;
    groups[0] = 0;
    {
        for (i = 1; i < limits.v[type]; i++) {
            u64 current = list->config->entries[i].time;
            if (current == previous) {
                groups[i] = lastGroup;
            } else {
                previous = current;
                groups[i] = i;
                lastGroup = i;
            }
        }
    }
    i = 0;
    if (i < list->rows) {
        grp = groups;
        msgId = type + 88;
        do {
            label = NewString_ReadMsgData(work->msgData, msgId);
            number = String_New(255, HEAP_ID_109);
            expanded = String_New(255, HEAP_ID_109);
            valueString = String_New(255, HEAP_ID_109);
            switch (type) {
            case 0:
                fieldString = NewString_ReadMsgData(list->msgData, list->config->entries[i].value);
                BufferString(fmt, 1, fieldString, 0, 1, 2);
                break;
            case 1:
                month = list->config->entries[i].value;
                fieldString = String_New(255, HEAP_ID_109);
                BufferMonthNameAbbr(fmt, 1, month);
                break;
            case 2:
                species = list->config->entries[i].value;
                fieldString = String_New(255, HEAP_ID_109);
                GetSpeciesNameIntoArrayInt(species, HEAP_ID_109, speciesName);
                CopyU16ArrayToString(fieldString, speciesName);
                BufferString(fmt, 1, fieldString, 0, 1, 2);
                break;
            }
            value = list->config->entries[i].time;
            String16_FormatUnsignedLongLong(valueString, value, VsRecorder_CountDigits(value), PRINTING_MODE_LEFT_ALIGN, TRUE);
            String16_FormatInteger(number, *grp + 1, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            BufferString(fmt, 0, number, 0, 1, 2);
            StringExpandPlaceholders(fmt, expanded, label);
            AddTextPrinterParameterizedWithColor(win, 0, expanded, 0, (s32)((u32)list->config->lineHeight * 16U * (u32)i), TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            if (list->config->lineHeight == 2 && valueTemplate) {
                BufferString(fmt, 2, valueString, 0, 1, 2);
                StringExpandPlaceholders(fmt, expanded, valueTemplate);
                AddTextPrinterParameterizedWithColor(win, 0, expanded, 16, (s32)((u32)list->config->lineHeight * 16U * (u32)i + 16U), TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            }
            String_Delete(label);
            String_Delete(number);
            String_Delete(fieldString);
            String_Delete(expanded);
            String_Delete(valueString);
            MessageFormat_ResetBuffers(fmt);
            grp++;
        } while (++i < list->rows);
    }
    ScheduleWindowCopyToVram(win);
    MessageFormat_Delete(fmt);
}

// two columns of prepared strings (items[2 * row], items[2 * row + 1])
void VsRecorder_InitPairList(VsRecorderListWindow *list, VsRecorderWork *work, VsRecorderListConfig *config, String **items) {
    Window *window = &list->window;
    int i;
    int count;
    list->count = config->count;
    list->state = 0;
    list->drawnPos = 0;
    list->rows = config->rows;
    list->config = config;
    list->msgData = work->msgData;
    list->pages = list->count / list->rows + 1;
    list->topMargin = list->rows / 2;
    list->bottomMargin = list->rows / 2;
    count = list->count;
    if (count < list->rows) {
        list->rows = count;
        list->topMargin = count - 1;
        list->bottomMargin = list->rows - 1;
    }
    list->isPairList = 1;
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, (u8)config->bgId, (u8)config->x, (u8)config->y, (u8)config->width, (u8)config->height, 14, (u16)config->baseTile);
    FillWindowPixelBuffer(window, 0);
    for (i = 0; i < list->rows; i++, items += 2) {
        AddTextPrinterParameterizedWithColor(window, 0, items[0], 0, config->lineHeight * 8 * i + 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        AddTextPrinterParameterizedWithColor(window, 0, items[1], 0x88, config->lineHeight * 8 * i + 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
    }
    ScheduleWindowCopyToVram(window);
}

int VsRecorder_RedrawPairList(VsRecorderListWindow *list, VsRecorderWork *work, String **items) {
    int start;
    int i;
    Window *window;

    (void)work;
    if (list->cursor == list->drawnPos) {
        return 0;
    }
    if (list->rows == list->count) {
        return 0;
    }

    window = &list->window;
    FillWindowPixelBuffer(window, 0);
    {
        int edge = list->topMargin;
        int display = list->cursorRow;

        start = 0;
        if (display >= edge) {
            int selected = list->cursor;
            int count = list->count;

            start = selected - edge;
            if (selected >= count - (edge + 1)) {
                start = count - list->rows;
            }
        }
    }
    i = 0;
    if (list->rows > 0) {
        items += start * 2;
        do {
            if (items[0] != NULL) {
                AddTextPrinterParameterizedWithColor(window, 0, items[0], 0, list->config->lineHeight * 8 * i + 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
                AddTextPrinterParameterizedWithColor(window, 0, items[1], 0x88, list->config->lineHeight * 8 * i + 4, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            }
            i++;
            items += 2;
        } while (i < list->rows);
    }
    CopyWindowToVram(window);
    list->drawnPos = list->cursor;
    return 0;
}

int VsRecorder_RedrawRankingList(VsRecorderListWindow *list, VsRecorderWork *work, void *unused, u32 type, String *valueTemplate) {
    Window *win;
    String *label;
    String *number;
    String *fieldString;
    String *valueString;
    int row;
    int lastGroup;
    u64 previous;
    int start;
    int end;
    int month;
    int species;
    u64 value;
    int i;
    MessageFormat *fmt;
    String *expanded;

    if (list->scroll == list->drawnPos) {
        return 0;
    }
    if (list->rows == list->count) {
        return 0;
    }
    win = &list->window;
    fmt = VsRecorder_NewMessageFormat(HEAP_ID_109);
    FillWindowPixelBuffer(win, 0);
    row = 0;
    lastGroup = 0;
    start = list->scroll;
    end = start + list->rows;
    {
        int groups[30] = { 0 };
        u16 speciesName[256];
        VsRecorderRankLimits limits;

        if (end >= list->count) {
            end = list->count;
            start = list->count - list->rows;
        }
        limits = sVsRecorderRankLimitsRedraw;
        previous = list->config->entries[0].time;
        groups[0] = 0;
        for (i = 1; i < limits.v[type]; i++) {
            u64 current = list->config->entries[i].time;
            if (current == previous) {
                groups[i] = lastGroup;
            } else {
                previous = current;
                groups[i] = i;
                lastGroup = i;
            }
        }
        for (i = start; i < end; i++) {
            label = NewString_ReadMsgData(work->msgData, type + 88);
            number = String_New(255, HEAP_ID_109);
            expanded = String_New(255, HEAP_ID_109);
            valueString = String_New(255, HEAP_ID_109);
            switch (type) {
            case 0:
                fieldString = NewString_ReadMsgData(list->msgData, list->config->entries[i].value);
                BufferString(fmt, 1, fieldString, 0, 1, 2);
                break;
            case 1:
                month = list->config->entries[i].value;
                fieldString = String_New(255, HEAP_ID_109);
                BufferMonthNameAbbr(fmt, 1, month);
                break;
            case 2:
                species = list->config->entries[i].value;
                fieldString = String_New(255, HEAP_ID_109);
                GetSpeciesNameIntoArrayInt(species, HEAP_ID_109, speciesName);
                CopyU16ArrayToString(fieldString, speciesName);
                BufferString(fmt, 1, fieldString, 0, 1, 2);
                break;
            }
            value = list->config->entries[i].time;
            String16_FormatUnsignedLongLong(valueString, value, VsRecorder_CountDigits(value), PRINTING_MODE_LEFT_ALIGN, TRUE);
            String16_FormatInteger(number, groups[i] + 1, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            BufferString(fmt, 0, number, 0, 1, 2);
            StringExpandPlaceholders(fmt, expanded, label);
            AddTextPrinterParameterizedWithColor(win, 0, expanded, 0, (s32)((u32)list->config->lineHeight * 16U * (u32)row), TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            if (list->config->lineHeight == 2 && valueTemplate) {
                BufferString(fmt, 2, valueString, 0, 1, 2);
                StringExpandPlaceholders(fmt, expanded, valueTemplate);
                AddTextPrinterParameterizedWithColor(win, 0, expanded, 16, (s32)((u32)list->config->lineHeight * 16U * (u32)row + 16U), TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            }
            row++;
            String_Delete(label);
            String_Delete(number);
            String_Delete(fieldString);
            String_Delete(expanded);
            String_Delete(valueString);
            MessageFormat_ResetBuffers(fmt);
        }
    }
    ScheduleWindowCopyToVram(win);
    MessageFormat_Delete(fmt);
    list->drawnPos = list->scroll;
    return 0;
}

// touch on a row returns its entry value; redraws when scrolled
u32 VsRecorder_UpdateMessageList(VsRecorderListWindow *list, VsRecorderWork *work) {
    u32 result = 0;
    Window *window;
    int i;
    int start, end;
    String *string;
    int j;

    if (list->config->hitboxes != NULL) {
        for (i = 0; i < list->rows; i++) {
            if (TouchscreenHitbox_TouchNewIsIn(&list->config->hitboxes[i])) {
                VsRecorder_OnTouchConfirm(work);
                result = list->config->entries[list->scroll + i].value;
            }
        }
    }
    if (list->scroll == list->drawnPos) {
        return result;
    }
    if (list->rows == list->count) {
        return result;
    }
    window = &list->window;
    FillWindowPixelBuffer(window, 0);
    start = list->scroll;
    j = 0;
    end = start + list->rows;
    if (end >= list->count) {
        start = list->count - list->rows;
        end = list->count;
    }
    for (i = start; i < end; i++) {
        string = NewString_ReadMsgData(list->msgData, list->config->entries[i].msgId);
        AddTextPrinterParameterizedWithColor(window, 0, string, 0, j * (list->config->lineHeight * 16), TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
        String_Delete(string);
        j++;
    }
    CopyWindowToVram(window);
    list->drawnPos = list->scroll;
    return result;
}

int VsRecorder_RedrawNameList(VsRecorderListWindow *list, VsRecorderWork *work) {
    int i;
    int start;
    Window *window;

    if (list->cursor == list->drawnPos) {
        return 0;
    }
    if (list->rows == list->count) {
        return 0;
    }

    window = &list->window;
    FillWindowPixelBuffer(window, 0);
    start = 0;
    if (list->cursorRow >= list->topMargin) {
        start = list->cursor - list->topMargin;
        if (list->cursor >= list->count - (list->topMargin + 1)) {
            start = list->count - list->rows;
        }
    }
    i = 0;
    if (list->rows > 0) {
        VsRecorderWork *item = (VsRecorderWork *)((GlobalTerminalNetVideoHeader **)work + start);
        int y = 4;
        do {
            String *nameStr = String_New(0xFF, HEAP_ID_109);
            String *number = String_New(0xFF, HEAP_ID_109);
            String *message = NewString_ReadMsgData(work->msgData, msg_0013_00099);
            int width;
            String16_FormatInteger(number, start + i + 1, 2, PRINTING_MODE_RIGHT_ALIGN, TRUE);
            CopyU16ArrayToString(nameStr, (const u16 *)item->net.videoList.headers[0]);
            VsRecorder_SanitizeName(work, nameStr);
            width = FontID_String_GetWidth(0, number, 0);
            AddTextPrinterParameterizedWithColor(window, 0, number, 0x10 - width, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            AddTextPrinterParameterizedWithColor(window, 0, message, 0x10, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            AddTextPrinterParameterizedWithColor(window, 0, nameStr, 0x16, y, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(15, 13, 0), NULL);
            String_Delete(nameStr);
            String_Delete(number);
            String_Delete(message);
            item = (VsRecorderWork *)((GlobalTerminalNetVideoHeader **)item + 1);
            i++;
            y += 0x18;
        } while (i < list->rows);
    }
    CopyWindowToVram(window);
    list->drawnPos = list->cursor;
    return 0;
}

// move the cursor one row (delta < 0 up, > 0 down) and keep it inside the visible window
void VsRecorder_ListMoveCursor(VsRecorderListWindow *list, s16 delta) {
    s16 cursorRow = 0;
    int direction = 0xFF;

    if (delta < 0) {
        if (list->cursor > 0) {
            list->cursor--;
            direction = 0;
            PlaySE(SEQ_SE_PL_BUTTON);
        }
    } else if (delta > 0) {
        if (list->cursor < list->count - 1) {
            list->cursor++;
            direction = 1;
            PlaySE(SEQ_SE_PL_BUTTON);
        }
    }

    switch (direction) {
    case 0: {
        int start;
        if (list->cursor < list->bottomMargin) {
            cursorRow = list->cursor;
        } else {
            start = list->count - list->bottomMargin;
            if (list->cursor >= start) {
                cursorRow = list->bottomMargin + 1;
                if (list->cursor >= start) {
                    cursorRow += list->cursor - start;
                }
            } else {
                cursorRow = list->bottomMargin;
                if (list->cursor <= list->bottomMargin) {
                    cursorRow += list->cursor - list->bottomMargin;
                }
            }
        }
        break;
    }
    case 1:
        if (list->cursor <= list->topMargin) {
            cursorRow = list->cursor;
        } else {
            cursorRow = list->topMargin;
            if (list->cursor >= list->count - list->topMargin) {
                cursorRow += list->cursor - (list->count - list->topMargin - 1);
            }
        }
        break;
    }

    if (cursorRow < 0) {
        GF_AssertFail();
    }
    if (direction != 0xFF) {
        list->cursorRow = cursorRow;
    }

    {
        s16 scroll = 0;
        if (list->cursorRow >= list->topMargin) {
            scroll = list->cursor - list->topMargin;
            if (list->cursor >= list->count - (list->topMargin + 1)) {
                scroll = list->count - list->rows;
            }
        }
        list->scroll = scroll;
    }
    VsRecorder_UpdateListArrowsCursor(list);
}

void VsRecorder_ListScroll(VsRecorderListWindow *list, s16 delta) {
    s16 old = list->scroll;
    list->scroll += delta;
    if (list->scroll < 0) {
        list->scroll = 0;
    }
    if (list->rows == list->count) {
        list->scroll = 0;
    }
    if (list->scroll + list->rows > list->count) {
        list->scroll = list->count - list->rows;
    }
    if (old != list->scroll) {
        PlaySE(SEQ_SE_PL_BUTTON);
    }
    VsRecorder_UpdateListArrows(list);
}

void VsRecorder_RemoveListWindow(VsRecorderListWindow *list) {
    ClearWindowTilemapAndCopyToVram(&list->window);
    RemoveWindow(&list->window);
}

void VsRecorder_ClearListWindow(VsRecorderListWindow *list) {
    memset(list, 0, sizeof(VsRecorderListWindow));
}

// up / down arrows of a list (main screen: vram 1, sub screen: vram 2)
void VsRecorder_CreateListArrows(VsRecorderListWindow *list, VsRecorderWork *work, NNS_G2D_VRAM_TYPE vram) {
    NARC *narc = work->narc;
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;
    ManagedSpriteTemplate template;

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x7F, FALSE, vram, 200000);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x7E, FALSE, 200000);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x7D, FALSE, 200000);
    template.x = 128;
    template.y = 96;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 1;
    template.bgPriority = 1;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    template.pal = 0;
    template.vram = vram;
    template.vramTransfer = 0;
    template.resIdList[0] = 200000;
    template.resIdList[2] = 200000;
    template.resIdList[3] = 200000;
    if (vram == NNS_G2D_VRAM_TYPE_2DMAIN) {
        template.resIdList[1] = 9999;
    } else {
        template.resIdList[1] = 10000;
    }
    list->arrows[0] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
    list->arrows[1] = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
    ManagedSprite_SetPaletteOverride(list->arrows[0], 2);
    ManagedSprite_SetPaletteOverride(list->arrows[1], 2);
    ManagedSprite_SetAnim(list->arrows[0], 0);
    ManagedSprite_SetAnim(list->arrows[1], 0);
    ManagedSprite_TickFrame(list->arrows[0]);
    ManagedSprite_TickFrame(list->arrows[1]);
    ManagedSprite_SetFlipMode(list->arrows[1], 2);
    if (vram == NNS_G2D_VRAM_TYPE_2DMAIN) {
        ManagedSprite_SetPositionXY(list->arrows[0], 128, 24);
        ManagedSprite_SetPositionXY(list->arrows[1], 128, 120);
    } else {
        ManagedSprite_SetPositionXY(list->arrows[0], 128, 88);
        ManagedSprite_SetPositionXY(list->arrows[1], 128, 184);
    }
    VsRecorder_UpdateListArrows(list);
}

void VsRecorder_SetListArrowsY(VsRecorderListWindow *list, s16 upY, s16 downY) {
    ManagedSprite_SetPositionXY(list->arrows[0], 128, upY);
    ManagedSprite_SetPositionXY(list->arrows[1], 128, downY);
}

// scrolled lists: hide the arrow of an end that is reached
void VsRecorder_UpdateListArrows(VsRecorderListWindow *list) {
    ManagedSprite_SetDrawFlag(list->arrows[0], TRUE);
    ManagedSprite_SetDrawFlag(list->arrows[1], TRUE);
    if (list->scroll == 0) {
        ManagedSprite_SetDrawFlag(list->arrows[0], FALSE);
    }
    if (list->scroll + list->rows >= list->count) {
        ManagedSprite_SetDrawFlag(list->arrows[1], FALSE);
    }
    ManagedSprite_TickTwoFrames(list->arrows[0]);
    ManagedSprite_TickTwoFrames(list->arrows[1]);
}

// cursor lists: also hide both when everything fits
void VsRecorder_UpdateListArrowsCursor(VsRecorderListWindow *list) {
    ManagedSprite_SetDrawFlag(list->arrows[0], TRUE);
    ManagedSprite_SetDrawFlag(list->arrows[1], TRUE);
    if (list->scroll == 0) {
        ManagedSprite_SetDrawFlag(list->arrows[0], FALSE);
    }
    if (list->scroll + list->rows >= list->count) {
        ManagedSprite_SetDrawFlag(list->arrows[1], FALSE);
    }
    if (list->rows >= list->count) {
        ManagedSprite_SetDrawFlag(list->arrows[0], FALSE);
        ManagedSprite_SetDrawFlag(list->arrows[1], FALSE);
    }
    ManagedSprite_TickTwoFrames(list->arrows[0]);
    ManagedSprite_TickTwoFrames(list->arrows[1]);
}

void VsRecorder_FreeListArrows(VsRecorderListWindow *list, VsRecorderWork *work) {
    SpriteManager_UnloadCharObjById(work->spriteManager, 200000);
    SpriteManager_UnloadCellObjById(work->spriteManager, 200000);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 200000);
    Sprite_DeleteAndFreeResources(list->arrows[0]);
    Sprite_DeleteAndFreeResources(list->arrows[1]);
}

static ManagedSprite *VsRecorder_NewScrollSprite(void *unused, VsRecorderWork *work, NNS_G2D_VRAM_TYPE vram, int anim) {
    ManagedSpriteTemplate template;
    ManagedSprite *sprite;

    template.x = 0x80;
    template.y = 0x60;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 1;
    template.bgPriority = 1;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    template.pal = 0;
    template.vram = vram;
    template.vramTransfer = 0;
    template.resIdList[0] = 11924;
    template.resIdList[2] = 11924;
    template.resIdList[3] = 11924;
    if (vram == NNS_G2D_VRAM_TYPE_2DMAIN) {
        template.resIdList[1] = 9999;
    } else {
        template.resIdList[1] = 10000;
    }
    sprite = SpriteSystem_NewSprite(work->spriteSystem, work->spriteManager, &template);
    ManagedSprite_SetPaletteOverride(sprite, 2);
    ManagedSprite_SetAnim(sprite, anim);
    ManagedSprite_TickFrame(sprite);
    return sprite;
}

static void VsRecorder_DeleteTouchScrollSprites(VsRecorderTouchScroll *scroll) {
    Sprite_DeleteAndFreeResources(scroll->sprites[0]);
    Sprite_DeleteAndFreeResources(scroll->sprites[1]);
}

void VsRecorder_CreateTouchScrollSprites(VsRecorderTouchScroll *scroll, VsRecorderWork *work) {
    VsRecorder_CreateTouchScrollSpritesWithAnim(scroll, work, 5);
}

void VsRecorder_CreateTouchScrollSpritesWithAnim(VsRecorderTouchScroll *scroll, VsRecorderWork *work, int anim) {
    scroll->sprites[0] = VsRecorder_NewScrollSprite(scroll, work, NNS_G2D_VRAM_TYPE_2DSUB, anim);
    scroll->sprites[1] = VsRecorder_NewScrollSprite(scroll, work, NNS_G2D_VRAM_TYPE_2DSUB, anim);
    ManagedSprite_SetPositionXY(scroll->sprites[0], 24, 136);
    ManagedSprite_SetPositionXY(scroll->sprites[1], 232, 136);
}

void VsRecorder_InitTouchScroll(VsRecorderTouchScroll *scroll) {
    memset(scroll, 0, sizeof(VsRecorderTouchScroll));
}

void VsRecorder_FreeTouchScroll(VsRecorderTouchScroll *scroll) {
    VsRecorder_DeleteTouchScrollSprites(scroll);
}

// direction of a drag that moved 16 pixels or more
static void VsRecorder_TouchScrollTrack(VsRecorderTouchScroll *scroll, int y) {
    int delta = y - scroll->lastY;
    if (delta * delta >= 0x100) {
        scroll->direction = delta < 0 ? -1 : 1;
        scroll->lastY = y;
    } else {
        scroll->direction = 0;
    }
}

void VsRecorder_TouchScrollSetEnabled(VsRecorderTouchScroll *scroll, VsRecorderListWindow *list) {
    if (list->isPairList != 0) {
        if (list->count < 2) {
            scroll->enabled = FALSE;
            return;
        }
        scroll->enabled = TRUE;
        return;
    } else {
        if (list->rows >= list->count) {
            scroll->enabled = FALSE;
            return;
        }
        scroll->enabled = TRUE;
        return;
    }
}

void VsRecorder_UpdateTouchScroll(VsRecorderTouchScroll *scroll) {
    u32 x, y;
    BOOL held;
    int hit = 0;
    held = System_GetTouchHeldCoords(&x, &y);
    if (scroll->enabled != FALSE) {
        ManagedSprite_TickTwoFrames(scroll->sprites[0]);
        ManagedSprite_TickTwoFrames(scroll->sprites[1]);
    }
    if (scroll->held == FALSE && held == FALSE) {
        scroll->held = FALSE;
        scroll->timer = 0;
        scroll->startX = 0;
        scroll->lastY = 0;
        scroll->direction = 0;
        return;
    }
    if (TouchscreenHitbox_TouchHeldIsIn(sVsRecorderTouchScrollLeftHitbox) || TouchscreenHitbox_TouchHeldIsIn(sVsRecorderTouchScrollRightHitbox)) {
        hit = 1;
    }
    if (hit) {
        if (scroll->held == FALSE) {
            scroll->held = TRUE;
            scroll->startX = x;
            scroll->lastY = y;
            scroll->timer = 2;
        } else {
            scroll->timer--;
            VsRecorder_TouchScrollTrack(scroll, y);
        }
    } else if (scroll->held != FALSE) {
        scroll->held = FALSE;
        scroll->timer = 0;
        scroll->startX = 0;
        scroll->lastY = 0;
    }
}

void VsRecorder_AddGameStatsScore(VsRecorderWork *work, int score) {
    GameStats_AddScore(Save_GameStats_Get(work->saveData), score);
}

// import the downloaded battle video into the save's battle record
void VsRecorder_ImportBattleRecord(VsRecorderWork *work) {
    u32 unused;
    u8 *video;
    BattleRecord_Create(work->saveData, HEAP_ID_126, &unused);
    video = work->net.downloadedVideo;
    BattleRecord_Import(video, video + 0x80, video + 0xE4, NULL, work->saveData);
}

static void VsRecorder_FlipScreensTask(SysTask *task, void *data) {
    VsRecorderFlipArgs *args = data;
    VsRecorder_SetScreensFlipped(args->flipped);
    WirelessStatusIcon_Refresh();
    args->done = TRUE;
    SysTask_Destroy(task);
}

SysTask *VsRecorder_StartFlipScreens(VsRecorderWork *work, int flipped) {
    work->flipArgs.flipped = flipped;
    work->flipArgs.done = FALSE;
    return SysTask_CreateOnVBlankQueue(VsRecorder_FlipScreensTask, &work->flipArgs, 4);
}

u32 VsRecorder_IsFlipScreensDone(VsRecorderWork *work) {
    return work->flipArgs.done;
}

static void VsRecorder_VBlankTask(SysTask *task, void *data) {
    VsRecorderWork *work = data;
    if (work->spriteManager == NULL) {
        return;
    }
    if (VsRecorder_IsVBlankPaused(work) == 1) {
        return;
    }
    if (work->vblankBusy == 1) {
        return;
    }
    work->vblankBusy = 1;
    PokepicManager_HandleLoadImgAndOrPltt(work->pokepicManager);
    SpriteSystem_DrawSprites(work->spriteManager);
    SpriteSystem_TransferOam();
    work->vblankBusy = 0;
}

void VsRecorder_StartVBlankTask(VsRecorderWork *work) {
    work->task = SysTask_CreateOnVBlankQueue(VsRecorder_VBlankTask, work, 5);
}

BOOL VsRecorder_StepRecordSave(VsRecorderWork *work, u32 index, u32 sealed) {
    u32 result = VsRecorder_WriteRecordAsync(work->saveData, index, sealed, &work->recordSaveState, &work->recordWriteState);
    return result > 1;
}

void VsRecorder_SetBattleResult(VsRecorderWork *work, u32 result) {
    work->battleResult = result;
}

// server result callback: copy the received video list
void VsRecorder_OnVideoListReceived(VsRecorderWork *work, int *state) {
    VsRecorderNetData *net = &work->net;
    if (*state != 1) {
        int result = GlobalTerminalNet_GetVideoList(VsRecorder_GetNet(work), net->videoList.headers, 30);
        int i = 0;
        net->videoList.count = result;
        if (net->videoList.count > 0) {
            do {
                GF_ASSERT(work->net.videoList.headers[i]);
                MI_CpuCopy8(work->net.videoList.headers[i], &work->net.videoList.videos[i], sizeof(GlobalTerminalNetVideoHeader));
                work->net.videoList.flags[i] = work->net.videoList.headers[i]->data[0x27];
                i++;
            } while (i < net->videoList.count);
        }
    }
}

// the four saved videos (header + owner card); slot 0 owner is the player
void VsRecorder_LoadSavedVideos(VsRecorderWork *work) {
    int i;
    int result;

    for (i = 0; i < 4; i++) {
        BattleRecord_Load(work->saveData, HEAP_ID_109, &result, NULL, i);
        switch (result) {
        case 0:
            work->savedVideos[i] = NULL;
            work->videoOwners[i] = NULL;
            if (i == 0) {
                work->videoOwners[i] = FrontierCard_New(HEAP_ID_109);
                FrontierCard_InitFromSave(work->videoOwners[i], work->saveData);
            }
            break;
        case 2:
            work->savedVideos[i] = NULL;
            work->videoOwners[i] = NULL;
            if (i == 0) {
                work->videoOwners[i] = FrontierCard_New(HEAP_ID_109);
                FrontierCard_InitFromSave(work->videoOwners[i], work->saveData);
            }
            break;
        case 3:
            work->savedVideos[i] = NULL;
            work->videoOwners[i] = NULL;
            if (i == 0) {
                work->videoOwners[i] = FrontierCard_New(HEAP_ID_109);
                FrontierCard_InitFromSave(work->videoOwners[i], work->saveData);
            }
            break;
        case 1:
            work->savedVideos[i] = BattleRecord_CopyHeader(HEAP_ID_109);
            if (i == 0) {
                work->videoOwners[i] = FrontierCard_New(HEAP_ID_109);
                FrontierCard_InitFromSave(work->videoOwners[i], work->saveData);
            } else {
                work->videoOwners[i] = BattleRecord_CopyProfile(HEAP_ID_109);
            }
            break;
        }
        if (BattleRecord_Exists() == TRUE) {
            BattleRecord_Delete();
        }
    }
}

void VsRecorder_FreeSavedVideos(VsRecorderWork *work) {
    int i;

    for (i = 0; i < 4; i++) {
        if (work->savedVideos[i] != NULL) {
            Heap_Free(work->savedVideos[i]);
            work->savedVideos[i] = NULL;
        }
        if (work->videoOwners[i] != NULL) {
            Heap_Free(work->videoOwners[i]);
            work->videoOwners[i] = NULL;
        }
    }
}

// Pokemon icon resources (palette / cell / anim 100000; chars 100000 + index)
void VsRecorder_LoadMonIconGfx(VsRecorderWork *work) {
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;
    PaletteData *plttData = work->plttData;
    SpriteSystem_LoadPaletteBuffer(plttData, PLTTBUF_MAIN_OBJ, spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), FALSE, 3, NNS_G2D_VRAM_TYPE_2DMAIN, 100000);
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconCell64KNaix(), FALSE, 100000);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconAnim64KNaix(), FALSE, 100000);
}

void VsRecorder_UnloadMonIconGfx(VsRecorderWork *work) {
    SpriteManager_UnloadPlttObjById(work->spriteManager, 100000);
    SpriteManager_UnloadCellObjById(work->spriteManager, 100000);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 100000);
}

NARC *VsRecorder_OpenMonIconNarc(enum HeapID heapID) {
    return NARC_New(NARC_poketool_icongra_poke_icon, HEAP_ID_109);
}

void VsRecorder_CloseMonIconNarc(NARC *narc) {
    NARC_Delete(narc);
}

#pragma opt_lifetimes off
#pragma opt_dead_assignments off
#pragma opt_propagation off
#pragma opt_common_subs off

ManagedSprite *VsRecorder_NewMonIcon(VsRecorderWork *work, NARC *narc, int index, int species, int form, int isEgg) {
    int e, f;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    spriteSystem = work->spriteSystem;
    ManagedSpriteTemplate template;
    ManagedSprite *sprite;
    spriteManager = work->spriteManager;
    if (species == SPECIES_NONE) {
        return NULL;
    }
    f = form;
    e = isEgg;
    SpriteSystem_LoadCharResObjFromOpenNarcWithHardwareMappingType(spriteSystem, spriteManager, narc, GetMonIconNaixEx(species, e, f), FALSE, NNS_G2D_VRAM_TYPE_2DMAIN, index + 100000);
    template.x = 0;
    template.y = -48;
    template.z = 0;
    template.animation = 0;
    template.drawPriority = 0;
    template.pal = 0;
    template.vram = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.bgPriority = 0;
    template.vramTransfer = 0;
    template.resIdList[0] = index + 100000;
    template.resIdList[1] = 100000;
    template.resIdList[2] = 100000;
    template.resIdList[3] = 100000;
    template.resIdList[4] = -1;
    template.resIdList[5] = -1;
    sprite = SpriteSystem_NewSprite(spriteSystem, spriteManager, &template);
    ((void (*)(ManagedSprite *, int))ManagedSprite_SetPaletteOverrideOffset)(sprite, GetMonIconPaletteEx(species, f, e) + 4);
    ManagedSprite_SetAnim(sprite, 1);
    ManagedSprite_TickFrame(sprite);
    return sprite;
}

#pragma opt_common_subs reset
#pragma opt_propagation reset
#pragma opt_dead_assignments reset
#pragma opt_lifetimes reset

void VsRecorder_DeleteMonIcon(VsRecorderWork *work, u32 index, ManagedSprite *sprite) {
    if (sprite != NULL) {
        SpriteManager_UnloadCharObjById(work->spriteManager, 100000 + index);
        Sprite_DeleteAndFreeResources(sprite);
    }
}

void VsRecorder_DeleteSprite(void *unused0, void *unused1, ManagedSprite *sprite) {
    if (sprite != NULL) {
        Sprite_DeleteAndFreeResources(sprite);
    }
}

static void VsRecorder_PrintVideoInfo(VsRecorderVideoInfo *info, VsRecorderWork *work) {
    VsRecorderWindowRectSet templates = sVsRecorderVideoInfoLabelWindows;
    VsRecorderMsgIdSet msgIds = sVsRecorderVideoInfoLabelMsgIds;
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
    bodyStr = NewString_ReadMsgData(work->msgData, msg_0013_00007);
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
        unitStr = NewString_ReadMsgData(work->msgData, msg_0013_00009);
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
        str = NewString_ReadMsgData(work->msgData, msg_0013_00012);
    } else {
        str = NewString_ReadMsgData(work->msgData, msg_0013_00011);
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

static void VsRecorder_RemoveVideoInfoWindows(VsRecorderVideoInfo *info, VsRecorderWork *work) {
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

BOOL VsRecorder_HeaderUsesThreeMonLayout(void *header) {
    switch ((int)BattleRecordHeader_GetField(header, 3, 0)) {
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

static BOOL VsRecorderVideoInfo_UsesThreeMonLayout(VsRecorderVideoInfo *info) {
    return VsRecorder_HeaderUsesThreeMonLayout(info->header);
}

int VsRecorderVideoInfo_GetScreenId(VsRecorderVideoInfo *info) {
    return VsRecorderVideoInfo_UsesThreeMonLayout(info) == TRUE ? 0x51 : 0x52;
}

static void VsRecorder_CreateVideoPartyIcons(VsRecorderVideoInfo *info, VsRecorderWork *work) {
    int mon;
    int count = 0;
    VsRecorderIconLayout layout = sVsRecorderVideoPartyIconLayout;
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
    if (VsRecorderVideoInfo_UsesThreeMonLayout(info) == TRUE) {
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

void VsRecorder_CreateVideoInfo(VsRecorderVideoInfo *info, VsRecorderWork *work) {
    VsRecorder_PrintVideoInfo(info, work);
    VsRecorder_CreateVideoPartyIcons(info, work);
}

void VsRecorder_FreeVideoInfo(VsRecorderVideoInfo *info, VsRecorderWork *work) {
    int i;
    SpriteManager_UnloadPlttObjById(work->spriteManager, 100000);
    SpriteManager_UnloadCellObjById(work->spriteManager, 100000);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 100000);
    i = 0;
    do {
        if (info->monIcons[i]) {
            SpriteManager_UnloadCharObjById(work->spriteManager, i + 100000);
            Sprite_DeleteAndFreeResources(info->monIcons[i]);
            info->monIcons[i] = NULL;
        }
        i++;
    } while (i < 12);
    VsRecorder_RemoveVideoInfoWindows(info, work);
}

void VsRecorder_SetVideoPartyIconsDrawFlag(VsRecorderVideoInfo *info, BOOL draw) {
    int i;
    for (i = 0; i < 12; i++) {
        ManagedSprite *sprite = info->monIcons[i];
        if (sprite != NULL) {
            ManagedSprite_SetDrawFlag(sprite, draw);
        }
    }
}

int VsRecorder_GetCenteredTextX(Window *window, String *str) {
    int width = FontID_String_GetWidth(0, str, 0);
    return (GetWindowWidth(window) * 8 - width) / 2;
}

void VsRecorder_ResetRecordSave(VsRecorderWork *work) {
    work->recordSaveState = 0;
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
static void VsRecorder_TouchIndicatorTask(SysTask *task, void *data) {
    VsRecorderWork *work = data;
    s16 radius;
    work->touchIndicator.counter++;
    if (work->touchIndicator.counter >= 7) {
        SysTask_Destroy(task);
        work->touchIndicator.counter = 0;
        work->touchIndicator.active = FALSE;
        OrbitCursor_SetDefaultAnim(work->touchIndicator.cursor);
        OrbitCursor_SetRadius(work->touchIndicator.cursor, 0, 0);
        OrbitCursor_SetActive(work->touchIndicator.cursor, FALSE);
        OrbitCursor_SetVisible(work->touchIndicator.cursor, FALSE);
    } else {
        radius = (s16)(work->touchIndicator.counter * 2 + 6);
        OrbitCursor_SetRadius(work->touchIndicator.cursor, radius, radius);
    }
}

static void VsRecorder_ShowTouchIndicator(VsRecorderWork *work) {
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
    OrbitCursor_SetAltAnim(work->touchIndicator.cursor);
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

void VsRecorder_CreateLabeledSprite(VsRecorderWork *work, VsRecorderTextSprite *textSprite, NNS_G2D_VRAM_TYPE vram, int msgId, s16 x, s16 y, s16 textX, s16 textY, int anim, BOOL draw) {
    textSprite->sprite = VsRecorder_NewButtonSprite(work, vram);
    VsRecorder_CreateTextSprite(textSprite, (VsRecorderGfx *)&work->narc, vram);
    VsRecorder_SetTextSpriteMsg(textSprite, (VsRecorderGfx *)&work->narc, msgId);
    ManagedSprite_SetPositionXY(textSprite->sprite, x, y);
    TextOBJ_SetPosition(textSprite->text, textX, textY);
    TextOBJ_SetSpritesDrawFlag(textSprite->text, draw);
    ManagedSprite_SetAnim(textSprite->sprite, anim);
}

void VsRecorder_FreeLabeledSprite(VsRecorderTextSprite *textSprite) {
    VsRecorder_FreeTextSprite(textSprite);
    Sprite_DeleteAndFreeResources(textSprite->sprite);
}

// message of the message screen: type 0 / 1 by index, type 2 by group and index
u32 VsRecorder_SetupMessageScreenMsg(VsRecorderWork *work, u32 group, u32 index, u32 type) {
    u32 result;
    work->messageScreen.savedSubState = work->subState;
    work->messageScreen.savedStep = work->step;
    work->messageScreen.returnScreen = *work->screenPtr;
    work->messageScreen.active = TRUE;
    result = msg_0013_00194;
    switch (type) {
    case 0:
        switch (index) {
        case 0:
            result = msg_0013_00244;
            break;
        case 1:
            result = msg_0013_00245;
            break;
        case 2:
            result = msg_0013_00246;
            break;
        case 3:
            result = msg_0013_00247;
            break;
        case 4:
            result = msg_0013_00248;
            break;
        case 5:
            result = msg_0013_00249;
            break;
        case 6:
            result = msg_0013_00250;
            break;
        case 7:
            result = msg_0013_00251;
            break;
        case 8:
            result = msg_0013_00252;
            break;
        case 9:
            result = msg_0013_00253;
            break;
        case 10:
            result = msg_0013_00254;
            break;
        case 11:
            result = msg_0013_00255;
            break;
        case 12:
            result = msg_0013_00256;
            break;
        case 13:
            result = msg_0013_00257;
            break;
        case 14:
            result = msg_0013_00258;
            break;
        default:
            GF_AssertFail();
            break;
        }
        break;
    case 1:
        switch (index) {
        case 0:
            result = msg_0013_00259;
            break;
        case 1:
            result = msg_0013_00260;
            break;
        case 2:
            result = msg_0013_00261;
            break;
        case 3:
            result = msg_0013_00262;
            break;
        case 4:
            result = msg_0013_00263;
            break;
        case 5:
            result = msg_0013_00264;
            break;
        case 6:
            result = msg_0013_00265;
            break;
        case 7:
            result = msg_0013_00266;
            break;
        case 8:
            result = msg_0013_00267;
            break;
        case 9:
            result = msg_0013_00268;
            break;
        default:
            GF_AssertFail();
            break;
        }
        break;
    case 2:
        switch (group) {
        case VS_RECORDER_REQUEST_DRESS_UP_UPLOAD:
            switch (index) {
            case 0:
                result = msg_0013_00194;
                break;
            case 1:
                result = msg_0013_00195;
                break;
            case 2:
                result = msg_0013_00196;
                break;
            case 3:
                result = msg_0013_00197;
                break;
            case 4:
                result = msg_0013_00198;
                break;
            case 5:
                result = msg_0013_00199;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case VS_RECORDER_REQUEST_DRESS_UP_DOWNLOAD:
            switch (index) {
            case 0:
                result = msg_0013_00200;
                break;
            case 1:
                result = msg_0013_00201;
                break;
            case 2:
                result = msg_0013_00202;
                break;
            case 3:
                result = msg_0013_00203;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case VS_RECORDER_REQUEST_BOX_UPLOAD:
            switch (index) {
            case 0:
                result = msg_0013_00204;
                break;
            case 1:
                result = msg_0013_00205;
                break;
            case 2:
                result = msg_0013_00206;
                break;
            case 3:
                result = msg_0013_00207;
                break;
            case 4:
                result = msg_0013_00208;
                break;
            case 5:
                result = msg_0013_00209;
                break;
            case 6:
                result = msg_0013_00210;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case VS_RECORDER_REQUEST_BOX_DOWNLOAD:
            switch (index) {
            case 0:
                result = msg_0013_00211;
                break;
            case 1:
                result = msg_0013_00212;
                break;
            case 2:
                result = msg_0013_00213;
                break;
            case 3:
                result = msg_0013_00214;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case VS_RECORDER_REQUEST_RANKINGS_HEADER:
            switch (index) {
            case 0:
                result = msg_0013_00215;
                break;
            case 1:
                result = msg_0013_00216;
                break;
            case 2:
                result = msg_0013_00217;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case VS_RECORDER_REQUEST_RANKINGS:
            switch (index) {
            case 0:
                result = msg_0013_00218;
                break;
            case 1:
                result = msg_0013_00219;
                break;
            case 2:
                result = msg_0013_00220;
                break;
            case 3:
                result = msg_0013_00221;
                break;
            case 4:
                result = msg_0013_00222;
                break;
            case 5:
                result = msg_0013_00223;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case VS_RECORDER_REQUEST_VIDEO_UPLOAD:
            switch (index) {
            case 0:
                result = msg_0013_00224;
                break;
            case 1:
                result = msg_0013_00225;
                break;
            case 2:
                result = msg_0013_00226;
                break;
            case 3:
                result = msg_0013_00227;
                break;
            case 4:
                result = msg_0013_00228;
                break;
            case 5:
                result = msg_0013_00229;
                break;
            case 6:
                result = msg_0013_00230;
                break;
            case 7:
                result = msg_0013_00231;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case VS_RECORDER_REQUEST_VIDEO_SEARCH:
            switch (index) {
            case 0:
                result = msg_0013_00232;
                break;
            case 1:
                result = msg_0013_00233;
                break;
            case 2:
                result = msg_0013_00234;
                break;
            case 3:
                result = msg_0013_00235;
                break;
            default:
                GF_AssertFail();
                break;
            }
            break;
        case VS_RECORDER_REQUEST_VIDEO_DOWNLOAD:
            switch (index) {
            case 0:
                result = msg_0013_00236;
                break;
            case 1:
                result = msg_0013_00237;
                break;
            case 2:
                result = msg_0013_00238;
                break;
            case 3:
                result = msg_0013_00239;
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

// switch to the message screen; it returns to the current screen after a touch
void VsRecorder_ShowMessageScreen(VsRecorderWork *work, int group, u32 index, u32 type) {
    work->messageScreen.savedSubState = work->subState;
    work->messageScreen.savedStep = work->step;
    work->messageScreen.returnScreen = *work->screenPtr;
    work->messageScreen.step = VS_RECORDER_MESSAGE_SCREEN_SHOW;
    work->messageScreen.timer = 0;
    work->messageScreen.msgId = VsRecorder_SetupMessageScreenMsg(work, group, index, type);
    work->messageScreen.active = TRUE;
    *work->screenPtr = VS_RECORDER_SCREEN_MESSAGE;
}

void VsRecorder_MessageScreen(VsRecorderWork *work) {
    switch (work->messageScreen.step) {
    case VS_RECORDER_MESSAGE_SCREEN_SHOW:
        if (work->messageScreen.timer < 8) {
            work->messageScreen.timer = work->messageScreen.timer + 1;
        } else {
            VsRecorder_ShowTopMessage(work, work->messageScreen.msgId);
            work->messageScreen.step = work->messageScreen.step + 1;
        }
        break;
    case VS_RECORDER_MESSAGE_SCREEN_WAIT_TOUCH:
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

    do {
        void *header = work->savedVideos[i];
        if (header != NULL && BattleRecordHeader_GetField(header, 4, 0) == number) {
            return TRUE;
        }
        i++;
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

static void VsRecorder_SetPlaceholderName(String *str, enum HeapID heapID) {
    MsgData *msgData;
    String_SetEmpty(str);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0013_bin, heapID);
    ReadMsgDataIntoString(msgData, msg_0013_00332, str);
    DestroyMsgData(msgData);
}

void VsRecorder_ShowTouchHere(VsRecorderWork *work) {
    Window *window = &work->messageWindow;
    String *str;
    InitWindow(window);
    AddWindowParameterized(work->bgConfig, window, GF_BG_LYR_MAIN_2, 8, 0x10, 0x10, 2, 0xE, 0x12C);
    FillWindowPixelBuffer(window, 0);
    str = NewString_ReadMsgData(work->msgData, msg_0013_00192);
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
