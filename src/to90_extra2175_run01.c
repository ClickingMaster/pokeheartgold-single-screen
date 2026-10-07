#pragma force_active on
#include "global.h"
#include "gf_3d_render.h"
#pragma force_active on
typedef struct {u8 model[0x800];u8 rendering[0x60];void*data;int enabled;void*arg;} Block;
typedef struct {u8 p[0x90];Block*blocks[4];u8 p2[0x18];void*object;u32 pad;int height,width;} Work;
extern const VecFx32 ov01_02206BD8;
extern const MtxFx33 ov01_02206BE4;
extern void ov01_021F5FB8(void*,int,int);
extern void* ov01_021FBA00(void*);
extern void ov01_021F3A3C(VecFx32*,void*,void*,void*,void*);
void MapLoadManager_RenderLoadedMap(int,Work*,void*);
void MapLoadManager_RenderLoadedMap(int index,Work*w,void*arg){VecFx32 zero={0,0,0};VecFx32 scale;MtxFx33 matrix;Block*b;volatile void* volatile savedArg=arg;ov01_021F5FB8(w->blocks[index]->data,w->width,w->height);if(w->blocks[index]->enabled==1){scale=ov01_02206BD8;matrix=ov01_02206BE4;GF3dRender_DrawModel((NNSG3dRenderObj*)w->blocks[index]->rendering,&zero,&matrix,&scale);}b=w->blocks[index];if(b->enabled==1){void*data=ov01_021FBA00(w->object);ov01_021F3A3C(&zero,w->object,data,(void*)savedArg,b->arg);}}
