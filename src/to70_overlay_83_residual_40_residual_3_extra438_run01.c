#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {s16 x,y;void *sprite;} State;
extern void *Heap_Alloc(int,int),*ov83_0224714C(void *,void *,int,int,int,int,int,int);extern void Sprite_SetMatrix(void *,VecFx32 *);
State *ov83_02247454(void *,void *,int,int,int,s16,s16,int),*ov83_022474C4(void *,void *,int,int,int,s16,s16,int);
State *ov83_02247454(void *a,void *b,int c,int d,int e,s16 x,s16 y,int f){State *s=Heap_Alloc(107,8);VecFx32 v;memset(s,0,8);s->x=x;s->y=y;s->sprite=ov83_0224714C(a,b,c,d,e,0,f,0);v.x=x<<12;v.y=y<<12;Sprite_SetMatrix(s->sprite,&v);return s;}
State *ov83_022474C4(void *a,void *b,int c,int d,int e,s16 x,s16 y,int f){State *s=Heap_Alloc(107,8);VecFx32 v;memset(s,0,8);s->x=x;s->y=y;s->sprite=ov83_0224714C(a,b,c,d,e,0,f,1);v.x=x<<12;v.y=(y<<12)+0xc0000;Sprite_SetMatrix(s->sprite,&v);return s;}
