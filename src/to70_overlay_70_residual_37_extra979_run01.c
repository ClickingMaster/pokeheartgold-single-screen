#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0xc];void*a,*b,*c;u32 pad;void*arg;u8 p2[0x2c];int state,next;u32 p3;s16 mode;} Work;
extern int BgCellManager_IsCellMoving(void*,int);
extern void Sprite_SetDrawFlag(void*,int);
extern void Gts_SetSpritePosition(void*,int,int);
int ov70_02242FFC(Work*);
int ov70_02242FFC(Work*w){if(BgCellManager_IsCellMoving(w->arg,0)==0){Sprite_SetDrawFlag(w->a,1);if(w->mode==2){Gts_SetSpritePosition(w->b,228,120);Gts_SetSpritePosition(w->c,154,120);Sprite_SetDrawFlag(w->b,1);Sprite_SetDrawFlag(w->c,1);}w->state=w->next;}return -1;}
