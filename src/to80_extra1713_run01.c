#pragma force_active on
#include "global.h"
typedef struct {u8 a[4];u8 *data;u8 b[0x4c];void *sprites;} Render;
typedef VecFx32 V;extern V ov92_02263B68;
extern void Thunk_G3X_Reset(void);extern void Camera_PushLookAtToNNSGlb(void);
extern void ov92_0225D970(void*,void*,void*);extern int ModelAnimation_AdvanceLooped(void*,int);extern int ModelAnimation_Advance(void*,int);extern void ModelInstance_SetVisible(void*,int);extern void RequestSwap3DBuffers(int,int);extern void SpriteSystem_DrawSprites(void*);
void ov92_0225DA40(Render*);
void ov92_0225DA40(Render *r) {
 V light;int i;u8 *obj;u32 p6,p5,p4,p3,p2,p1;int a,b;
 Thunk_G3X_Reset();Camera_PushLookAtToNNSGlb();light=ov92_02263B68;VEC_Normalize(&light,&light);NNS_G3dGlbLightVector(GX_LIGHTID_0,(s16)light.x,(s16)light.y,(s16)light.z);NNS_G3dGlbLightColor(GX_LIGHTID_0,32767);NNS_G3dGlbMaterialColorDiffAmb(32767,32767,0);NNS_G3dGlbMaterialColorSpecEmi(32767,32767,0);
 for(i=0;i<8;i++){obj=r->data+0xb50+i*0x20c;NNS_G3dGeBufferOP_N(0x11,NULL,0);ov92_0225D970(obj,obj+0x1b0,obj+0x1e0);p6=1;NNS_G3dGeBufferOP_N(0x12,&p6,1);ModelAnimation_AdvanceLooped(r->data+0xbd8+i*0x20c,4096);}
 switch(*(int*)(r->data+0x110)){
 case 0:ModelInstance_SetVisible(r->data+0x320,1);ModelInstance_SetVisible(r->data+0x52c,0);ModelInstance_SetVisible(r->data+0x738,0);ModelAnimation_AdvanceLooped(r->data+0x3a8+(*(int*)(r->data+0x514))*20,4096);break;
 case 1:ModelInstance_SetVisible(r->data+0x320,0);ModelInstance_SetVisible(r->data+0x52c,1);ModelInstance_SetVisible(r->data+0x738,0);ModelAnimation_Advance(r->data+0x5b4,4096);break;
 case 2:ModelInstance_SetVisible(r->data+0x320,0);ModelInstance_SetVisible(r->data+0x52c,0);ModelInstance_SetVisible(r->data+0x738,1);ModelAnimation_Advance(r->data+0x7c0,4096);break;}
 obj=r->data+0x320;i=(*(int*)(r->data+0x110))*0x20c;NNS_G3dGeBufferOP_N(0x11,NULL,0);ov92_0225D970(obj+i,obj+0x1b0,obj+0x1e0);p5=1;NNS_G3dGeBufferOP_N(0x12,&p5,1);
 obj=r->data+0x114;NNS_G3dGeBufferOP_N(0x11,NULL,0);ov92_0225D970(obj,obj+0x1b0,obj+0x1e0);p4=1;NNS_G3dGeBufferOP_N(0x12,&p4,1);
 obj=r->data+0x944;NNS_G3dGeBufferOP_N(0x11,NULL,0);ov92_0225D970(obj,obj+0x1b0,obj+0x1e0);p3=1;NNS_G3dGeBufferOP_N(0x12,&p3,1);
 if(*(int*)(r->data+0x1fac)) {obj=r->data+0x1dbc;NNS_G3dGeBufferOP_N(0x11,NULL,0);ov92_0225D970(obj,obj+0x1b0,obj+0x1e0);p2=1;NNS_G3dGeBufferOP_N(0x12,&p2,1);if(*(int*)(r->data+0x1fa8)){a=ModelAnimation_Advance(r->data+0x1e44,4096);b=ModelAnimation_Advance(r->data+0x1e58,4096);if(a&&b)*(int*)(r->data+0x1fa8)=0;}}
 if(*(int*)(r->data+0x1da0)) {obj=r->data+0x1bb0;NNS_G3dGeBufferOP_N(0x11,NULL,0);ov92_0225D970(obj,obj+0x1b0,obj+0x1e0);p1=1;NNS_G3dGeBufferOP_N(0x12,&p1,1);if(*(int*)(r->data+0x1d9c))ModelAnimation_AdvanceLooped(r->data+0x1c4c,4096);}
 RequestSwap3DBuffers(0,1);SpriteSystem_DrawSprites(r->sprites);
}
