#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct State {u8 pad0[8];u8 step;u8 pad9[7];s16 x,index;u8 pad14[336];void *options;u8 pad168[452];void *items[4];} State;typedef struct Pos {s16 x,y;} Pos;extern Pos ov87_021E81A0[];extern Pos ov87_021E81A2[];
void ov87_021E7FEC(void *,int,int),ov87_021E7FE0(void *,int),ov87_021E8134(void *,int),ov87_021E7048(void *),GfGfx_EngineBTogglePlanes(int,int),PlaySE(int);int Options_GetFrame(void *),ov87_021E5B48(State *);
int ov87_021E5B48(State *s){int i;switch(s->step){case 0:s->x=-256;for(i=0;i<4;i++){ov87_021E7FEC(s->items[i],s->x+ov87_021E81A0[i].x,ov87_021E81A0[i].y);ov87_021E7FE0(s->items[i],i+20);}ov87_021E8134((u8 *)s+332,Options_GetFrame(s->options));ov87_021E7048(s);GfGfx_EngineBTogglePlanes(1,1);s->index=3;PlaySE(1376);s->step=1;break;case 1:s->x+=32;ov87_021E7FEC(s->items[s->index],s->x+ov87_021E81A0[s->index].x,ov87_021E81A2[s->index].x);if(s->x>=0){if(s->index==0)s->step=2;else{PlaySE(1376);s->index--;s->x=-256;}}break;case 2:s->x=0;return 1;}return 0;}
