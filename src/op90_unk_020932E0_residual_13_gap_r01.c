#pragma force_active on
#include "global.h"
#include "sprite.h"
typedef struct W{u8 p[15];u8 current,total;u8 p11[0x8ab];Sprite*sprite,*motion;u8 p8c4[0x3df4];void*control;}W;extern void sub_020943EC(W*),GridCursor_SetPosition(void*,int,int);extern BOOL PlaySE(u16);extern int GridCursor_GetPanel(void*),GridCursor_GetIndex(void*);void sub_02095540(W*);
static inline u8 count(W*w){if(w->current!=0)return w->current;return w->total;}
// @fn sub_02095540
void sub_02095540(W*w){if(w->current==0){w->current=w->total-1;}else{w->current--;}Sprite_SetAnimCtrlSeq(w->sprite,1);sub_020943EC(w);PlaySE(1500);switch(w->current){case 18:if(GridCursor_GetPanel(w->control)==2){u8 i=GridCursor_GetIndex(w->control)%6;VecFx32 pos;GridCursor_SetPosition(w->control,6,i);pos.x=(i*24+20)<<12;pos.y=40<<12;pos.z=0;Sprite_SetMatrix(w->motion,&pos);}goto done;default:if(GridCursor_GetPanel(w->control)==6)GridCursor_SetPosition(w->control,2,GridCursor_GetIndex(w->control));goto done;}done: return;}
