#include "global.h"
#include "particle_system_internal.h"
#include "math_util.h"
#pragma force_active on
#ifndef W19_PARTICLE_MOTION_OWNER_H
#define W19_PARTICLE_MOTION_OWNER_H
#include "global.h"
#include "library/spl_emitter.h"
#include "overlay07_script_slot_private.h"
typedef struct W02LinearMotion { s16 x,y; int duration,dx,dy,curX,curY; } W02LinearMotion;
typedef struct W02SinMotion { s16 x,y; int duration,angleX,ampX,angleY,ampY,stepX,stepY; } W02SinMotion;
typedef struct W02ScalarMotion { int value,duration,step,current; } W02ScalarMotion;
typedef struct Ov07LinearSlot { W02LinearMotion motion; u8 reserved18[12]; } Ov07LinearSlot;
typedef struct Ov07SinSlot { W02SinMotion motion; u8 reserved20[4]; } Ov07SinSlot;
typedef struct Ov07ScalarSlot { W02ScalarMotion motion; u8 reserved10[4]; } Ov07ScalarSlot;
typedef struct Ov07Work104 {
 s32 arg0; s16 delay; u16 arg6; s32 arg8; s16 argC,argE; s32 arg10;
 u32 handleA,handleB; s32 unk1C,unk20,unk24,counter,angle,wave;
 void *resource; SPLEmitter *node; Ov07T10Work header;
 u8 reserved58[0x50]; Ov07LinearSlot interpA; Ov07SinSlot interpB; Ov07ScalarSlot interpC;
} Ov07Work104;
typedef char Ov07Work104Size[(sizeof(Ov07Work104)==0x104)?1:-1];
#endif

extern BOOL ov07_022260DC(void *),ov07_022222B4(W02LinearMotion *);
extern void ParticleSystem_DeleteEmitter(ParticleSystem *,SPLEmitter *),BattleAnimSystem_DestroyTask(void *,void *),BattleAnimUtil_FreeWork(void *);
void ov07_022260F8(void *,void *);
static inline void PlaceEmitterX(SPLEmitter *emitter,fx32 displacement){emitter->emtr_pos.x=displacement+emitter->p_res->p_base->pos.x;}
static inline void PlaceEmitterY(SPLEmitter *emitter,fx32 displacement){emitter->emtr_pos.y=displacement+emitter->p_res->p_base->pos.y;}
void ov07_022260F8(void *task,void *data){Ov07Work104 *work=data;BOOL busy=ov07_022260DC(work->node);if(work->counter<work->delay){work->counter++;return;}if(!ov07_022222B4(&work->interpA.motion)&&!busy){ParticleSystem_DeleteEmitter(work->resource,work->node);BattleAnimSystem_DestroyTask(work->header.owner,task);BattleAnimUtil_FreeWork(work);return;}if((work->unk20<=work->unk1C&&work->unk24>=work->unk1C)||work->unk24==255){PlaceEmitterX(work->node,work->interpA.motion.x*172);PlaceEmitterY(work->node,work->interpA.motion.y*172);if(work->wave){work->angle+=360/(s16)work->arg6;PlaceEmitterY(work->node,work->interpA.motion.y*172+GF_SinDeg((u16)work->angle));}}}
