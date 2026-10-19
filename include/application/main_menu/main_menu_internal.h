#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_INTERNAL_H

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "message_format.h"
#include "overlay_manager.h"
#include "save.h"
#include "sprite.h"

// Text box printed by the shared message helpers (MainMenuTextPrinter_Init ... MainMenuTextPrinter_AddWindowAndPrint).
typedef struct MainMenuTextPrinter {
    u32 frameType;    // 0 frame 1, 1 frame 2, 2 no frame
    u32 drawFrame;    // draw the frame after printing
    BOOL clearWindow; // clear the window before printing the next message
    BOOL rightAlign;  // applies to the next message only
    Window *window;
    MessageFormat *msgFormat; // NULL: a temporary one is used
    u32 width;
    u32 height;
    u32 textX;
    u32 textY;
    u32 baseTile;
    u32 bgId;
    u32 palette; // of the window
    u32 msgFileId;
    u32 frameBaseTile;
    u32 framePalette;
    u32 fontId;
    u32 color;
    u8 fillColor;
    int msgId;
    int speed;
} MainMenuTextPrinter;

// Shared helpers of the main menu (screen setup, fades, text, sprites).
void MainMenuUtil_InitWork(enum HeapID heapID);
void *MainMenuUtil_CreateAppData(OverlayManager *manager, enum HeapID heapID, u32 size, u32 heapSize);
void MainMenuUtil_InitBg(BgConfig *bgConfig, int bgId, u32 screenSize, u32 screenBase, u32 charBase);
void MainMenuUtil_SetFadeWhite(u32 fadeWhite);
void MainMenuUtil_StartFade(u32 fadeType, u32 newState, int *state, u32 waitingState);
void MainMenuUtil_WaitFade(int *state);
void MainMenuTextPrinter_Init(MainMenuTextPrinter *printer, Window *window, u32 palette, u32 msgFileId, u32 frameBaseTile, u32 framePalette);
void MainMenuTextPrinter_SetSize(MainMenuTextPrinter *printer, u32 width, u32 height, u32 baseTile);
void MainMenuTextPrinter_SetFrameAndFont(MainMenuTextPrinter *printer, u32 frameType, u32 fontId);
void MainMenuTextPrinter_SetTextPos(MainMenuTextPrinter *printer, u32 textX, u32 textY);
int MainMenuTextPrinter_AddWindowAndPrint(BgConfig *bgConfig, MainMenuTextPrinter *printer, int x, int y, int msgId);
u32 MainMenuTextPrinter_GetTileCount(MainMenuTextPrinter *printer);
void MainMenuUtil_InitObjTransfer(void);
void MainMenuUtil_InitSprites(void);
void MainMenuUtil_LoadSpriteResources(NarcId narcId, u32 charFileId, u32 plttFileId, u32 cellFileId, u32 animFileId, u32 screen);
void MainMenuUtil_UpdateSprites(void);
Sprite *MainMenuUtil_CreateSprite(u32 screen, Sprite *sprite, u32 x, u32 y, u32 animSeq);
void MainMenuUtil_FreeSprites(void);
u32 MainMenuUtil_GetVersionMaskBit(int spriteId);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_INTERNAL_H
