#pragma force_active on
#include "global.h"
typedef struct Entry { int a; int b; u8 gap8[0x10]; int x; int y; u8 c; u8 d; u8 e; u8 pad23; int f; int active; u8 id; u8 pad2D[3]; } Entry;
typedef struct Src { int a; int b; u8 gap8[4]; u8 id; u8 padD[3]; int x; int y; u8 c; u8 d; } Src;
typedef struct W { u32 unk0; Entry entries[2]; Src src; u8 pad80[0xA1 - 0x80]; u8 cur; } W;
int ov01_021F5BF0(W *w);
// @fn ov01_021F5BF0
int ov01_021F5BF0(W *w){Src *src = &w->src; GF_ASSERT(w->entries[w->cur].active == FALSE); w->entries[w->cur].active = TRUE; w->entries[w->cur].id = src->id; w->entries[w->cur].x = src->x; w->entries[w->cur].y = src->y; w->entries[w->cur].c = src->c; w->entries[w->cur].d = src->d; w->entries[w->cur].a = src->a; w->entries[w->cur].b = src->b; w->entries[w->cur].e = 0; w->entries[w->cur].f = 0; return TRUE;}
