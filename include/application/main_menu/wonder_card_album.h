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
// gLockCapsuleCardTextBoxes for the special card).
typedef struct WonderCardTextBox {
    int page; // card side it belongs to (0: front, 1: back); 2: kept on both sides
    u32 x;
    u32 y;
    u32 width;
    u32 height;
    u32 fontId;
    u32 color;
    u32 fillColor; // font attribute on the card pages, fill colour for menus and messages
    u32 msgId;     // 0: none
    WonderCardTextBoxFunc func;
    u32 textX; // -1: centred
    u32 textY;
} WonderCardTextBox;

// Horizontal-offset effect that flips the card between its front and back (per-scanline BG offsets written by
// H-blank DMA from a double buffer).
typedef struct WonderCardFlipEffect {
    u8 buffers[2][0x600];
    DoubleBuffer *doubleBuffer;
    SysTask *task;
    fx32 speed;
    fx32 accel;
    BOOL active;
    int mode; // 1: card closes, 0: card opens
} WonderCardFlipEffect;

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
