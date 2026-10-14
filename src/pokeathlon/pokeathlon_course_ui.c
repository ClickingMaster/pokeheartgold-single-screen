#include "pokeathlon/pokeathlon_course_ui.h"

#include "global.h"

#include "pokeathlon/pokeathlon_course_ceremony.h"

#include "assert.h"
#include "font.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "options.h"
#include "player_data.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "char_tiles.h"

// text.h declares these with a u8 font id; the code was compiled against these.
u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, void *callback);
u8 TextPrinterCheckActive(u8 printerId);
void RemoveTextPrinter(u8 printerId);
void ResetAllTextPrinters(void);

extern const WindowTemplate gPokeathlonEventBannerWindowTemplates[2];
extern const u32 gPokeathlonCourseTrainerPicFileIds[6];
extern const TileRect gPokeathlonCourseMonPicRect;

static inline void *GetTexDictData(NNSG3dResTex *tex, u32 index) {
    if (tex) {
        return NNS_G3dGetResDataByIdx(&tex->dict, index);
    }
    return NULL;
}

BOOL PokeathlonCourseCeremony_IsOpeningFinished(PokeathlonCourseData *course) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyOpeningData *data = (PokeathlonCourseCeremonyOpeningData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (course == NULL) {
        GF_AssertFail();
    }
    if (copyArea == NULL) {
        GF_AssertFail();
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    return data->allFinished;
}

BOOL PokeathlonCourseCeremony_IsClosingFinished(PokeathlonCourseData *course) {
    u8 *copyArea = PokeathlonCourse_GetDataCopyArea(course);
    PokeathlonCourseCeremonyClosingData *data = (PokeathlonCourseCeremonyClosingData *)PokeathlonCourse_ToBytes(copyArea + 0xF0);
    if (course == NULL) {
        GF_AssertFail();
    }
    if (copyArea == NULL) {
        GF_AssertFail();
    }
    if (data == NULL) {
        GF_AssertFail();
    }
    return data->allFinished;
}

PokeathlonEventBanner *PokeathlonEventBanner_New(u32 event, enum HeapID heapId) {
    PokeathlonEventBanner *banner = Heap_Alloc(heapId, sizeof(PokeathlonEventBanner));
    MI_CpuFill8(banner, 0, sizeof(PokeathlonEventBanner));
    banner->heapId = heapId;
    banner->event = event;
    banner->narc = NARC_New(NARC_a_2_1_9, heapId);
    return banner;
}

void PokeathlonEventBanner_Free(PokeathlonEventBanner *banner) {
    int i = 0;
    Window *window = &banner->nameWindow;
    while (i < 2) {
        ClearWindowTilemapAndCopyToVram(window);
        RemoveWindow(window);
        i++;
        window++;
    }
    NARC_Delete(banner->narc);
    Heap_Free(banner);
}

void PokeathlonEventBanner_AddWindows(PokeathlonEventBanner *banner, BgConfig *bgConfig) {
    const WindowTemplate *template = gPokeathlonEventBannerWindowTemplates;
    Window *window;
    int i;
    banner->bgConfig = bgConfig;
    i = 0;
    window = &banner->nameWindow;
    while (i < 2) {
        AddWindow(banner->bgConfig, window, template);
        FillWindowPixelBuffer(window, 0);
        ClearWindowTilemap(window);
        i++;
        template ++;
        window++;
    }
}

void PokeathlonEventBanner_LoadGraphics(PokeathlonEventBanner *banner) {
    u8 event = banner->event;
    GfGfxLoader_GXLoadPalFromOpenNarc(banner->narc, 11, GF_PAL_LOCATION_SUB_BGEXT, (enum GFPalSlotOffset)(6 << 12), 0, banner->heapId);
    GfGfxLoader_LoadCharDataFromOpenNarc(banner->narc, event + 12, banner->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, banner->heapId);
    GfGfxLoader_LoadScrnDataFromOpenNarc(banner->narc, event + 22, banner->bgConfig, GF_BG_LYR_SUB_3, 0, 0, TRUE, banner->heapId);
}

void PokeathlonEventBanner_PrintTexts(PokeathlonEventBanner *banner) {
    u8 event = banner->event;
    MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 309, banner->heapId);
    String *string = String_New(256, banner->heapId);
    PokeathlonEventBanner_PrintText(&banner->nameWindow, msgData, string, event + 166, 4);
    PokeathlonEventBanner_PrintText(&banner->placeWindow, msgData, string, event + 187, 0);
    String_Delete(string);
    DestroyMsgData(msgData);
}

void PokeathlonEventBanner_PrintText(Window *window, MsgData *msgData, String *string, u32 msgId, u32 font) {
    ReadMsgDataIntoString(msgData, msgId, string);
    FillWindowPixelBuffer(window, 0);
    AddTextPrinterParameterizedWithColor(window, font, string, 0, 0, 0xFF, 0x10200, NULL); // no transfer; colours 1, 2, 0
    ScheduleWindowCopyToVram(window);
}

PokeathlonCourseMessageBox *PokeathlonCourseMessageBox_New(enum HeapID heapId) {
    PokeathlonCourseMessageBox *box = Heap_Alloc(heapId, sizeof(PokeathlonCourseMessageBox));
    MI_CpuFill8(box, 0, sizeof(PokeathlonCourseMessageBox));
    box->heapId = heapId;
    return box;
}

#pragma force_active on

void PokeathlonCourseMessageBox_Init(PokeathlonCourseMessageBox *box, BgConfig *bgConfig, u32 bgLayer, u32 linkMode, SaveData *saveData) {
    u32 textSpeed = 4;
    if (linkMode == 0) {
        textSpeed = Options_GetTextFrameDelay(Save_PlayerData_GetOptionsAddr(saveData));
    }
    box->bgConfig = bgConfig;
    box->printerId = -1;
    box->bgLayer = bgLayer;
    box->linkMode = linkMode;
    box->textSpeed = textSpeed;
    box->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 309, box->heapId);
    box->npcMsgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 311, box->heapId);
    box->msgFormat = MessageFormat_New(box->heapId);
    ResetAllTextPrinters();
    PokeathlonCourseMessageBox_LoadFrame(box->bgConfig, (u8)box->bgLayer, box->heapId);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)480, box->heapId);
    BG_FillCharDataRange(box->bgConfig, (GFBgLayer)box->bgLayer, 0, 1, 0);
}

#pragma force_active reset

void PokeathlonCourseMessageBox_Free(PokeathlonCourseMessageBox *box) {
    DestroyMsgData(box->msgData);
    DestroyMsgData(box->npcMsgData);
    MessageFormat_Delete(box->msgFormat);
    if (box->string != NULL) {
        String_Delete(box->string);
    }
    Heap_Free(box);
}

// Returns whether the box is still busy (printing, or holding a printed message in link mode).
BOOL PokeathlonCourseMessageBox_Update(PokeathlonCourseMessageBox *box) {
    if (box->printerId != -1) {
        if (!TextPrinterCheckActive((u8)box->printerId)) {
            box->holdTimer = 20;
            String_Delete(box->string);
            box->string = NULL;
            box->printerId = -1;
        }
        box->busy = TRUE;
    } else {
        GF_ASSERT(box->printerId == -1);
        if (box->holdTimer != 0) {
            if (box->linkMode == 0) {
                box->busy = FALSE;
            } else {
                box->holdTimer--;
                box->busy = box->holdTimer != 0;
            }
        } else {
            box->busy = FALSE;
        }
    }
    return box->busy;
}

void PokeathlonCourseMessageBox_Print(PokeathlonCourseMessageBox *box, u32 msgId) {
    box->printerId = PokeathlonCourseMessageBox_PrintToWindow(box->bgConfig, &box->window, &box->string, box->textSpeed, box->msgFormat, box->msgData, msgId, (u8)box->bgLayer, box->heapId);
}

void PokeathlonCourseMessageBox_PrintNpcLine(PokeathlonCourseMessageBox *box, u32 msgId) {
    box->printerId = PokeathlonCourseMessageBox_PrintToWindow(box->bgConfig, &box->window, &box->string, box->textSpeed, box->msgFormat, box->npcMsgData, msgId, (u8)box->bgLayer, box->heapId);
}

void PokeathlonCourseMessageBox_Close(PokeathlonCourseMessageBox *box) {
    if (box->printerId != -1) {
        RemoveTextPrinter((u8)box->printerId);
    }
    if (box->window.bgConfig != NULL) {
        ClearFrameAndWindow2(&box->window, FALSE);
        ClearWindowTilemapAndCopyToVram(&box->window);
        RemoveWindow(&box->window);
    }
}

MessageFormat *PokeathlonCourseMessageBox_GetMsgFormat(PokeathlonCourseMessageBox *box) {
    if (box == NULL) {
        GF_AssertFail();
    }
    if (box->msgFormat == NULL) {
        GF_AssertFail();
    }
    return box->msgFormat;
}

void PokeathlonCourseMessageBox_ShowWaitingIcon(PokeathlonCourseMessageBox *box) {
    if (box == NULL) {
        GF_AssertFail();
    }
    if (box->waitingIcon != NULL) {
        GF_AssertFail();
    }
    box->waitingIcon = WaitingIcon_New(&box->window, 0x3D2);
}

void PokeathlonCourseMessageBox_HideWaitingIcon(PokeathlonCourseMessageBox *box) {
    if (box == NULL) {
        GF_AssertFail();
    }
    if (box->waitingIcon == NULL) {
        GF_AssertFail();
    }
    WaitingIcon_Delete(box->waitingIcon);
}

void PokeathlonCourseMessageBox_LoadFrame(BgConfig *bgConfig, int layer, enum HeapID heapId) {
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)0x1C0, heapId);
    LoadUserFrameGfx2(bgConfig, (GFBgLayer)layer, 0x3D2, 13, 0, heapId);
}

#pragma force_active on

u8 PokeathlonCourseMessageBox_PrintToWindow(BgConfig *bgConfig, Window *window, String **string, u32 textSpeed, MessageFormat *msgFormat, MsgData *msgData, u32 msgId, u8 bgLayer, enum HeapID heapId) {
    u8 printerId;
    if (window->bgConfig == NULL) {
        AddWindowParameterized(bgConfig, window, bgLayer, 2, 19, 27, 4, 15, 1);
    }
    FillWindowPixelBuffer(window, 15);
    *string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, msgId, heapId);
    printerId = AddTextPrinterParameterizedWithColor(window, 1, *string, 0, 0, textSpeed, 0x1020F, NULL); // colours 1, 2, 15
    DrawFrameAndWindow2(window, FALSE, 0x3D2, 13);
    return printerId;
}

#pragma force_active reset

BOOL PokeathlonCourseMessageBox_IsBusy(PokeathlonCourseMessageBox *box) {
    return box->busy;
}

#pragma force_active on

void PokeathlonCourse_LoadTrainerPic(ManagedSprite *sprite, int fileId, int index, enum HeapID heapId) {
    PokeathlonCourse_LoadTrainerPicIntoSprite(sprite->sprite, fileId, index, heapId);
}

#pragma force_active reset

// Converts texture index of the overworld model fileId (NARC data/mmodel/mmodel) into the sprite's characters
// and palette.
#pragma force_active on

void PokeathlonCourse_LoadTrainerPicIntoSprite(Sprite *sprite, int fileId, int index, enum HeapID heapId) {
    NARC *narc;
    void *file;
    NNSG3dResTex *tex;
    void *buffer;
    u8 *texData;
    int offset;
    int location;
    int paletteOffset;
    NNS_G2D_VRAM_TYPE vramType;
    thunk_Sprite_SetDrawFlag(sprite, FALSE);
    narc = NARC_New(NARC_data_mmodel_mmodel, heapId);
    file = NARC_AllocAndReadWholeMember(narc, fileId, heapId);
    tex = NNS_G3dGetTex(file);
    offset = (*(u32 *)GetTexDictData(tex, index) & 0xFFFF) << 3;
    texData = (u8 *)tex + tex->texInfo.ofsTex;
    buffer = Heap_AllocAtEnd(heapId, 0x200);
    CharTiles_CopyRectFromBuffer(texData + offset, 4, 0, 0, 4, 4, buffer);
    PokeathlonCourse_LoadSpriteChars(sprite, buffer, 0x200);
    Heap_Free(buffer);
    vramType = Sprite_GetVramType(sprite);
    paletteOffset = tex->plttInfo.ofsPlttData;
    DC_FlushRange((u8 *)tex + paletteOffset, 0x20);
    location = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(sprite), vramType);
    if (vramType == NNS_G2D_VRAM_TYPE_2DMAIN) {
        GX_LoadOBJPltt((u8 *)tex + paletteOffset, location, 0x20);
    } else {
        GXS_LoadOBJPltt((u8 *)tex + paletteOffset, location, 0x20);
    }
    Heap_Free(file);
    NARC_Delete(narc);
    thunk_Sprite_SetDrawFlag(sprite, TRUE);
}

void PokeathlonCourse_LoadTeamTrainerPic(ManagedSprite **sprites, PokeathlonCourseData *course, u8 team, int index) {
    PokeathlonCourse_LoadPlayerTrainerPic(sprites[team]->sprite, course, team, index);
}

#pragma force_active reset

void PokeathlonCourse_LoadPlayerTrainerPic(Sprite *sprite, PokeathlonCourseData *course, u32 team, u32 index) {
    PlayerProfile *profile = PokeathlonCourse_GetPlayerProfileFromData(course, team);
    enum HeapID heapId = PokeathlonCourse_GetHeapID(course);
    u16 fileId;
    if (PlayerProfile_GetTrainerGender(profile) == 0) {
        fileId = PokeathlonCourse_GetTrainerPicFileId(0);
    } else {
        fileId = PokeathlonCourse_GetTrainerPicFileId(1);
    }
    PokeathlonCourse_LoadTrainerPicIntoSprite(sprite, fileId, (u8)index, heapId);
}

u16 PokeathlonCourse_GetTrainerPicFileId(u32 kind) {
    if (kind >= 6) {
        GF_AssertFail();
    }
    return gPokeathlonCourseTrainerPicFileIds[kind];
}

void PokeathlonCourse_LoadMonPic(ManagedSprite *sprite, PokeathlonCourseObjectInfo *info, BOOL mainScreen, u32 palette, enum HeapID heapId) {
    PokeathlonCourse_LoadMonPicEx(sprite, info, mainScreen, palette, heapId, TRUE);
}

void PokeathlonCourse_LoadMonPicWithPalette(ManagedSprite *sprite, PokeathlonCourseObjectInfo *info, BOOL mainScreen, u32 palette, enum HeapID heapId, BOOL loadPalette) {
    PokeathlonCourse_LoadMonPicEx(sprite, info, mainScreen, palette, heapId, loadPalette);
}

// Front picture of the Pokemon (no sprite if species is 0).
#pragma force_active on

void PokeathlonCourse_LoadMonPicEx(ManagedSprite *sprite, PokeathlonCourseObjectInfo *info, BOOL mainScreen, u32 grayScale, enum HeapID heapId, BOOL loadPalette) {
    PokepicTemplate template;
    TileRect rect;
    void *buffer;
    if (info->species == SPECIES_NONE) {
        ManagedSprite_SetDrawFlag(sprite, FALSE);
        return;
    }
    GetMonSpriteCharAndPlttNarcIdsEx(&template, info->species, info->gender, 2, info->shiny, (u8)info->form, info->personality);
    rect = gPokeathlonCourseMonPicRect;
    buffer = Heap_AllocAtEnd(heapId, 0xC80);
    CharTiles_LoadMonRectAsObjFromNarc((NarcId) template.narcID, template.charDataID, heapId, &rect, buffer, info->personality, FALSE, 2, info->species);
    PokeathlonCourse_LoadSpriteChars(sprite->sprite, buffer, 0xC80);
    if (loadPalette) {
        PokeathlonCourse_LoadSpritePalette(sprite, (NarcId) template.narcID, template.palDataID, mainScreen, grayScale, heapId);
    }
    ManagedSprite_SetDrawFlag(sprite, TRUE);
    Heap_Free(buffer);
}

void PokeathlonCourse_LoadMonIcon(ManagedSprite *sprite, const u16 *monForm, int palette, NARC *narc, u32 offset, enum HeapID heapId) {
    NNSG2dCharacterData *charData;
    void *file = GfGfxLoader_GetCharDataFromOpenNarc(narc, GetMonIconNaixEx(monForm[0], FALSE, monForm[1]), FALSE, &charData, heapId);
    GF_ASSERT(file);
    PokeathlonCourse_LoadSpriteCharsAt(sprite->sprite, charData->pRawData, 0x200, offset);
    Heap_Free(file);
    ManagedSprite_SetPaletteOverride(sprite, palette + GetMonIconPaletteEx(monForm[0], monForm[1], FALSE));
}

#pragma force_active reset

void PokeathlonCourse_LoadSpriteCharsAt(Sprite *sprite, void *data, u32 size, u32 offset) {
    NNS_G2D_VRAM_TYPE vramType = Sprite_GetVramType(sprite);
    u32 location = NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprite), vramType);
    DC_FlushRange(data, size);
    switch (vramType) {
    case NNS_G2D_VRAM_TYPE_2DMAIN:
        GX_LoadOBJ(data, location + offset, size);
        break;
    case NNS_G2D_VRAM_TYPE_2DSUB:
        GXS_LoadOBJ(data, location + offset, size);
        break;
    default:
        GF_AssertFail();
        break;
    }
}

void PokeathlonCourse_LoadSpriteChars(Sprite *sprite, void *data, u32 size) {
    PokeathlonCourse_LoadSpriteCharsAt(sprite, data, size, 0);
}
