#pragma force_active on
#include "global.h"
#pragma force_active on
void Thunk_G3X_Reset(void),Camera_SetStaticPtr(void *),Camera_ApplyPerspectiveType(int,void *),Camera_PushLookAtToNNSGlb(void);

void sub_020181EC(void *),ov89_0225A5A4(void *);
typedef struct Tail{u8 pad[0x7c];u32 flags;}Tail;
void AddWindowParameterized(void *,void *,int,u8,u8,int,int,int,u16),FillWindowPixelBuffer(void *,int);
extern const u8 ov89_0225C9CC[];
void ov89_02259C0C(u8 *),ov89_02259CD0(u8 *);
void ov89_02259C0C(u8 *w){VecFx32 scale,trans;MtxFx33 m;u32 op;
 scale.x=4096;scale.y=4096;scale.z=4096;trans.x=0;trans.y=0;trans.z=0;MTX_Identity33_(&m);Thunk_G3X_Reset();
 Camera_SetStaticPtr(*(void **)(w+204));Camera_ApplyPerspectiveType(0,*(void **)(w+204));Camera_PushLookAtToNNSGlb();
 NNS_G3dGlbLightVector(GX_LIGHTID_0,0,-4096,0);NNS_G3dGlbLightColor(GX_LIGHTID_0,0x739c);NNS_G3dGlbMaterialColorDiffAmb(32767,32767,0);NNS_G3dGlbMaterialColorSpecEmi(32767,32767,0);
 NNS_G3dGlbSetBaseTrans(&trans);NNS_G3dGlbSetBaseRot(&m);NNS_G3dGlbSetBaseScale(&scale);
 NNS_G3dGeBufferOP_N(17,0,0);sub_020181EC(w+232);ov89_0225A5A4(w+404);op=1;NNS_G3dGeBufferOP_N(18,&op,1);
}
