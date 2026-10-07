#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Template {s16 x,y,z,anim;int priority,palette,vram;int resources[6];int extra[2];} Template;
typedef struct State {void *ctx,*system,*manager;u8 pad[12];void *pic;u8 pad1c[40];s16 y,attr;void *sprites[6];u8 pad60[24];int priority[6];} State;
void *ov07_022324D8(void *,int);int ov07_0221C468(void *);void *ov07_0221FA48(void *,int);int Pokepic_GetAttr(void *,int);int ov07_0221FAA0(void *,int);
void ov07_0221F9E8(Template *,void *);void *SpriteSystem_NewSprite(void *,void *,Template *);void ManagedSprite_SetDrawFlag(void *,int);
void ManagedSprite_SetFlipMode(void *,int);void ManagedSprite_SetPositionXY(void *,s16,s16);void ov07_0221C410(void *,void (*)(void),void *);void ov07_0222EF88(void);
extern const s16 FX_SinCosTable_[];
void ov07_0222F0B0(void *,void *,void *,void *);
void ov07_0222F0B0(void *ctx,void *system,void *manager,void *first) {
 State *s=ov07_022324D8(ctx,144);s16 x,y,dx,dy;Template t;int i;
 s->ctx=ctx;s->system=system;s->manager=manager;
 s->pic=ov07_0221FA48(s->ctx,ov07_0221C468(s->ctx));
 x=Pokepic_GetAttr(s->pic,0);y=Pokepic_GetAttr(s->pic,1);s->y=y;
 s->attr=ov07_0221FAA0(s->ctx,ov07_0221C468(s->ctx));ov07_0221F9E8(&t,s->ctx);
 for(i=0;i<6;i++) {
  int angle;
  if(i==0)s->sprites[i]=first;else s->sprites[i]=SpriteSystem_NewSprite(system,manager,&t);
  ManagedSprite_SetDrawFlag(s->sprites[i],0);
  angle=((i/2)*0x1555)>>4;
  dx=(s32)(((s64)FX_SinCosTable_[angle*2+1]*0x30000+0x800)>>12)>>12;
  dy=(s32)(((s64)FX_SinCosTable_[angle*2]*0x30000+0x800)>>12)>>12;

  if(i%2==0){dx=x+dx;ManagedSprite_SetFlipMode(s->sprites[i],1);}else dx=x-dx;
  dy=y-dy;ManagedSprite_SetPositionXY(s->sprites[i],dx,dy);s->priority[i]=i<2?8:0;
 }
 ov07_0221C410(s->ctx,ov07_0222EF88,s);
}
