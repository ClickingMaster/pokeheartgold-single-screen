#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u32 flags,offset;} Track;
typedef struct {u32 x;u16 frames,pad;u32 flags;} Anim;
void getTransDataEx_(s32*,s32,const Track*,const Anim*);
void getTransDataEx_(s32*out,s32 frame,const Track*track,const Anim*anim){
 const u8*data;u32 index,offset;offset=track->offset;index=frame>>12;data=(const u8*)anim+offset;u32 flags=track->flags;int factor,shift;u32 last;s32 a,b;
 if(anim->frames-1==(frame>>12)){
 if(flags&0xc0000000){if(flags&0x40000000)index=(index&1)+(index>>1);else index=(index&3)+(index>>2);}
 if(anim->flags&2){u32 frac=frame&0xfff;s32 start,first;if(flags&0x20000000){start=((s16*)data)[index];first=((s16*)data)[0];}else{start=((s32*)data)[index];first=((s32*)data)[0];}*out=start+((s32)(frac*(first-start))>>12);}
 else {s32 value;if(flags&0x20000000)value=((s16*)data)[index];else value=((s32*)data)[index];*out=value;}return;
 }
 if(flags&0xc0000000){last=(flags&0x1fff0000)>>16;if(flags&0x40000000){if(index>=last){index=last>>1;goto normal;}index>>=1;frame&=0x1fff;factor=2;shift=1;goto interpolate;}
 if(index>=last){index=(index&3)+(index>>2);goto normal;}index>>=2;frame&=0x3fff;factor=4;shift=2;goto interpolate;
 }
 normal:frame&=0xfff;factor=1;shift=0;
 interpolate:if(flags&0x20000000){a=((s16*)data)[index];b=((s16*)data)[index+1];}else{a=((s32*)data)[index];b=((s32*)data)[index+1];}*out=(a*factor+((frame*(b-a))>>12))>>shift;
}
