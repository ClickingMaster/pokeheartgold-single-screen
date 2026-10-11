#pragma force_active on
#include "global.h"
typedef struct Pocket { void *slots; u8 pad4[2]; s16 cursor; u8 pocketId; u8 page; u8 pad[2]; } Pocket;
typedef struct View { u32 unk0; Pocket pockets[8]; u8 current; } View;
typedef struct Item { u8 pad[6]; s16 y; } Item;
typedef struct Work { u8 pad[0x234]; View *view; u8 pad2[0x672 - 0x238]; u8 unk672; } Work;
int BagApp_GetPageItemCount(Work *);
int BagApp_GetMovingItemPageSlot(Work *);
void BagApp_LoadSubScreenLayout(Work *, int, int, int);
void BagApp_DrawItemList(Work *, int, int, int);
void BagApp_ShowMovingItemIcon(Work *, Item *, int);
void BagApp_PrintPageNumber(Work *, int, int, int);
void BagApp_RedrawMovePage(Work *w, Item *item);
// @fn BagApp_RedrawMovePage
void BagApp_RedrawMovePage(Work *w, Item *item) {
BagApp_LoadSubScreenLayout(w, 1, BagApp_GetPageItemCount(w), BagApp_GetMovingItemPageSlot(w)); BagApp_DrawItemList(w, item->y, BagApp_GetMovingItemPageSlot(w), 1); BagApp_ShowMovingItemIcon(w, item, w->unk672); BagApp_PrintPageNumber(w, w->view->pockets[w->view->current].page, w->view->pockets[w->view->current].cursor, 0);
}
