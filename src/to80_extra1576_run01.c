#pragma force_active on
#include "global.h"
typedef struct {u32 x0;struct {u32 x0,x4,x8;fx32 speed;} *data;fx32 scale;}Config;
typedef union {u32 all;struct {u32 pad0:16;u32 direction:4;u32 sound:2;u32 pad22:5;u32 special:1;u32 locked:1;u32 pad29:3;};}Flags;
typedef struct {u32 x0;Config *config;u32 x8,xc,x10,x14;int type;u32 x1c;VecFx32 target,pos,direction,velocity;u8 pad50[8];s8 count;u8 pad59[7];Flags flags;}Work;
extern void ov96_02215944(VecFx32 *,int,int),ov96_02218784(void *,int);
extern int ov96_022186CC(VecFx32 *);
void ov96_022187A8(Work *,void *);
static inline void Zero(VecFx32 *v){v->x=0;v->y=0;v->z=0;}
void ov96_022187A8(Work *w,void *ctx){VecFx32 delta,normal,move,scaled;fx32 mag=0,speed;int active;VEC_Subtract(&w->target,&w->pos,&delta);mag=VEC_Mag(&delta);active=mag>0?1:0;
 if(w->flags.special){if(w->type==0)GF_AssertFail();if(w->type==6||w->type==9)speed=4096;else speed=0xb33;
 if(w->count<=0){VecFx32 vnormal={0,0,0},zero={0,0,0};VEC_Normalize(&w->velocity,&vnormal);VEC_MultAdd(speed,&vnormal,&zero,&scaled);scaled.x=scaled.x<0?-scaled.x:scaled.x;scaled.y=scaled.y<0?-scaled.y:scaled.y;ov96_02215944(&w->velocity,scaled.x,scaled.y);}
 VEC_Add(&w->pos,&w->velocity,&w->pos);w->target=w->pos;if(w->velocity.x!=0)return;if(w->velocity.y!=0)return;ov96_02218784(ctx,(u8)w->flags.sound);w->x10=0;w->flags.all&=0xf7ffffff;
 }
 if(!w->flags.locked&&w->count<=0&&active){VEC_Normalize(&delta,&normal);if(!w->flags.special)w->flags.direction=ov96_022186CC(&normal);speed=w->config->data->speed;if(w->config->scale<30*4096)speed=FX_Mul(speed,2048);if(mag<=speed)move=delta;else{VecFx32 zero2={0,0,0};VEC_MultAdd(speed,&normal,&zero2,&move);}VEC_Add(&w->pos,&move,&w->pos);w->direction=normal;}}
