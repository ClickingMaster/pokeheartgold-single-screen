#include "global.h"
#pragma force_active on

#include "overlay89_r8_private.h"
typedef struct ModelResource {NNSG3dResFileHeader *file;NNSG3dResMdlSet *set;NNSG3dResMdl *model;NNSG3dResTex *texture;} ModelResource;
typedef ModelInstance Model;
typedef struct Effect {Model others[3];u16 counter;u8 state,pad;int output[4][4];} Effect;
typedef struct Work {u8 a[12];ModelResource resource;Model primary;Effect effect;} Work;
void sub_020182B0(ModelInstance *,fx32 *,fx32 *,fx32 *);
void sub_020181B0(ModelInstance *,ModelResource *);
void sub_020182A8(ModelInstance *,fx32,fx32,fx32);
void ov89_0225ADA4(void *,void *,int *,int);
int ov89_0225B164(Ov89Work *,Ov89Object *);
typedef char ModelSize[sizeof(Model)==120?1:-1];
typedef char ResourceSize[sizeof(ModelResource)==16?1:-1];
typedef char EffectOffset[offsetof(Work,effect)==0x94?1:-1];
typedef char OutputOffset[(offsetof(Work,effect)+offsetof(Effect,output))==0x200?1:-1];
typedef char ViewSize[sizeof(Work)==0x240?1:-1];
int ov89_0225B164(Ov89Work *unused,Ov89Object *raw){Work *w=(Work *)raw;fx32 x,y,z;int i;Effect *effect=&w->effect;Model *others=effect->others;switch(effect->state){case 0:sub_020182B0(&w->primary,&x,&y,&z);for(i=0;i<3;i++){sub_020181B0(&others[i],&w->resource);sub_020182A8(&others[i],x,y,z);}effect->state++;case 1:
{sub_020182B0(&w->primary,&x,&y,&z);sub_020182A8(&w->primary,(s32)((u32)x-8192),(s32)((u32)y-8192),z);sub_020182B0(&others[0],&x,&y,&z);sub_020182A8(&others[0],(s32)((u32)x+8192),(s32)((u32)y-8192),z);sub_020182B0(&others[1],&x,&y,&z);sub_020182A8(&others[1],(s32)((u32)x-8192),(s32)((u32)y+8192),z);sub_020182B0(&others[2],&x,&y,&z);sub_020182A8(&others[2],(s32)((u32)x+8192),(s32)((u32)y+8192),z);effect->counter++;if(effect->counter>30)return 1;}break;default:break;}ov89_0225ADA4(&w->primary,w,effect->output[0],0);for(i=0;i<3;i++,others++)ov89_0225ADA4(others,w,effect->output[i+1],0);return 0;}
