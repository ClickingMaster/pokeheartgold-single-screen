#pragma force_active on
#include "global.h"
extern u16 ov91_02261D64[][4],ov91_02261C0A[],ov91_02261BF8[],ov91_02261BFE[];
extern void ModelResource_LoadFromNarc(void *,void *,int,int),ModelInstance_Init(void *,void *),ModelInstance_SetPosition(void *,int,int,int),ModelInstance_SetScale(void *,int,int,int),ModelInstance_SetVisible(void *,int),ModelInstance_SetRotation(void *,int,int);
extern int GF_DegreeToSinCosIdxNoWrap(int);
extern void ModelAnimation_LoadFromNarc(void *,void *,void *,int,int,int),ModelInstance_AddAnimation(void *,void *);
void ov91_02260F50(u8 *,int,void *,int,int,int,volatile int);
void ov91_02260F50(u8 *w,int unused,void *resource,int kind,int sel,int heap,volatile int flag) {
 int i,id;u8 *cur,*data;flag=flag;data=w+0x168;cur=w;
 for(i=0;i<3;i++,data+=16,cur+=120) {
  if(i==0)id=36;else if(i==1)id=37;else id=ov91_02261C0A[kind-1];
  ModelResource_LoadFromNarc(data,resource,id,heap);ModelInstance_Init(cur,data);ModelInstance_SetPosition(cur,0,-0x62000,0);ModelInstance_SetScale(cur,0x1800,0x1800,0x1800);
  if(i==1)ModelInstance_SetVisible(cur,0);
  if((u32)i<=1)ModelInstance_SetRotation(cur,ov91_02261D64[kind-1][sel],1);
  else ModelInstance_SetRotation(cur,GF_DegreeToSinCosIdxNoWrap(180),1);
 }
 for(i=0,cur=w+0x198;i<3;i++,cur+=20) {
  ModelAnimation_LoadFromNarc(cur,w+0x168+ov91_02261BFE[i]*16,resource,ov91_02261BF8[i],heap,flag);
  ModelInstance_AddAnimation(w+ov91_02261BFE[i]*120,cur);
 }
 *(u32*)(w+0x1d4)=4096;
}
