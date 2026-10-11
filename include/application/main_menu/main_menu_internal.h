#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_INTERNAL_H

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "message_format.h"
#include "overlay_manager.h"
#include "save.h"
#include "sprite.h"

// Text box printed by the overlay's shared message helpers (MainMenuTextPrinter_Init ... MainMenuTextPrinter_AddWindowAndPrint).
typedef struct MainMenuTextPrinter {
    u32 frameType;            // 0x00, 0 frame 1, 1 frame 2, 2 no frame
    u32 drawFrame;            // 0x04, draw the frame after printing
    BOOL clearWindow;         // 0x08, clear the window before printing the next message
    BOOL rightAlign;          // 0x0C, applies to the next message only
    Window *window;           // 0x10
    MessageFormat *msgFormat; // 0x14, NULL: a temporary one is used
    u32 width;                // 0x18
    u32 height;               // 0x1C
    u32 textX;                // 0x20
    u32 textY;                // 0x24
    u32 baseTile;             // 0x28
    u32 bgId;                 // 0x2C
    u32 palette;              // 0x30, of the window
    u32 msgFileId;            // 0x34
    u32 frameBaseTile;        // 0x38
    u32 framePalette;         // 0x3C
    u32 fontId;               // 0x40
    u32 color;                // 0x44
    u8 fillColor;             // 0x48
    int msgId;                // 0x4C
    int speed;                // 0x50
} MainMenuTextPrinter;        // size: 0x54

// Shared helpers of the overlay (screen setup, fades, text, sprites).
void MainMenuUtil_InitWork(enum HeapID heapID);
void *MainMenuUtil_CreateAppData(OverlayManager *manager, enum HeapID heapID, u32 size, u32 heapSize);
void MainMenuUtil_InitBg(BgConfig *bgConfig, int bgId, u32 screenSize, u32 screenBase, u32 charBase);
void MainMenuUtil_SetFadeWhite(u32 a0);
void MainMenuUtil_StartFade(u32 a0, u32 newState, int *state, u32 waitingState);
void MainMenuUtil_WaitFade(int *state);
void MainMenuTextPrinter_Init(MainMenuTextPrinter *printer, Window *window, u32 palette, u32 msgFileId, u32 frameBaseTile, u32 framePalette);
void MainMenuTextPrinter_SetSize(MainMenuTextPrinter *printer, u32 width, u32 height, u32 baseTile);
void MainMenuTextPrinter_SetFrameAndFont(MainMenuTextPrinter *printer, u32 frameType, u32 fontId);
void MainMenuTextPrinter_SetTextPos(MainMenuTextPrinter *printer, u32 textX, u32 textY);
int MainMenuTextPrinter_AddWindowAndPrint(BgConfig *bgConfig, MainMenuTextPrinter *printer, int x, int y, int msgId);
u32 MainMenuTextPrinter_GetTileCount(MainMenuTextPrinter *printer);
void MainMenuUtil_InitObjTransfer(void);
void MainMenuUtil_InitSprites(void);
void MainMenuUtil_LoadSpriteResources(NarcId narcId, u32 charFileId, u32 plttFileId, u32 cellFileId, u32 animFileId, u32 a5);
void MainMenuUtil_UpdateSprites(void);
Sprite *MainMenuUtil_CreateSprite(u32 a0, Sprite *sprite, u32 x, u32 y, u32 animSeq);
void MainMenuUtil_FreeSprites(void);
u32 ov74_02235A68(int a0);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_INTERNAL_H
