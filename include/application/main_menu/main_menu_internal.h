#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_INTERNAL_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_INTERNAL_H

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "heap.h"
#include "message_format.h"
#include "overlay_manager.h"
#include "save.h"
#include "sprite.h"

// Text box printed by the overlay's shared message helpers (ov74_02235414 ... ov74_02235568).
typedef struct MainMenuTextPrinter {
    u32 unk_00;               // 0x00
    u32 unk_04;               // 0x04
    BOOL clearWindow;         // 0x08, clear the window before printing the next message
    BOOL rightAlign;          // 0x0C, applies to the next message only
    Window *window;           // 0x10
    MessageFormat *msgFormat; // 0x14, NULL: a temporary one is used
    u32 width;                // 0x18
    u32 height;               // 0x1C
    u32 textX;                // 0x20
    u32 textY;                // 0x24
    u32 baseTile;             // 0x28
    u32 unk_2C;               // 0x2C
    u32 unk_30;               // 0x30
    u32 msgFileId;            // 0x34
    u32 unk_38;               // 0x38
    u32 unk_3C;               // 0x3C
    u32 fontId;               // 0x40
    u32 color;                // 0x44
    u8 fillColor;             // 0x48
    int msgId;                // 0x4C
    int speed;                // 0x50
} MainMenuTextPrinter;        // size: 0x54

// Shared helpers of the overlay (screen setup, fades, text, sprites).
void ov74_022352A0(enum HeapID heapID);
void *ov74_022352D0(OverlayManager *manager, enum HeapID heapID, u32 size, u32 heapSize);
void ov74_02235308(BgConfig *bgConfig, u8 bgId, u32 screenSize, u32 screenBase, u32 charBase);
void ov74_02235390(u32 a0);
void ov74_0223539C(u32 a0, u32 newState, int *state, u32 waitingState);
void ov74_022353FC(int *state);
void ov74_02235414(MainMenuTextPrinter *printer, Window *window, u32 unk30, u32 msgFileId, u32 unk38, u32 unk3C);
void ov74_02235464(MainMenuTextPrinter *printer, u32 width, u32 height, u32 baseTile);
void ov74_0223546C(MainMenuTextPrinter *printer, u32 unk00, u32 fontId);
void ov74_02235474(MainMenuTextPrinter *printer, u32 textX, u32 textY);
int ov74_02235568(BgConfig *bgConfig, MainMenuTextPrinter *printer, int x, int y, int msgId);
u32 ov74_02235634(MainMenuTextPrinter *printer);
void ov74_0223563C(void);
void ov74_02235690(void);
void ov74_02235728(NarcId narcId, u32 charFileId, u32 plttFileId, u32 cellFileId, u32 animFileId, u32 a5);
void ov74_022358BC(void);
Sprite *ov74_02235930(u32 a0, Sprite *sprite, u32 x, u32 y, u32 animSeq);
void ov74_022359BC(void);
u32 ov74_02235A68(int a0);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MAIN_MENU_INTERNAL_H
