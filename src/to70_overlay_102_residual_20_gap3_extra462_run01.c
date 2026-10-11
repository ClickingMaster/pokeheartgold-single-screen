#pragma force_active on
#include "global.h"
typedef struct {void *p;u8 pad[0x5c];void *spr1,*spr2;u8 pad2[0x178];s16 y1,y2;u8 ticks,direction;} W;
extern void ov102_021EA678(W *,int),BgSetPosTextAndCommit(void *,int,int,int),Sprite_OffsetPositionXY(void *,s16,s16),HBlankInterruptDisable(void);
extern void *EasyChatAppView_GetBgConfig(void *);
extern void Main_SetHBlankIntrCB(void *,void *);
int ov102_021EA754(W *);
extern void Main_SetHBlankIntrCB(void *,void *);
int ov102_021EA754(W *w){void *bg;int delta;if(w->ticks>5){if(w->direction==0)ov102_021EA678(w,1);else ov102_021EA678(w,0);return 1;}bg=EasyChatAppView_GetBgConfig(w->p);if(w->direction)delta=-8;else delta=8;w->y2+=delta;w->y1+=delta;BgSetPosTextAndCommit(bg,0,3,w->y1);BgSetPosTextAndCommit(bg,4,3,w->y2);Sprite_OffsetPositionXY(w->spr1,0,-delta);Sprite_OffsetPositionXY(w->spr2,0,-delta);w->ticks++;if(w->direction&&w->ticks==6){Main_SetHBlankIntrCB(0,0);HBlankInterruptDisable();}return 0;}
