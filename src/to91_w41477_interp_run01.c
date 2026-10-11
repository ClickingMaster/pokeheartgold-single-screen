#include "global.h"
#include "gf_3d_render.h"
#pragma force_active on
typedef struct Ov49Interpolation {
    s32 duration;
    s32 x;
    s32 y;
    s32 z;
    s32 deltaX;
    s32 startX;
    s32 deltaY;
    s32 startY;
    s32 deltaZ;
    s32 startZ;
} Ov49Interpolation;

typedef struct Ov49Ctl {
    s16 state;
    u16 kind;
    u16 count;
    u8 active;
    u8 phase;
    Ov49Interpolation emitters[4];
    struct {
        u8 raw[0xC];
    } motions[4];
    void *objects[4];
    void *callbackArg;
} Ov49Ctl;

typedef struct ModelInstance {
    NNSG3dRenderObj render;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[3];
} ModelInstance;

extern BOOL ov49_02265434(Ov49Interpolation *,s32);
extern void ov49_022655E0(void *,u32 *,u32 *,u32 *);
extern void ModelInstance_SetPosition(ModelInstance *,fx32,fx32,fx32);
extern void ov49_022683FC(Ov49Ctl *,u32,VecFx32 *,fx32,fx32,fx32,u32,u32);
u32 ov49_02268334(void *);
#pragma opt_propagation off
u32 ov49_02268334(void *data){
 Ov49Ctl *ctl=data;
 struct {u32 completed;VecFx32 position;} output;
 int i=0;int count=ctl->count;
 if(count>0){
  Ov49Interpolation *interpolation=ctl->emitters;
  u8 *positionBytes=(u8*)ctl+8;
  u8 *objectBytes=(u8*)ctl;
  do {
   output.completed=ov49_02265434(interpolation,ctl->state);
   ov49_022655E0(positionBytes,(u32*)&output.position.x,(u32*)&output.position.y,(u32*)&output.position.z);
   ModelInstance_SetPosition(*(void**)(objectBytes+0xd8),output.position.x,output.position.y,output.position.z);
   if(output.completed==1){
    if(ctl->phase==0)ov49_022683FC(ctl,i,&output.position,-0x3000,0x4000,-0x3000,0x3000,3);
    else ov49_022683FC(ctl,i,&output.position,0xa000,0x5000,0x6000,0xa000,4);
   }
   i++;interpolation++;positionBytes+=40;objectBytes+=4;
  }while(i<ctl->count);
 }
 if(output.completed==1){int phase=ctl->phase+1;if(phase<3){ctl->phase=phase;ctl->state=0;}else return 1;}
 return 0;
}
