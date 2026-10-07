#pragma force_active on
#include "global.h"
typedef struct { u32 unk0; int key; u8 pad8[0x14]; } Ent;
typedef struct { Ent ents[25]; u8 pad[0x2D0 - 25 * 0x1C]; int count; } Lst;
int sub_020863C0(Lst *l, int key);
// @fn sub_020863C0
int sub_020863C0(Lst *l, int key){int i; int found = 0; int count = l->count; for (i = 0; i < count; i++) { if (l->ents[i].key == key) { found = TRUE; } else if (found == TRUE) { return i - 1; } } return count - 1;}
