#pragma force_active on
#include "global.h"
typedef struct Pocket { void *slots; u8 pad4[2]; s16 cursor; u8 pocketId; u8 page; u8 pad[2]; } Pocket;
typedef struct View { u32 unk0; Pocket pockets[8]; u8 current; } View;
typedef struct Item { u8 pad[6]; s16 y; } Item;
typedef struct Work { u8 pad[0x234]; View *view; u8 pad2[0x672 - 0x238]; u8 unk672; } Work;
int ov15_021FA074(Work *);
int ov15_021FA098(Work *);
void ov15_021FD574(Work *, int, int, int);
void ov15_021FF364(Work *, int, int, int);
void ov15_022001C4(Work *, Item *, int);
void ov15_021FF6BC(Work *, int, int, int);
void ov15_021FAD80(Work *w, Item *item);
// @fn ov15_021FAD80
void ov15_021FAD80(Work *w, Item *item) {
ov15_021FD574(w, 1, ov15_021FA074(w), ov15_021FA098(w)); ov15_021FF364(w, item->y, ov15_021FA098(w), 1); ov15_022001C4(w, item, w->unk672); ov15_021FF6BC(w, w->view->pockets[w->view->current].page, w->view->pockets[w->view->current].cursor, 0);
}
