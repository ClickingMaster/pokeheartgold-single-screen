#pragma force_active on
#include "global.h"
#include "gf_3d_render.h"
typedef struct ModelResource {
    NNSG3dResFileHeader *file;
    NNSG3dResMdlSet *set;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
} ModelResource;
typedef struct ModelAnimation {
    void *file;
    void *resource;
    NNSG3dAnmObj *object;
    fx32 frame;
    BOOL borrowed;
} ModelAnimation;
typedef struct ModelInstance {
    NNSG3dRenderObj render;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[3];
} ModelInstance;

#pragma force_active on
typedef struct Slot {u32 used;ModelInstance instance;ModelAnimation animation;} Slot;
typedef struct State {ModelResource primaryResource;ModelInstance primaryInstance;ModelResource resources[4];Slot slots[8][4];} State;
typedef struct Owner {u8 prefix[0xd8];State models;u8 tail[0x3850-0x13a0];} Owner;
typedef char OwnerSize[sizeof(Owner)==0x3850?1:-1];
typedef char SlotSize[sizeof(Slot)==144?1:-1];
typedef char InstanceSize[sizeof(ModelInstance)==120?1:-1];
typedef char SlotBase[offsetof(State,slots)==0xc8?1:-1];
void ModelInstance_Init(ModelInstance *,ModelResource *);
void ModelInstance_SetPosition(ModelInstance *,fx32,fx32,fx32);
void ModelInstance_SetScale(ModelInstance *,fx32,fx32,fx32);
void ModelInstance_SetVisible(ModelInstance *,BOOL);
void ModelAnimation_SetFrame(ModelAnimation *,fx32);
void ModelInstance_AddAnimation(ModelInstance *,ModelAnimation *);
BOOL PlaySE(u16);
int ov93_0225DF38(void *,u32);
#pragma force_active off
static inline int InitializeSlot(State *s,int i,int type,ModelResource *resource){ModelInstance *instance=&s->slots[i][type].instance;ModelAnimation *animation;ModelInstance_Init(instance,resource);ModelInstance_SetPosition(instance,0,-102400,0);ModelInstance_SetScale(instance,4096,4096,4096);ModelInstance_SetVisible(instance,TRUE);animation=&s->slots[i][type].animation;ModelAnimation_SetFrame(animation,0);ModelInstance_AddAnimation(instance,animation);s->slots[i][type].used=1;PlaySE(1425);return 1;}
#pragma force_active on
int ov93_0225DF38(void *raw,u32 payload){Owner *owner=raw;s32 value=(s32)payload;State *s=&owner->models;int type,i;if(value>1200)type=3;else if(value>2)type=2;else if(value>1)type=1;else type=0;ModelResource *resource=&s->resources[type];for(i=0;i<8;i++)if(s->slots[i][type].used==0)return InitializeSlot(s,i,type,resource);return 0;}
