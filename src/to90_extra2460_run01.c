#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Data {u16 count,pad;s16 ratio;u16 offset;} Data;
typedef struct Frame {u16 time,value;} Frame;
Data *NNSi_G3dGetTexPatAnmDataByIdx(void*,int);
Frame *NNSi_G3dGetTexPatAnmFV(void*,int,u32);
#pragma force_active on
Frame *NNSi_G3dGetTexPatAnmFV(void *base,int index,u32 time){Data *d=NNSi_G3dGetTexPatAnmDataByIdx(base,index);u32 frame;Frame *frames=(Frame*)((u8*)base+d->offset);frame=((u32)(d->ratio*time))>>12;while(frame && frames[frame].time>=time)frame--;while(frame+1<d->count && frames[frame+1].time<=time)frame++;return &frames[frame];}
