#include <nitro/gx/g2.h>

#include "global.h"

#include "application/vs_recorder/vs_recorder_internal.h"

#include "assert.h"
#include "bg_window.h"
#include "font_sprite.h"
#include "gf_gfx_loader.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "palette.h"
#include "render_text.h"
#include "sprite_system.h"
#include "string.h"

// VS Recorder screen framework, part 2: menu row labels, text sprites, menu row rebuild, alpha blend / fade
// steps and the colour-scheme palettes.

typedef struct VsRecorderSchemeTable {
    u32 values[7];
} VsRecorderSchemeTable;

// per colour scheme (VsRecorderWork.colorScheme)
extern const u32 ov40_02244E40[7];
extern const u32 ov40_02244E5C[7]; // blend colour
extern const u32 ov40_02244E78[7]; // BG palette file
extern const u32 ov40_02244E94[7];
extern const u32 ov40_02244EB0[7]; // BG frame palette file
extern const u32 ov40_02244ECC[7]; // OBJ palette file

void OrbitCursor_ReloadPalette(void *banner, int colorScheme);

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
            String *template = NewString_ReadMsgData(work->msgData, 7);
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
            String *template = NewString_ReadMsgData(work->msgData, 7);
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
    struct {
        int i;
        VsRecorderMenuRow *rows[2];
    } local;

    int j;
    VsRecorderMenuRow *choiceRow;
    VsRecorderMenuRow *pathRow;
    VsRecorderMenuRow **cursor;

    local.i = 0;
    choiceRow = work->choiceRows;
    pathRow = work->pathRows;
    do {
        local.rows[0] = choiceRow;
        local.rows[1] = pathRow;
        j = 0;
        cursor = local.rows;
        while (j < 2) {
            TextOBJLayout_Delete((*cursor)->textLayout);
            TextOBJ_Destroy((*cursor)->text);
            sub_02021B5C(&(*cursor)->charAlloc);
            j++;
            cursor++;
        }
        choiceRow++;
        pathRow++;
        local.i++;
    } while (local.i < 5);
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
    sub_02021AC8(FontOAM_GetCharSize(&window, vram, HEAP_ID_109), TRUE, vram, &textSprite->charAlloc);
    ManagedSprite_GetPositionXY(textSprite->sprite, &position[1], &position[0]);
    template.fontSystem = gfx->fontSystem;
    template.window = &window;
    template.spriteList = SpriteManager_GetSpriteList(gfx->spriteManager);
    template.plttResourceProxy = SpriteManager_FindPlttResourceProxy(gfx->spriteManager, plttResId);
    template.sprite = textSprite->sprite->sprite;
    template.offset = textSprite->charAlloc.offset;
    template.x = position[1] + 0x24;
    template.y = position[0] - 8;
    template.unk_20 = 3;
    template.unk_24 = 0;
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
    sub_02021B5C(&textSprite->charAlloc);
}

void ov40_0222D6EC(VsRecorderWork *work, int vram) {
    NARC *narc = work->narc;
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x31, FALSE, vram, 11924);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x1A, FALSE, 11924);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x1B, FALSE, 11924);
}

void ov40_0222D73C(VsRecorderWork *work, int vram) {
    NARC *narc = work->narc;
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x7A, FALSE, vram, 11924);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x7B, FALSE, 11924);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x7C, FALSE, 11924);
}

void ov40_0222D78C(VsRecorderWork *work, int vram) {
    NARC *narc = work->narc;
    SpriteSystem *spriteSystem = work->spriteSystem;
    SpriteManager *spriteManager = work->spriteManager;

    SpriteSystem_LoadCharResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x1E, FALSE, vram, 11924);
    SpriteSystem_LoadCellResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x20, FALSE, 11924);
    SpriteSystem_LoadAnimResObjFromOpenNarc(spriteSystem, spriteManager, narc, 0x1F, FALSE, 11924);
}

void ov40_0222D7DC(VsRecorderWork *work) {
    SpriteManager_UnloadCharObjById(work->spriteManager, 11924);
    SpriteManager_UnloadCellObjById(work->spriteManager, 11924);
    SpriteManager_UnloadAnimObjById(work->spriteManager, 11924);
}

ManagedSprite *ov40_0222D800(VsRecorderWork *work, NNS_G2D_VRAM_TYPE vram) {
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
    sub_02088030(work);
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
    sub_02088030(work);
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

#pragma force_active on

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

#pragma force_active reset

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
    VsRecorderSchemeTable table = *(const VsRecorderSchemeTable *)ov40_02244E5C;
    return table.values[work->colorScheme];
}

u32 ov40_0222DAF0(VsRecorderWork *work) {
    VsRecorderSchemeTable table = *(const VsRecorderSchemeTable *)ov40_02244E94;
    if (work->mode != 0) {
        return 0x7E05;
    }
    return table.values[work->colorScheme];
}

// NARC_a_1_9_1 palette file of the colour scheme: 0 BG frame, 1 ?, 2 OBJ, 3 BG
u32 VsRecorder_GetSchemeFileId(VsRecorderWork *work, u32 kind) {
    VsRecorderSchemeTable bgFrame = *(const VsRecorderSchemeTable *)ov40_02244EB0;
    VsRecorderSchemeTable b = *(const VsRecorderSchemeTable *)ov40_02244E40;
    VsRecorderSchemeTable obj = *(const VsRecorderSchemeTable *)ov40_02244ECC;
    VsRecorderSchemeTable bg = *(const VsRecorderSchemeTable *)ov40_02244E78;
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

#pragma force_active on

u16 *ov40_0222DD68(enum HeapID heapID, int unused, int *count) {
    u32 size;
    u16 *data = GfGfxLoader_LoadFromNarc_GetSizeOut(NARC_application_zukanlist_zkn_data_zukan_data, 13, FALSE, heapID, FALSE, &size);
    *count = size >> 1;
    return data;
}

#pragma force_active reset

void ov40_0222DD8C(VsRecorderWork *work, u32 value) {
    work->unk_0870 = value;
}

u32 ov40_0222DD94(VsRecorderWork *work) {
    return work->unk_0870;
}
