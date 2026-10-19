#include "application/choose_starter/choose_starter_sub_res.naix"
#include "msgdata/msg.naix"
#include "msgdata/msg/msg_0278.h"
#include "pokeathlon/pokeathlon_box.h"

#include "char_tiles.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "pokemon.h"
#include "sprite_transfer.h"
#include "text.h"

static void PokeathlonBox_DeleteSprites(PokeathlonBox_SubGraphics *subGraphics);
static void PokeathlonBox_LoadBgGraphics(BgConfig *bgConfig, enum HeapID heapID);
static void PokeathlonBox_InitBgFromTemplates(BgConfig *bgConfig);
static void PokeathlonBox_PrintStaticTexts(PokeathlonBox_Graphics *graphics);
static void PokeathlonBox_PrintMonTexts(PokeathlonBox_Graphics *graphics, PokeathlonBox_MonInfo *mon);
static void PokeathlonBox_ShowMonSprites(PokeathlonBox_Graphics *graphics, PokeathlonBox_MonInfo *mon);
static void PokeathlonBox_ShowStatStars(PokeathlonBox_SubGraphics *subGraphics, PokeathlonBox_MonInfo *mon);
static void PokeathlonBox_HideMonSprites();
static void PokeathlonBox_LoadSpriteResources(PokeathlonBox_SubGraphics *subGraphics);
static void PokeathlonBox_CreateSprites(PokeathlonBox_SubGraphics *subGraphics);
static Sprite *PokeathlonBox_CreateSprite(PokeathlonBox_SubGraphics *sub, int resourceId, int drawPriority, int priority, int x, int y, int anim, BOOL draw);
static void PokeathlonBox_InitSpriteTemplate(SpriteTemplate *template, SpriteList *spriteList, SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen, enum HeapID heapID);
static void PokeathlonBox_GetMonPicVramOffsets(PokeathlonBox_SubGraphics *sub);
static void PokeathlonBox_LoadMonPic();
static void PokeathlonBox_TransferMonPic(PokeathlonBox_SubGraphics *subGraphics);

static const u8 sPokeathlonBoxMonSpriteRows[5] = { 1, 3, 2, 4, 0 };

static const u8 sPokeathlonBoxSpriteRows[5] = { 1, 3, 2, 4, 0 };

static const BgTemplate sPokeathlonBoxBgTemplate_Sub0 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe000,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 0,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = FALSE,
};

static const BgTemplate sPokeathlonBoxBgTemplate_Sub2 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf000,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_23,
    .priority = 2,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = FALSE,
};

static const BgTemplate sPokeathlonBoxBgTemplate_Sub1 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xe800,
    .charBase = GX_BG_CHARBASE_0x00000,
    .bgExtPltt = GX_BG_EXTPLTT_01,
    .priority = 1,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = FALSE,
};

static const BgTemplate sPokeathlonBoxBgTemplate_Sub3 = {
    .x = 0,
    .y = 0,
    .bufferSize = 0x800,
    .baseTile = 0,
    .size = GF_BG_SCR_SIZE_256x256,
    .colorMode = GX_BG_COLORMODE_16,
    .screenBase = GX_BG_SCRBASE_0xf800,
    .charBase = GX_BG_CHARBASE_0x08000,
    .bgExtPltt = GX_BG_EXTPLTT_23,
    .priority = 3,
    .areaOver = GX_BG_AREAOVER_XLU,
    .dummy = 0,
    .mosaic = FALSE,
};

PokeathlonBox_Graphics *PokeathlonBox_InitGraphics(enum HeapID heapID) {
    PokeathlonBox_Graphics *ptr = Heap_Alloc(heapID, sizeof(PokeathlonBox_Graphics));
    MI_CpuFill8(ptr, 0, sizeof(PokeathlonBox_Graphics));
    ptr->heapID = heapID;
    ptr->sub.heapID = heapID;
    return ptr;
}

void PokeathlonBox_SetupGraphics(PokeathlonBox_Graphics *graphics, BgConfig *bgConfig, u8 courseId, u32 isSupreme) {
    graphics->bgConfig = bgConfig;
    PokeathlonBox_LoadBgGraphics(bgConfig, graphics->heapID);
    AddWindowParameterized(graphics->bgConfig, &graphics->window1, GF_BG_LYR_SUB_0, 1, 1, 17, 2, 15, 0x01);
    AddWindowParameterized(graphics->bgConfig, &graphics->window2, GF_BG_LYR_SUB_0, 22, 1, 9, 2, 15, 0x23);
    AddWindowParameterized(graphics->bgConfig, &graphics->window3, GF_BG_LYR_SUB_1, 1, 15, 8, 2, 15, 0x35);
    AddWindowParameterized(graphics->bgConfig, &graphics->window4, GF_BG_LYR_SUB_1, 9, 15, 3, 2, 15, 0x45);
    AddWindowParameterized(graphics->bgConfig, &graphics->window5, GF_BG_LYR_SUB_1, 13, 5, 10, 10, 15, 0x4B);
    AddWindowParameterized(graphics->bgConfig, &graphics->window6, GF_BG_LYR_SUB_1, 1, 19, 12, 4, 15, 0xAF);
    graphics->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0278_bin, graphics->heapID);
    graphics->msgFormat = MessageFormat_New(graphics->heapID);
    graphics->nickname = String_New(11, graphics->heapID);
    graphics->courseId = courseId;
    graphics->isSupreme = isSupreme;
    PokeathlonBox_LoadSpriteResources(&graphics->sub);
    PokeathlonBox_CreateSprites(&graphics->sub);
    PokeathlonBox_PrintStaticTexts(graphics);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

void PokeathlonBox_DrawMonInfo(PokeathlonBox_Graphics *graphics, PokeathlonBox_MonInfo *mon) {
    if (mon->displayMode == 2) {
        PokeathlonBox_ShowMonSprites(graphics, mon);
        PokeathlonBox_PrintMonTexts(graphics, mon);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_ON);
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_ON);
        return;
    }
    PokeathlonBox_HideMonSprites();
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG1, GF_PLANE_TOGGLE_OFF);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_BG2, GF_PLANE_TOGGLE_OFF);
}

void PokeathlonBox_RenderAndAnimateSprites(PokeathlonBox_Graphics *graphics) {
    if (graphics->sub.spriteList != NULL) {
        SpriteList_RenderAndAnimateSprites(graphics->sub.spriteList);
    }
}

void PokeathlonBox_ReleaseGraphics(PokeathlonBox_Graphics *graphics) {
    u8 i;

    DestroyMsgData(graphics->msgData);
    MessageFormat_Delete(graphics->msgFormat);
    String_Delete(graphics->nickname);
    RemoveWindow(&graphics->window1);
    RemoveWindow(&graphics->window2);
    RemoveWindow(&graphics->window3);
    RemoveWindow(&graphics->window4);
    RemoveWindow(&graphics->window5);
    RemoveWindow(&graphics->window6);
    FreeBgTilemapBuffer(graphics->bgConfig, GF_BG_LYR_SUB_0);
    FreeBgTilemapBuffer(graphics->bgConfig, GF_BG_LYR_SUB_1);
    FreeBgTilemapBuffer(graphics->bgConfig, GF_BG_LYR_SUB_2);
    FreeBgTilemapBuffer(graphics->bgConfig, GF_BG_LYR_SUB_3);

    PokeathlonBox_SubGraphics *sub = &graphics->sub;
    PokeathlonBox_DeleteSprites(sub);
    for (i = 0; i < 3; i++) {
        SpriteTransfer_DeleteCharTransferTask(sub->resources[i].charResObj);
        SpriteTransfer_DeletePlttTransferTask(sub->resources[i].plttResObj);
    }
    for (i = 0; i < 6; i++) {
        Destroy2DGfxResObjMan(sub->mgrs[i]);
    }
    SpriteList_Delete(sub->spriteList);
    Heap_Free(graphics);
}

static void PokeathlonBox_DeleteSprites(PokeathlonBox_SubGraphics *subGraphics) {
    int i, j;

    for (i = 0; i < 5; i++) {
        Sprite_Delete(subGraphics->sprites[i]);
    }
    for (i = 0; i < 5; i++) {
        Sprite_Delete(subGraphics->statRows[i].sprite);
        for (j = 0; j < 5; j++) {
            Sprite_Delete(subGraphics->statRows[i].sprites[j]);
        }
    }

    Sprite_Delete(subGraphics->monPicSprite);
    Heap_Free(subGraphics->monPicChars);
}

static void PokeathlonBox_LoadBgGraphics(BgConfig *bgConfig, enum HeapID heapID) {
    PokeathlonBox_InitBgFromTemplates(bgConfig);
    GfGfxLoader_GXLoadPal(NARC_a_2_4_1, 0, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0, 0, heapID);
    GfGfxLoader_LoadCharData(NARC_a_2_4_1, 1, bgConfig, GF_BG_LYR_SUB_3, 0, 0, 1, heapID);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_1, 2, bgConfig, GF_BG_LYR_SUB_3, 0, 0x600, 1, heapID);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_SUB_3);
    GfGfxLoader_LoadScrnData(NARC_a_2_4_1, 3, bgConfig, GF_BG_LYR_SUB_2, 0, 0x600, 1, heapID);
    BgCommitTilemapBufferToVram(bgConfig, GF_BG_LYR_SUB_2);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, heapID);
}

static void PokeathlonBox_InitBgFromTemplates(BgConfig *bgConfig) {
    BgTemplate bgTemplate1 = sPokeathlonBoxBgTemplate_Sub0;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_0, &bgTemplate1, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_0);

    BgTemplate bgTemplate2 = sPokeathlonBoxBgTemplate_Sub1;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_1, &bgTemplate2, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_1);

    BgTemplate bgTemplate3 = sPokeathlonBoxBgTemplate_Sub2;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_2, &bgTemplate3, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_2);

    BgTemplate bgTemplate4 = sPokeathlonBoxBgTemplate_Sub3;
    InitBgFromTemplate(bgConfig, GF_BG_LYR_SUB_3, &bgTemplate4, 0);
    BgClearTilemapBufferAndCommit(bgConfig, GF_BG_LYR_SUB_3);
}

static void PokeathlonBox_PrintStaticTexts(PokeathlonBox_Graphics *graphics) {
    BufferPokeathlonCourseName(graphics->msgFormat, 0, (u8)graphics->courseId);
    String *str = ReadMsgData_ExpandPlaceholders(graphics->msgFormat, graphics->msgData, msg_0278_00000, graphics->heapID);
    FillWindowPixelBuffer(&graphics->window1, 0);
    AddTextPrinterParameterizedWithColor(&graphics->window1, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    CopyWindowToVram(&graphics->window1);
    String_Delete(str);
    FillWindowPixelBuffer(&graphics->window2, 0);
    if (graphics->isSupreme != 0) {
        str = NewString_ReadMsgData(graphics->msgData, msg_0278_00001);
        AddTextPrinterParameterizedWithColor(&graphics->window2, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
        String_Delete(str);
    }
    CopyWindowToVram(&graphics->window2);
    FillWindowPixelBuffer(&graphics->window5, 0);
    str = NewString_ReadMsgData(graphics->msgData, msg_0278_00008);
    AddTextPrinterParameterizedWithColor(&graphics->window5, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(str);
    str = NewString_ReadMsgData(graphics->msgData, msg_0278_00004);
    AddTextPrinterParameterizedWithColor(&graphics->window5, 0, str, 0, 16, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(str);
    str = NewString_ReadMsgData(graphics->msgData, msg_0278_00006);
    AddTextPrinterParameterizedWithColor(&graphics->window5, 0, str, 0, 32, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(str);
    str = NewString_ReadMsgData(graphics->msgData, msg_0278_00005);
    AddTextPrinterParameterizedWithColor(&graphics->window5, 0, str, 0, 48, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(str);
    str = NewString_ReadMsgData(graphics->msgData, msg_0278_00007);
    AddTextPrinterParameterizedWithColor(&graphics->window5, 0, str, 0, 64, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), NULL);
    String_Delete(str);
    CopyWindowToVram(&graphics->window5);
}

static void PokeathlonBox_PrintMonTexts(PokeathlonBox_Graphics *graphics, PokeathlonBox_MonInfo *mon) {
    String *str;

    CopyU16ArrayToString(graphics->nickname, mon->nickname);
    FillWindowPixelBuffer(&graphics->window3, 0);
    AddTextPrinterParameterizedWithColor(&graphics->window3, 0, graphics->nickname, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    CopyWindowToVram(&graphics->window3);
    if (mon->gender == 0) {
        str = NewString_ReadMsgData(graphics->msgData, msg_0278_00002);
        FillWindowPixelBuffer(&graphics->window4, 0);
        AddTextPrinterParameterizedWithColor(&graphics->window4, 0, str, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(7, 8, 0), 0);
        CopyWindowToVram(&graphics->window4);
        String_Delete(str);
    } else if (mon->gender == 1) {
        str = NewString_ReadMsgData(graphics->msgData, msg_0278_00003);
        FillWindowPixelBuffer(&graphics->window4, 0);
        AddTextPrinterParameterizedWithColor(&graphics->window4, 0, str, 4, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(3, 4, 0), 0);
        CopyWindowToVram(&graphics->window4);
        String_Delete(str);
    } else {
        FillWindowPixelBuffer(&graphics->window4, 0);
        CopyWindowToVram(&graphics->window4);
    }
    str = GetSpeciesName(mon->species, graphics->heapID);
    BufferString(graphics->msgFormat, 0, str, 2, 1, 2);
    String_Delete(str);
    str = ReadMsgData_ExpandPlaceholders(graphics->msgFormat, graphics->msgData, msg_0278_00009, graphics->heapID);
    FillWindowPixelBuffer(&graphics->window6, 0);
    AddTextPrinterParameterizedWithColor(&graphics->window6, 0, str, 0, 0, TEXT_SPEED_NOTRANSFER, MAKE_TEXT_COLOR(1, 2, 0), 0);
    CopyWindowToVram(&graphics->window6);
    String_Delete(str);
}

static void PokeathlonBox_ShowMonSprites(PokeathlonBox_Graphics *graphics, PokeathlonBox_MonInfo *mon) {
    u8 i;
    PokeathlonBox_SubGraphics *sub = &graphics->sub;
    u8 medalSlots[5];

    PokeathlonBox_LoadMonPic(&graphics->sub);
    PokeathlonBox_TransferMonPic(&graphics->sub);
    Sprite_SetDrawFlag(graphics->sub.monPicSprite, 1);
    PokeathlonBox_ShowStatStars(&graphics->sub, mon);
    medalSlots = sPokeathlonBoxMonSpriteRows;
    for (i = 0; i < 5; i++) {
        if (mon->medals[medalSlots[i]] != 0) {
            Sprite_SetDrawFlag(sub->sprites[medalSlots[i]], 1);
        } else {
            Sprite_SetDrawFlag(sub->sprites[medalSlots[i]], 0);
        }
    }
}

// The direction sprite of stat row i.
#define STAT_ROW_SPRITE(subGraphics, i) (((PokeathlonBox_SubGraphics *)((u8 *)(subGraphics) + (i) * sizeof(PokeathlonBox_StatRow)))->statRows[0].sprite)

static void PokeathlonBox_ShowStatStars(PokeathlonBox_SubGraphics *subGraphics, PokeathlonBox_MonInfo *mon) {
    u8 i;
    u8 j;

    for (i = 0; i < 5; i++) {
        const PokeathlonBox_MonInfo *shifted = (const PokeathlonBox_MonInfo *)((const u8 *)mon + i * sizeof(PokeathlonBox_StatChange));
        s8 direction = shifted->statChanges[0].direction;
        if (direction > 0) {
            Sprite_SetDrawFlag(STAT_ROW_SPRITE(subGraphics, i), 1);
            Sprite_SetAnimCtrlSeq(STAT_ROW_SPRITE(subGraphics, i), 4);
        } else if (direction < 0) {
            Sprite_SetDrawFlag(STAT_ROW_SPRITE(subGraphics, i), 1);
            Sprite_SetAnimCtrlSeq(STAT_ROW_SPRITE(subGraphics, i), 5);
        } else {
            Sprite_SetDrawFlag(STAT_ROW_SPRITE(subGraphics, i), 0);
        }
        for (j = 0; j < 5; j++) {
            switch (shifted->statChanges[0].stars[j]) {
            case 0:
                Sprite_SetDrawFlag(subGraphics->statRows[i].sprites[j], 0);
                break;
            case 1:
                Sprite_SetDrawFlag(subGraphics->statRows[i].sprites[j], 1);
                Sprite_SetAnimCtrlSeq(subGraphics->statRows[i].sprites[j], 3);
                break;
            case 2:
                Sprite_SetDrawFlag(subGraphics->statRows[i].sprites[j], 1);
                Sprite_SetAnimCtrlSeq(subGraphics->statRows[i].sprites[j], 0);
                break;
            case 3:
                Sprite_SetDrawFlag(subGraphics->statRows[i].sprites[j], 1);
                Sprite_SetAnimCtrlSeq(subGraphics->statRows[i].sprites[j], 1);
                break;
            case 4:
                Sprite_SetDrawFlag(subGraphics->statRows[i].sprites[j], 1);
                Sprite_SetAnimCtrlSeq(subGraphics->statRows[i].sprites[j], 2);
                break;
            default:
                GF_AssertFail();
                break;
            }
        }
    }
}

static void PokeathlonBox_HideMonSprites(PokeathlonBox_Graphics *graphics) {
    PokeathlonBox_SubGraphics *sub = &graphics->sub;
    u8 i;
    u8 j;

    for (i = 0; i < 5; i++) {
        Sprite_SetDrawFlag(sub->sprites[i], FALSE);
        Sprite_SetDrawFlag(sub->unk1C8[i], FALSE);
    }
    for (i = 0; i < 5; i++) {
        Sprite_SetDrawFlag(sub->statRows[i].sprite, FALSE);
        for (j = 0; j < 5; j++) {
            Sprite_SetDrawFlag(sub->statRows[i].sprites[j], FALSE);
        }
    }
    Sprite_SetDrawFlag(sub->monPicSprite, FALSE);
}

static void PokeathlonBox_LoadSpriteResources(PokeathlonBox_SubGraphics *sub) {
    int i;

    sub->spriteList = G2dRenderer_Init(66, &sub->renderer, sub->heapID);
    G2dRenderer_SetSubSurfaceCoords(&sub->renderer, 0, FX32_CONST(256));
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    for (i = 0; i < 6; i++) {
        sub->mgrs[i] = Create2DGfxResObjMan(3, (GfGfxResType)i, sub->heapID);
    }
    sub->resources[0].charResObj = AddCharResObjFromNarc(sub->mgrs[0], NARC_a_2_4_1, 7, TRUE, 1, NNS_G2D_VRAM_TYPE_2DSUB, sub->heapID);
    sub->resources[0].plttResObj = AddPlttResObjFromNarc(sub->mgrs[1], NARC_a_2_4_1, 4, FALSE, 1, NNS_G2D_VRAM_TYPE_2DSUB, 4, sub->heapID);
    sub->resources[0].cellResObj = AddCellOrAnimResObjFromNarc(sub->mgrs[2], NARC_a_2_4_1, 6, TRUE, 1, GF_GFX_RES_TYPE_CELL, sub->heapID);
    sub->resources[0].animResObj = AddCellOrAnimResObjFromNarc(sub->mgrs[3], NARC_a_2_4_1, 5, TRUE, 1, GF_GFX_RES_TYPE_ANIM, sub->heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(sub->resources[0].charResObj);
    SpriteTransfer_CreateExtPlttTransferTask(sub->resources[0].plttResObj);
    sub->resources[1].charResObj = AddCharResObjFromNarc(sub->mgrs[0], NARC_a_1_6_2, 66, FALSE, 2, NNS_G2D_VRAM_TYPE_2DSUB, sub->heapID);
    sub->resources[1].plttResObj = AddPlttResObjFromNarc(sub->mgrs[1], NARC_a_1_6_2, 65, FALSE, 2, NNS_G2D_VRAM_TYPE_2DSUB, 4, sub->heapID);
    sub->resources[1].cellResObj = AddCellOrAnimResObjFromNarc(sub->mgrs[2], NARC_a_1_6_2, 67, FALSE, 2, GF_GFX_RES_TYPE_CELL, sub->heapID);
    sub->resources[1].animResObj = AddCellOrAnimResObjFromNarc(sub->mgrs[3], NARC_a_1_6_2, 68, FALSE, 2, GF_GFX_RES_TYPE_ANIM, sub->heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(sub->resources[1].charResObj);
    SpriteTransfer_CreateExtPlttTransferTask(sub->resources[1].plttResObj);
    sub->resources[2].charResObj = AddCharResObjFromNarc(sub->mgrs[0], NARC_application_choose_starter_choose_starter_sub_res, NARC_choose_starter_sub_res_choose_starter_sub_res_00000009_NCGR, FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, sub->heapID);
    sub->resources[2].plttResObj = AddPlttResObjFromNarc(sub->mgrs[1], NARC_application_choose_starter_choose_starter_sub_res, NARC_choose_starter_sub_res_choose_starter_sub_res_00000006_NCLR, FALSE, 3, NNS_G2D_VRAM_TYPE_2DSUB, 4, sub->heapID);
    sub->resources[2].cellResObj = AddCellOrAnimResObjFromNarc(sub->mgrs[2], NARC_application_choose_starter_choose_starter_sub_res, NARC_choose_starter_sub_res_choose_starter_sub_res_00000010_NCER, FALSE, 3, GF_GFX_RES_TYPE_CELL, sub->heapID);
    sub->resources[2].animResObj = AddCellOrAnimResObjFromNarc(sub->mgrs[3], NARC_application_choose_starter_choose_starter_sub_res, NARC_choose_starter_sub_res_choose_starter_sub_res_00000010_NCER, FALSE, 3, GF_GFX_RES_TYPE_ANIM, sub->heapID);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(sub->resources[2].charResObj);
    SpriteTransfer_CreateExtPlttTransferTask(sub->resources[2].plttResObj);
}

static void PokeathlonBox_CreateSprites(PokeathlonBox_SubGraphics *sub) {
    u8 i;
    u8 j;
    int x;
    int y;

    for (i = 0; i < 5; i++) {
        x = i * 32 + 104;
        sub->sprites[i] = PokeathlonBox_CreateSprite(sub, 1, 2, 0, x, 168, i, FALSE);
        sub->unk1C8[i] = PokeathlonBox_CreateSprite(sub, 1, 1, 0, x, 168, 5, FALSE);
    }
    for (i = 0; i < 5; i++) {
        y = sPokeathlonBoxSpriteRows[i] * 16 + 48;
        sub->statRows[i].sprite = PokeathlonBox_CreateSprite(sub, 2, 1, 0, 96, y, 4, FALSE);
        for (j = 0; j < 5; j++) {
            sub->statRows[i].sprites[j] = PokeathlonBox_CreateSprite(sub, 2, 2, 0, j * 16 + 168, y, 3, FALSE);
        }
    }
    sub->monPicSprite = PokeathlonBox_CreateSprite(sub, 3, 0, 0, 48, 72, 0, FALSE);
    sub->monPicChars = Heap_Alloc(sub->heapID, 0xC80);
    PokeathlonBox_GetMonPicVramOffsets(sub);
}

static Sprite *PokeathlonBox_CreateSprite(PokeathlonBox_SubGraphics *sub, int resourceId, int drawPriority, int priority, int x, int y, int anim, BOOL draw) {
    SpriteTemplate template;
    SpriteResourcesHeader header;
    Sprite *sprite;

    CreateSpriteResourcesHeader(&header, resourceId, resourceId, resourceId, resourceId, -1, -1, FALSE, priority, sub->mgrs[0], sub->mgrs[1], sub->mgrs[2], sub->mgrs[3], NULL, NULL);
    PokeathlonBox_InitSpriteTemplate(&template, sub->spriteList, &header, NNS_G2D_VRAM_TYPE_2DSUB, sub->heapID);
    template.position.x = x << FX32_SHIFT;
    template.position.y = (y << FX32_SHIFT) + FX32_CONST(256);
    template.drawPriority = drawPriority;
    sprite = Sprite_CreateAffine(&template);
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetAnimCtrlSeq(sprite, anim);
    Sprite_SetPriority(sprite, 1);
    Sprite_SetDrawFlag(sprite, draw);
    return sprite;
}

static void PokeathlonBox_InitSpriteTemplate(SpriteTemplate *template, SpriteList *spriteList, SpriteResourcesHeader *header, NNS_G2D_VRAM_TYPE whichScreen, enum HeapID heapID) {
    template->spriteList = spriteList;
    template->header = header;
    template->position.z = 0;
    template->scale.x = FX32_ONE;
    template->scale.y = FX32_ONE;
    template->scale.z = FX32_ONE;
    template->rotation = 0;
    template->drawPriority = 0;
    template->whichScreen = whichScreen;
    template->heapID = heapID;
}

static void PokeathlonBox_GetMonPicVramOffsets(PokeathlonBox_SubGraphics *sub) {
    SpriteResource *plttResObj = sub->resources[2].plttResObj;
    NNSG2dImageProxy *charProxy = SpriteTransfer_GetCharProxy(sub->resources[2].charResObj);
    NNSG2dImagePaletteProxy *plttProxy = SpriteTransfer_GetPaletteProxy(plttResObj, charProxy);
    sub->monPicCharOffset = NNS_G2dGetImageLocation(charProxy, NNS_G2D_VRAM_TYPE_2DSUB);
    sub->monPicPlttOffset = NNS_G2dGetImagePaletteLocation(plttProxy, NNS_G2D_VRAM_TYPE_2DSUB);
}

// Loads the front sprite chars and palette of `mon` into the buffers drawn by PokeathlonBox_TransferMonPic.

static void PokeathlonBox_LoadMonPic(PokeathlonBox_SubGraphics *sub, PokeathlonBox_MonInfo *mon) {
    PokepicTemplate pokepic;
    NNSG2dPaletteData *plttData;
    void *file;

    GetMonSpriteCharAndPlttNarcIdsEx(&pokepic, mon->species, mon->gender, MON_PIC_FACING_FRONT, mon->isShiny, mon->form, mon->personality);
    CharTiles_LoadMonPicAsObjFromNarc((NarcId)pokepic.narcID, pokepic.charDataID, sub->heapID, sub->monPicChars, mon->personality, FALSE, MON_PIC_FACING_FRONT, mon->species);
    file = AllocAndReadWholeNarcMemberByIdPair((NarcId)pokepic.narcID, pokepic.palDataID, sub->heapID);
    if (NNS_G2dGetUnpackedPaletteData(file, &plttData) != TRUE) {
        GF_AssertFail();
    }
    MIi_CpuCopy16(plttData->pRawData, (u16 *)sub->monPicPalette, 32);
    Heap_Free(file);
}

static void PokeathlonBox_TransferMonPic(PokeathlonBox_SubGraphics *sub) {
    DC_FlushRange(sub->monPicChars, 0xc80);
    GXS_LoadOBJ(sub->monPicChars, sub->monPicCharOffset, 0xc80);
    DC_FlushRange(sub->monPicPalette, 0x20);
    GXS_LoadOBJPltt(sub->monPicPalette, sub->monPicPlttOffset, 0x20);
}
