#pragma force_active on
#include "global.h"
extern void GfGfx_EngineATogglePlanes(int,int);
typedef struct {u8 p[0xb81];u8 state,tick;} Work;
int ov05_0221C80C(Work *);
int ov05_0221C80C(Work *w){if(w->tick==0){GfGfx_EngineATogglePlanes(2,1);*(vu32*)0x04000000=(*(vu32*)0x04000000&0xffff1fff)|0x2000;G2_SetWnd0InsidePlane(31,1);G2_SetWndOutsidePlane(29,0);}if(w->tick==16){w->tick=0;w->state=32;*(vu32*)0x04000000 &=0xffff1fff;return 1;}{u8 tick=w->tick;u16 y=(((0x47-tick)<<8)&0xff00)|(u8)(tick+0x48);*(vu16*)0x04000040=255;*(vu16*)0x04000044=y;}w->tick++;return 0;}
