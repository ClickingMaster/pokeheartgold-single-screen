#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct { s32 x,y,z; } Triple;
extern u8 *_021DA700; extern u8 _021DB510[],_021DB514[],_021DB518[];
void NNSi_G3dGetJointScaleMaya(u32 *,const s32 *,const u8 *,u32);
void NNSi_G3dGetJointScaleMaya(u32 *result,const s32 *scale,const u8 *node,u32 flags) {
 u8 kind=node[3];
 if(flags&4) {
 result[0]|=1;
 if(kind&2) { u32 index=node[1]; u32 *bits=(u32*)(_021DA700+0xc4); bits[index>>5]|=1<<(index&31); }
 } else {
 result[1]=scale[0];result[2]=scale[1];result[3]=scale[2];
 if(kind&2) {
 u32 index=node[1]; u32 *bits=(u32*)(_021DA700+0xc4); int offset=(s16)index*(s16)24;
 bits[index>>5]&=~(1<<(index&31));
 *(s32*)(_021DB510+offset)=scale[3]; *(s32*)(_021DB514+offset)=scale[4]; *(s32*)(_021DB518+offset)=scale[5];
 }
 }
 if(kind&1) {
 u32 index=node[2]; result[0]|=32;
 if(((u32*)(_021DA700+0xc4))[index>>5]&(1<<(index&31))) result[0]|=8;
 else *(Triple*)(result+4)=*(Triple*)(_021DB510+index*24);
 }
 result[0]|=16;
}
