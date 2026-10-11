#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x4c];void *bg;u8 q[0x38c];void *msg;u8 r[0x94];void *obj;} W;
extern void *BgCellManager_New(void*,int,int,int);extern void BgCellManager_InitCell(void*,int,int,int,int);extern void BgCellManager_LoadCellTilemapFromOpenNarc(void*,int,void*,int,int);
void ov81_0224271C(W*);
void ov81_0224271C(W*w) {w->obj=BgCellManager_New(w->bg,2,1,100);BgCellManager_InitCell(w->obj,0,4,18,4);BgCellManager_LoadCellTilemapFromOpenNarc(w->obj,0,w->msg,144,1);}
