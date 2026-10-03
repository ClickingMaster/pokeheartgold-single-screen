#pragma force_active on
#include "global.h"
typedef struct {int heap;u8 pad[20];u32 *script;u8 pad2[168];void *bg,*palette;} Work;
extern void *ov07_0221DF1C(Work*);extern int ov07_0221FB7C(int,int),ov07_0221DDB0(void*,Work*,int);
extern void ToggleBgLayer(u8,int),SetBgControlParam(void*,u8,int,u8),BgClearTilemapBufferAndCommit(void*,u8),Heap_Free(void*);
extern void GfGfxLoader_LoadCharData(int,int,void*,int,int,int,int,int),GfGfxLoader_LoadScrnData(int,int,void*,int,int,int,int,int),PaletteData_LoadNarc(void*,int,int,int,int,int,int);
void ov07_0221EF54(Work*);
void ov07_0221EF54(Work *w){void *temp=ov07_0221DF1C(w);int id,member,variant;w->script++;id=*(volatile u32*)w->script;w->script++;ToggleBgLayer(3,0);SetBgControlParam(w->bg,3,0,0);member=ov07_0221FB7C(id,0);GfGfxLoader_LoadCharData(7,member,w->bg,3,0,0,1,w->heap);member=ov07_0221FB7C(id,1);PaletteData_LoadNarc(w->palette,7,member,w->heap,0,32,144);BgClearTilemapBufferAndCommit(w->bg,3);variant=2;if(ov07_0221DDB0(temp,w,7)==1)variant=3;member=ov07_0221FB7C(id,variant);GfGfxLoader_LoadScrnData(7,member,w->bg,3,0,0,1,w->heap);ToggleBgLayer(3,1);Heap_Free(temp);}
