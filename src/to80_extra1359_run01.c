#pragma force_active on
#include "global.h"
typedef struct {int heap;u8 pad[192];void *bg;u8 pad2[200];int narc,charMember,unknown,scrnMember;} Work;
extern int ov07_0221FB04(Work*,int),ov07_0221BFC0(Work*);
extern void SetBgPriority(u8,u8),SetBgControlParam(void*,u8,int,u8),ToggleBgLayer(u8,int),BgClearTilemapBufferAndCommit(void*,u8),ov07_0221FB30(Work*,int);
extern void SetBgPriority(u8,u8);
extern void SetBgPriority(u8,u8);
extern void GfGfxLoader_LoadCharData(int,int,void*,int,int,int,int,int),GfGfxLoader_LoadScrnData(int,int,void*,int,int,int,int,int);
void ov07_0221EBA4(Work*,int,int);
void ov07_0221EBA4(Work *w,int bg,int mode){SetBgPriority(bg,ov07_0221FB04(w,2));ToggleBgLayer(bg,0);if(ov07_0221BFC0(w)==1)SetBgControlParam(w->bg,bg,2,4);else{SetBgControlParam(w->bg,bg,0,mode);SetBgControlParam(w->bg,bg,2,4);}BgClearTilemapBufferAndCommit(w->bg,bg);if(ov07_0221BFC0(w)==1)GfGfxLoader_LoadCharData(w->narc,w->charMember,w->bg,bg,0,0,1,w->heap);else ov07_0221FB30(w,bg);GfGfxLoader_LoadScrnData(w->narc,w->scrnMember,w->bg,bg,0,0,1,w->heap);}
