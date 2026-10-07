#include "global.h"
#include "filesystem.h"
#include "heap.h"
#pragma force_active on
typedef struct RenderModel {u32 loaded;NNSG3dResFileHeader *file;NNSG3dResMdlSet *set;NNSG3dResMdl *model;NNSG3dResTex *texture;} RenderModel;
typedef struct ModelState {VecFx32 position,previous,scale;u8 native24[24];s32 angle;u32 native40;s32 delta;u8 native48[4];s16 rotationX,rotationY,rotationZ;u16 native52;RenderModel model;NNSG3dRenderObj render;} ModelState;
extern const int ov85_021EA5C4[];
extern const s32 ov85_021EA5DC[];
void sub_020696C4(RenderModel *,int,NARC *,int,enum HeapID,BOOL);
void sub_02069714(RenderModel *);
/* Proposed finite canonical RenderModel view for actual render provider; whole provider proof required before credit. */
void sub_02069978(NNSG3dRenderObj *,void *);
void ov85_021E752C(void *);
void ov85_021E752C(void *object) {
    u8 *bytes=object;
    ModelState *state=(ModelState *)(bytes+0xd4);
    int *selection=*(void **)(bytes+0xcc);
    int index=selection[2];
    sub_020696C4(&state->model,0,*(void **)(bytes+0xd80),ov85_021EA5C4[index],(enum HeapID)102,FALSE);
    sub_02069714(&state->model);
    sub_02069978(&state->render,&state->model);
    state->angle=0;
    state->delta=ov85_021EA5DC[index];
    state->scale.x=FX32_ONE;state->scale.y=FX32_ONE;state->scale.z=FX32_ONE;
    state->rotationX=0;state->rotationY=0;state->rotationZ=0;
    state->position.x=0;state->position.y=-147456;state->position.z=0;
    state->previous=state->position;
}
