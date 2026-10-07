#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 data[0x1860];} State;
typedef struct {s16 y,x;} Point;
extern void ManagedSprite_GetPositionXY(void *,s16 *,s16 *),ManagedSprite_SetPositionXY(void *,s16,s16),ov18_021F1598(State *,int,int);
static inline int sum(int a,int b){return b+a;}
void ov18_021F1760(State *,int,int,int);
void ov18_021F1760(State *s,int first,int speed,int row){
 int which,base;u16 i,wraps=0; i=wraps;base=(row+5)*5;Point p;u8 *entry;
 for(;i<30;i++){
  which=first+i+s->data[0x185e]*30;entry=s->data+which*4;
  ManagedSprite_GetPositionXY(*(void **)(entry+0x670),&p.x,&p.y);
  if(p.y==-16){p.y=224;ov18_021F1598(s,wraps+(sum(base,15*s->data[0x1859])),which);wraps++;}
  p.y+=speed;ManagedSprite_SetPositionXY(*(void **)(entry+0x670),p.x,p.y);
 }
}
