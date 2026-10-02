#pragma force_active on
#include "global.h"
typedef struct {s16 x,y,z;u16 anim;int priority,pal,screen;int res[6];int bg,transfer;} Template;
typedef struct {s16 x,y;} XY;
typedef struct{XY values[8];} CoordBank;
extern CoordBank ov57_0223BDD4;extern void *SpriteSystem_NewSprite(void *,void *,Template *);extern void ManagedSprite_SetPositionXY(void *,s16,s16);extern void ManagedSprite_TickFrame(void *);extern void ManagedSprite_SetAnimationFrame(void *,int);
typedef struct {u8 p[0xdc];void *system,*manager;u8 pE4[0x330];void *sprites[8];} Work;
void ov57_02238A00(Work *);
void ov57_02238A00(Work *w){Template t;CoordBank coords;void *system=w->system,*manager=w->manager;void **cursor;int i;t.x=0;t.y=0;t.z=0;t.anim=0;t.priority=60;t.screen=2;t.bg=1;t.res[4]=-1;t.res[5]=-1;t.res[1]=26007;t.res[2]=27037;t.res[3]=28035;t.transfer=0;t.pal=0;cursor=w->sprites;for(i=0;i<8;i++){t.res[0]=25000+i;*cursor++=SpriteSystem_NewSprite(system,manager,&t);}coords=ov57_0223BDD4;for(i=0;i<8;i++){ManagedSprite_SetPositionXY(w->sprites[i],coords.values[i].x,(s16)(coords.values[i].y-1));ManagedSprite_TickFrame(w->sprites[i]);ManagedSprite_SetAnimationFrame(w->sprites[i],0);}}
