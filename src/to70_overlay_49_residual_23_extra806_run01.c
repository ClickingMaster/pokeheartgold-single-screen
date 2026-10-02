#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[48];void *menu;u8 q[6];u16 total;int flag;u8 r[16];void *top,*bottom;} State;
extern void Sprite_SetDrawFlag(void *,int),ListMenuGetScrollAndRow(void *,u16 *,u16 *);
void ov49_0225B070(State *);
void ov49_0225B070(State *s){u16 scroll;if(s->flag==0){Sprite_SetDrawFlag(s->top,0);Sprite_SetDrawFlag(s->bottom,0);return;}ListMenuGetScrollAndRow(s->menu,&scroll,0);if(scroll==0)Sprite_SetDrawFlag(s->top,0);else Sprite_SetDrawFlag(s->top,1);if(scroll>=s->total-7)Sprite_SetDrawFlag(s->bottom,0);else Sprite_SetDrawFlag(s->bottom,1);}
