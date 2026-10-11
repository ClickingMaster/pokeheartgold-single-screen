#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x2c];void*input;u8 q[0x320-0x30];void*sprite;} View;typedef struct {u8 p[0x34];View*v;} Work;
extern void*GridInputHandler_GetDpadBox(void*);extern void PCBox_SetSpriteVisible(View*,int,int),DpadMenuBox_GetPosition(void*,u8*,u8*),ManagedSprite_SetPositionXY(void*,s16,s16),GridInputHandler_SetNextInput(void*,int);
void PCBox_PlaceCursorOnGridInput(Work*,int),ov14_021F6B60(Work*,int);
void PCBox_PlaceCursorOnGridInput(Work*w,int arg){void*box=GridInputHandler_GetDpadBox(w->v->input);struct{u8 y,x;} pos;PCBox_SetSpriteVisible(w->v,9,1);DpadMenuBox_GetPosition(box,&pos.x,&pos.y);ManagedSprite_SetPositionXY(w->v->sprite,pos.x,pos.y);}
void ov14_021F6B60(Work*w,int arg){GridInputHandler_SetNextInput(w->v->input,(u8)arg);PCBox_PlaceCursorOnGridInput(w,arg);}
