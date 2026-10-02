#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W{u8 pad[12];int state,mode;u8 gap[4];void*bg;u8 tail[120672];u16 condition;}W;extern void FillBgTilemapRect(void*,int,int,int,int,int,int,int),BgCommitTilemapBufferToVram(void*,int),ov112_021EA688(W*,int),ov112_021EC460(W*,int,int,int);int ov112_021EC55C(W*);
int ov112_021EC55C(W*w){if(w->mode==1){w->state=1;return 19;}else if(w->mode==2){w->state=22;return 19;}else if(w->mode==4)return 43;else if(w->condition)return 5;FillBgTilemapRect(w->bg,1,0,0,0,32,24,16);FillBgTilemapRect(w->bg,2,0,0,0,32,24,16);BgCommitTilemapBufferToVram(w->bg,2);FillBgTilemapRect(w->bg,4,0,0,0,32,24,16);FillBgTilemapRect(w->bg,5,0,0,0,32,24,16);BgCommitTilemapBufferToVram(w->bg,4);BgCommitTilemapBufferToVram(w->bg,5);ov112_021EA688(w,3);ov112_021EA688(w,4);ov112_021EC460(w,2,11,3);return 64;}
