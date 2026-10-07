#include "global.h"
#include "bg_window.h"
#pragma force_active on
typedef struct Work{void*owner;u8 pad4[8];BgConfig*bg;u8 pad10[0x534];u16 map1[24][16],map2[24][16];}Work;
void ov05_0221DE6C(void*,u8);
static inline void CopyPanelMaps(Work *w,u8 width,s32 destination,u8 source) {
 CopyToBgTilemapRect(w->bg,1,(u8)destination,0,width,24,w->map1,source,0,16,24);
 CopyToBgTilemapRect(w->bg,2,(u8)destination,0,width,24,w->map2,source,0,16,24);
}
void ov05_0221DE6C(void *data,u8 width) {
 Work *w=data;
 s32 destination;
 if(*((u8*)w->owner+0x29)==0) {
 destination=32-width;CopyPanelMaps(w,width,destination,0);
 } else {
 destination=16-width;CopyPanelMaps(w,width,0,(u8)destination);
 }
 ScheduleBgTilemapBufferTransfer(w->bg,1);
 ScheduleBgTilemapBufferTransfer(w->bg,2);
}
