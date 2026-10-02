#pragma force_active on
#include "global.h"
#include "bg_window.h"
#pragma force_active on
extern GraphicsModes ov85_021EA8F4;extern BgTemplate ov85_021EA980,ov85_021EA99C,ov85_021EA92C,ov85_021EA948,ov85_021EA964;void ov85_021E8C5C(BgConfig*);
void ov85_021E8C5C(BgConfig*b){GraphicsModes modes=ov85_021EA8F4;BgTemplate t4,t5,t6,t0,t1;SetBothScreensModesAndDisable(&modes);t4=ov85_021EA980;InitBgFromTemplate(b,4,&t4,0);BgClearTilemapBufferAndCommit(b,4);t5=ov85_021EA99C;InitBgFromTemplate(b,5,&t5,0);BgClearTilemapBufferAndCommit(b,5);t6=ov85_021EA92C;InitBgFromTemplate(b,6,&t6,0);t0=ov85_021EA948;InitBgFromTemplate(b,0,&t0,0);BgClearTilemapBufferAndCommit(b,0);t1=ov85_021EA964;InitBgFromTemplate(b,1,&t1,0);BG_ClearCharDataRange(0,32,0,(enum HeapID)102);BG_ClearCharDataRange(4,32,0,(enum HeapID)102);}
