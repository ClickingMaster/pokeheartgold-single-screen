#pragma force_active on
#include "global.h"
typedef struct {s32 x,y,z;u8 count,index,timer,state;u8 props[6];} Env;
typedef struct {u8 a[0x34];void *resources;u8 b[0x1c];void *anims,*oneshot;u8 c[0x40];void *props;} Field;
extern Field *TaskManager_GetFieldSystem(void*);extern Env *TaskManager_GetEnvironment(void*);extern void **AreaDataManager_GetMapPropModel(int,void*);extern void *AreaDataManager_GetMapPropTexture(void*);extern void MapPropOneShotAnimationManager_LoadPropAnimations(void*,void*,int,int,void*,void*,void*,int,int,int);extern int sub_02054C20(void*,int,void**,void*);extern void *MapProp_GetRenderSurface(void*);extern int PlaySE(u16);extern void *MapPropManager_LoadOne(void*,int,VecFx32*,VecFx32*,void*);extern void *MapPropManager_GetMapPropByIndex_Checked_RequireActive(void*,int);extern void MapPropOneShotAnimationManager_SetAnimationRenderObj(void*,int,int,void*);extern void MapPropOneShotAnimationManager_PlayAnimation(void*,int,int);extern int PlayFanfare(u16);extern int MapPropOneShotAnimationManager_IsAnimationLoopFinished(void*,int);extern int IsFanfarePlaying(void);extern void MapPropOneShotAnimationManager_UnloadAnimation(void*,void*,int);extern void MapPropManager_RemoveMapPropByIndex(int,void*);extern void Heap_Free(void*);extern VecFx32 ov02_02253D90[],ov02_02253D94[],ov02_02253D98[];
static inline u8 *FirstEntry(u8 *dict,u32 index){if(dict!=NULL&&index<dict[1]){u8 *h=dict+*(u16*)(dict+6);return h+4;}return NULL;}
static inline u8 *FirstModel(u8 *set,u32 index){u8 *entry;if(set){entry=FirstEntry(set+8,index);if(entry)return set+*(u32*)entry;}return NULL;}
int PokecenterAnimRun(void*);
int PokecenterAnimRun(void *task){Field *f=TaskManager_GetFieldSystem(task);Env *e=TaskManager_GetEnvironment(task);void *prop;VecFx32 pos;u8 *a,*b;u8 i;
 switch(e->state){
 case 0:a=(u8*)AreaDataManager_GetMapPropModel(0x6b,f->resources);b=(u8*)AreaDataManager_GetMapPropModel(0x25,f->resources);
 a=FirstModel((u8*)NNS_G3dGetMdlSet(*(void**)a),0);b=FirstModel((u8*)NNS_G3dGetMdlSet(*(void**)b),0);
 MapPropOneShotAnimationManager_LoadPropAnimations(f->anims,f->oneshot,0x10,0x6b,NULL,a,AreaDataManager_GetMapPropTexture(f->resources),1,1,0);if(!sub_02054C20(f,0x25,&prop,NULL))GF_AssertFail();a=MapProp_GetRenderSurface(prop);MapPropOneShotAnimationManager_LoadPropAnimations(f->anims,f->oneshot,0x20,0x25,a,b,AreaDataManager_GetMapPropTexture(f->resources),1,1,0);e->state++;break;
 case 1:{VecFx32 zero={0};pos.x=e->x+ov02_02253D90[e->index].x;pos.y=e->y+ov02_02253D94[e->index].x;pos.z=e->z+ov02_02253D98[e->index].x;PlaySE(1533);e->props[e->index]=(u32)MapPropManager_LoadOne(f->props,0x6b,&pos,&zero,f->anims);e->state++;break;}
 case 2:if(e->timer<12)e->timer++;else{e->timer=0;e->index++;if(e->index<e->count)e->state=1;else{MapPropOneShotAnimationManager_SetAnimationRenderObj(f->oneshot,0x10,0,MapProp_GetRenderSurface(MapPropManager_GetMapPropByIndex_Checked_RequireActive(f->props,e->props[0])));e->state++;}}break;
 case 3:MapPropOneShotAnimationManager_PlayAnimation(f->oneshot,0x10,0);MapPropOneShotAnimationManager_PlayAnimation(f->oneshot,0x20,0);PlayFanfare(1183);e->state++;break;
 case 4:if(MapPropOneShotAnimationManager_IsAnimationLoopFinished(f->oneshot,0x10)&&MapPropOneShotAnimationManager_IsAnimationLoopFinished(f->oneshot,0x20)&&!IsFanfarePlaying()){MapPropOneShotAnimationManager_UnloadAnimation(f->anims,f->oneshot,0x20);MapPropOneShotAnimationManager_UnloadAnimation(f->anims,f->oneshot,0x10);for(i=0;i<e->count;i=(u8)(i+1))MapPropManager_RemoveMapPropByIndex(e->props[i],f->props);e->state++;}break;
 case 5:Heap_Free(e);return 1;}return 0;
}
