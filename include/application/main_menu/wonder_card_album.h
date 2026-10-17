#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_WONDER_CARD_ALBUM_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_WONDER_CARD_ALBUM_H

#include "application/main_menu/mystery_gift_app.h"

#include "bg_window.h"
#include "list_menu.h"
#include "mystery_gift.h"
#include "overlay_manager.h"
#include "scanline_effect.h"
#include "screen_fade.h"
#include "sys_task.h"

typedef struct WonderCardAlbum WonderCardAlbum;

// Prints the variable part of a text box; returns TRUE if the box's message should be printed after it.
typedef BOOL (*WonderCardTextBoxFunc)(void *data, Window *window, u32 color);

// A text box of the Wonder Card screens (tables: sWonderCardAlbumTextBoxes for the album,
// sLockCapsuleCardTextBoxes for the special card).
typedef struct WonderCardTextBox {
    int page;                   // 0x00, card side it belongs to (0: front, 1: back); 2: kept on both sides
    u32 x;                      // 0x04
    u32 y;                      // 0x08
    u32 width;                  // 0x0C
    u32 height;                 // 0x10
    u32 fontId;                 // 0x14
    u32 color;                  // 0x18
    u32 fillColor;              // 0x1C, font attribute on the card pages, fill colour for menus and messages
    u32 msgId;                  // 0x20, 0: none
    WonderCardTextBoxFunc func; // 0x24
    u32 textX;                  // 0x28, -1: centred
    u32 textY;                  // 0x2C
} WonderCardTextBox;            // size: 0x30

// Horizontal-offset effect that flips the card between its front and back (per-scanline BG offsets written by
// H-blank DMA from a double buffer).
typedef struct WonderCardFlipEffect {
    u8 buffers[2][0x600];       // 0x000
    DoubleBuffer *doubleBuffer; // 0xC00
    SysTask *task;              // 0xC04
    fx32 speed;                 // 0xC08
    fx32 accel;                 // 0xC0C
    BOOL active;                // 0xC10
    int mode;                   // 0xC14, 1: card closes, 0: card opens
} WonderCardFlipEffect;         // size: 0xC18

int WonderCardAlbum_SelectShare(OverlayManager *manager);
int WonderCardAlbum_SelectTrash(OverlayManager *manager);
int WonderCardAlbum_ConfirmTrash(OverlayManager *manager);
int WonderCardAlbum_ReturnToCardMenu(OverlayManager *manager);
BOOL WonderCardAlbum_PrintMessageOnly(void);
BOOL WonderCardAlbum_PrintCardTitle(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_PrintGiftStatus(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_BufferReceiveDate(WonderCardAlbum *album);
BOOL WonderCardAlbum_PrintCardText(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_PrintSharesLeft(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_ShowCardMenu(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_ShowTrashYesNo(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_ShowShareYesNo(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_BufferZero(WonderCardAlbum *album);
BOOL WonderCardAlbum_Init(OverlayManager *manager, int *state);
BOOL WonderCardAlbum_Main(OverlayManager *manager, int *state);
BOOL WonderCardAlbum_Exit(OverlayManager *manager, int *state);
void WonderCardAlbum_DrawCardOnBg(BgConfig *bgConfig, WonderCard *card, enum HeapID heapID);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_WONDER_CARD_ALBUM_H
