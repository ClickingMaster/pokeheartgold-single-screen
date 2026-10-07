#pragma force_active on
#include "global.h"
#include "bg_window.h"
extern const GraphicsModes ov111_021E6B7C;typedef struct {BgTemplate items[5];} Templates;extern const Templates ov111_021E6CA4;extern u8 _021E6B74[];
typedef struct {enum HeapID heap;u32 pad;BgConfig *bg;u8 pad2[0x10];void *narc;} W;
extern void *NARC_New(int,enum HeapID);
void ov111_021E5F50(W *);
#pragma force_active on
void ov111_021E5F50(W *w){GraphicsModes modes;Templates templates;int i;const u8 *ids;const BgTemplate *t;w->bg=BgConfig_Alloc(w->heap);modes=ov111_021E6B7C;SetBothScreensModesAndDisable(&modes);templates=ov111_021E6CA4;ids=_021E6B74;i=0;t=templates.items;for(;i<5;t++,ids++,i++){InitBgFromTemplate(w->bg,(GFBgLayer)*ids,t,0);BgClearTilemapBufferAndCommit(w->bg,(GFBgLayer)*ids);BG_FillCharDataRange(w->bg,(GFBgLayer)*ids,0,1,0);}w->narc=NARC_New(173,w->heap);}
