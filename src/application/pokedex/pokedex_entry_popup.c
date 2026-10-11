#include "application/pokedex/pokedex_entry_popup.h"

#include "global.h"

#include "msgdata/msg.naix"

#include "dex_mon_measures.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "obj_char_transfer.h"
#include "pokedex_util.h"
#include "sound_playback.h"
#include "sound_system.h"
#include "sprite_transfer.h"
#include "text.h"
#include "unk_02009D48.h"

extern const BgTemplate sEntryPopupBg1Template;
extern const BgTemplate sEntryPopupBg2Template;
extern const BgTemplate sEntryPopupBg3Template;
extern const WindowTemplate sEntryPopupWindowTemplates[9];
extern const ObjCharTransferTemplate sEntryPopupObjCharTransferTemplate;

OverlayManager *PokedexEntryPopup_Create(UnkStruct_50C *unkStruct) {
    PokedexEntryPopup *popup = Heap_Alloc(unkStruct->heapID, sizeof(PokedexEntryPopup));
    memset(popup, 0, sizeof(PokedexEntryPopup));
    memcpy(popup, unkStruct, sizeof(UnkStruct_50C));
    popup->gfxNarc = NARC_New(NARC_graphic_zukan_gra, popup->heapId);
    popup->state = 0;
    popup->done = 0;
    popup->task = SysTask_CreateOnMainQueue((SysTaskFunc)PokedexEntryPopup_Task, popup, 0);
    return (OverlayManager *)popup;
}

s32 PokedexEntryPopup_IsDone(OverlayManager *man) {
    return ((PokedexEntryPopup *)man)->done;
}

void PokedexEntryPopup_Delete(OverlayManager *man) {
    PokedexEntryPopup *popup = (PokedexEntryPopup *)man;

    PokedexEntryPopup_FreeSprites(popup);
    PokedexEntryPopup_RemoveWindows(popup);
    PokedexEntryPopup_FreeBgs(popup);
    NARC_Delete(popup->gfxNarc);
    SysTask_Destroy(popup->task);
    Heap_Free(popup);
}

// Fades in the screen, plays the cry, then flags done; the caller deletes the popup.
void PokedexEntryPopup_Task(SysTask *task, PokedexEntryPopup *popup) {
    switch (popup->state) {
    case 0:
        *(vu16 *)REG_BLDCNT_ADDR = 0;
        PokedexEntryPopup_ReadMonData(popup);
        PokedexEntryPopup_InitBgs(popup);
        PokedexEntryPopup_DrawWindows(popup);
        PokedexEntryPopup_InitSprites(popup);
        PokedexEntryPopup_CreatePokepic(popup);
        PokedexEntryPopup_StartFadeIn(popup);
        popup->state = 1;
        break;
    case 1:
        if (PokedexEntryPopup_IsFadeDone(popup) == TRUE) {
            popup->state = 2;
        }
        break;
    case 2:
        PlayCryEx(14, popup->species, 0x1FF, 0x1FF, 0x1FF, 0);
        popup->state = 3;
        break;
    case 3:
        if (IsCryFinished() == FALSE) {
            popup->done = 1;
            popup->state = 4;
        }
        break;
    case 4:
        break;
    }
    SpriteList_RenderAndAnimateSprites(popup->spriteList);
    PokedexEntryPopup_FlashBanner(popup);
}

void PokedexEntryPopup_ReadMonData(PokedexEntryPopup *popup) {
    BOOL lock = AcquireMonLock(popup->mon);
    popup->species = GetMonData(popup->mon, MON_DATA_SPECIES, NULL);
    popup->form = GetMonData(popup->mon, MON_DATA_FORM, NULL);
    popup->type1 = GetMonData(popup->mon, MON_DATA_TYPE_1, NULL);
    popup->type2 = GetMonData(popup->mon, MON_DATA_TYPE_2, NULL);
    ReleaseMonLock(popup->mon, lock);
}

void PokedexEntryPopup_InitBgs(PokedexEntryPopup *popup) {
    SetBgPriority(GF_BG_LYR_MAIN_0, 0);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_BG0, GF_PLANE_TOGGLE_ON);
    {
        BgTemplate bg1 = sEntryPopupBg1Template;
        InitBgFromTemplate(popup->bgConfig, GF_BG_LYR_MAIN_1, &bg1, GF_BG_TYPE_TEXT);
        BG_ClearCharDataRange(GF_BG_LYR_MAIN_1, 0x20, 0, popup->heapId);
        BgTemplate bg2 = sEntryPopupBg2Template;
        InitBgFromTemplate(popup->bgConfig, GF_BG_LYR_MAIN_2, &bg2, GF_BG_TYPE_TEXT);
        BgTemplate bg3 = sEntryPopupBg3Template;
        InitBgFromTemplate(popup->bgConfig, GF_BG_LYR_MAIN_3, &bg3, GF_BG_TYPE_TEXT);
    }
    GfGfxLoader_LoadCharDataFromOpenNarc(popup->gfxNarc, 19, popup->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, popup->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(popup->gfxNarc, 20, popup->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, TRUE, popup->heapId);
    PaletteData_LoadOpenNarc(popup->paletteData, popup->gfxNarc, 18, popup->heapId, PLTTBUF_MAIN_BG, 0, 0);
}

void PokedexEntryPopup_FreeBgs(PokedexEntryPopup *popup) {
    FreeBgTilemapBuffer(popup->bgConfig, GF_BG_LYR_MAIN_3);
    FreeBgTilemapBuffer(popup->bgConfig, GF_BG_LYR_MAIN_2);
    FreeBgTilemapBuffer(popup->bgConfig, GF_BG_LYR_MAIN_1);
}

void PokedexEntryPopup_StartFadeIn(PokedexEntryPopup *popup) {
    PaletteData_BeginPaletteFade(popup->paletteData, PLTTBUF_MAIN_BG_F | PLTTBUF_MAIN_OBJ_F, 0xFFFF, 1, 16, 0, RGB_BLACK);
    Pokepic_StartPaletteFade(popup->pokepic, 16, 0, 0, RGB_BLACK);
    PaletteData_SetAutoTransparent(popup->paletteData, FALSE);
}

BOOL PokedexEntryPopup_IsFadeDone(PokedexEntryPopup *popup) {
    if (PaletteData_GetSelectedBuffersBitmask(popup->paletteData) == 0 && Pokepic_ResumePaletteFade(popup->pokepic) == FALSE) {
        return TRUE;
    }
    return FALSE;
}

// The banner (BG 2 rows 0-1) alternates between palettes 7 and 0 every 16 frames.
void PokedexEntryPopup_FlashBanner(PokedexEntryPopup *popup) {
    popup->bannerFlashTimer++;
    if (popup->bannerFlashTimer == 16) {
        BgTilemapRectChangePalette(popup->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 32, 2, 7);
        ScheduleBgTilemapBufferTransfer(popup->bgConfig, GF_BG_LYR_MAIN_2);
    } else if (popup->bannerFlashTimer == 32) {
        BgTilemapRectChangePalette(popup->bgConfig, GF_BG_LYR_MAIN_2, 0, 0, 32, 2, 0);
        ScheduleBgTilemapBufferTransfer(popup->bgConfig, GF_BG_LYR_MAIN_2);
        popup->bannerFlashTimer = 0;
    }
}

void PokedexEntryPopup_DrawWindows(PokedexEntryPopup *popup) {
    MsgData *msgData;
    MessageFormat *msgFormat;
    String *strbuf;
    String *string;
    u32 i;
    int x;

    for (i = 0; i < NELEMS(popup->windows); i++) {
        AddWindow(popup->bgConfig, &popup->windows[i], &sEntryPopupWindowTemplates[i]);
        FillWindowPixelBuffer(&popup->windows[i], 0);
    }
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, NARC_msg_msg_0802_bin, popup->heapId);
    msgFormat = MessageFormat_New(popup->heapId);
    strbuf = String_New(0x400, popup->heapId);
    PokedexApp_PrintMessage(&popup->windows[0], msgData, 144, 112, 0, 4, MAKE_TEXT_COLOR(2, 1, 0), 2);
    BufferIntegerAsString(msgFormat, 0, Pokedex_ConvertToCurrentDexNo(popup->natDexEnabled, popup->species), 3, PRINTING_MODE_LEADING_ZEROS, TRUE);
    string = NewString_ReadMsgData(msgData, 9);
    StringExpandPlaceholders(msgFormat, strbuf, string);
    PokedexApp_PrintString(&popup->windows[1], strbuf, 1, 0, 4, MAKE_TEXT_COLOR(2, 1, 0), 0);
    String_Delete(string);
    string = ov18_021E590C(popup->species, 2, popup->heapId);
    PokedexApp_PrintString(&popup->windows[2], string, 0, 0, 4, MAKE_TEXT_COLOR(2, 1, 0), 0);
    String_Delete(string);
    string = ov18_021E595C(popup->species, 2, popup->heapId);
    x = GetWindowWidth(&popup->windows[3]) * 8 - 4;
    PokedexApp_PrintString(&popup->windows[3], string, x, 0, 4, MAKE_TEXT_COLOR(2, 1, 0), 1);
    String_Delete(string);
    string = ov18_021E59A8(popup->species, 2, 0, popup->heapId);
    x = (GetWindowWidth(&popup->windows[4]) * 8 - FontID_String_GetWidthMultiline(0, string, 0)) / 2;
    PokedexApp_PrintString(&popup->windows[4], string, x, 0, 0, MAKE_TEXT_COLOR(2, 1, 0), 0);
    String_Delete(string);
    PokedexApp_PrintMessage(&popup->windows[5], msgData, 10, 20, 0, 0, MAKE_TEXT_COLOR(2, 1, 0), 2);
    PokedexApp_PrintMessage(&popup->windows[7], msgData, 11, 20, 0, 0, MAKE_TEXT_COLOR(2, 1, 0), 2);
    MessageFormat_Delete(msgFormat);
    DestroyMsgData(msgData);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, GetDexHeightMsgBank(), popup->heapId);
    PokedexApp_PrintMessage(&popup->windows[6], msgData, popup->species, 4, 0, 0, MAKE_TEXT_COLOR(2, 1, 0), 0);
    DestroyMsgData(msgData);
    msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, GetDexWeightMsgBank(), popup->heapId);
    PokedexApp_PrintMessage(&popup->windows[8], msgData, popup->species, 4, 0, 0, MAKE_TEXT_COLOR(2, 1, 0), 0);
    DestroyMsgData(msgData);
    String_Delete(strbuf);
    for (i = 0; i < NELEMS(popup->windows); i++) {
        ScheduleWindowCopyToVram(&popup->windows[i]);
    }
}

void PokedexEntryPopup_RemoveWindows(PokedexEntryPopup *popup) {
    u32 i;

    for (i = 0; i < 9; i++) {
        RemoveWindow(&popup->windows[i]);
    }
}

void PokedexEntryPopup_InitObjCharTransfer(PokedexEntryPopup *popup) {
    ObjCharTransferTemplate template = sEntryPopupObjCharTransferTemplate;
    template.heapID = popup->heapId;
    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_64K, (GXOBJVRamModeChar)16);
}

void PokedexEntryPopup_DestroyObjCharTransfer(void) {
    ObjCharTransfer_Destroy();
}

void PokedexEntryPopup_CreateResourceManagers(PokedexEntryPopup *popup) {
    int i;

    for (i = 0; i < 4; i++) {
        popup->resourceManagers[i] = Create2DGfxResObjMan(8, (GfGfxResType)i, popup->heapId);
    }
}

void PokedexEntryPopup_DestroyResourceManagers(PokedexEntryPopup *popup) {
    int i;
    u8 *cur = (u8 *)popup;

    // a byte pointer walks the struct (indexing resourceManagers[i] changes the code)
    for (i = 0; i < 4; i++) {
        Destroy2DGfxResObjMan(((PokedexEntryPopup *)cur)->resourceManagers[0]);
        cur += sizeof(GF_2DGfxResMan *);
    }
}

void PokedexEntryPopup_InitSprites(PokedexEntryPopup *popup) {
    popup->spriteList = G2dRenderer_Init(32, &popup->renderer, popup->heapId);
    ClearMainOAM(popup->heapId);
    PokedexEntryPopup_InitObjCharTransfer(popup);
    PokedexEntryPopup_CreateResourceManagers(popup);
    ov18_021F8FF8(popup);
    PokedexEntryPopup_LoadTypeIconResources(popup);
    PokedexEntryPopup_LoadFootprintResources(popup);
    ov18_021F94BC(popup);
    PokedexEntryPopup_CreateTypeIconSprites(popup);
    PokedexEntryPopup_CreateFootprintSprite(popup);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
}

// Banner sprite resources
void ov18_021F8FF8(PokedexEntryPopup *popup) {
    PokedexEntryPopup_LoadSpriteResources(popup->sprites[0].resources, popup->resourceManagers, popup->heapId, popup->gfxNarc, 29, 32, 30, 31, 2, 0xC618, 0xC618, 0xC618, 0xC618);
    PaletteData_LoadPaletteSlotFromHardware(popup->paletteData, PLTTBUF_MAIN_OBJ, SpriteTransfer_GetPlttOffset(popup->sprites[0].resources[1], NNS_G2D_VRAM_TYPE_2DMAIN) << 4, 0x40);
}

void ov18_021F9054(PokedexEntryPopup *popup) {
    PokedexEntryPopup_FreeSpriteResources(popup->sprites[0].resources, popup->resourceManagers);
}

// Both type icons share the first icon's palettes, cell and animation.
void PokedexEntryPopup_LoadTypeIconResources(PokedexEntryPopup *popup) {
    PokedexEntryPopup_LoadSpriteResources(popup->sprites[1].resources, popup->resourceManagers, popup->heapId, popup->gfxNarc, PokedexApp_GetTypeIconCharFile(popup->type1), 35, 33, 34, 4, 0xC619, 0xC619, 0xC619, 0xC619);
    PokedexEntryPopup_LoadSpriteResources(popup->sprites[2].resources, popup->resourceManagers, popup->heapId, popup->gfxNarc, PokedexApp_GetTypeIconCharFile(popup->type2), -1, 33, 34, 4, 0xC61A, 0xC61A, 0xC61A, 0xC61A);
    PaletteData_LoadPaletteSlotFromHardware(popup->paletteData, PLTTBUF_MAIN_OBJ, SpriteTransfer_GetPlttOffset(popup->sprites[1].resources[1], NNS_G2D_VRAM_TYPE_2DMAIN) << 4, 0x80);
}

void PokedexEntryPopup_UnloadTypeIconResources(PokedexEntryPopup *popup) {
    PokedexEntryPopup_FreeSpriteResources(popup->sprites[1].resources, popup->resourceManagers);
    SpriteTransfer_DeleteCharTransferTask(popup->sprites[2].resources[0]);
    DestroySingle2DGfxResObj(popup->resourceManagers[0], popup->sprites[2].resources[0]);
    DestroySingle2DGfxResObj(popup->resourceManagers[2], popup->sprites[2].resources[2]);
    DestroySingle2DGfxResObj(popup->resourceManagers[3], popup->sprites[2].resources[3]);
}

void PokedexEntryPopup_LoadFootprintResources(PokedexEntryPopup *popup) {
    NARC *narc = NARC_New(ov18_021E5900(), popup->heapId);
    PokedexEntryPopup_LoadSpriteResources(popup->sprites[3].resources, popup->resourceManagers, popup->heapId, popup->gfxNarc, 77, -1, 78, 79, 1, 0xC61B, 0xC61B, 0xC61B, 0xC61B);
    popup->sprites[3].resources[1] = PokedexEntryPopup_LoadPaletteResource(popup->resourceManagers[1], popup->heapId, narc, ov18_021E5908(), 1, 0xC61B);
    PaletteData_LoadPaletteSlotFromHardware(popup->paletteData, PLTTBUF_MAIN_OBJ, SpriteTransfer_GetPlttOffset(popup->sprites[3].resources[1], NNS_G2D_VRAM_TYPE_2DMAIN) << 4, 0x20);
    NARC_Delete(narc);
}

void PokedexEntryPopup_UnloadFootprintResources(PokedexEntryPopup *popup) {
    PokedexEntryPopup_FreeSpriteResources(popup->sprites[3].resources, popup->resourceManagers);
}

void PokedexEntryPopup_FreeSprites(PokedexEntryPopup *popup) {
    PokedexEntryPopup_DeleteFootprintSprite(popup);
    PokedexEntryPopup_DeleteTypeIconSprites(popup);
    ov18_021F9508(popup);
    PokedexEntryPopup_UnloadFootprintResources(popup);
    PokedexEntryPopup_UnloadTypeIconResources(popup);
    ov18_021F9054(popup);
    PokedexEntryPopup_DestroyResourceManagers(popup);
    PokedexEntryPopup_DestroyObjCharTransfer();
    SpriteList_Delete(popup->spriteList);
}

// plttFileId -1: no palette (the caller shares or loads one itself)
void PokedexEntryPopup_LoadSpriteResources(SpriteResource **resources, GF_2DGfxResMan **managers, enum HeapID heapId, NARC *narc, int charFileId, int plttFileId, int cellFileId, int animFileId, int plttNum, int charId, int plttId, int cellId, int animId) {
    resources[0] = AddCharResObjFromOpenNarc(managers[0], narc, charFileId, TRUE, charId, NNS_G2D_VRAM_TYPE_2DMAIN, heapId);
    SpriteTransfer_CreateCharTransferTask_AllocAtEnd(resources[0]);
    sub_0200A740(resources[0]);
    if (plttFileId != -1) {
        resources[1] = PokedexEntryPopup_LoadPaletteResource(managers[1], heapId, narc, plttFileId, plttNum, plttId);
    }
    resources[2] = AddCellOrAnimResObjFromOpenNarc(managers[2], narc, cellFileId, TRUE, cellId, GF_GFX_RES_TYPE_CELL, heapId);
    resources[3] = AddCellOrAnimResObjFromOpenNarc(managers[3], narc, animFileId, TRUE, animId, GF_GFX_RES_TYPE_ANIM, heapId);
}

SpriteResource *PokedexEntryPopup_LoadPaletteResource(GF_2DGfxResMan *manager, enum HeapID heapId, NARC *narc, int fileId, int plttNum, int id) {
    SpriteResource *resource = AddPlttResObjFromOpenNarc(manager, narc, fileId, FALSE, id, NNS_G2D_VRAM_TYPE_2DMAIN, plttNum, heapId);
    SpriteTransfer_CreatePlttTransferTask(resource);
    sub_0200A740(resource);
    return resource;
}

void PokedexEntryPopup_FreeSpriteResources(SpriteResource **resources, GF_2DGfxResMan **managers) {
    SpriteTransfer_DeleteCharTransferTask(resources[0]);
    SpriteTransfer_DeletePlttTransferTask(resources[1]);
    DestroySingle2DGfxResObj(managers[0], resources[0]);
    DestroySingle2DGfxResObj(managers[1], resources[1]);
    DestroySingle2DGfxResObj(managers[2], resources[2]);
    DestroySingle2DGfxResObj(managers[3], resources[3]);
}

void PokedexEntryPopup_MakeResourcesHeader(SpriteResource **resources, GF_2DGfxResMan **managers, SpriteResourcesHeader *header, int priority) {
    int charId = GF2DGfxResObj_GetResID(resources[0]);
    int plttId = GF2DGfxResObj_GetResID(resources[1]);
    int cellId = GF2DGfxResObj_GetResID(resources[2]);
    int animId = GF2DGfxResObj_GetResID(resources[3]);
    CreateSpriteResourcesHeader(header, charId, plttId, cellId, animId, -1, -1, 0, priority, managers[0], managers[1], managers[2], managers[3], NULL, NULL);
}

// The second icon uses the first one's palette and is hidden for single-type species.
void PokedexEntryPopup_CreateTypeIconSprites(PokedexEntryPopup *popup) {
    SimpleSpriteTemplate template;
    SpriteResourcesHeader header;
    int charId;
    int plttId;
    int cellId;
    int animId;

    PokedexEntryPopup_MakeResourcesHeader(popup->sprites[1].resources, popup->resourceManagers, &header, 1);
    template.spriteList = popup->spriteList;
    template.header = &header;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.priority = 0;
    template.heapID = popup->heapId;
    template.position.x = 168 * FX32_ONE;
    template.position.y = 72 * FX32_ONE;
    popup->sprites[1].sprite = Sprite_Create(&template);
    Sprite_SetPalIndexRespectVramOffset(popup->sprites[1].sprite, PokedexApp_GetTypeIconPalette(popup->type1));
    charId = GF2DGfxResObj_GetResID(popup->sprites[2].resources[0]);
    plttId = GF2DGfxResObj_GetResID(popup->sprites[1].resources[1]);
    cellId = GF2DGfxResObj_GetResID(popup->sprites[2].resources[2]);
    animId = GF2DGfxResObj_GetResID(popup->sprites[2].resources[3]);
    CreateSpriteResourcesHeader(&header, charId, plttId, cellId, animId, -1, -1, 0, 1, popup->resourceManagers[0], popup->resourceManagers[1], popup->resourceManagers[2], popup->resourceManagers[3], NULL, NULL);
    template.spriteList = popup->spriteList;
    template.header = &header;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.priority = 0;
    template.heapID = popup->heapId;
    template.position.x = 217 * FX32_ONE;
    template.position.y = 72 * FX32_ONE;
    popup->sprites[2].sprite = Sprite_Create(&template);
    if (popup->type2 == 0 || popup->type1 == popup->type2) {
        Sprite_SetDrawFlag(popup->sprites[2].sprite, FALSE);
    } else {
        Sprite_SetPalIndexRespectVramOffset(popup->sprites[2].sprite, PokedexApp_GetTypeIconPalette(popup->type2));
    }
}

void PokedexEntryPopup_DeleteTypeIconSprites(PokedexEntryPopup *popup) {
    Sprite_Delete(popup->sprites[1].sprite);
    Sprite_Delete(popup->sprites[2].sprite);
}

// Banner sprite
void ov18_021F94BC(PokedexEntryPopup *popup) {
    SimpleSpriteTemplate template;
    SpriteResourcesHeader header;

    PokedexEntryPopup_MakeResourcesHeader(popup->sprites[0].resources, popup->resourceManagers, &header, 1);
    template.spriteList = popup->spriteList;
    template.header = &header;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.priority = 0;
    template.heapID = popup->heapId;
    template.position.x = 112 * FX32_ONE;
    template.position.y = 32 * FX32_ONE;
    popup->sprites[0].sprite = Sprite_Create(&template);
}

void ov18_021F9508(PokedexEntryPopup *popup) {
    Sprite_Delete(popup->sprites[0].sprite);
}

// The footprint is copied straight into the sprite's VRAM.
void PokedexEntryPopup_CreateFootprintSprite(PokedexEntryPopup *popup) {
    SimpleSpriteTemplate template;
    SpriteResourcesHeader header;
    void *footprint;
    u32 location;

    PokedexEntryPopup_MakeResourcesHeader(popup->sprites[3].resources, popup->resourceManagers, &header, 1);
    template.spriteList = popup->spriteList;
    template.header = &header;
    template.whichScreen = NNS_G2D_VRAM_TYPE_2DMAIN;
    template.priority = 0;
    template.heapID = popup->heapId;
    template.position.x = 120 * FX32_ONE;
    template.position.y = 80 * FX32_ONE;
    popup->sprites[3].sprite = Sprite_Create(&template);
    footprint = PokedexApp_LoadFootprintGraphics(popup->species, popup->heapId);
    location = NNS_G2dGetImageLocation(Sprite_GetImageProxy(popup->sprites[3].sprite), NNS_G2D_VRAM_TYPE_2DMAIN);
    DC_FlushRange(footprint, 0x80);
    GX_LoadOBJ(footprint, location, 0x80);
    Heap_Free(footprint);
}

void PokedexEntryPopup_DeleteFootprintSprite(PokedexEntryPopup *popup) {
    Sprite_Delete(popup->sprites[3].sprite);
}

void PokedexEntryPopup_HideSprites(OverlayManager *man) {
    u32 i;
    u8 *cur = (u8 *)man;
    int draw = FALSE;

    // a byte pointer walks the sprite entries
    for (i = 0; i < 4; i++) {
        Sprite_SetDrawFlag(((PokedexEntryPopup *)cur)->sprites[0].sprite, draw);
        cur += sizeof(PokedexEntryPopupSprite);
    }
}

void PokedexEntryPopup_CreatePokepic(PokedexEntryPopup *popup) {
    PokepicTemplate template;
    GetPokemonSpriteCharAndPlttNarcIds(&template, popup->mon, 2);
    popup->pokepic = PokepicManager_CreatePokepic(popup->pokepicManager, &template, 48, 72, 0, 0, NULL, NULL);
}

Pokepic *PokedexEntryPopup_GetPokepic(OverlayManager *man) {
    return ((PokedexEntryPopup *)man)->pokepic;
}
