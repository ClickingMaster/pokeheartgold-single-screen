#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_UTIL_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_UTIL_H

#include "global.h"

#include "application/main_menu/main_menu_internal.h"

#include "bg_window.h"
#include "save.h"
#include "sprite.h"
#include "sprite_resource.h"
#include "sprite_resource_header.h"

// Shared services of the main menu: the Mystery Gift GBA cartridge, screen setup, fades, the message box printer
// (MainMenuTextPrinter), sprites on both screens and the gift graphics of the card screens.

// Sprite resources of one screen (main 0, sub 1)
typedef struct MainMenuUtilSpriteRes {
    SpriteResource *charRes;
    SpriteResource *plttRes;
    SpriteResource *cellRes;
    SpriteResource *animRes;
    u8 unk_10[8];
} MainMenuUtilSpriteRes;

// Sprites of both screens (gMainMenuUtilSprites)
typedef struct MainMenuUtilSprites {
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[6];
    MainMenuUtilSpriteRes spriteRes[2];
    SpriteResourcesHeader resHeaders[2];
} MainMenuUtilSprites;

// The shared work of the main menu (gMainMenuUtilWork).
typedef struct MainMenuUtilWork {
    BgConfig *bgConfig;
    u32 unk_004;
    enum HeapID heapId;
    int nextState; // after the running fade
    BOOL fadeWhite;
    int saveState;
    SaveData *saveData;
    u8 unk_01C[0x18];
    MainMenuUtilSprites sprites;
    u8 unk_1F0[0x18];
    Sprite *giftSprite; // sub screen
    int giftSpriteDelay;
    fx32 subScreenY;
    u8 monCharData[0xC80];
    u16 narcIds[8];                                        // PokepicTemplate of the gift Pokemon
    void (*vblankCallback)(struct MainMenuUtilWork *work); // run once by the VBlank callback
} MainMenuUtilWork;

void MainMenuUtil_SetCryptoHeap(enum HeapID heapID);
u32 MainMenuUtil_GetCartridgeGiftSize(void);
BOOL MainMenuUtil_ReadCartridgeGift(void *dest, int size);
u32 MainMenuUtil_SpritesActive(void);
MainMenuUtilSprites *MainMenuUtil_GetSpriteList(void);
void MainMenuUtil_UpdateGiftSprite(int hideGiftSprite);
int MainMenuUtil_GetGiftPalette(int giftType);
void MainMenuUtil_ShowGiftBox(MainMenuUtilWork *work, int giftType, void *gift);
void MainMenuUtil_ShowGiftPokemon(MainMenuUtilWork *work, int giftType, void *gift);
void MainMenuUtil_ShowGiftItem(MainMenuUtilWork *work, int giftType, void *gift);
void MainMenuUtil_LoadCardSubScreenPalette(MainMenuUtilWork *work);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_UTIL_H
