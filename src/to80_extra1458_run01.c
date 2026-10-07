#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#include "renderer_joint_family_private.h"
typedef MdlJointResult Result;
typedef NNSG3dAnmObj Obj;
extern void VEC_CrossProduct(const VecFx32*,const VecFx32*,VecFx32*);
extern void blendScaleVec_(VecFx32*,const VecFx32*,fx32,int);
int NNSi_G3dAnmBlendJnt(Result*,const Obj*,u32);
static inline int usable(const Obj *p,u32 index) {return index<p->numMapData && (p->mapData[index]&0x300)==0x100;}
static inline int sample(Result *r,const Obj *p,u32 index) {if(p->funcAnm==0)return 0;((MdlJointEvaluator)p->funcAnm)(r,p,p->mapData[index]&255);return 1;}
static inline fx32 mulFx(fx32 a,fx32 b) {return (fx32)(((s64)a*b)>>12);}
int NNSi_G3dAnmBlendJnt(Result *result,const Obj *list,u32 index) {
 Result temp;VecFx32 first,third;const Obj *last,*p;fx32 total;int count,iteration;
 if(!list)return 0;
 if(!list->next) {
  if(index<list->numMapData) {
   u16 map=list->mapData[index];
   if((map&0x300)==0x100) {
    if(!list->funcAnm)return 0;
    ((MdlJointEvaluator)list->funcAnm)(result,list,map&255);return 1;
   }return 0;
  }return 0;
 }
 total=0;count=0;iteration=0;p=list;last=list;
 do {
  if(usable(p,index)) {
   if(p->ratio>0x1000)total+=0x1000;else if(p->ratio>0)total+=p->ratio;
   last=p;count++;
  }
 } while((p=p->next)!=0);
 if(total==0)return 0;
 if(count==1) {
  u16 map=last->mapData[index];
  if(!last->funcAnm)return 0;
  ((MdlJointEvaluator)last->funcAnm)(result,last,map&255);return 1;
 }
 MIi_CpuClearFast(0,(u32*)result,0x58);result->flags=~0;
 p=list; do {
  if(usable(p,index) && p->ratio>0 && p->funcAnm) {
   fx32 ratio;
   ((MdlJointEvaluator)p->funcAnm)(&temp,p,p->mapData[index]&255);
   if(iteration==0) {MIi_CpuCopy32((const u32*)&temp.rotVectors[0],(u32*)&first,12);MIi_CpuCopy32((const u32*)&temp.rotVectors[2],(u32*)&third,12);}
   if(total==0x1000)ratio=p->ratio;else ratio=FX_Div(p->ratio,total);
   blendScaleVec_(&result->scaleVector,&temp.scaleVector,ratio,temp.flags&1);
   blendScaleVec_(&result->scaleEx1,&temp.scaleEx1,ratio,temp.flags&8);
   blendScaleVec_(&result->scaleEx2,&temp.scaleEx2,ratio,temp.flags&16);
   if(!(temp.flags&4)) {
    result->transVector.x+=mulFx(ratio,temp.transVector.x);result->transVector.y+=mulFx(ratio,temp.transVector.y);result->transVector.z+=mulFx(ratio,temp.transVector.z);
   }
   if(!(temp.flags&2)) {
    result->rotVectors[0].x+=(ratio*temp.rotVectors[0].x)>>12;result->rotVectors[0].y+=(ratio*temp.rotVectors[0].y)>>12;result->rotVectors[0].z+=(ratio*temp.rotVectors[0].z)>>12;
    result->rotVectors[1].x+=(ratio*temp.rotVectors[1].x)>>12;result->rotVectors[1].y+=(ratio*temp.rotVectors[1].y)>>12;result->rotVectors[1].z+=(ratio*temp.rotVectors[1].z)>>12;
   } else {result->rotVectors[0].x+=ratio;result->rotVectors[1].y+=ratio;}
   result->flags&=temp.flags;
  }
 p=p->next;iteration++;
 } while(p);
 VEC_CrossProduct(&result->rotVectors[0],&result->rotVectors[1],&result->rotVectors[2]);
 if(result->rotVectors[0].x==0 && result->rotVectors[0].y==0 && result->rotVectors[0].z==0)MIi_CpuCopy32((const u32*)&first,(u32*)&result->rotVectors[0],12);
 else VEC_Normalize(&result->rotVectors[0],&result->rotVectors[0]);
 if(result->rotVectors[2].x==0 && result->rotVectors[2].y==0 && result->rotVectors[2].z==0)MIi_CpuCopy32((const u32*)&third,(u32*)&result->rotVectors[2],12);
 else VEC_Normalize(&result->rotVectors[2],&result->rotVectors[2]);
 VEC_CrossProduct(&result->rotVectors[2],&result->rotVectors[0],&result->rotVectors[1]);return 1;
}
