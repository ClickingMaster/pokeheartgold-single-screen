#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "pokepic.h"
typedef struct{u8 a,b;u16 pad;s16 x,y;void*v,*owner,*extra;Pokepic*pic;ManagedSprite*sprite;u8 p[0x24];u8 movement[36];u8 other[40];}W;
extern W*ov07_022324D8(void*,int);extern void*ov07_0221C514(void*);extern void*ov07_0221C4A0(void*);extern ManagedSprite*ov07_0221C4E8(void*,int);
extern u32 ov07_0221C4A8(void *p, int i);extern int ov07_0221C470(void*);extern int ov07_0221C468(void*);
extern Pokepic*ov07_0221FA48(void*,int);extern void ov07_02231FA0(Pokepic*,s16*);extern int ov07_02222004(void*,int);extern int ov07_0222202C(void*,int);
extern void ov07_02222268(void*,int,s16,s16,s16,int);extern int ov07_0221FAF8(void*,int);extern void ov07_02231E44(void*,int,int,int);
extern void ov07_02222AC4(void*,s16,s16,s16,int,int);extern void ov07_02228D08(void*);extern void ov07_0221C410(void*,void(*)(void*),void*);
void ov07_02228D64(void*);
void ov07_02228D64(void*owner){W*w=ov07_022324D8(owner,140);int index,a,b,c,d;w->a=0;w->b=0;w->owner=owner;w->extra=ov07_0221C514(owner);w->v=ov07_0221C4A0(w->owner);w->sprite=ov07_0221C4E8(w->owner,0);if(((s32)ov07_0221C4A8(owner, 0))==0)index=ov07_0221C470(w->owner);else index=ov07_0221C468(w->owner);w->pic=ov07_0221FA48(w->owner,index);ov07_02231FA0(w->pic,&w->x);w->y-=Pokepic_GetAttr(w->pic,41);a=ov07_02222004(w->owner,index);b=ov07_0222202C(w->owner,index);ov07_02222268(w->movement,w->x,(s16)(w->x+(-20)*a),w->y,(s16)(w->y+20*b),20);c=ov07_0221FAF8(owner,2);d=ov07_0221FAF8(owner,1);ov07_02231E44(w->owner,(1<<c)|32|(1<<d)|1,-1,-1);ov07_02222AC4(w->other,31,0,0,31,15);ManagedSprite_SetOamMode(w->sprite,(GXOamMode)1);ov07_0221C410(w->owner,ov07_02228D08,w);}
