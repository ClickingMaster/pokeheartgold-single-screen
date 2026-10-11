#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 active,type,index,pad;u8 object[0x78];void *anim[4];int frames[4];} Actor;
typedef struct {int x;void *mdl;int y;void *tex;} Model;
typedef struct {u8 data[20];} Animation;
typedef struct {Model models[2];void *resources[2][3];Animation animations[2][4];} Resources;
extern void NNS_G3dReleaseMdlSet(NNSG3dResMdlSet *);
extern void GF_AssertFail(void);
extern int WiFiPlazaMap_IsModelInstanceInView(Model *,void *);extern void ModelInstance_AddAnimation(void *,Animation *),ModelAnimation_SetFrame(Animation *,int),ModelInstance_Draw(void *),ModelInstance_RemoveAnimation(void *,Animation *);
void ov49_0225DAFC(Actor *,Resources *);
void ov49_0225DAFC(Actor *a,Resources *r){int i;if(a->active&&WiFiPlazaMap_IsModelInstanceInView(&r->models[a->index],a->object)){if(a->type>=3)GF_AssertFail();if(a->index>=2)GF_AssertFail();r->models[a->index].tex=NNS_G3dGetTex(r->resources[a->index][a->type]);if(!NNS_G3dBindMdlSet(r->models[a->index].mdl,r->models[a->index].tex))GF_AssertFail();for(i=0;i<4;i++){if(a->anim[i]){ModelInstance_AddAnimation(a->object,&r->animations[a->index][i]);ModelAnimation_SetFrame(&r->animations[a->index][i],a->frames[i]);}}ModelInstance_Draw(a->object);for(i=0;i<4;i++){if(a->anim[i])ModelInstance_RemoveAnimation(a->object,&r->animations[a->index][i]);}NNS_G3dReleaseMdlSet(r->models[a->index].mdl);}}
