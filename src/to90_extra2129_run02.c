#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#include "renderer_joint_family_private.h"
#pragma force_active on
#pragma opt_lifetimes off
#pragma opt_pointer_analysis off
void NNSi_G3dGetJointScaleSi3d(MdlJointResult *out,const s32 *scale,const u8 *cmd,u32 flag) {
 u32 current=cmd[1],parent=cmd[2];
 if(flag&4) {
 out->flags |=1;
 if(_021DA700->scaleFlags[parent>>5]&(1u<<(parent&31))) {
 _021DA700->scaleFlags[current>>5] |=1u<<(current&31);out->flags |=0x18;
 } else {
 MIi_CpuCopy32((const u32 *)((u8 *)&_021DB504+parent*24),(u32 *)((u8 *)&_021DB504+current*24),24);
 MIi_CpuCopy32((const u32 *)((u8 *)&_021DB504+parent*24),(u32 *)out->inverse,24);
 }
 } else {
 out->scale[0]=scale[0];out->scale[1]=scale[1];out->scale[2]=scale[2];
 if(_021DA700->scaleFlags[parent>>5]&(1u<<(parent&31))) {
 MIi_CpuCopy32((const u32 *)scale,(u32 *)((u8 *)&_021DB504+current*24),24);
 _021DA700->scaleFlags[current>>5] &=~(1u<<(current&31));out->flags |=0x18;
 } else {
 _021DA700->scaleFlags[current>>5] &=~(1u<<(current&31));
 MdlCache504Vol[current*6]=(s64)scale[0]*MdlCache504Vol[parent*6]>>12;
 MdlCache508Vol[current*6]=(s64)scale[1]*MdlCache508Vol[parent*6]>>12;
 MdlCache50CVol[current*6]=(s64)scale[2]*MdlCache50CVol[parent*6]>>12;
 MdlCache510Vol[current*6]=(s64)scale[3]*MdlCache510Vol[parent*6]>>12;
 MdlCache514Vol[current*6]=(s64)scale[4]*MdlCache514Vol[parent*6]>>12;
 MdlCache518Vol[current*6]=(s64)scale[5]*MdlCache518Vol[parent*6]>>12;
 MIi_CpuCopy32((const u32 *)((u8 *)&_021DB504+parent*24),(u32 *)out->inverse,24);
 }
 }
}
