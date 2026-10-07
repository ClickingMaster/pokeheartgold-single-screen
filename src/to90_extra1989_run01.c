#pragma force_active on
#include "global.h"
#include "bg_window.h"
#pragma force_active on
extern void ov108_021E9830(void *);
extern void SetBothScreensModesAndDisable(const GraphicsModes *);
extern const GraphicsModes ov108_021EAAC0;
extern const BgTemplate ov108_021EAB54,ov108_021EAB1C,ov108_021EAB38,ov108_021EAB8C,ov108_021EAAE4,ov108_021EAB00,ov108_021EAB70;
typedef struct { enum HeapID heap; u8 pad[0x434]; BgConfig *bg; } Work;
void ov108_021E9850(Work *);
void ov108_021E9850(Work *w) {
GraphicsModes modes; BgTemplate b1,b2,b3,b4,b5,b6,b7;
ov108_021E9830(w);
*(volatile u16 *)0x04000304 |= 0x8000;
w->bg=BgConfig_Alloc(w->heap);
modes=ov108_021EAAC0; SetBothScreensModesAndDisable(&modes);
b1=ov108_021EAB54;InitBgFromTemplate(w->bg,1,&b1,0);BgClearTilemapBufferAndCommit(w->bg,1);
b2=ov108_021EAB1C;InitBgFromTemplate(w->bg,2,&b2,0);BgClearTilemapBufferAndCommit(w->bg,2);
b3=ov108_021EAB38;InitBgFromTemplate(w->bg,3,&b3,0);BgClearTilemapBufferAndCommit(w->bg,3);
b4=ov108_021EAB8C;InitBgFromTemplate(w->bg,4,&b4,0);BgClearTilemapBufferAndCommit(w->bg,4);
b5=ov108_021EAAE4;InitBgFromTemplate(w->bg,5,&b5,0);BgClearTilemapBufferAndCommit(w->bg,5);
b6=ov108_021EAB00;InitBgFromTemplate(w->bg,6,&b6,0);BgClearTilemapBufferAndCommit(w->bg,6);
b7=ov108_021EAB70;InitBgFromTemplate(w->bg,7,&b7,0);BgClearTilemapBufferAndCommit(w->bg,7);
BG_ClearCharDataRange(3,32,0,w->heap);
BG_ClearCharDataRange(4,32,0,w->heap);
BG_ClearCharDataRange(7,32,0,w->heap);
}
