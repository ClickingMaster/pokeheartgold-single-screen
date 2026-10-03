#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 flags;u8 pad4[0x14];s32 x,y;u16 sin,cos;s32 a,b;} Result;
typedef struct {u32 flags,offset;} Value;
typedef struct {Value x,y,rotation,a,b;} Record;
extern s32 GetTexSRTAnmVectorVal_(const u8*,u32,u32,u32);extern u32 GetTexSRTAnmSinCosVal_(const u8*,u32,u32,u32);
static inline Record*findRecord(const u8*base,u32 idx){const u8*dict=base+8;if(dict&&idx<base[9]){const u8*entries=dict+*(const u16*)(base+14);return(Record*)(entries+4+*(const u16*)entries*idx);}return 0;}
void GetTexSRTAnm_(const u8*,u32,u32,Result*);
void GetTexSRTAnm_(const u8*base,u32 idx,u32 frame,Result*out){Record*record=findRecord(base,idx);u32 flags;int a,b;u32 rotation;flags=out->flags;a=GetTexSRTAnmVectorVal_(base,record->a.flags,record->a.offset,frame);b=GetTexSRTAnmVectorVal_(base,record->b.flags,record->b.offset,frame);if(a==0&&b==0)flags|=4;else{out->a=a;out->b=b;flags&=~4;}rotation=GetTexSRTAnmSinCosVal_(base,record->rotation.flags,record->rotation.offset,frame);if(rotation==0x10000000)flags|=2;else{out->sin=rotation;out->cos=rotation>>16;flags&=~2;}a=GetTexSRTAnmVectorVal_(base,record->x.flags,record->x.offset,frame);b=GetTexSRTAnmVectorVal_(base,record->y.flags,record->y.offset,frame);if(a==4096&&b==4096)flags|=1;else{out->x=a;out->y=b;flags&=~1;}out->flags=flags;}
