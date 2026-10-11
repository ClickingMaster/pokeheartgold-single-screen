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

// A text box of the Wonder Card screens (tables in asm data: ov74_0223C340 for the album,
// ov74_0223C700 for the special card).
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

void WonderCardAlbum_LoadShareSprites(WonderCardAlbum *album);
void WonderCardAlbum_DeleteSprites(WonderCardAlbum *album);
void WonderCardAlbum_CreateButtonSprites(WonderCardAlbum *album);
void WonderCardAlbum_EraseWindowIfInUse(Window *window, BOOL dontCopyToVram);
void WonderCardAlbum_ClearWindowIfInUse(Window *window, BOOL dontCopyToVram);
void WonderCardAlbum_CreateListMenu(WonderCardAlbum *album, const MysteryGiftMenuItem *items, int count, Window *window, int itemsAbove);
void WonderCardAlbum_PrintMessage(Window *window, int msgId);
void WonderCardAlbum_LoadShareScreenBg(BgConfig *bgConfig);
int WonderCardAlbum_SelectShare(OverlayManager *manager);
int WonderCardAlbum_SelectTrash(OverlayManager *manager);
int WonderCardAlbum_ConfirmTrash(OverlayManager *manager);
int WonderCardAlbum_ReturnToCardMenu(OverlayManager *manager);
void WonderCardAlbum_DestroyListMenu(WonderCardAlbum *album);
BOOL WonderCardAlbum_PrintMessageOnly(void);
BOOL WonderCardAlbum_PrintCardTitle(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_PrintGiftStatus(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_BufferReceiveDate(WonderCardAlbum *album);
BOOL WonderCardAlbum_PrintCardText(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_PrintSharesLeft(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_ShowCardMenu(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_ShowTrashYesNo(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_ShowShareYesNo(WonderCardAlbum *album, Window *window, u32 color);
BOOL WonderCardAlbum_ShowWirelessYesNo(WonderCardAlbum *album, Window *window, u32 color);
BOOL ov74_0222D7D0(WonderCardAlbum *album);
void WonderCardAlbum_StartFade(WonderCardAlbum *album, enum FadeType fadeType, int nextState, int *state);
void WonderCardAlbum_DrawCard(WonderCardAlbum *album, int unused, int page);
int WonderCardAlbum_PrintTextBox(WonderCardAlbum *album, Window *window, int boxId, int baseTile);
int WonderCardAlbum_FindNextCard(WonderCardAlbum *album, int index, int step);
void WonderCardAlbum_LoadScreenData(WonderCardAlbum *album, int fileId, int bgId, int offset);
void WonderCardAlbum_LoadCardBg(WonderCardAlbum *album, int page);
void WonderCardAlbum_HandleListMenuInput(OverlayManager *manager, int *state, MysteryGiftMenuFunc cancelFunc);
void WonderCardAlbum_CreateMonIconSprites(WonderCardAlbum *album);
BOOL WonderCardAlbum_Init(OverlayManager *manager, int *state);
void WonderCardAlbum_FlipEffectTask(SysTask *task, WonderCardFlipEffect *effect);
void WonderCardAlbum_StartFlipEffect(WonderCardAlbum *album, int mode, fx32 speed, fx32 accel);
void WonderCardAlbum_StopFlipEffect(WonderCardAlbum *album);
BOOL WonderCardAlbum_UpdateFlipEffect(WonderCardAlbum *album);
void WonderCardAlbum_WaitForSave(WonderCardAlbum *album);
void WonderCardAlbum_CountShare(WonderCardAlbum *album);
void WonderCardAlbum_HandleShareInput(WonderCardAlbum *album, BOOL canShare, int *state);
BOOL WonderCardAlbum_Main(OverlayManager *manager, int *state);
int WonderCardAlbum_StartSharing(WonderCardAlbum *album);
int WonderCardAlbum_TakeEarliest(s32 *values);
int WonderCardAlbum_CountReceivers(void);
int WonderCardAlbum_UpdateReceiverList(WonderCardAlbum *album, Window *window);
void WonderCardAlbum_PrintReceiverCount(WonderCardAlbum *album, Window *window, int count);
void WonderCardAlbum_StopSharing(WonderCardAlbum *album, int *state, int nextState);
BOOL WonderCardAlbum_Exit(OverlayManager *manager);
void WonderCardAlbum_DrawCardOnBg(BgConfig *bgConfig, WonderCard *card, enum HeapID heapID);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_WONDER_CARD_ALBUM_H
