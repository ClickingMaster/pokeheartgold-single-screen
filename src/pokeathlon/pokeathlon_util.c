#include "pokeathlon/pokeathlon_util.h"

#include "global.h"

#include "constants/species.h"

#include "msgdata/msg.naix"

#include "filesystem_files_def.h"
#include "heap.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "screen_fade.h"
#include "char_tiles.h"

#define MON_PIC_BUFFER_SIZE 0xC80

#define TEXT_SPEED_NOTRANSFER     0xFF
#define TEXT_COLOR_BLACK_ON_WHITE 0x10200

// text.h declares the font id as FontID (u8); this module passes it on as a u32 without truncating it
u8 AddTextPrinterParameterizedWithColor(Window *window, u32 fontId, String *string, u32 x, u32 y, u32 textSpeed, u32 color, PrinterCallback_t callback);

u32 MATH_CountPopulation(u32 value);

extern const u32 _0221F194[2];
extern const TileRect ov98_0221F19C;

void *PokeathlonUtil_GetSpriteSystem(PokeathlonSpriteContext *ctx) {
    GF_ASSERT(ctx != NULL);
    return ctx->spriteSystem;
}

void *PokeathlonUtil_GetSpriteManager(PokeathlonSpriteContext *ctx) {
    GF_ASSERT(ctx != NULL);
    return ctx->spriteManager;
}

PokeathlonSpriteContext *PokeathlonUtil_CreateSpriteContext(enum HeapID heapId, SpriteResourceCountsListUnion *counts, int maxSprites) {
    PokeathlonSpriteContext *ctx = Heap_Alloc(heapId, sizeof(PokeathlonSpriteContext));
    OamManagerParam oam;
    OamCharTransferParam transfer;

    ctx->heapId = heapId;
    ctx->spriteSystem = SpriteSystem_Alloc(heapId);
    ctx->spriteManager = SpriteManager_New(ctx->spriteSystem);
    ctx->narc = NARC_New(NARC_poketool_icongra_poke_icon, ctx->heapId);
    ctx->paletteOffset = -1;
    oam = ov98_0221F1C0;
    transfer = ov98_0221F1AC;
    transfer.maxTasks = maxSprites;
    SpriteSystem_Init(ctx->spriteSystem, &oam, &transfer, 32);
    SpriteSystem_InitSprites(ctx->spriteSystem, ctx->spriteManager, maxSprites);
    SpriteSystem_InitManagerWithCapacities(ctx->spriteSystem, ctx->spriteManager, counts);
    ctx->paletteData = PaletteData_Init(ctx->heapId);
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, TRUE);
    return ctx;
}

void PokeathlonUtil_FreeSpriteContext(PokeathlonSpriteContext *ctx, ManagedSprite **sprites, u32 count) {
    u32 i;
    NARC_Delete(ctx->narc);
    PaletteData_Free(ctx->paletteData);
    for (i = 0; i < count; i++) {
        if (sprites[i] != NULL) {
            Sprite_DeleteAndFreeResources(sprites[i]);
            sprites[i] = NULL;
        }
    }
    SpriteSystem_FreeResourcesAndManager(ctx->spriteSystem, ctx->spriteManager);
    SpriteSystem_Free(ctx->spriteSystem);
    Heap_Free(ctx);
}

void PokeathlonUtil_DrawSprites(PokeathlonSpriteContext *ctx) {
    GF_ASSERT(ctx != NULL);
    SpriteSystem_DrawSprites(ctx->spriteManager);
}

ManagedSprite *PokeathlonUtil_NewSprite(PokeathlonSpriteContext *ctx, const ManagedSpriteTemplate *template) {
    return SpriteSystem_NewSprite(ctx->spriteSystem, ctx->spriteManager, template);
}

void PokeathlonUtil_CreateSpriteRow(PokeathlonSpriteContext *ctx, ManagedSprite **sprites, const int *resources, u32 activeCount, u32 startIndex, s16 x, s16 y, BOOL subScreen) {
    ManagedSpriteTemplate template = { 0 };
    u32 i;
    s32 xpos;
    ManagedSprite **out;

    GF_ASSERT(activeCount <= 10);
    xpos = x;
    out = sprites + startIndex;
    for (i = 0; i < 10; i++) {
        template.animation = i < activeCount;
        template.resIdList[0] = resources[0];
        template.resIdList[1] = resources[1];
        template.resIdList[2] = resources[2];
        template.resIdList[3] = resources[3];
        template.vram = subScreen ? NNS_G2D_VRAM_TYPE_2DMAIN : NNS_G2D_VRAM_TYPE_2DSUB;
        template.x = xpos;
        template.y = y;
        {
            ManagedSprite *sprite = PokeathlonUtil_NewSprite(ctx, &template);
            *out++ = sprite;
            ManagedSprite_SetAnimateFlag(sprite, TRUE);
        }
        xpos += 16;
    }
}

void ov98_0221E784(SpriteSystem *system, SpriteManager *manager, const int *resourceIds, int vram) {
    SpriteSystem_LoadCharResObj(system, manager, NARC_a_0_0_8, 76, FALSE, vram, resourceIds[0]);
    SpriteSystem_LoadPlttResObj(system, manager, NARC_a_0_0_8, 75, FALSE, 1, vram, resourceIds[1]);
    SpriteSystem_LoadCellResObj(system, manager, NARC_a_0_0_8, 77, FALSE, resourceIds[2]);
    SpriteSystem_LoadAnimResObj(system, manager, NARC_a_0_0_8, 78, FALSE, resourceIds[3]);
}

void PokeathlonUtil_LoadMonPicSprite(ManagedSprite *sprite, const PokeathlonMonInfo *mon, BOOL isMain, BOOL grayscale, enum HeapID heapId) {
    PokepicTemplate pokepic;
    TileRect rect;
    void *charBuf;

    if (mon->species == SPECIES_NONE) {
        ManagedSprite_SetDrawFlag(sprite, FALSE);
        return;
    }

    GetMonSpriteCharAndPlttNarcIdsEx(&pokepic, mon->species, mon->gender, MON_PIC_FACING_FRONT, mon->shiny, mon->form, mon->personality);
    rect = ov98_0221F19C;
    charBuf = Heap_AllocAtEnd(heapId, MON_PIC_BUFFER_SIZE);
    CharTiles_LoadMonRectAsObjFromNarc((NarcId)pokepic.narcID, pokepic.charDataID, heapId, &rect, charBuf, mon->personality, FALSE, MON_PIC_FACING_FRONT, mon->species);
    PokeathlonUtil_LoadSpriteChars(sprite, charBuf, MON_PIC_BUFFER_SIZE);
    PokeathlonUtil_LoadSpritePalette(sprite, (NarcId)pokepic.narcID, pokepic.palDataID, isMain, grayscale, heapId);
    ManagedSprite_SetDrawFlag(sprite, TRUE);
    Heap_Free(charBuf);
}

void PokeathlonUtil_LoadMonIconResources(PokeathlonSpriteContext *ctx, const int *resIds, int count, int screen, BOOL flag) {
    SpriteSystem *spriteSystem = ctx->spriteSystem;
    SpriteManager *spriteManager = ctx->spriteManager;
    int fileId;
    u32 cellFileId;
    u32 animFileId;
    int i;

    if (flag) {
        fileId = 0x1C;
    } else {
        fileId = 0x1D;
    }
    cellFileId = GetMonIconCell64KNaix();
    animFileId = GetMonIconAnim64KNaix();
    for (i = 0; i < count; i++) {
        SpriteSystem_LoadCharResObj(spriteSystem, spriteManager, NARC_a_1_7_7, fileId, TRUE, _0221F194[screen], resIds[GF_GFX_RES_TYPE_CHAR] + i);
    }
    SpriteSystem_LoadCellResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, cellFileId, FALSE, resIds[GF_GFX_RES_TYPE_CELL]);
    SpriteSystem_LoadAnimResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, animFileId, FALSE, resIds[GF_GFX_RES_TYPE_ANIM]);
    if (ctx->paletteOffset == -1) {
        ctx->paletteOffset = SpriteSystem_LoadPlttResObj(spriteSystem, spriteManager, NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), FALSE, 3, _0221F194[screen], resIds[GF_GFX_RES_TYPE_PLTT]);
    }
}

void PokeathlonUtil_SetMonIconSprite(PokeathlonSpriteContext *ctx, ManagedSprite *sprite, const PokeathlonMonInfo *mon, BOOL isMain, BOOL animate) {
    NNSG2dCharacterData *charData;
    void *buf;
    u32 form;
    u32 species;
    u32 size;
    u8 pal;

    size = 0x200;
    if (animate) {
        size <<= 1;
    }
    species = mon->species;
    form = mon->form;
    buf = PokeathlonUtil_LoadCharDataFromNarc(ctx, GetMonIconNaixEx(species, FALSE, form), &charData);
    PokeathlonUtil_LoadSpriteChars(sprite, charData->pRawData, size);
    Heap_Free(buf);
    ManagedSprite_SetDrawFlag(sprite, FALSE);
    pal = GetMonIconPaletteEx(species, form, FALSE);
    ManagedSprite_SetPaletteOverride(sprite, pal + ctx->paletteOffset);
    if (animate) {
        ManagedSprite_SetAnim(sprite, 1);
        ManagedSprite_SetAnimSpeed(sprite, FX32_ONE);
        ManagedSprite_SetAnimateFlag(sprite, TRUE);
    }
}

void PokeathlonUtil_LoadSpriteChars(ManagedSprite *sprite, void *data, u32 size) {
    NNS_G2D_VRAM_TYPE type = Sprite_GetVramType(sprite->sprite);
    u32 address = NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprite->sprite), type);

    DC_FlushRange(data, size);
    switch (type) {
    case NNS_G2D_VRAM_TYPE_2DMAIN:
        GX_LoadOBJ(data, address, size);
        break;
    case NNS_G2D_VRAM_TYPE_2DSUB:
        GXS_LoadOBJ(data, address, size);
        break;
    default:
        GF_ASSERT(FALSE);
        break;
    }
}

void PokeathlonUtil_LoadSpritePalette(ManagedSprite *sprite, NarcId narcId, s32 memberNo, BOOL isMain, BOOL grayscale, enum HeapID heapId) {
    NNS_G2D_VRAM_TYPE vramType;
    enum GFPalLoadLocation location;
    u32 palLocation;

    if (isMain) {
        vramType = NNS_G2D_VRAM_TYPE_2DMAIN;
        location = GF_PAL_LOCATION_MAIN_OBJ;
    } else {
        vramType = NNS_G2D_VRAM_TYPE_2DSUB;
        location = GF_PAL_LOCATION_SUB_OBJ;
    }
    palLocation = NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(sprite->sprite), vramType);
    GfGfxLoader_GXLoadPal(narcId, memberNo, location, (enum GFPalSlotOffset)palLocation, 0x20, heapId);
    if (grayscale) {
        u16 *palette;
        if (isMain) {
            palette = (u16 *)(HW_OBJ_PLTT + palLocation);
        } else {
            palette = (u16 *)(HW_DB_OBJ_PLTT + palLocation);
        }
        TintPalette_GrayScale(palette, 0x20);
    }
}

void *PokeathlonUtil_LoadCharDataFromNarc(PokeathlonSpriteContext *ctx, int member, NNSG2dCharacterData **out) {
    return GfGfxLoader_GetCharDataFromOpenNarc(ctx->narc, member, FALSE, out, ctx->heapId);
}

PokeathlonTextContext *PokeathlonUtil_CreateTextContext(enum HeapID heapId, BgConfig *bgConfig, int windowCount, const WindowTemplate *templates, s32 msgBank) {
    PokeathlonTextContext *ctx;
    u32 windowsSize;
    int i;

    FontID_Alloc(4, heapId);
    ctx = Heap_Alloc(heapId, sizeof(PokeathlonTextContext));
    MI_CpuFill8(ctx, 0, sizeof(PokeathlonTextContext));
    windowsSize = windowCount * sizeof(Window);
    ctx->windows = Heap_Alloc(heapId, windowsSize);
    MI_CpuFill8(ctx->windows, 0, windowsSize);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, heapId);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_12_OFFSET, heapId);
    for (i = 0; i < windowCount; i++) {
        AddWindow(bgConfig, &ctx->windows[i], &templates[i]);
        FillWindowPixelBuffer(&ctx->windows[i], 0);
        ClearWindowTilemap(&ctx->windows[i]);
    }
    ctx->msgDataFixed = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, NARC_msg_msg_0237_bin, heapId);
    ctx->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, msgBank, heapId);
    ctx->msgFormat = MessageFormat_New(heapId);
    ctx->string = String_New(0x400, heapId);
    ctx->heapId = heapId;
    ctx->windowCount = windowCount;
    return ctx;
}

void PokeathlonUtil_FreeTextContext(PokeathlonTextContext *ctx, int count) {
    int i;
    FontID_Release(4);
    String_Delete(ctx->string);
    MessageFormat_Delete(ctx->msgFormat);
    DestroyMsgData(ctx->msgDataFixed);
    DestroyMsgData(ctx->msgData);
    for (i = 0; i < count; i++) {
        ClearWindowTilemapAndCopyToVram(&ctx->windows[i]);
        RemoveWindow(&ctx->windows[i]);
    }
    Heap_Free(ctx->windows);
    Heap_Free(ctx);
}

void ov98_0221EBD8(PokeathlonTextContext *ctx, u32 window, u32 message, BOOL center) {
    ov98_0221EBEC(ctx, window, message, center, 0, 0);
}

void ov98_0221EBEC(PokeathlonTextContext *ctx, u32 window, u32 message, BOOL center, u8 y, u32 font) {
    PokeathlonUtil_PrintMessageEx(ctx, window, message, center, 0, y, font, 0);
}

void ov98_0221EC08(PokeathlonTextContext *ctx, u32 window, u32 message, u8 x, u8 y) {
    PokeathlonUtil_PrintMessageEx(ctx, window, message, FALSE, x, y, 0, 0);
}

void PokeathlonUtil_PrintMessageCentered(PokeathlonTextContext *ctx, u32 window, u32 message, u8 fill) {
    PokeathlonUtil_PrintMessageEx(ctx, window, message, TRUE, 0, 0, 0, fill);
}

void PokeathlonUtil_PrintMessageEx(PokeathlonTextContext *ctx, u32 window, u32 message, BOOL center, u8 x, u8 y, u32 fontId, u8 fill) {
    u8 xOffset = 0;
    String *string;

    GF_ASSERT(window < ctx->windowCount);
    string = ReadMsgData_ExpandPlaceholders(ctx->msgFormat, ctx->msgData, message, ctx->heapId);
    if (center) {
        xOffset = GetWindowWidth(&ctx->windows[window]) * 8;
        xOffset -= FontID_String_GetWidthMultiline(fontId, string, 0);
        xOffset /= 2;
    }
    FillWindowPixelBuffer(&ctx->windows[window], fill);
    AddTextPrinterParameterizedWithColor(&ctx->windows[window], fontId, string, xOffset + x, y, TEXT_SPEED_NOTRANSFER, TEXT_COLOR_BLACK_ON_WHITE, NULL);
    ScheduleWindowCopyToVram(&ctx->windows[window]);
    String_Delete(string);
}

void PokeathlonUtil_PrintMessageWithNumber(PokeathlonTextContext *ctx, u32 window, u32 message, s32 number, u32 digits, u32 field) {
    String *source;
    BufferIntegerAsString(ctx->msgFormat, field, number, digits, PRINTING_MODE_RIGHT_ALIGN, TRUE);
    source = NewString_ReadMsgData(ctx->msgData, message);
    StringExpandPlaceholders(ctx->msgFormat, ctx->string, source);
    PokeathlonUtil_ClearWindow(ctx, window);
    AddTextPrinterParameterizedWithColor(&ctx->windows[window], 0, ctx->string, 0, 0, TEXT_SPEED_NOTRANSFER, TEXT_COLOR_BLACK_ON_WHITE, NULL);
    ScheduleWindowCopyToVram(&ctx->windows[window]);
    String_Delete(source);
}

void PokeathlonUtil_PrintMessage(PokeathlonTextContext *ctx, u32 window, u32 message) {
    PokeathlonUtil_PrintMessageAt(ctx, window, message, 0, 0);
}

void PokeathlonUtil_PrintMessageAt(PokeathlonTextContext *ctx, u32 window, u32 message, u32 x, u8 y) {
    String *source = NewString_ReadMsgData(ctx->msgData, message);
    StringExpandPlaceholders(ctx->msgFormat, ctx->string, source);
    PokeathlonUtil_ClearWindow(ctx, window);
    AddTextPrinterParameterizedWithColor(&ctx->windows[window], 0, ctx->string, x, y, TEXT_SPEED_NOTRANSFER, TEXT_COLOR_BLACK_ON_WHITE, NULL);
    ScheduleWindowCopyToVram(&ctx->windows[window]);
    String_Delete(source);
}

void PokeathlonUtil_BufferNumber(PokeathlonTextContext *ctx, s32 number, u32 digits, u32 field) {
    BufferIntegerAsString(ctx->msgFormat, field, number, digits, PRINTING_MODE_RIGHT_ALIGN, TRUE);
}

void PokeathlonUtil_PrintMessageWithPlayerName(PokeathlonTextContext *ctx, u32 window, u32 message, u32 field, PlayerProfile *profile) {
    String *source;
    BufferPlayersName(ctx->msgFormat, field, profile);
    source = NewString_ReadMsgData(ctx->msgData, message);
    StringExpandPlaceholders(ctx->msgFormat, ctx->string, source);
    PokeathlonUtil_ClearWindow(ctx, window);
    AddTextPrinterParameterizedWithColor(&ctx->windows[window], 0, ctx->string, 0, 0, TEXT_SPEED_NOTRANSFER, TEXT_COLOR_BLACK_ON_WHITE, NULL);
    ScheduleWindowCopyToVram(&ctx->windows[window]);
    String_Delete(source);
}

void PokeathlonUtil_PrintSpeciesName(PokeathlonTextContext *ctx, u32 window, u32 message) {
    MsgData *msgData = ctx->msgDataFixed;
    GF_ASSERT(message <= 493);
    ReadMsgDataIntoString(msgData, message, ctx->string);
    FillWindowPixelBuffer(&ctx->windows[window], 0);
    AddTextPrinterParameterizedWithColor(&ctx->windows[window], 0, ctx->string, 0, 0, TEXT_SPEED_NOTRANSFER, TEXT_COLOR_BLACK_ON_WHITE, NULL);
    ScheduleWindowCopyToVram(&ctx->windows[window]);
}

void PokeathlonUtil_ClearAndCopyWindow(PokeathlonTextContext *ctx, u32 index) {
    PokeathlonUtil_ClearWindow(ctx, index);
    ScheduleWindowCopyToVram(&ctx->windows[index]);
}

void PokeathlonUtil_PrintString(PokeathlonTextContext *ctx, u32 window, String *string, u32 x, u8 y) {
    PokeathlonUtil_ClearWindow(ctx, window);
    AddTextPrinterParameterizedWithColor(&ctx->windows[window], 0, string, x, y, TEXT_SPEED_NOTRANSFER, TEXT_COLOR_BLACK_ON_WHITE, NULL);
    ScheduleWindowCopyToVram(&ctx->windows[window]);
}

void PokeathlonUtil_ClearWindowTilemap(PokeathlonTextContext *ctx, u32 index) {
    ClearWindowTilemapAndScheduleTransfer(&ctx->windows[index]);
}

void PokeathlonUtil_SetWindowY(PokeathlonTextContext *ctx, u32 index, u8 y) {
    SetWindowY(&ctx->windows[index], y);
}

void *PokeathlonUtil_GetMessageFormat(PokeathlonTextContext *ctx) {
    GF_ASSERT(ctx != NULL);
    GF_ASSERT(ctx->msgFormat != NULL);
    return ctx->msgFormat;
}

void PokeathlonUtil_ClearWindow(PokeathlonTextContext *ctx, u32 index) {
    FillWindowPixelBuffer(&ctx->windows[index], 0);
}

BOOL PokeathlonUtil_CheckExitInput(void) {
    TouchscreenHitbox hitbox = ov98_0221F1E0;
    BOOL result = FALSE;
    if (TouchscreenHitbox_TouchNewIsIn(&hitbox) || (gSystem.newKeys & PAD_BUTTON_B)) {
        result = TRUE;
    }
    return result;
}

u32 ov98_0221EF64(u32 value) {
    if (value >= 450) {
        return 2;
    }
    if (value >= 420) {
        return 1;
    }
    return 0;
}

u8 ov98_0221EF80(s32 value) {
    u8 i = 0;
    do {
        if (ov98_0221F220[i] <= value) {
            break;
        }
        i++;
    } while (i < 10);
    return 10 - i;
}

u16 ov98_0221EFA4(u32 row, u32 column) {
    return ov98_0221F1F8[row][column];
}

BOOL ov98_0221EFB4(u32 direction, u32 column, u16 value) {
    u16 limit = ov98_0221EFA4(direction, column);
    if (value == 0xFFFF) {
        return FALSE;
    }
    if (direction == 0) {
        return value < limit;
    }
    return value > limit;
}

BOOL ov98_0221EFE8(u32 direction, u32 column, u16 value) {
    u16 limit = ov98_0221EFA4(direction, column);
    if (value == 0xFFFF) {
        return FALSE;
    }
    if (direction == 0) {
        return value <= limit;
    }
    return value >= limit;
}

u16 ov98_0221F01C(const void *records, u32 index) {
    return *(const u16 *)((const u8 *)records + index * 8);
}

u32 PokeathlonUtil_CountSpeciesWithAllMedals(const u8 *data) {
    int i;
    u16 count = 0;
    GF_ASSERT(data != NULL);
    for (i = 0; i < 0x1ED; i++) {
        if (MATH_CountPopulation(data[i]) == 5) {
            count++;
        }
    }
    return count;
}

u8 ov98_0221F058(const u16 (*records)[22]) {
    u8 i;
    u8 count = 0;
    for (i = 0; i < 10; i++) {
        u16 value = ov98_0221F01C(&records[i], 0);
        if (ov98_0221EFE8(i, 1, value)) {
            count++;
        }
    }
    return count;
}

void PokeathlonUtil_ResetDisplay(void) {
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
    reg_GX_POWCNT &= 0xFFFF7FFF;
    PokeathlonUtil_SetGfxBanks();
}

void PokeathlonUtil_DisableDisplay(void) {
    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT &= 0xFFFFE0FF;
    reg_GXS_DB_DISPCNT &= 0xFFFFE0FF;
}

u32 PokeathlonUtil_GetDecimalDigit(u32 value, u32 digit) {
    u32 remainder;
    GF_ASSERT(digit != 0 && digit < 5);
    remainder = value % ov98_0221F1E4[digit];
    digit--;
    return remainder / ov98_0221F1E4[digit];
}

u8 PokeathlonUtil_CountDecimalDigits(s32 value) {
    u8 count = 0;
    if (value == 0) {
        return 1;
    }
    while (1) {
        if (value == 0) {
            break;
        }
        value /= 10;
        count++;
    }
    return count;
}

void PokeathlonUtil_SetGfxBanks(void) {
    GraphicsBanks banks = ov98_0221F248;
    GfGfx_SetBanks(&banks);
}
