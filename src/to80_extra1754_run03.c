#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct{u32 flags,offset;} Track;
typedef struct{fx32 scale,inverse;} Scale;
typedef struct{s16 scale,inverse;} SmallScale;
typedef struct{u32 p;u16 frames,p6;u32 flags;} Header;
void getScaleDataEx_(Scale*,fx32,Track*,Header*);
void getScaleDataEx_(Scale*out,fx32 time,Track*track,Header*base){u8*data;u32 frame;frame=time>>12;data=(u8*)base+track->offset;u32 flags=track->flags;u32 next;int factor,shift;
 if(base->frames-1==(time>>12)){if(flags&0xC0000000){if(flags&0x40000000)frame=(frame&1)+(frame>>1);else frame=(frame&3)+(frame>>2);}if(base->flags&2)next=0;else{if(flags&0x20000000){SmallScale*p=(SmallScale*)data;out->scale=p[frame].scale;out->inverse=p[frame].inverse;}else{Scale*p=(Scale*)data;out->scale=p[frame].scale;out->inverse=p[frame].inverse;}return;}goto normal;}
 if(flags&0xC0000000){u32 last=(flags&0x1FFF0000)>>16;if(flags&0x40000000){if(frame>=last){frame=last>>1;next=frame+1;goto normal;}frame>>=1;next=frame+1;time&=0x1FFF;factor=2;shift=1;goto blend;}else{if(frame>=last){frame=(frame&3)+(frame>>2);next=frame+1;goto normal;}frame>>=2;next=frame+1;time&=0x3FFF;factor=4;shift=2;goto blend;}}
 next=frame+1;
 normal:time&=0xFFF;factor=1;shift=0;
 blend:{int a,b,c,d;if(flags&0x20000000){SmallScale*p=(SmallScale*)data;a=p[frame].scale;b=p[frame].inverse;c=p[next].scale;d=p[next].inverse;}else{Scale*p=(Scale*)data;a=p[frame].scale;b=p[frame].inverse;c=p[next].scale;d=p[next].inverse;}
 out->scale=(a*factor+((time*(c-a))>>12))>>shift;out->inverse=(b*factor+((time*(d-b))>>12))>>shift;}}
