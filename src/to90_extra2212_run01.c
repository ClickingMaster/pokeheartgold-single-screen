#pragma force_active on
#include "global.h"
#include "bg_window.h"
#pragma force_active on
typedef struct{BgConfig*bg;}Work;
void ov43_0222B408(void*,Work*);
void ov43_0222B408(void*unused,Work*w){int i;for(i=0;i<4;i++){int y=i*5+4;BgTilemapRectChangePalette(w->bg,2,2,(u8)y,28,4,2);ScheduleBgTilemapBufferTransfer(w->bg,2);}}
