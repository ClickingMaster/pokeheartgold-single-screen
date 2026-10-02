#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Work {u32 field0,field4;void *ctx;u8 padc[0x18];void *sprite;} Work;
typedef struct Flow {u8 pad0[8];u32 duration;u8 padc[4];s16 x,y;void*pic;u8 pad18[8];u8 state;u8 pad21[0x1b];s16 movement[18];void *ctx;s16 dx,dy;} Flow;
extern Work *ov07_022324D8(void*,int);
extern int ov07_0221C4A8(void*,int);
extern void ov07_02231FE4(void*,void*),ov07_0221EB98(void*,int);
extern void *ov07_0221C4E8(void*,int);
extern void ManagedSprite_SetOamMode(void*,int),ManagedSprite_SetAffineOverwriteMode(void*,int),ManagedSprite_SetAffineScale(void*,float,float);
extern void ov07_0222519C(void*,void*),ov07_0221C410(void*,void(*)(void*,void*),void*);
extern void ov07_02222268(void*,s16,s16,s16,s16,u16);
extern int ov07_022222B4(void*);
extern void Pokepic_SetAttr(void*,int,int),ov07_0221C448(void*,void*),Heap_Free(void*);
void ov07_022251CC(void*);void ov07_0222525C(void*,Flow*);
void ov07_022251CC(void*ctx){Work*w=ov07_022324D8(ctx,40);ov07_0221C4A8(ctx,0);ov07_02231FE4(ctx,&w->field4);ov07_0221EB98(w->ctx,2);w->sprite=ov07_0221C4E8(w->ctx,0);ManagedSprite_SetOamMode(w->sprite,2);ManagedSprite_SetAffineOverwriteMode(w->sprite,2);ManagedSprite_SetAffineScale(w->sprite,1.2f,1.2f);*(volatile u32*)0x04000000=(*(volatile u32*)0x04000000&0xffff1fff)|0x8000;*(volatile u16*)0x0400004a=(*(volatile u16*)0x0400004a&~0x3f)|0x1b;*(volatile u16*)0x0400004a=(*(volatile u16*)0x0400004a&0xffffc0ff)|0x700;ov07_0221C410(w->ctx,ov07_0222519C,w);}
void ov07_0222525C(void*task,Flow*w){switch(w->state){case 0:ov07_02222268(w->movement,w->x,(s16)(w->x+w->dx),w->y,(s16)(w->y+w->dy),(u16)w->duration);w->state++;case 1:if(!ov07_022222B4(w->movement))w->state++;Pokepic_SetAttr(w->pic,0,w->movement[0]);Pokepic_SetAttr(w->pic,1,w->movement[1]);break;default:ov07_0221C448(w->ctx,task);Heap_Free(w);break;}}
