#pragma force_active on
// clang-format off
#include "global.h"
#pragma force_active on
#include <nitro/code32.h>
// clang-format on
typedef union{u32 raw;struct{u32 low:16,last:13,pad:1,half:1,scaled:1;}bits;} Flags;
typedef struct{Flags flags;u32 offset;} Track;
typedef struct{VecFx32 row[3];} Matrix;
typedef struct{u32 p;u16 frames,p6;u32 flags,small,large;} Header;
int getRotDataByIdx_(Matrix*,s16*,s16*,u32);
void getRotData_(Matrix*,fx32,Track*,Header*);
#define CROSS() {fx32 x,y,z;x=(s32)((u32)out->row[0].y*(u32)out->row[1].z-(u32)out->row[0].z*(u32)out->row[1].y)>>12;y=(s32)((u32)out->row[0].z*(u32)out->row[1].x-(u32)out->row[0].x*(u32)out->row[1].z)>>12;z=(s32)((u32)out->row[0].x*(u32)out->row[1].y-(u32)out->row[0].y*(u32)out->row[1].x)>>12;out->row[2].x=x;out->row[2].y=y;out->row[2].z=z;}
void getRotData_(Matrix*out,fx32 time,Track*track,Header*base){Matrix weighted,average;u32 large,small;u16*indices;u32 rawFlags,flags;register int bits;register u32 frame;rawFlags=track->flags.raw;frame=time>>12;indices=(u16*)((u8*)base+track->offset);small=base->small;large=base->large;
if(rawFlags&0xC0000000){u32 last=(rawFlags&0x1FFF0000)>>16;if(rawFlags&0x40000000){if(frame&1){if(frame>last){frame=(last>>1)+1;goto direct;}flags=frame>>1;goto avg;}frame>>=1;goto direct;}else{u32 rest=frame&3;if(rest){if(frame>last){frame=rest+(last>>2);goto direct;}if(frame&1){u32 a;bits=0;if(frame&2){flags=frame>>2;a=flags+1;}else{a=frame>>2;flags=a+1;}bits|=getRotDataByIdx_(out,(s16*)((u8*)base+small),(s16*)((u8*)base+large),indices[a]);bits|=getRotDataByIdx_(&weighted,(s16*)((u8*)base+small),(s16*)((u8*)base+large),indices[flags]);
out->row[0].x=(s32)(3U*(u32)out->row[0].x+(u32)weighted.row[0].x);
out->row[0].y=(s32)(3U*(u32)out->row[0].y+(u32)weighted.row[0].y);
out->row[0].z=(s32)(3U*(u32)out->row[0].z+(u32)weighted.row[0].z);
out->row[1].x=(s32)(3U*(u32)out->row[1].x+(u32)weighted.row[1].x);
out->row[1].y=(s32)(3U*(u32)out->row[1].y+(u32)weighted.row[1].y);
out->row[1].z=(s32)(3U*(u32)out->row[1].z+(u32)weighted.row[1].z);
VEC_Normalize(&out->row[0],&out->row[0]);VEC_Normalize(&out->row[1],&out->row[1]);if(!bits){
out->row[2].x=(s32)(3U*(u32)out->row[2].x+(u32)weighted.row[2].x);
out->row[2].y=(s32)(3U*(u32)out->row[2].y+(u32)weighted.row[2].y);
out->row[2].z=(s32)(3U*(u32)out->row[2].z+(u32)weighted.row[2].z);
VEC_Normalize(&out->row[2],&out->row[2]);return;}CROSS();return;}flags=frame>>2;goto avg;}frame>>=2;goto direct;}}
goto direct;
avg:bits=0;bits|=getRotDataByIdx_(out,(s16*)((u8*)base+small),(s16*)((u8*)base+large),indices[flags]);bits|=getRotDataByIdx_(&average,(s16*)((u8*)base+small),(s16*)((u8*)base+large),indices[flags+1]);
out->row[0].x=(s32)((u32)out->row[0].x+(u32)average.row[0].x);
out->row[0].y=(s32)((u32)out->row[0].y+(u32)average.row[0].y);
out->row[0].z=(s32)((u32)out->row[0].z+(u32)average.row[0].z);
out->row[1].x=(s32)((u32)out->row[1].x+(u32)average.row[1].x);
out->row[1].y=(s32)((u32)out->row[1].y+(u32)average.row[1].y);
out->row[1].z=(s32)((u32)out->row[1].z+(u32)average.row[1].z);
VEC_Normalize(&out->row[0],&out->row[0]);VEC_Normalize(&out->row[1],&out->row[1]);if(!bits){
out->row[2].x=(s32)((u32)out->row[2].x+(u32)average.row[2].x);
out->row[2].y=(s32)((u32)out->row[2].y+(u32)average.row[2].y);
out->row[2].z=(s32)((u32)out->row[2].z+(u32)average.row[2].z);
VEC_Normalize(&out->row[2],&out->row[2]);return;}CROSS();return;direct:bits=getRotDataByIdx_(out,(s16*)((u8*)base+small),(s16*)((u8*)base+large),indices[frame]);if(bits){CROSS();}else VEC_Normalize(&out->row[2],&out->row[2]);return;
}
