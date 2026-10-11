#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_UTIL_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_UTIL_H

#include "global.h"

#include "application/main_menu/main_menu_internal.h"

#include "bg_window.h"
#include "save.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

// Shared services of the main menu overlay (src/application/main_menu/main_menu_util.c): heap of the crypto library,
// the Mystery Gift GBA cartridge, screen setup, fades, the message box printer (MainMenuTextPrinter), sprites on both
// screens and the gift graphics of the card screens.

// Sprite resources of one screen (main 0, sub 1)
typedef struct MainMenuUtilSpriteRes {
    SpriteResource *charRes;
    SpriteResource *plttRes;
    SpriteResource *cellRes;
    SpriteResource *animRes;
    u8 unk_10[8];
} MainMenuUtilSpriteRes;

// Sprites of both screens (ov74_0223D488)
typedef struct MainMenuUtilSprites {
    SpriteList *spriteList;              // 0x000
    G2dRenderer renderer;                // 0x004
    GF_2DGfxResMan *resMan[6];           // 0x12C
    MainMenuUtilSpriteRes spriteRes[2];  // 0x144
    SpriteResourcesHeader resHeaders[2]; // 0x174
} MainMenuUtilSprites;                   // size: 0x1BC

// The overlay's shared work (bss gMainMenuUtilWork, 0xEA8 bytes). The code reaches parts of it through the labels
// ov74_0223D488 (sprites, +0x34), ov74_0223D48C (+0x38), ov74_0223D654 (+0x200) and ov74_0223E2D4 (+0xE80).
typedef struct MainMenuUtilWork {
    BgConfig *bgConfig;                                    // 0x000
    u32 unk_004;                                           // 0x004
    enum HeapID heapId;                                    // 0x008
    int nextState;                                         // 0x00C, after the running fade
    BOOL fadeWhite;                                        // 0x010
    int saveState;                                         // 0x014
    SaveData *saveData;                                    // 0x018
    u8 unk_01C[0x18];                                      // 0x01C
    MainMenuUtilSprites sprites;                           // 0x034
    u8 unk_1F0[0x18];                                      // 0x1F0
    Sprite *giftSprite;                                    // 0x208, sub screen
    int giftSpriteDelay;                                   // 0x20C
    fx32 subScreenY;                                       // 0x210
    u8 monCharData[0xC80];                                 // 0x214
    u16 narcIds[8];                                        // 0xE94, PokepicTemplate of the gift Pokemon
    void (*vblankCallback)(struct MainMenuUtilWork *work); // 0xEA4, run once by the VBlank callback
} MainMenuUtilWork;                                        // size: 0xEA8

void MainMenuUtil_SetCryptoHeap(enum HeapID heapID);
void *MainMenuUtil_CryptoAlloc(u32 size);
int MainMenuUtil_CheckGiftCartridgeSignature(void);
BOOL MainMenuUtil_IsGiftCartridgeInserted(void);
u32 MainMenuUtil_GetCartridgeGiftSize(void);
BOOL MainMenuUtil_ReadCartridgeGift(void *dest, int size);
int MainMenuTextPrinter_Print(MainMenuTextPrinter *printer, int msgId);
u32 MainMenuUtil_SpritesActive(void);
SpriteList **MainMenuUtil_GetSpriteList(void);
void MainMenuUtil_SetSubScreenOffset(fx32 x, fx32 y);
void MainMenuUtil_UpdateGiftSprite(int hideGiftSprite);
void MainMenuUtil_VBlankCB(void);
int MainMenuUtil_GetGiftPalette(int giftType);
void MainMenuUtil_LoadGiftMonSprite(Sprite *sprite, Pokemon *mon, int species, int form, void *charData, u16 *narcIds);
void MainMenuUtil_ShowGiftBox(MainMenuUtilWork *work);
void MainMenuUtil_ShowGiftPokemon(MainMenuUtilWork *work, int giftType, void *gift);
void MainMenuUtil_ShowGiftItem(MainMenuUtilWork *work, int giftType, void *gift);
void MainMenuUtil_LoadCardSubScreenPalette(MainMenuUtilWork *work);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_UTIL_H
