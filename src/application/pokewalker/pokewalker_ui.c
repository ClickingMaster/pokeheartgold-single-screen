// Pokewalker connection app: shared UI helpers. Pokewalker picture files, course unlocking, the yes/no menu,
// 2bpp pixel access and Spinda's spots, window banks, message strings and printing, the waiting icon, sprites
// (cursor, Pokemon icons) and the particle effect shown while data is transferred.

#include "global.h"

#include "application/pokewalker/pokewalker_app.h"
#include "msgdata/msg/msg_0274.h"

#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "math_util.h"
#include "pm_string.h"
#include "pokemon.h"
#include "pokemon_icon_idx.h"
#include "render_window.h"
#include "sound_playback.h"
#include "system.h"
#include "text.h"
#include "touchscreen.h"
#include "unk_02009D48.h"
#include "vram_transfer_manager.h"

// Pixel positions of one of Spinda's four spots, terminated by x = 0xFF.
typedef struct PokewalkerSpotPixel {
    u8 x;
    u8 y;
} PokewalkerSpotPixel;

extern const u16 sPokewalkerPictureFiles[][2]; // Pokewalker picture file per species: {male, female}
extern const u8 sWindowBankScreens[];          // window bank -> 0 main screen, 1 sub screen
extern const u8 sPokewalkerAppSpritePalettes[];              // palettes of the first 17 sprites
extern const OamCharTransferParam sPokewalkerAppOamCharTransferParam;
extern const SpriteResourceCountsListUnion sPokewalkerAppSpriteResourceCounts;
extern const OamManagerParam sPokewalkerAppOamManagerParam;
// Cursor positions {x, y, anim, palette}; each column has its own label.
extern const u8 sPokewalkerAppCursorX[];
extern const u8 sPokewalkerAppCursorY[];
extern const u8 sPokewalkerAppCursorAnims[];
extern const u8 sPokewalkerAppCursorPalettes[];
extern const ManagedSpriteTemplate sPokewalkerAppSpriteTemplates[];
extern const ManagedSpriteTemplate sPokewalkerAppMainSpriteTemplate;
extern const ManagedSpriteTemplate sPokewalkerAppSubSpriteTemplate;
extern const PokewalkerSpotPixel *sSpindaSpotPixels[];
extern const TouchscreenHitbox *sTouchButtons[];
extern const WindowTemplate *sWindowBank2Templates[];
extern const WindowTemplate *sWindowBank1Templates[];
extern const WindowTemplate *sWindowBank7Templates[];
extern const WindowTemplate *sWindowBank0Templates[];
extern const WindowTemplate *sWindowBank5Templates[];
extern const WindowTemplate *sWindowBank3Templates[];
extern const WindowTemplate *sWindowBank4Templates[];
extern const WindowTemplate *sWindowBank6Templates[];

u32 PokewalkerApp_GetBoxMonIconFile(BoxPokemon *boxMon) {
    return Boxmon_GetIconNaix(boxMon) - 7;
}

u32 PokewalkerApp_GetSpeciesIconFile(u16 species) {
    return GetMonIconNaixEx(species, FALSE, 0) - 7;
}

int PokewalkerApp_GetPictureFile(int species, int gender, int form) {
    int base = SanitizeMonForm((u16)species, (u8)form);
    switch (species) {
    case SPECIES_BURMY:
        return base + 0x26E;
    case SPECIES_WORMADAM:
        return base + 0x271;
    case SPECIES_SHELLOS:
        return base + 0x274;
    case SPECIES_GASTRODON:
        return base + 0x276;
    case SPECIES_CHERRIM:
        return base + 0x278;
    case SPECIES_ARCEUS:
        return base + 0x27A;
    case SPECIES_CASTFORM:
        return base + 0x26A;
    case SPECIES_DEOXYS:
        return base + 0x24A;
    case SPECIES_UNOWN:
        return base + 0x24E;
    case SPECIES_SHAYMIN:
        return base + 0x28C;
    case SPECIES_ROTOM:
        return base + 0x28E;
    case SPECIES_GIRATINA:
        return base + 0x294;
    case SPECIES_PICHU:
        return base + 0x296;
    default:
        if (gender != MON_FEMALE) {
            gender = MON_MALE;
        }
        return sPokewalkerPictureFiles[species][gender];
    }
}

void PokewalkerApp_UnlockNextCourse(PokewalkerApp *app) {
    u32 steps, watts;
    u32 count;
    u32 i;
    Pokewalker_GetStepsAndWatts(app->pokewalker, &steps, &watts);
    if (Pokedex_GetNatDexFlag(Save_Pokedex_Get(app->saveData))) {
        count = 20;
    } else {
        count = 8;
    }
    for (i = 0; i < count; i++) {
        if (sPokewalkerCourses[i].wattsRequired <= watts && !Pokewalker_CourseIsUnlocked(app->pokewalker, i)) {
            Pokewalker_UnlockCourse(app->pokewalker, i);
            app->unlockedCourse = i;
            return;
        }
    }
}

int PokewalkerApp_GetWattsToNextCourse(PokewalkerApp *app, int watts) {
    int count;
    int i;
    if (Pokedex_GetNatDexFlag(Save_Pokedex_Get(app->saveData))) {
        count = 20;
    } else {
        count = 8;
    }
    for (i = 0; i < count; i++) {
        if (!Pokewalker_CourseIsUnlocked(app->pokewalker, i)) {
            if (sPokewalkerCourses[i].wattsRequired > watts) {
                return sPokewalkerCourses[i].wattsRequired - watts;
            }
            return 0;
        }
    }
    return -1;
}

void PokewalkerApp_ShowYesNoMenu(PokewalkerApp *app, int selection) {
    PokewalkerApp_LoadTilemap(app, 2, 13);
    PokewalkerApp_SetCursorPosition(app, 0);
    PokewalkerApp_PrintString(app, 3, app->yesText, 4, MAKE_TEXT_COLOR(15, 1, 0));
    PokewalkerApp_PrintString(app, 4, app->noText, 4, MAKE_TEXT_COLOR(15, 1, 0));
    PokewalkerApp_SetCursorPosition(app, selection);
    app->menuSelection = selection;
    app->menuState = 0;
}

void PokewalkerApp_ShowSaveEraseStepsMenu(PokewalkerApp *app, int selection) {
    String *saveSteps = NewString_ReadMsgData(app->msgData, msg_0274_00150);
    String *eraseSteps = NewString_ReadMsgData(app->msgData, msg_0274_00151);
    PokewalkerApp_LoadTilemap(app, 2, 13);
    PokewalkerApp_SetCursorPosition(app, 0);
    PokewalkerApp_PrintString(app, 3, saveSteps, 4, MAKE_TEXT_COLOR(15, 1, 0));
    PokewalkerApp_PrintString(app, 4, eraseSteps, 4, MAKE_TEXT_COLOR(15, 1, 0));
    PokewalkerApp_SetCursorPosition(app, selection);
    app->menuSelection = selection;
    app->menuState = 0;
    String_Delete(eraseSteps);
    String_Delete(saveSteps);
}

// Returns 0 (first choice), 1 (second choice) or 2 (still choosing).
#pragma force_active on
int PokewalkerApp_HandleTwoChoiceMenu(PokewalkerApp *app) {
    if (app->menuState == 0) {
        switch ((u32)PokewalkerApp_GetTouchedButton(0)) {
        case 0:
            app->menuSelection = 0;
            PokewalkerApp_SetCursorPosition(app, 0);
            PokewalkerApp_PressCursor(app);
            PlaySE(SEQ_SE_DP_DECIDE);
            app->menuState = 1;
            break;
        case 1:
            app->menuSelection = 2;
            PokewalkerApp_SetCursorPosition(app, 1);
            PokewalkerApp_PressCursor(app);
            PlaySE(SEQ_SE_DP_DECIDE);
            app->menuState = 1;
            break;
        case TOUCH_MENU_NO_INPUT:
            if (gSystem.newKeys & PAD_KEY_UP) {
                if (app->menuSelection != 0) {
                    PlaySE(SEQ_SE_DP_SELECT);
                }
                PokewalkerApp_SetCursorPosition(app, 0);
                app->menuSelection = 0;
            } else if (gSystem.newKeys & PAD_KEY_DOWN) {
                if (app->menuSelection != 1) {
                    PlaySE(SEQ_SE_DP_SELECT);
                }
                PokewalkerApp_SetCursorPosition(app, 1);
                app->menuSelection = 1;
            } else if (gSystem.newKeys & PAD_BUTTON_A) {
                app->menuState = 1;
                PokewalkerApp_PressCursor(app);
                PlaySE(SEQ_SE_DP_DECIDE);
            } else if (gSystem.newKeys & PAD_BUTTON_B) {
                PokewalkerApp_SetCursorPosition(app, 1);
                PokewalkerApp_PressCursor(app);
                app->menuSelection = 1;
                app->menuState = 1;
                PlaySE(SEQ_SE_DP_SELECT);
            }
            break;
        }
    } else if (app->menuState == 1) {
        if (!ManagedSprite_IsAnimated(app->sprites[0])) {
            app->menuState = 2;
        }
    } else if (app->menuState == 2) {
        if (app->menuSelection != 0) {
            return 1;
        }
        return 0;
    }
    return 2;
}
#pragma force_active reset

int PokewalkerApp_GetTouchedButton(int index) {
    GF_ASSERT(index < 6);
    return TouchscreenHitbox_FindRectAtTouchNew(sTouchButtons[index]);
}

void PokewalkerApp_LoadSubBg1Tilemap(PokewalkerApp *app) {
    NARC *narc = NARC_New((NarcId)0xF9, HEAP_ID_154);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_OFF);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0x12, app->bgConfig, GF_BG_LYR_SUB_1, 0, 0, TRUE, HEAP_ID_154);
    ToggleBgLayer(GF_BG_LYR_SUB_1, GF_PLANE_TOGGLE_ON);
    NARC_Delete(narc);
}

void PokewalkerApp_ShowSubBg1(PokewalkerApp *app, int show) {
    ToggleBgLayer(GF_BG_LYR_SUB_1, show);
}

// Color (0-3) of pixel (x, y) of a 64-pixel wide picture in the Pokewalker's 2bpp format.
u8 PokewalkerApp_GetPixel2bpp(const u8 *picture, int x, int y) {
    int bit = y % 8;
    int offset = x * 2 + (y / 8) * 128;
    return (((picture[offset + 1] >> bit) & 1) << 1) | ((picture[offset] >> bit) & 1);
}

#pragma force_active on
void PokewalkerApp_SetPixel2bpp(u8 *picture, int x, int y, int color) {
    int row = y / 8;
    int bit = y % 8;
    int high = color & 2;
    int mask;
    int offset;
    if (color & 1) {
        mask = 1 << bit;
        offset = x * 2 + row * 128;
        picture[offset] |= mask;
    } else {
        mask = 1 << bit;
        offset = x * 2 + row * 128;
        picture[offset] &= 0xFF ^ mask;
    }
    if (high) {
        picture[offset + 1] |= mask;
    } else {
        picture[offset + 1] &= 0xFF ^ mask;
    }
}

// Draws Spinda's four spots: each personality byte places one spot (low nibble x, high nibble y, offset by -8),
// only where the mask picture has color 2.
void PokewalkerApp_DrawSpindaSpots(u8 *picture, const u8 *mask, u32 personality, int x, int y) {
    int i;
    for (i = 0; i < 4; i++) {
        const PokewalkerSpotPixel *pixels = sSpindaSpotPixels[i];
        u8 j = 0;
        if (pixels[0].x != 0xFF) {
            do {
                int px = (personality & 0xF) - 8 + (x + pixels[j].x);
                int py = y + pixels[j].y + ((personality & 0xF0) >> 4) - 8;
                if (PokewalkerApp_GetPixel2bpp(mask, px, py) == 2) {
                    PokewalkerApp_SetPixel2bpp(picture, px, py, 1);
                }
                j++;
            } while (pixels[j].x != 0xFF);
        }
        personality >>= 8;
    }
}
#pragma force_active reset

// Draws Spinda's spots on both frames of its Pokewalker picture.
void PokewalkerApp_DrawSpindaSpotsOnPicture(void *picture, const void *mask, u32 personality) {
    int y;
    int x;
    PokewalkerApp_DrawSpindaSpots(picture, mask, personality, -8, -14);
    PokewalkerApp_DrawSpindaSpots((u8 *)picture + 0x300, (const u8 *)mask + 0x300, personality, -22, -14);
    for (y = 0; y < 48; y++) {
        for (x = 0; x < 64; x++) {
            PokewalkerApp_GetPixel2bpp(picture, x, y);
        }
    }
}

#define ADD_WINDOWS(windows, templates, count, bankField)  \
    do {                                                   \
        Window *window;                                    \
        const WindowTemplate *const *template = templates; \
        i = 0;                                             \
        window = windows;                                  \
        do {                                               \
            AddWindow(app->bgConfig, window, *template);   \
            i++;                                           \
            template ++;                                   \
            window++;                                      \
        } while (i < count);                               \
        app->bankField = bank;                             \
    } while (0)

// Adds the windows of a bank (0, 2: sub screen; others: main screen).
#pragma force_active on
void PokewalkerApp_AddWindowBank(PokewalkerApp *app, int bank) {
    u32 i;
    GF_ASSERT(bank < 8);
    switch (bank) {
    case 0:
        ADD_WINDOWS(app->subWindows, sWindowBank0Templates, 7, subWindowBank);
        break;
    case 1:
        ADD_WINDOWS(app->windows, sWindowBank1Templates, 6, windowBank);
        break;
    case 2:
        ADD_WINDOWS(app->subWindows, sWindowBank2Templates, 2, subWindowBank);
        break;
    case 3:
        ADD_WINDOWS(app->windows, sWindowBank3Templates, 8, windowBank);
        break;
    case 4:
        ADD_WINDOWS(app->windows, sWindowBank4Templates, 9, windowBank);
        break;
    case 5:
        ADD_WINDOWS(app->windows, sWindowBank5Templates, 7, windowBank);
        break;
    case 6:
        ADD_WINDOWS(app->windows, sWindowBank6Templates, 16, windowBank);
        break;
    case 7:
        // main screen windows, but recorded as the sub screen bank
        ADD_WINDOWS(app->windows, sWindowBank7Templates, 6, subWindowBank);
        break;
    }
    app->windowCounts[sWindowBankScreens[bank]] = i;
}
#pragma force_active reset

#undef ADD_WINDOWS

void PokewalkerApp_RemoveWindowBank(PokewalkerApp *app, int bank) {
    if (sWindowBankScreens[bank] == 0) {
        u16 i = 0;
        while (i < app->windowCounts[0]) {
            RemoveWindow(&app->windows[i]);
            i++;
        }
        app->windowBank = 0xFF;
    } else {
        u16 i = 0;
        while (i < app->windowCounts[1]) {
            RemoveWindow(&app->subWindows[i]);
            i++;
        }
        app->subWindowBank = 0xFF;
    }
}

void PokewalkerApp_OnModeChangeDummy(PokewalkerApp *app) {
}

void PokewalkerApp_LoadStrings(PokewalkerApp *app) {
    app->eraseRecordsText = NewString_ReadMsgData(app->msgData, msg_0274_00076);
    app->goForStrollText = NewString_ReadMsgData(app->msgData, msg_0274_00073);
    app->receiveGiftText = NewString_ReadMsgData(app->msgData, msg_0274_00074);
    app->returnFromStrollText = NewString_ReadMsgData(app->msgData, msg_0274_00075);
    app->sendHomeText = NewString_ReadMsgData(app->msgData, msg_0274_00078);
    app->saveAndStrollText = NewString_ReadMsgData(app->msgData, msg_0274_00077);
    app->blankText = NewString_ReadMsgData(app->msgData, msg_0274_00080);
    app->exitText = NewString_ReadMsgData(app->msgData, msg_0274_00079);
    app->strollResultsText = NewString_ReadMsgData(app->msgData, msg_0274_00063);
    app->giftsReceivedText = NewString_ReadMsgData(app->msgData, msg_0274_00064);
    app->walkerRecordsText = NewString_ReadMsgData(app->msgData, msg_0274_00065);
    app->yourPokemonText = NewString_ReadMsgData(app->msgData, msg_0274_00066);
    app->itemsObtainedText = NewString_ReadMsgData(app->msgData, msg_0274_00071);
    app->strollResultsText2 = NewString_ReadMsgData(app->msgData, msg_0274_00063);
    app->itemsObtainedText2 = NewString_ReadMsgData(app->msgData, msg_0274_00071);
    app->exitText2 = NewString_ReadMsgData(app->msgData, msg_0274_00079);
    app->stepsToDateText = NewString_ReadMsgData(app->msgData, msg_0274_00059);
    app->wattsToDateText = NewString_ReadMsgData(app->msgData, msg_0274_00061);
    app->totalWattsText = NewString_ReadMsgData(app->msgData, msg_0274_00082);
    app->totalStepsText = NewString_ReadMsgData(app->msgData, msg_0274_00085);
    app->yesText = NewString_ReadMsgData(app->msgData, msg_0274_00148);
    app->noText = NewString_ReadMsgData(app->msgData, msg_0274_00149);
}

void PokewalkerApp_FreeStrings(PokewalkerApp *app) {
    String_Delete(app->eraseRecordsText);
    String_Delete(app->goForStrollText);
    String_Delete(app->receiveGiftText);
    String_Delete(app->returnFromStrollText);
    String_Delete(app->sendHomeText);
    String_Delete(app->saveAndStrollText);
    String_Delete(app->blankText);
    String_Delete(app->exitText);
    String_Delete(app->strollResultsText);
    String_Delete(app->walkerRecordsText);
    String_Delete(app->giftsReceivedText);
    String_Delete(app->yourPokemonText);
    String_Delete(app->itemsObtainedText);
    String_Delete(app->strollResultsText2);
    String_Delete(app->itemsObtainedText2);
    String_Delete(app->exitText2);
    String_Delete(app->stepsToDateText);
    String_Delete(app->wattsToDateText);
    String_Delete(app->totalWattsText);
    String_Delete(app->totalStepsText);
    String_Delete(app->yesText);
    String_Delete(app->noText);
}

void PokewalkerApp_PrintExit(PokewalkerApp *app, int window) {
    PokewalkerApp_PrintString(app, window, app->exitText, 4, MAKE_TEXT_COLOR(15, 1, 0));
}

void PokewalkerApp_PrintMessage(PokewalkerApp *app, int window, int msgId, u32 color) {
    String *string = NewString_ReadMsgData(app->msgData, msgId);
    StringExpandPlaceholders(app->msgFormat, app->string, string);
    PokewalkerApp_PrintString(app, window, app->string, 0, color);
    String_Delete(string);
}

void PokewalkerApp_PrintString(PokewalkerApp *app, int window, String *string, FontID fontId, u32 color) {
    Window *win = &app->windows[window];
    FillWindowPixelBuffer(win, 0);
    AddTextPrinterParameterizedWithColor(win, fontId, string, 0, 0, TEXT_SPEED_INSTANT, color, NULL);
}

void PokewalkerApp_PrintStringDefaultColor(PokewalkerApp *app, int window, String *string, FontID fontId) {
    Window *win = &app->windows[window];
    FillWindowPixelBuffer(win, 0);
    AddTextPrinterParameterizedWithColor(win, fontId, string, 0, 0, TEXT_SPEED_INSTANT, MAKE_TEXT_COLOR(1, 2, 0), NULL);
}

void PokewalkerApp_PrintSubString(PokewalkerApp *app, int window, String *string, FontID fontId, u32 color) {
    Window *win = &app->subWindows[window];
    FillWindowPixelBuffer(win, 0);
    AddTextPrinterParameterizedWithColor(win, fontId, string, 0, 0, TEXT_SPEED_INSTANT, color, NULL);
}

void PokewalkerApp_PrintSubMessage(PokewalkerApp *app, int window, int msgId, u32 color) {
    String *string = NewString_ReadMsgData(app->msgData, msgId);
    StringExpandPlaceholders(app->msgFormat, app->string, string);
    PokewalkerApp_PrintSubString(app, window, app->string, 0, color);
    String_Delete(string);
}

// Prints a message in a framed window at the player's text speed; returns the text printer id.
#pragma force_active on
u8 PokewalkerApp_StartMessageBox(PokewalkerApp *app, int window, int msgId) {
    String *string = NewString_ReadMsgData(app->msgData, msgId);
    Window *windows = app->windows;
    u8 printerId;
    DrawFrameAndWindow2(&windows[window], TRUE, 0x3E2, 13);
    FillWindowPixelBuffer(&windows[window], 0xFF);
    StringExpandPlaceholders(app->msgFormat, app->string, string);
    printerId = AddTextPrinterParameterized(&windows[window], 1, app->string, 0, 0, Options_GetTextFrameDelay(app->options), NULL);
    String_Delete(string);
    return printerId;
}

void PokewalkerApp_ShowMessageBox(PokewalkerApp *app, int window, int msgId) {
    String *string = NewString_ReadMsgData(app->msgData, msgId);
    Window *windows = app->windows;
    DrawFrameAndWindow2(&windows[window], TRUE, 0x3E2, 13);
    FillWindowPixelBuffer(&windows[window], 0xFF);
    StringExpandPlaceholders(app->msgFormat, app->string, string);
    AddTextPrinterParameterized(&windows[window], 1, app->string, 0, 0, TEXT_SPEED_INSTANT, NULL);
    String_Delete(string);
}
#pragma force_active reset

void PokewalkerApp_CreateWaitingIcon(PokewalkerApp *app) {
    app->waitingIcon = WaitingIcon_New(&app->windows[2], 0x3E2);
}

void PokewalkerApp_DeleteWaitingIcon(PokewalkerApp *app) {
    WaitingIcon_Delete(app->waitingIcon);
}

void PokewalkerApp_InitSprites(PokewalkerApp *app) {
    GfGfx_EngineATogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GfGfx_EngineBTogglePlanes(GX_PLANEMASK_OBJ, GF_PLANE_TOGGLE_ON);
    GF_CreateVramTransferManager(0x40, HEAP_ID_154);
    PokewalkerApp_InitSpriteSystem(app);
    PokewalkerApp_LoadSpriteResources(app);
    PokewalkerApp_CreateSprites(app);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(app->spriteSystem), 0, FX32_CONST(256));
    PokewalkerApp_LoadMonIconPalettes();
}

void PokewalkerApp_FreeSprites(PokewalkerApp *app) {
    u32 i;
    for (i = 0; i < NELEMS(app->sprites); i++) {
        Sprite_DeleteAndFreeResources(app->sprites[i]);
    }
    SpriteSystem_FreeResourcesAndManager(app->spriteSystem, app->spriteManager);
    SpriteSystem_Free(app->spriteSystem);
}

void PokewalkerApp_InitSpriteSystem(PokewalkerApp *app) {
    SpriteResourceCountsListUnion counts = sPokewalkerAppSpriteResourceCounts;
    OamManagerParam oam;
    OamCharTransferParam transfer;

    app->spriteSystem = SpriteSystem_Alloc(HEAP_ID_154);
    app->spriteManager = SpriteManager_New(app->spriteSystem);
    oam = sPokewalkerAppOamManagerParam;
    transfer = sPokewalkerAppOamCharTransferParam;
    SpriteSystem_Init(app->spriteSystem, &oam, &transfer, 0x20);
    SpriteSystem_InitSprites(app->spriteSystem, app->spriteManager, NELEMS(app->sprites));
    SpriteSystem_InitManagerWithCapacities(app->spriteSystem, app->spriteManager, &counts);
}

void PokewalkerApp_LoadSpriteResources(PokewalkerApp *app) {
    NARC *narc = NARC_New((NarcId)0xF9, HEAP_ID_154);

    SpriteSystem_LoadCharResObjFromOpenNarc(app->spriteSystem, app->spriteManager, narc, 7, TRUE, NNS_G2D_VRAM_TYPE_2DSUB, 0xC226);
    SpriteSystem_LoadPlttResObjFromOpenNarc(app->spriteSystem, app->spriteManager, narc, 3, FALSE, 2, NNS_G2D_VRAM_TYPE_2DSUB, 0xC226);
    SpriteSystem_LoadCellResObjFromOpenNarc(app->spriteSystem, app->spriteManager, narc, 0x14, FALSE, 0xC226);
    SpriteSystem_LoadAnimResObjFromOpenNarc(app->spriteSystem, app->spriteManager, narc, 0x16, FALSE, 0xC226);
    SpriteSystem_LoadCharResObjFromOpenNarc(app->spriteSystem, app->spriteManager, narc, 6, TRUE, NNS_G2D_VRAM_TYPE_2DMAIN, 0xC227);
    SpriteSystem_LoadPlttResObjFromOpenNarc(app->spriteSystem, app->spriteManager, narc, 2, FALSE, 7, NNS_G2D_VRAM_TYPE_2DMAIN, 0xC227);
    SpriteSystem_LoadCellResObjFromOpenNarc(app->spriteSystem, app->spriteManager, narc, 0x13, FALSE, 0xC227);
    SpriteSystem_LoadAnimResObjFromOpenNarc(app->spriteSystem, app->spriteManager, narc, 0x15, FALSE, 0xC227);
    NARC_Delete(narc);
}

// Sprites: 0-16 from templates (0 = cursor, 9.. = Pokemon icons), 17-116 particles on the main screen,
// 117-216 particles on the sub screen.
void PokewalkerApp_CreateSprites(PokewalkerApp *app) {
    u32 i = 0;
    const ManagedSpriteTemplate *template = sPokewalkerAppSpriteTemplates;
    const u8 *palette = sPokewalkerAppSpritePalettes;
    PokewalkerApp *slot = app; // advanced by one sprite pointer per sprite: slot->sprites[0] is sprite i

    for (; i < 17;) {
        slot->sprites[0] = SpriteSystem_NewSpriteWithYOffset(app->spriteSystem, app->spriteManager, template, FX32_CONST(256));
        ManagedSprite_SetPaletteOverride(slot->sprites[0], *palette);
        ManagedSprite_SetDrawFlag(slot->sprites[0], FALSE);
        i++;
        template ++;
        slot = (PokewalkerApp *)((u8 *)slot + sizeof(ManagedSprite *));
        palette++;
    }
    ManagedSprite_SetPriority(app->sprites[1], 2);
    for (; i < 117; i++) {
        app->sprites[i] = SpriteSystem_NewSprite(app->spriteSystem, app->spriteManager, &sPokewalkerAppMainSpriteTemplate);
        ManagedSprite_SetPaletteOverride(app->sprites[i], 0);
        ManagedSprite_SetDrawFlag(app->sprites[i], FALSE);
        ManagedSprite_SetPriority(app->sprites[i], 2);
    }
    for (; i < NELEMS(app->sprites); i++) {
        app->sprites[i] = SpriteSystem_NewSpriteWithYOffset(app->spriteSystem, app->spriteManager, &sPokewalkerAppSubSpriteTemplate, FX32_CONST(256));
        ManagedSprite_SetPaletteOverride(app->sprites[i], 0);
        ManagedSprite_SetDrawFlag(app->sprites[i], FALSE);
        ManagedSprite_SetPriority(app->sprites[i], 2);
    }
    PokewalkerApp_ClearParticles(app);
    ManagedSprite_SetDrawPriority(app->sprites[8], 2);
    ManagedSprite_SetAffineOverwriteMode(app->sprites[8], 1);
    ManagedSprite_SetAnimateFlag(app->sprites[3], TRUE);
    ManagedSprite_SetAnimateFlag(app->sprites[4], TRUE);
}

void PokewalkerApp_ResetCursor(PokewalkerApp *app) {
    ManagedSprite_SetAnimateFlag(app->sprites[0], TRUE);
    ManagedSprite_SetAnim(app->sprites[0], 8);
    ManagedSprite_SetDrawFlag(app->sprites[0], FALSE);
}

void PokewalkerApp_SetCursorPosition(PokewalkerApp *app, int position) {
    int i = position * 4;
    ManagedSprite *cursor = app->sprites[0];
    ManagedSprite_SetPositionXYWithSubscreenOffset(cursor, sPokewalkerAppCursorX[i], sPokewalkerAppCursorY[i], FX32_CONST(256));
    ManagedSprite_SetAnim(app->sprites[0], sPokewalkerAppCursorAnims[i]);
    ManagedSprite_SetPaletteOverride(app->sprites[0], sPokewalkerAppCursorPalettes[i]);
    ManagedSprite_SetDrawFlag(app->sprites[0], TRUE);
}

void PokewalkerApp_HideCursor(PokewalkerApp *app) {
    ManagedSprite_SetDrawFlag(app->sprites[0], FALSE);
}

// Plays the cursor's "pressed" animation.
void PokewalkerApp_PressCursor(PokewalkerApp *app) {
    int anim = ManagedSprite_GetActiveAnim(app->sprites[0]);
    ManagedSprite_SetAnim(app->sprites[0], anim + 3);
}

#pragma force_active on
void ov112_021EA5A4(PokewalkerApp *app, int anim) {
    ManagedSprite_SetAnim(app->sprites[1], 0);
    ManagedSprite_SetDrawFlag(app->sprites[1], TRUE);
    if (anim == 0) {
        ManagedSprite_SetDrawFlag(app->sprites[2], FALSE);
    } else if (anim > 0) {
        ManagedSprite_SetDrawPriority(app->sprites[1], 2);
        ManagedSprite_SetDrawPriority(app->sprites[2], 1);
        ManagedSprite_SetDrawFlag(app->sprites[2], TRUE);
        ManagedSprite_SetAnim(app->sprites[2], anim);
        ManagedSprite_SetAnimateFlag(app->sprites[2], TRUE);
    }
}

void PokewalkerApp_UpdateConnectionAnim(PokewalkerApp *app) {
    ManagedSprite *sprite = app->sprites[2];
    if (ov112_021E7668()) {
        if (ManagedSprite_GetActiveAnim(sprite) == 2) {
            ManagedSprite_SetAnim(app->sprites[2], 1);
        }
    } else {
        if (ManagedSprite_GetActiveAnim(sprite) == 1) {
            ManagedSprite_SetAnim(app->sprites[2], 2);
        }
    }
}
#pragma force_active reset

void ov112_021EA64C(PokewalkerApp *app) {
    ManagedSprite_SetDrawFlag(app->sprites[1], FALSE);
    ManagedSprite_SetDrawFlag(app->sprites[2], FALSE);
}

void PokewalkerApp_ShowSprite(PokewalkerApp *app, int index) {
    ManagedSprite_SetDrawFlag(app->sprites[index], TRUE);
}

void PokewalkerApp_HideSprite(PokewalkerApp *app, int index) {
    ManagedSprite_SetDrawFlag(app->sprites[index], FALSE);
}

void PokewalkerApp_SetSpriteAnim(PokewalkerApp *app, int index, int anim) {
    ManagedSprite_SetAnim(app->sprites[index], anim);
}

void PokewalkerApp_SetSpritePosition(PokewalkerApp *app, int index, int x, int y) {
    ManagedSprite_SetPositionXYWithSubscreenOffset(app->sprites[index], (s16)x, (s16)y, FX32_CONST(256));
}

// Draws particle i on the sub screen (y < 0) or on the main screen.
void PokewalkerApp_DrawParticle(PokewalkerApp *app, int index, int frame, int x, int y) {
    if (y < 0) {
        ManagedSprite_SetPositionXYWithSubscreenOffset(app->sprites[index + 117], (s16)x, (s16)(y + 208), FX32_CONST(256));
        ManagedSprite_SetAnim(app->sprites[index + 117], frame + 4);
        ManagedSprite_SetDrawFlag(app->sprites[index + 117], TRUE);
        ManagedSprite_SetDrawFlag(app->sprites[index + 17], FALSE);
    } else {
        ManagedSprite_SetPositionXY(app->sprites[index + 17], (s16)x, (s16)y);
        ManagedSprite_SetAnim(app->sprites[index + 17], frame + 19);
        ManagedSprite_SetDrawFlag(app->sprites[index + 117], FALSE);
        ManagedSprite_SetDrawFlag(app->sprites[index + 17], TRUE);
    }
}

void PokewalkerApp_SpawnRisingParticle(PokewalkerApp *app) {
    int i = PokewalkerApp_FindFreeParticle(app->particles);
    if (i >= 0) {
        PokewalkerAppParticle *particle;
        app->particles[i].active = TRUE;
        particle = &app->particles[i];
        particle->speed = MTRandom() % 6 + 2;
        particle->angle = MTRandom() % 300;
        particle->x = 128;
        particle->y = 116;
        particle->swayFactor = MTRandom() % 40 + 40;
        particle->life = MTRandom() % 60 + 120;
    }
}

void PokewalkerApp_SpawnFallingParticle(PokewalkerApp *app) {
    int i = PokewalkerApp_FindFreeParticle(app->particles);
    if (i >= 0) {
        PokewalkerAppParticle *particle;
        app->particles[i].active = TRUE;
        particle = &app->particles[i];
        particle->speed = ~0 * (MTRandom() % 5 + 5);
        particle->angle = MTRandom() % 300;
        particle->x = 128;
        particle->y = -109;
        particle->swayFactor = MTRandom() % 40 + 40;
        particle->life = MTRandom() % 60 + 120;
    }
}

// Shortens the life of every active particle; returns how many are active.
int PokewalkerApp_AgeParticles(PokewalkerApp *app) {
    int i;
    int count = 0;
    PokewalkerAppParticle *particle = app->particles;
    i = count;
    while (i < 100) {
        if (particle->active) {
            count++;
            particle->life -= 20;
        }
        i++;
        particle++;
    }
    return count;
}

void PokewalkerApp_UpdateParticles(PokewalkerApp *app) {
    int i;
    PokewalkerAppParticle *particle;
    i = 0;
    particle = app->particles;
    while (i < 100) {
        int amplitude = particle->swayFactor * (particle->y + 180) / 320;
        if (particle->active) {
            int frame;
            particle->dy = GF_SinDeg(particle->angle) / 360;
            particle->dx = GF_CosDeg(particle->angle) * amplitude / FX32_ONE;
            particle->angle += 10;
            frame = (particle->angle % 360) / 73;
            PokewalkerApp_DrawParticle(app, i, frame, particle->x + particle->dx, particle->y + particle->dy);
            particle->y -= particle->speed;
            particle->life--;
            if (particle->y < -110 || particle->life < 0 || particle->y > 160) {
                particle->active = FALSE;
                ManagedSprite_SetDrawFlag(app->sprites[i + 117], FALSE);
                ManagedSprite_SetDrawFlag(app->sprites[i + 17], FALSE);
            }
            if ((particle->angle % 360) >= 0 && (particle->angle % 360) <= 180) {
                ManagedSprite_SetDrawPriority(app->sprites[i + 117], 1);
                ManagedSprite_SetDrawPriority(app->sprites[i + 17], 1);
            } else {
                ManagedSprite_SetDrawPriority(app->sprites[i + 117], 3);
                ManagedSprite_SetDrawPriority(app->sprites[i + 17], 3);
            }
        }
        particle++;
        i++;
    }
}

void PokewalkerApp_ClearParticles(PokewalkerApp *app) {
    PokewalkerAppParticle *particle = app->particles;
    int i;
    for (i = 0; i < 100; i++, particle++) {
        particle->active = FALSE;
        particle->life = 0;
    }
}

int PokewalkerApp_FindFreeParticle(PokewalkerAppParticle *particles) {
    int i = 0;
    PokewalkerAppParticle *particle = particles;
    while (i < 100) {
        if (particle->active == FALSE) {
            return i;
        }
        i++;
        particle++;
    }
    return -1;
}

void PokewalkerApp_ShowMotionSprites(PokewalkerApp *app, const PokewalkerAppMotion *motion) {
    if (motion->y < 0) {
        ManagedSprite_SetDrawFlag(app->sprites[14], TRUE);
    } else {
        ManagedSprite_SetDrawFlag(app->sprites[13], TRUE);
    }
    ManagedSprite_SetAnimateFlag(app->sprites[14], TRUE);
    ManagedSprite_SetAnimateFlag(app->sprites[13], TRUE);
    ManagedSprite_SetAnim(app->sprites[14], 10);
    ManagedSprite_SetAnim(app->sprites[13], 24);
}

void PokewalkerApp_HideMotionSprites(PokewalkerApp *app) {
    ManagedSprite_SetDrawFlag(app->sprites[14], FALSE);
    ManagedSprite_SetDrawFlag(app->sprites[13], FALSE);
}

void PokewalkerApp_SetMotionSpritePos(PokewalkerApp *app, int x, int y) {
    if (y < 0) {
        ManagedSprite_SetPositionXYWithSubscreenOffset(app->sprites[14], (s16)x, (s16)(y + 208), FX32_CONST(256));
        ManagedSprite_SetDrawFlag(app->sprites[14], TRUE);
        ManagedSprite_SetDrawFlag(app->sprites[13], FALSE);
    } else {
        ManagedSprite_SetPositionXY(app->sprites[13], (s16)x, (s16)y);
        ManagedSprite_SetDrawFlag(app->sprites[13], TRUE);
        ManagedSprite_SetDrawFlag(app->sprites[14], FALSE);
    }
}

#pragma force_active on
void PokewalkerApp_InitMotion(PokewalkerApp *app, PokewalkerAppMotion *motion, int kind) {
    if (kind == 0) {
        motion->x = 128;
        motion->y = 130;
        motion->delta = -10;
        motion->time = 0;
    } else if (kind == 1) {
        motion->x = 128;
        motion->y = -110;
        motion->delta = 10;
        motion->time = 0;
    } else if (kind == 2) {
        motion->x = 128;
        motion->y = 130;
        motion->delta = 0;
        motion->time = 0;
    }
    motion->unk_08 = 0;
    motion->unk_0A = 0;
    PokewalkerApp_ShowMotionSprites(app, motion);
}
#pragma force_active reset

// Moves the object up (delta < 0) or down; returns TRUE once it has stopped and been hidden.
BOOL PokewalkerApp_UpdateMotion(PokewalkerApp *app, PokewalkerAppMotion *motion) {
    s8 delta = motion->delta;
    if (delta < 0) {
        motion->y += delta;
        if (motion->y < -110) {
            motion->delta = 0;
        }
        PokewalkerApp_SetMotionSpritePos(app, motion->x, motion->y);
        return FALSE;
    }
    if (delta > 0 && delta <= 100) {
        motion->y += delta;
        if (motion->y > 108) {
            motion->delta = 101;
        }
        PokewalkerApp_SetMotionSpritePos(app, motion->x, motion->y);
        return FALSE;
    }
    if (delta != 0) {
        PokewalkerApp_SetMotionSpritePos(app, motion->x, motion->y);
        return FALSE;
    }
    PokewalkerApp_HideMotionSprites(app);
    return TRUE;
}

void PokewalkerApp_LoadMonIconPalettes(void) {
    GfGfxLoader_GXLoadPal(NARC_poketool_icongra_poke_icon, GetMonIconPlttNaix(), GF_PAL_LOCATION_MAIN_OBJ, (enum GFPalSlotOffset)0xA0, 0x60, HEAP_ID_154);
}

void PokewalkerApp_LoadMonIcon(PokewalkerApp *app, int slot, int species, int form) {
    app->monIconFile = GfGfxLoader_GetCharData(NARC_poketool_icongra_poke_icon, GetMonIconNaixEx(species, FALSE, form), FALSE, &app->monIconCharData, HEAP_ID_154);
    DC_FlushRange(app->monIconCharData->pRawData, 0x400);
    PokewalkerApp_SetMonIconGraphics(app->monIconCharData, species, form, FALSE, slot, app->sprites[slot + 9]);
    Heap_Free(app->monIconFile);
}

void PokewalkerApp_SetMonIconGraphics(NNSG2dCharacterData *charData, int species, int form, int isEgg, int slot, ManagedSprite *sprite) {
    GX_LoadOBJ(charData->pRawData, ((slot << 5) + 0xC0) << 5, 0x400);
    ManagedSprite_SetPaletteOverride(sprite, GetMonIconPaletteEx(species, form, isEgg) + 5);
}
