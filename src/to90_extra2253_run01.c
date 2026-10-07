#pragma force_active on
#include "global.h"
#include "bg_window.h"
#pragma force_active on
typedef struct {BgConfig *bg;u32 pad[2];enum HeapID heap;} Work;
typedef struct {BgTemplate templates[7];} Bank;
extern GraphicsModes ov99_021E9F80;extern Bank ov99_021EA0BC;extern u8 ov99_021EA348[];
extern void *NARC_New(int,enum HeapID);
void ov99_021E7D30(Work*);
void ov99_021E7D30(Work*w){GraphicsModes display;Bank bank;int i;
w->bg=BgConfig_Alloc(w->heap);display=ov99_021E9F80;SetBothScreensModesAndDisable(&display);bank=ov99_021EA0BC;i=0;
do{InitBgFromTemplate(w->bg,ov99_021EA348[i],&bank.templates[i],0);BgClearTilemapBufferAndCommit(w->bg,ov99_021EA348[i]);BG_ClearCharDataRange(ov99_021EA348[i],32,0,w->heap);i++;}while(i<7);
}
