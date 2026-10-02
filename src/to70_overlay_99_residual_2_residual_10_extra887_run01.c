#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "filesystem.h"
typedef struct {u32 first;BgConfig *bg;NARC *narc;enum HeapID heap;} Work;
extern GraphicsModes ov99_021E9D9C;typedef struct {BgTemplate t[4];} Templates;extern Templates ov99_021E9E60;extern u8 ov99_021E9D88[];
void ov99_021E695C(Work *);
void ov99_021E695C(Work *w){GraphicsModes modes;Templates templates;int i;w->bg=BgConfig_Alloc(w->heap);modes=ov99_021E9D9C;SetBothScreensModesAndDisable(&modes);templates=ov99_021E9E60;for(i=0;i<4;i++){InitBgFromTemplate(w->bg,ov99_021E9D88[i],&templates.t[i],0);BgClearTilemapBufferAndCommit(w->bg,ov99_021E9D88[i]);BG_ClearCharDataRange(ov99_021E9D88[i],32,0,w->heap);}w->narc=NARC_New((NarcId)177,w->heap);}
