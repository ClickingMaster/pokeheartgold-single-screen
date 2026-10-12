#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x34];void*maps;u8 pad2[0x1c];void*anim,*resources;}Field;
typedef struct{u32 a,b;}Pair;extern Pair ov02_022536E8;extern int sub_02054C90(Field*,void*,int,void**,int*);extern void*AreaDataManager_GetMapPropTexture(void*),*MapProp_GetRenderSurface(void*),*MapProp_GetResModel(void*);extern void MapPropOneShotAnimationManager_LoadPropAnimations(void*,void*,int,int,void*,void*,void*,int,int,int);
void ov02_0224BF58(Field*,int);
void ov02_0224BF58(Field*f,int id){void*prop;int model;Pair v=ov02_022536E8;if(sub_02054C90(f,&v,2,&prop,&model)){void*a=AreaDataManager_GetMapPropTexture(f->maps);void*b=MapProp_GetRenderSurface(prop);void*c=MapProp_GetResModel(prop);MapPropOneShotAnimationManager_LoadPropAnimations(f->anim,f->resources,id,model,b,c,a,2,1,0);}else GF_AssertFail();}
