#pragma force_active on
#include "global.h"
#include "sound.h"
#pragma force_active on
extern void PlaySE(u16);
typedef struct Pair{s16 x,z;} Pair;
typedef struct Shot{u16 angle,pad;VecFx32 velocity;fx32 magnitude;VecFx32 pos;u32 reserved,flag;} Shot;
extern int ov91_0225EE48(void *,Pair *),ov91_0225EE64(void *,Pair *),ov91_0225EE88(void *);
extern void ov91_02260400(s16,s16,VecFx32 *),ov91_0226045C(VecFx32 *,VecFx32 *,u16,u32);
extern void ov91_0225E2E4(void *,Shot *,int),ov91_02260E88(void *,s16,s16),ov91_0225EE9C(void *);
void ov91_0225EA98(u8 *);
#define W(x) (*(u32 *)(p+(x)))
void ov91_0225EA98(u8 *p){
 Pair last,first;Shot shot;int flag=0,count;fx32 mag;
 if(!W(0x20)){W(0x48)++;return;}
 if(!ov91_0225EE48(p+0x20,&last))GF_AssertFail();
 if(!ov91_0225EE64(p+0x20,&first))GF_AssertFail();
 count=ov91_0225EE88(p+0x20);
 if(count>=2){
  shot.angle=W(0x14);shot.pad=0;
  shot.velocity.x=(s16)((last.x-first.x)/count)<<12;
  shot.velocity.z=(s16)((last.z-first.z)/count)<<12;shot.velocity.y=0;
  mag=VEC_Mag(&shot.velocity);
  if(mag<0x8000){if(mag){VEC_Normalize(&shot.velocity,&shot.velocity);shot.velocity.x=(s64)(shot.velocity.x)*0x8000+0x800>>12;shot.velocity.z=(s64)(shot.velocity.z)*0x8000+0x800>>12;}else flag=1;}
  else if(mag>0x1e200){VEC_Normalize(&shot.velocity,&shot.velocity);shot.velocity.x=((s64)shot.velocity.x*0x1e200+0x800)>>12;shot.velocity.z=((s64)shot.velocity.z*0x1e200+0x800)>>12;}
  if(!flag){shot.velocity.y=0x26000;shot.magnitude=VEC_Mag(&shot.velocity);shot.magnitude=((s64)shot.magnitude*0x99a+0x800)>>12;VEC_Normalize(&shot.velocity,&shot.velocity);
   ov91_02260400(first.x,first.z,&shot.pos);shot.pos.y=0;
   ov91_0226045C(&shot.pos,&shot.pos,shot.angle,W(0x10));ov91_0226045C(&shot.velocity,&shot.velocity,shot.angle,W(0x10));
   if((s32)W(0x1c)>=5)shot.flag=1;else shot.flag=0;
   ov91_0225E2E4(p,&shot,1);PlaySE(0x64f);
  }
 }else flag=1;
 if(flag==1)ov91_02260E88(p+0x854c,first.x,first.z);
 ov91_0225EE9C(p);W(0x48)=0;
}
