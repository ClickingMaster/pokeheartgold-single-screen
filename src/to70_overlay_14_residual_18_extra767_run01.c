#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x34];u8*ptr;}Work;extern int BgCellManager_UpdateCellMove(void*,int);extern void ov14_021F47B8(Work*,int);
int ov14_021E9554(Work*w);
int ov14_021E9590(Work*w);
int ov14_021E9554(Work*w){int a=BgCellManager_UpdateCellMove(*(void**)(w->ptr+0x2f0),0);int b=BgCellManager_UpdateCellMove(*(void**)(w->ptr+0x2f0),15);ov14_021F47B8(w,8);if(a==0 && b==0)return 0;return 1;}
int ov14_021E9590(Work*w){int a=BgCellManager_UpdateCellMove(*(void**)(w->ptr+0x2f0),0);ov14_021F47B8(w,-8);return a;}
