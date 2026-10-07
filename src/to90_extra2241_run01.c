#pragma force_active on
#include "global.h"
#include "bg_window.h"
#pragma force_active on
typedef struct {u32 pad;BgConfig *bg;void*narc;enum HeapID heap;} Work;
typedef struct {BgTemplate templates[8];} Bank;
extern GraphicsModes ov99_021E9564;extern Bank ov99_021E9760;extern u8 _021E954C[];
extern void *NARC_New(int,enum HeapID);
void ov99_021E5C88(Work*);
void ov99_021E5C88(Work*w){GraphicsModes display;Bank bank;int i;
w->bg=BgConfig_Alloc(w->heap);display=ov99_021E9564;SetBothScreensModesAndDisable(&display);bank=ov99_021E9760;i=0;
do{InitBgFromTemplate(w->bg,_021E954C[i],&bank.templates[i],0);BgClearTilemapBufferAndCommit(w->bg,_021E954C[i]);BG_ClearCharDataRange(_021E954C[i],32,0,w->heap);i++;}while(i<8);
G2x_SetBlendAlpha_(0x4000050,8,4,15,0);G2x_SetBlendAlpha_(0x4001050,8,2,15,0);w->narc=NARC_New(177,w->heap);}
