#pragma force_active on
#include "global.h"
extern void GfGfx_EngineATogglePlanes(int,int),GfGfx_EngineBTogglePlanes(int,int),FreeBgTilemapBuffer(void*,int),Heap_Free(void*);
void ov81_02240458(void*);
void ov81_02240458(void*w){GfGfx_EngineATogglePlanes(31,0);GfGfx_EngineBTogglePlanes(31,0);FreeBgTilemapBuffer(w,3);FreeBgTilemapBuffer(w,2);FreeBgTilemapBuffer(w,1);FreeBgTilemapBuffer(w,4);FreeBgTilemapBuffer(w,5);FreeBgTilemapBuffer(w,6);FreeBgTilemapBuffer(w,7);Heap_Free(w);}
