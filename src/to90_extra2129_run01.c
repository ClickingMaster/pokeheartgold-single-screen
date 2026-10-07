#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u32 flags;fx32 translation[3],scale[3],other[3];union{fx32 matrix[12];struct{fx32 rotation[9],inverse[3];}parts;}rotation;}Joint;
void NNSi_G3dSendJointSRTSi3d(Joint*);
void NNSi_G3dSendJointSRTSi3d(Joint*j){fx32 scale[3];int combined=0;u32 mask=j->flags&24;if(!mask)NNS_G3dGeBufferOP_N(27,(u32*)j->other,3);if(!(j->flags&4)){if(mask)combined=1;else{scale[0]=(fx32)(((s64)j->rotation.parts.inverse[0]*j->scale[0])>>12);scale[1]=(fx32)(((s64)j->rotation.parts.inverse[1]*j->scale[1])>>12);scale[2]=(fx32)(((s64)j->rotation.parts.inverse[2]*j->scale[2])>>12);NNS_G3dGeBufferOP_N(28,(u32*)scale,3);}}if(!(j->flags&2)){if(combined)NNS_G3dGeBufferOP_N(25,(u32*)j->rotation.matrix,12);else NNS_G3dGeBufferOP_N(26,(u32*)j->rotation.matrix,9);}else if(combined)NNS_G3dGeBufferOP_N(28,(u32*)j->rotation.parts.inverse,3);if(!mask)NNS_G3dGeBufferOP_N(27,(u32*)j->scale,3);if(!(j->flags&1))NNS_G3dGeBufferOP_N(27,(u32*)j->translation,3);}
