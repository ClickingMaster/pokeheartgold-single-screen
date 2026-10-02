#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u8 pad[64];void*bg;};void FreeBgTilemapBuffer(void*,int);void Heap_Free(void*);void ov113_021E6490(struct S*);
void ov113_021E6490(struct S*s){FreeBgTilemapBuffer(s->bg,3);FreeBgTilemapBuffer(s->bg,2);FreeBgTilemapBuffer(s->bg,1);FreeBgTilemapBuffer(s->bg,7);FreeBgTilemapBuffer(s->bg,6);FreeBgTilemapBuffer(s->bg,5);Heap_Free(s->bg);{volatile u16*p=(volatile u16*)0x04000304;u32 v=*p;v|=0x8000;*p=v;}}
