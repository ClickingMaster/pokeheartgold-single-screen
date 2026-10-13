#include "application/main_menu/main_menu_util.h"

#include "global.h"

#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/species.h"

#include "application/main_menu/main_menu_internal.h"
#include "msgdata/msg.naix"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "oam_manager.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "overlay_manager.h"
#include "render_window.h"
#include "screen_fade.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "unk_02009D48.h"
#include "sprite_resource.h"
#include "vram_transfer_manager.h"

// GBA cartridge access (NitroSDK CTRDG) and the crypto library
u32 CTRDG_GetAgbGameCode(void);
u16 CTRDG_GetAgbMakerCode(void);
void CTRDG_Init(void);
BOOL CTRDG_IsAgbCartridge(void);
BOOL CTRDG_IsExisting(void);
void CTRDG_Enable(BOOL enable);
void CTRDG_Read32(const void *src, void *dest);
BOOL CTRDG_CpuCopy8(const void *src, void *dest, u32 size);
BOOL CTRDG_CpuCopy16(const void *src, void *dest, u32 size);
void CRYPTO_SetAllocator(void *(*alloc)(u32), void (*free)(void *));
BOOL CRYPTO_VerifySignature(const void *data, u32 size, const void *signature, const void *publicKey);

// Declared here rather than through text.h / font.h / pokemon.h / item.h: the font id is passed as an int, and the
// gift graphics use the pieces' own signatures of the Pokemon and item helpers (matching).
int AddTextPrinterParameterizedWithColor(Window *window, int fontId, String *string, int x, int y, int textSpeed, u32 color, void *callback);
int FontID_String_GetWidth(int fontId, String *string, int letterSpacing);
u8 GetFontAttribute(u8 fontId, int attr);
u32 GetMonGender(void *mon);
u32 MonIsShiny(void *mon);
void GetMonSpriteCharAndPlttNarcIdsEx(void *template, u16 species, u8 gender, int whichFacing, u8 shiny, u8 form, int personality);
u32 GetMonData(void *mon, int field, void *dest);
void CharTiles_LoadMonObjBlocksFromNarc(u16 narcId, u16 charId, enum HeapID heapId, int x, int y, int width, int height, void *dest, u32 personality, int isAnimated, int whichFacing, int species);
u32 GetItemIndexMapping(u16 itemId, int attr);
u32 GetItemIconCell(u32 plttId);
u32 GetItemIconAnim(u32 cell);

typedef struct MainMenuUtilGiftPalettes {
    u8 data[34]; // gift type, palette; ...
} MainMenuUtilGiftPalettes;

// A gift as shown by the card screens (only the Pokemon is read here)
typedef struct MainMenuUtilGift {
    u16 type;
    u8 unk_02[2];
    u32 itemId;
    u8 mon[1]; // Pokemon
} MainMenuUtilGift;

// The work and the labels inside it; sMainMenuUtilCryptoHeapId + 1 is gMainMenuUtilWork (some functions take its address through the
// preceding label: a second literal-pool entry of the same address, matching)
extern enum HeapID sMainMenuUtilCryptoHeapId; // heap of the crypto library
extern MainMenuUtilWork gMainMenuUtilWork;
extern MainMenuUtilSprites ov74_0223D488; // = &gMainMenuUtilWork.sprites
extern G2dRenderer ov74_0223D48C;         // = &gMainMenuUtilWork.sprites.renderer
extern u32 ov74_0223D654[];               // = gMainMenuUtilWork + 0x200
extern u32 ov74_0223E2D4[];               // = gMainMenuUtilWork + 0xE80
extern const u8 sGiftCartridgePublicKey[];
extern const ObjCharTransferTemplate _0223B710;
extern const BgTemplate _0223B720;
extern const MainMenuUtilGiftPalettes _0223B73C;
extern u32 sGiftCartridgeGameCodes[];
extern u16 ov74_0223CC5E[];

void MainMenuUtil_SetCryptoHeap(enum HeapID heapID) {
    sMainMenuUtilCryptoHeapId = heapID;
}

void *MainMenuUtil_CryptoAlloc(u32 size) {
    return Heap_Alloc(sMainMenuUtilCryptoHeapId, size);
}

#pragma force_active on

// A Mystery Gift GBA cartridge: one of the game codes of sGiftCartridgeGameCodes, with a payload signed with the key sGiftCartridgePublicKey
int MainMenuUtil_CheckGiftCartridgeSignature(void) {
    u8 payload[0x4A8];
    u8 signature[0x80];
    u32 gameCode = CTRDG_GetAgbGameCode();
    u32 code = (u8)(gameCode >> 24) | ((gameCode << 24) | (((gameCode >> 8) << 24) >> 8) | (((gameCode >> 16) << 24) >> 16));
    u32 i = 0;
    if (i < sGiftCartridgeGameCodes[0]) {
        // sGiftCartridgeGameCodes, reached through the end of the preceding table (second literal-pool entry, matching)
        u32 *codes = (u32 *)(ov74_0223CC5E + 0x11F);
        do {
            if (code == *codes) {
                CTRDG_Enable(TRUE);
                CTRDG_CpuCopy8((void *)0x08100000, payload, 0x4A8);
                CTRDG_CpuCopy8((void *)0x08020000, signature, 0x80);
                CTRDG_Enable(FALSE);
                CRYPTO_SetAllocator(MainMenuUtil_CryptoAlloc, Heap_Free);
                if (CRYPTO_VerifySignature(payload, 0x4A8, signature, sGiftCartridgePublicKey)) {
                    return TRUE;
                }
            }
            i++;
            codes++;
        } while (i < *codes);
    }
    return FALSE;
}

#pragma force_active reset

BOOL MainMenuUtil_IsGiftCartridgeInserted(void) {
    CTRDG_Init();
    if (CTRDG_IsAgbCartridge() == FALSE) {
        return FALSE;
    }
    if (CTRDG_GetAgbMakerCode() != 0x3130) { // "01"
        return FALSE;
    }
    if (MainMenuUtil_CheckGiftCartridgeSignature() == FALSE) {
        return FALSE;
    }
    return TRUE;
}

// Size of the gift on the cartridge (0: no gift cartridge)
u32 MainMenuUtil_GetCartridgeGiftSize(void) {
    u32 size;
    if (MainMenuUtil_IsGiftCartridgeInserted() == FALSE) {
        return 0;
    }
    CTRDG_Enable(TRUE);
    CTRDG_Read32((void *)0x08100000, &size);
    CTRDG_Enable(FALSE);
    return size;
}

BOOL MainMenuUtil_ReadCartridgeGift(void *dest, int size) {
    BOOL success;
    if (MainMenuUtil_IsGiftCartridgeInserted() == FALSE) {
        return FALSE;
    }
    if (size == 0) {
        size = MainMenuUtil_GetCartridgeGiftSize();
    }
    CTRDG_Enable(TRUE);
    success = CTRDG_CpuCopy16((void *)0x08100100, dest, size);
    CTRDG_Enable(FALSE);
    if (CTRDG_IsExisting() == FALSE) {
        return FALSE;
    }
    return success;
}

#pragma force_active on

void MainMenuUtil_InitWork(enum HeapID heapID) {
    memset(&sMainMenuUtilCryptoHeapId + 1, 0, sizeof(MainMenuUtilWork));
    gMainMenuUtilWork.unk_004 = 1;
    gMainMenuUtilWork.heapId = heapID;
}

#pragma force_active reset

void *MainMenuUtil_CreateAppData(OverlayManager *manager, enum HeapID heapID, u32 size, u32 heapSize) {
    void *data;
    Heap_Create(HEAP_ID_3, heapID, heapSize);
    data = OverlayManager_CreateAndGetData(manager, size, heapID);
    memset(data, 0, size);
    SetMasterBrightnessByColor(PM_LCD_TOP, RGB_BLACK);
    SetMasterBrightnessByColor(PM_LCD_BOTTOM, RGB_BLACK);
    return data;
}

#pragma force_active on

void MainMenuUtil_InitBg(BgConfig *bgConfig, int bgId, u32 screenSize, u32 screenBase, u32 charBase) {
    BgTemplate template = _0223B720;
    template.size = screenSize;
    switch (screenSize) {
    case GF_BG_SCR_SIZE_256x256:
        template.bufferSize = 0x800;
        break;
    case GF_BG_SCR_SIZE_256x512:
        template.bufferSize = 0x1000;
        break;
    case GF_BG_SCR_SIZE_512x256:
        template.bufferSize = 0x1000;
        break;
    case GF_BG_SCR_SIZE_512x512:
        template.bufferSize = 0x2000;
        break;
    }
    template.screenBase = screenBase >> 11;
    template.charBase = charBase >> 14;
    InitBgFromTemplate(bgConfig, (u8)bgId, &template, GF_BG_TYPE_TEXT);
    BgClearTilemapBufferAndCommit(bgConfig, (u8)bgId);
}

#pragma force_active reset

void MainMenuUtil_SetFadeWhite(u32 fadeWhite) {
    gMainMenuUtilWork.fadeWhite = fadeWhite;
}

#pragma force_active on

void MainMenuUtil_StartFade(u32 fadeType, u32 nextState, int *state, u32 waitingState) {
    MainMenuUtilWork *work = (MainMenuUtilWork *)(&sMainMenuUtilCryptoHeapId + 1);
    if (gMainMenuUtilWork.fadeWhite == FALSE) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)fadeType, (enum FadeType)fadeType, RGB_BLACK, 6, 1, work->heapId);
    } else {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, (enum FadeType)fadeType, (enum FadeType)fadeType, RGB_WHITE, 6, 1, work->heapId);
    }
    if (state != NULL) {
        *state = waitingState;
    }
    work->nextState = nextState;
}

void MainMenuUtil_WaitFade(int *state) {
    MainMenuUtilWork *work = &gMainMenuUtilWork;
    if (IsPaletteFadeFinished()) {
        *state = work->nextState;
    }
}

#pragma force_active reset

// The message box printer (MainMenuTextPrinter)
void MainMenuTextPrinter_Init(MainMenuTextPrinter *printer, Window *window, u32 palette, u32 msgFileId, u32 frameBaseTile, u32 framePalette) {
    memset(printer, 0, sizeof(MainMenuTextPrinter));
    printer->drawFrame = 1;
    printer->clearWindow = TRUE;
    printer->msgId = -1;
    printer->window = window;
    printer->palette = palette;
    printer->msgFileId = msgFileId;
    printer->frameBaseTile = frameBaseTile;
    printer->framePalette = framePalette;
    printer->bgId = 0;
    printer->fontId = 0;
    printer->color = 0x01020F; // text 1, shadow 2, background 15
    printer->fillColor = 15;
    printer->frameType = 0;
    printer->textY = 0;
    printer->textX = 0;
    printer->msgFormat = NULL;
    printer->speed = 0xFF;
}

void MainMenuTextPrinter_SetSize(MainMenuTextPrinter *printer, u32 width, u32 height, u32 baseTile) {
    printer->width = width;
    printer->height = height;
    printer->baseTile = baseTile;
}

void MainMenuTextPrinter_SetFrameAndFont(MainMenuTextPrinter *printer, u32 frameType, u32 fontId) {
    printer->frameType = frameType;
    printer->fontId = fontId;
}

void MainMenuTextPrinter_SetTextPos(MainMenuTextPrinter *printer, u32 textX, u32 textY) {
    printer->textX = textX;
    printer->textY = textY;
}

#pragma force_active on

// Prints message msgId of the printer's message file (if it is not already shown); returns the text printer id
int MainMenuTextPrinter_Print(MainMenuTextPrinter *printer, int msgId) {
    MainMenuUtilWork *work = &gMainMenuUtilWork;
    int printerId;
    if (msgId != -1 && printer->msgId != msgId) {
        printer->msgId = msgId;
        if (printer->clearWindow == TRUE) {
            FillWindowPixelBuffer(printer->window, printer->fillColor);
        }
        if (printer->msgId != -1) {
            MsgData *msgData = NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, printer->msgFileId, work->heapId);
            MessageFormat *msgFormat = printer->msgFormat;
            String *string;
            if (msgFormat == NULL) {
                msgFormat = MessageFormat_New(work->heapId);
            }
            string = ReadMsgData_ExpandPlaceholders(msgFormat, msgData, printer->msgId, work->heapId);
            if (printer->rightAlign == FALSE) {
                printerId = AddTextPrinterParameterizedWithColor(printer->window, printer->fontId, string, printer->textX, printer->textY, printer->speed, printer->color, NULL);
            } else {
                int width = FontID_String_GetWidth(printer->fontId, string, GetFontAttribute(printer->fontId, 2));
                int x = GetWindowWidth(printer->window) * 8 - width;
                printerId = AddTextPrinterParameterizedWithColor(printer->window, printer->fontId, string, x, printer->textY, printer->speed, printer->color, NULL);
                printer->rightAlign = FALSE;
            }
            String_Delete(string);
            if (printer->msgFormat == NULL) {
                MessageFormat_Delete(msgFormat);
            }
            DestroyMsgData(msgData);
        }
    }
    printer->speed = 0xFF;
    return printerId;
}

// Adds the printer's window at x, y (moves it if it exists; -1: keep), prints msgId and draws the frame
int MainMenuTextPrinter_AddWindowAndPrint(BgConfig *bgConfig, MainMenuTextPrinter *printer, int x, int y, int msgId) {
    u32 result;
    if (printer->window->bgConfig == NULL) {
        AddWindowParameterized(bgConfig, printer->window, (u8)printer->bgId, (u8)x, (u8)y, (u8)printer->width, (u8)printer->height, (u8)printer->palette, (u16)printer->baseTile);
        result = MainMenuTextPrinter_Print(printer, msgId);
    } else {
        if (x != -1) {
            SetWindowX(printer->window, (u8)x);
        }
        if (y != -1) {
            SetWindowY(printer->window, (u8)y);
        }
        result = MainMenuTextPrinter_Print(printer, msgId);
    }
    if (printer->drawFrame == 1) {
        switch (printer->frameType) {
        case 0:
            DrawFrameAndWindow1(printer->window, FALSE, (u16)printer->frameBaseTile, (u8)printer->framePalette);
            break;
        case 1:
            DrawFrameAndWindow2(printer->window, FALSE, (u16)printer->frameBaseTile, (u8)printer->framePalette);
            break;
        case 2:
        default:
            CopyWindowToVram(printer->window);
            break;
        }
    }
    return result;
}

#pragma force_active reset

u32 MainMenuTextPrinter_GetTileCount(MainMenuTextPrinter *printer) {
    return printer->width * printer->height;
}

#pragma force_active on

void MainMenuUtil_InitObjTransfer(void) {
    ObjCharTransferTemplate template = _0223B710;
    template.heapID = gMainMenuUtilWork.heapId;
    ObjCharTransfer_InitEx(&template, GX_OBJVRAMMODE_CHAR_1D_32K, GX_OBJVRAMMODE_CHAR_1D_32K);
    ObjPlttTransfer_Init(30, gMainMenuUtilWork.heapId);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}

#pragma force_active reset

u32 MainMenuUtil_SpritesActive(void) {
    return gMainMenuUtilWork.sprites.spriteList != NULL;
}

#pragma force_active on

void MainMenuUtil_InitSprites(void) {
    MainMenuUtilWork *work = (MainMenuUtilWork *)(&sMainMenuUtilCryptoHeapId + 1);
    int i;
    NNS_G2dInitOamManagerModule();
    OamManager_Create(0, 126, 0, 32, 0, 126, 0, 32, gMainMenuUtilWork.heapId);
    gMainMenuUtilWork.sprites.spriteList = G2dRenderer_Init(128, &ov74_0223D48C, gMainMenuUtilWork.heapId);
    G2dRenderer_SetSubSurfaceCoords(&ov74_0223D48C, 0, FX32_CONST(256));
    *(fx32 *)((u8 *)ov74_0223D654 + 0x10) = FX32_CONST(192);
    for (i = 0; i < 6; i++) {
        work->sprites.resMan[i] = Create2DGfxResObjMan(32, (GfGfxResType)i, work->heapId);
    }
}

MainMenuUtilSprites *MainMenuUtil_GetSpriteList(void) {
    MainMenuUtilWork *work = (MainMenuUtilWork *)(&sMainMenuUtilCryptoHeapId + 1);
    GF_ASSERT(gMainMenuUtilWork.sprites.spriteList != NULL);
    return &work->sprites;
}

#pragma force_active reset

#pragma force_active on
#pragma push
#pragma opt_common_subs off

// Loads the char/palette/cell/animation resources of one screen (0 main, 1 sub; -1: none) from a NARC
void MainMenuUtil_LoadSpriteResources(NarcId narcId, u32 charFileId, u32 plttFileId, u32 cellFileId, u32 animFileId, u32 screen) {
    MainMenuUtilWork *work = &gMainMenuUtilWork;
    int vram = screen == 0 ? NNS_G2D_VRAM_TYPE_2DMAIN : NNS_G2D_VRAM_TYPE_2DSUB;
    int compressed = TRUE;
    if (narcId == NARC_itemtool_itemdata_item_icon) {
        compressed = FALSE;
    }
    if (charFileId != -1) {
        work->sprites.spriteRes[screen].charRes = AddCharResObjFromNarc(work->sprites.resMan[GF_GFX_RES_TYPE_CHAR], narcId, charFileId, compressed, screen, vram, work->heapId);
    }
    if (plttFileId != -1) {
        work->sprites.spriteRes[screen].plttRes = AddPlttResObjFromNarc(work->sprites.resMan[GF_GFX_RES_TYPE_PLTT], narcId, plttFileId, FALSE, screen, vram, 3, work->heapId);
    }
    if (cellFileId != -1) {
        work->sprites.spriteRes[screen].cellRes = AddCellOrAnimResObjFromNarc(work->sprites.resMan[GF_GFX_RES_TYPE_CELL], narcId, cellFileId, compressed, screen, GF_GFX_RES_TYPE_CELL, work->heapId);
    }
    if (animFileId != -1) {
        work->sprites.spriteRes[screen].animRes = AddCellOrAnimResObjFromNarc(work->sprites.resMan[GF_GFX_RES_TYPE_ANIM], narcId, animFileId, compressed, screen, GF_GFX_RES_TYPE_ANIM, work->heapId);
    }
    {
        // spriteRes[screen] through a work pointer shifted by the screen's offset, computed once (matching)
        int offset = screen * sizeof(MainMenuUtilSpriteRes);
        SpriteTransfer_CreateCharTransferTask(((MainMenuUtilWork *)((u8 *)work + offset))->sprites.spriteRes[0].charRes);
        SpriteTransfer_CreatePlttTransferTask(((MainMenuUtilWork *)((u8 *)work + offset))->sprites.spriteRes[0].plttRes);
    }
    CreateSpriteResourcesHeader(&work->sprites.resHeaders[screen], screen, screen, screen, screen, -1, -1, FALSE, 0, work->sprites.resMan[GF_GFX_RES_TYPE_CHAR], work->sprites.resMan[GF_GFX_RES_TYPE_PLTT], work->sprites.resMan[GF_GFX_RES_TYPE_CELL], work->sprites.resMan[GF_GFX_RES_TYPE_ANIM], NULL, NULL);
    if (screen == 0) {
        GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    } else {
        GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    }
    Main_SetVBlankIntrCB((GFIntrCB)MainMenuUtil_VBlankCB, NULL);
}

#pragma pop
#pragma force_active reset

void MainMenuUtil_SetSubScreenOffset(fx32 x, fx32 y) {
    G2dRenderer_SetSubSurfaceCoords(&ov74_0223D48C, x, y);
    *(fx32 *)((u8 *)ov74_0223D654 + 0x10) = y;
}

void MainMenuUtil_UpdateSprites(void) {
    MainMenuUtil_UpdateGiftSprite(FALSE);
}

#pragma force_active on

// Per frame: the gift sprite of the sub screen appears after its delay and slides down; renders the sprites
void MainMenuUtil_UpdateGiftSprite(int hideGiftSprite) {
    MainMenuUtilWork *work = &gMainMenuUtilWork;
    if (ov74_0223D654[2]) {
        if (work->giftSpriteDelay == 0) {
            VecFx32 *pos;
            if (!Sprite_GetDrawFlag(work->giftSprite) && hideGiftSprite == FALSE) {
                Sprite_SetDrawFlag(work->giftSprite, TRUE);
            }
            pos = Sprite_GetMatrixPtr(work->giftSprite);
            if (pos->y < FX32_CONST(384)) {
                pos->y += FX32_CONST(3);
            }
        } else {
            work->giftSpriteDelay--;
        }
    }
    if (work->sprites.spriteList) {
        SpriteList_RenderAndAnimateSprites(work->sprites.spriteList);
    }
}

#pragma force_active reset

#pragma force_active on

// Creates (if needed) and shows a sprite on screen 0 (main) / 1 (sub)
Sprite *MainMenuUtil_CreateSprite(u32 screen, Sprite *sprite, u32 x, u32 y, u32 animSeq) {
    SpriteTemplate template;
    MainMenuUtilWork *work = &gMainMenuUtilWork;
    if (!sprite) {
        template.spriteList = work->sprites.spriteList;
        x <<= FX32_SHIFT;
        template.header = &work->sprites.resHeaders[screen];
        template.position.z = 0;
        template.scale.x = FX32_ONE;
        template.scale.y = FX32_ONE;
        template.scale.z = FX32_ONE;
        template.rotation = 0;
        template.position.x = x;
        template.position.y = y << FX32_SHIFT;
        template.drawPriority = 10;
        template.whichScreen = screen == 0 ? NNS_G2D_VRAM_TYPE_2DMAIN : NNS_G2D_VRAM_TYPE_2DSUB;
        template.heapID = work->heapId;
        if (template.whichScreen == NNS_G2D_VRAM_TYPE_2DSUB) {
            template.position.y += work->subScreenY;
        }
        sprite = Sprite_CreateAffine(&template);
    }
    Sprite_SetAnimActiveFlag(sprite, TRUE);
    Sprite_SetPriority(sprite, 0);
    Sprite_SetAnimCtrlSeq(sprite, animSeq);
    Sprite_SetDrawFlag(sprite, TRUE);
    return sprite;
}

void MainMenuUtil_FreeSprites(void) {
    MainMenuUtilWork *work = &gMainMenuUtilWork;
    MainMenuUtilSprites *sprites = &ov74_0223D488;
    int i;
    if (ov74_0223D654[2]) {
        Sprite_Delete(work->giftSprite);
        work->giftSprite = NULL;
    }
    if (sprites->spriteRes[0].charRes) {
        SpriteTransfer_DeleteCharTransferTask(sprites->spriteRes[0].charRes);
    }
    if (sprites->spriteRes[1].charRes) {
        SpriteTransfer_DeleteCharTransferTask(sprites->spriteRes[1].charRes);
    }
    if (sprites->spriteRes[0].plttRes) {
        SpriteTransfer_DeletePlttTransferTask(sprites->spriteRes[0].plttRes);
    }
    if (sprites->spriteRes[1].plttRes) {
        SpriteTransfer_DeletePlttTransferTask(sprites->spriteRes[1].plttRes);
    }
    for (i = 0; i < 6; i++) {
        Destroy2DGfxResObjMan(sprites->resMan[i]);
        sprites->resMan[i] = NULL;
    }
    SpriteList_Delete(sprites->spriteList);
    sprites->spriteList = NULL;
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    Main_SetVBlankIntrCB(NULL, NULL);
}

#pragma force_active reset

u32 ov74_02235A68(int spriteId) {
    return 1 << (spriteId - 10);
}

#pragma force_active on

void MainMenuUtil_VBlankCB(void) {
    MainMenuUtilWork *work = &gMainMenuUtilWork;
    if (ov74_0223E2D4[9]) { // = work->vblankCallback
        work->vblankCallback(work);
        work->vblankCallback = NULL;
    }
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    if (work->bgConfig) {
        DoScheduledBgGpuUpdates(work->bgConfig);
    }
    {
        // OS_SetIrqCheckFlag(OS_IE_V_BLANK), with the address of the check flag spelled out (matching)
        volatile u32 *dtcm = (volatile u32 *)0x027E0000;
        dtcm[0x3FF8 / 4] |= OS_IE_V_BLANK;
    }
}

// Palette of the gift graphics per gift type (pairs in _0223B73C, 0xFF-terminated)
int MainMenuUtil_GetGiftPalette(int giftType) {
    MainMenuUtilGiftPalettes palettes = _0223B73C;
    int i;
    u8 *entry;
    i = 0;
    // u8 compared with -1: the 0xFF terminator never matches, the search ends at the gift type (matching)
    if (palettes.data[0] != -1) {
        entry = palettes.data;
        do {
            if (giftType == *entry) {
                return (palettes.data + 1)[i * 2];
            }
            entry += 2;
            i++;
        } while (*entry != -1);
    }
    return 0;
}

#pragma force_active reset

// Loads the front sprite of the gift Pokemon into the gift sprite of the sub screen
void MainMenuUtil_LoadGiftMonSprite(Sprite *sprite, Pokemon *mon, int species, int form, void *charData, u16 *narcIds) {
    u32 gender = GetMonGender(mon);
    u8 shiny = MonIsShiny(mon);
    GetMonSpriteCharAndPlttNarcIdsEx(narcIds, species, gender, 2, shiny, form, 0);
    {
        u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);
        CharTiles_LoadMonObjBlocksFromNarc(narcIds[0], narcIds[1], gMainMenuUtilWork.heapId, 0, 0, 10, 10, charData, personality, 0, 2, species);
    }
    DC_FlushRange(charData, 0xC80);
    GXS_LoadOBJ(charData, NNS_G2dGetImageLocation(Sprite_GetImageProxy(sprite), NNS_G2D_VRAM_TYPE_2DSUB) + 0xC80, 0xC80);
    GfGfxLoader_GXLoadPal((NarcId)narcIds[0], narcIds[2], GF_PAL_LOCATION_SUB_OBJ, (enum GFPalSlotOffset)(NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(sprite), NNS_G2D_VRAM_TYPE_2DSUB) + 0x60), 0x20, gMainMenuUtilWork.heapId);
}

// Gift graphics of the card screens: the gift box
void MainMenuUtil_ShowGiftBox(MainMenuUtilWork *work, int giftType, void *gift) {
    MainMenuUtil_LoadSpriteResources(NARC_a_1_1_3, 39, 36, 38, 37, 1);
    MainMenuUtil_SetSubScreenOffset(0, FX32_CONST(256));
    work->giftSprite = MainMenuUtil_CreateSprite(1, work->giftSprite, 128, 0, 0);
}

#pragma force_active on

// The gift Pokemon (types 1, 13), Egg (2) or Manaphy Egg (7)
void MainMenuUtil_ShowGiftPokemon(MainMenuUtilWork *work, int giftType, void *gift) {
    void *mon;
    MainMenuUtil_LoadSpriteResources(NARC_a_0_5_2, 10, 0, 11, 12, 1);
    MainMenuUtil_SetSubScreenOffset(0, FX32_CONST(256));
    work->giftSprite = MainMenuUtil_CreateSprite(1, work->giftSprite, 128, 0, 1);
    mon = ((MainMenuUtilGift *)gift)->mon;
    switch (giftType) {
    case 1:
    case 13: {
        u32 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
        u32 form = GetMonData(mon, MON_DATA_FORM, NULL);
        MainMenuUtil_LoadGiftMonSprite(work->giftSprite, mon, species, form, work->monCharData, work->narcIds);
        break;
    }
    case 2:
        MainMenuUtil_LoadGiftMonSprite(work->giftSprite, mon, SPECIES_EGG, 0, work->monCharData, work->narcIds);
        break;
    case 7:
        MainMenuUtil_LoadGiftMonSprite(work->giftSprite, mon, SPECIES_EGG, 1, work->monCharData, work->narcIds);
        break;
    }
}

#pragma force_active reset

// The gift item (type 3) or the item of a special gift
void MainMenuUtil_ShowGiftItem(MainMenuUtilWork *work, int giftType, void *gift) {
    u32 item;
    u32 charId;
    u32 plttId;
    u32 cell;
    u32 anim;
    switch (giftType) {
    case 3:
        item = ((MainMenuUtilGift *)gift)->itemId;
        break;
    case 8:
        item = ITEM_MEMBER_CARD;
        break;
    case 9:
        item = ITEM_OAKS_LETTER;
        break;
    case 10:
        item = ITEM_AZURE_FLUTE;
        break;
    case 12:
        item = ITEM_SECRET_KEY;
        break;
    case 14:
        item = ITEM_TOWN_MAP;
        break;
    case 15:
        item = ITEM_PHOTO_ALBUM;
        break;
    }
    charId = GetItemIndexMapping(item, 1);
    plttId = GetItemIndexMapping(item, 2);
    cell = GetItemIconCell(plttId);
    anim = GetItemIconAnim(cell);
    MainMenuUtil_LoadSpriteResources(NARC_itemtool_itemdata_item_icon, charId, plttId, cell, anim, 1);
    MainMenuUtil_SetSubScreenOffset(0, FX32_CONST(256));
    work->giftSprite = MainMenuUtil_CreateSprite(1, work->giftSprite, 128, 0, 0);
}

// VBlank task of the card screens: the sub screen palette
void MainMenuUtil_LoadCardSubScreenPalette(MainMenuUtilWork *work) {
    GfGfxLoader_GXLoadPal(NARC_a_1_1_3, 33, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_8_OFFSET, 0xC0, work->heapId);
}
