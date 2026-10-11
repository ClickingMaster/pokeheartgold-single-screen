#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x2f0];void*bg;}Inner;typedef struct{u8 p[0x34];Inner *inner;}Work;
extern int BgCellManager_UpdateCellMove(void*,int);int ov14_021E94BC(Work*);
int ov14_021E94BC(Work*w){int a=BgCellManager_UpdateCellMove(w->inner->bg,8);int b=BgCellManager_UpdateCellMove(w->inner->bg,9);int c=BgCellManager_UpdateCellMove(w->inner->bg,10);int d=BgCellManager_UpdateCellMove(w->inner->bg,11);if(a==0&&b==0&&c==0&&d==0)return 0;return 1;}
