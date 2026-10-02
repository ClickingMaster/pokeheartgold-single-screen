#pragma force_active on
#include "global.h"
#include "bg_window.h"
typedef struct {u8 pad[4];int state;u8 pad2[0x10];BgConfig *bg;u8 pad3[0x1ea98-0x1c];Window first,second;} Work;
extern int ov112_021E9750(Work *);extern void ov112_021EA570(Work *);
int ov112_021EE1E8(Work *);
int ov112_021EE1E8(Work *w){switch(ov112_021E9750(w)){case 0:ov112_021EA570(w);FillBgTilemapRect(w->bg,1,0,0,0,32,24,16);FillBgTilemapRect(w->bg,2,0,0,0,32,24,16);ClearWindowTilemapAndCopyToVram(&w->first);ClearWindowTilemapAndCopyToVram(&w->second);BgCommitTilemapBufferToVram(w->bg,2);return 47;case 1:FillBgTilemapRect(w->bg,1,0,0,16,32,8,16);ClearWindowTilemapAndCopyToVram(&w->first);ClearWindowTilemapAndCopyToVram(&w->second);ov112_021EA570(w);w->state=0;return 66;default:return 46;}}
