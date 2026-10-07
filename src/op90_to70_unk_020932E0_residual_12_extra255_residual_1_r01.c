#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct Ent { Sprite *sprite; int owner; int state; } Ent;
typedef struct Slot { u32 ent; u32 active; u32 pad[2]; } Slot;
typedef struct Elem { Sprite *sprite; u8 pad[0x30]; } Elem;
typedef struct Stat { u32 first; u32 active; } Stat;
typedef struct W {
    u8 pad0[0xE];
    u8 count;
    u8 target;
    u8 pad10[0x198 - 0x10];
    Elem a[31];
    u8 pad7E4[4];
    Elem b[3];
    u8 pad884[8];
    Slot slots[3];
    u8 pad8BC[8];
    Sprite *cursor;
    u8 pad8C8[8];
    Ent *ents;
    u8 pad8D4[0x46A4 - 0x8D4];
    Stat *stats;
} W;
void sub_02094F5C(W *w, int i);
// @fn sub_02094F5C
void sub_02094F5C(W *w, int i){u32 j = w->slots[i].ent; int state = w->ents[j].state; int owner = w->ents[j].owner; Sprite_SetDrawFlag(w->ents[j].sprite, FALSE); w->ents[j].owner = -1; w->count--; if (state == w->target) { Sprite_SetOamMode(w->a[owner].sprite, GX_OAM_MODE_NORMAL); } else { Sprite_SetMatrix(w->cursor, Sprite_GetMatrixPtr(w->b[i].sprite)); Sprite_SetDrawFlag(w->cursor, TRUE); Sprite_SetAnimCtrlSeq(w->cursor, 50); } w->slots[i].active = 0; Sprite_SetDrawFlag(w->b[i].sprite, FALSE); w->stats[i].active = 0;}
