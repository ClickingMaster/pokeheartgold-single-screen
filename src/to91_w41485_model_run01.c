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
void sub_020181B0(ModelInstance *,ModelResource *);
void sub_020182A8(ModelInstance *,fx32,fx32,fx32);
void sub_020182C4(ModelInstance *,fx32,fx32,fx32);
void sub_020182A0(ModelInstance *,BOOL);
void sub_02018198(ModelAnimation *,fx32);
void sub_020181D4(ModelInstance *,ModelAnimation *);
BOOL PlaySE(u16);
int ov93_0225DF38(void *,u32);
#pragma force_active off
static inline int InitializeSlot(State *s,int i,int type,ModelResource *resource){ModelInstance *instance=&s->slots[i][type].instance;ModelAnimation *animation;sub_020181B0(instance,resource);sub_020182A8(instance,0,-102400,0);sub_020182C4(instance,4096,4096,4096);sub_020182A0(instance,TRUE);animation=&s->slots[i][type].animation;sub_02018198(animation,0);sub_020181D4(instance,animation);s->slots[i][type].used=1;PlaySE(1425);return 1;}
#pragma force_active on
int ov93_0225DF38(void *raw,u32 payload){Owner *owner=raw;s32 value=(s32)payload;State *s=&owner->models;int type,i;if(value>1200)type=3;else if(value>2)type=2;else if(value>1)type=1;else type=0;ModelResource *resource=&s->resources[type];for(i=0;i<8;i++)if(s->slots[i][type].used==0)return InitializeSlot(s,i,type,resource);return 0;}
