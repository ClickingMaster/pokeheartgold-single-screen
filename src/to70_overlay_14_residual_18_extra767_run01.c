#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x34];u8*ptr;}Work;extern int BgCellManager_UpdateCellMove(void*,int);extern void PCBox_MoveBoxTraySprites(Work*,int);
int PCBox_UpdateBoxTraySlideIn(Work*w);
int PCBox_UpdateBoxTraySlideOut(Work*w);
int PCBox_UpdateBoxTraySlideIn(Work*w){int a=BgCellManager_UpdateCellMove(*(void**)(w->ptr+0x2f0),0);int b=BgCellManager_UpdateCellMove(*(void**)(w->ptr+0x2f0),15);PCBox_MoveBoxTraySprites(w,8);if(a==0 && b==0)return 0;return 1;}
int PCBox_UpdateBoxTraySlideOut(Work*w){int a=BgCellManager_UpdateCellMove(*(void**)(w->ptr+0x2f0),0);PCBox_MoveBoxTraySprites(w,-8);return a;}
