#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x5c];void *model;u8 pad2[8];VecFx32 position,scale;u16 x,y,z;} Work;
extern void Camera_PushLookAtToNNSGlb(void);extern int GF3d_ModelBoxTest(void *,VecFx32 *,MtxFx33 *,VecFx32 *);
int ov71_02247738(Work *);
int ov71_02247738(Work *w){MtxFx33 matrix,rot;int one,result;MTX_Identity33(&matrix);MTX_RotX33(&rot,FX_SinIdx(w->x),FX_CosIdx(w->x));MTX_Concat33(&rot,&matrix,&matrix);MTX_RotY33(&rot,FX_SinIdx(w->y),FX_CosIdx(w->y));MTX_Concat33(&rot,&matrix,&matrix);MTX_RotZ33(&rot,FX_SinIdx(w->z),FX_CosIdx(w->z));MTX_Concat33(&rot,&matrix,&matrix);NNS_G3dGeBufferOP_N(17,NULL,0);Camera_PushLookAtToNNSGlb();result=GF3d_ModelBoxTest(w->model,&w->position,&matrix,&w->scale);one=1;NNS_G3dGeBufferOP_N(18,(u32 *)&one,1);return result;}
