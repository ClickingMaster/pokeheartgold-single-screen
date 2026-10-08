#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8*script;void*render;u8 pad[196];u32 used[8];} State;
typedef struct {MtxFx43 pos;MtxFx33 vec;} Mix;
typedef struct {MtxFx44 clip;MtxFx33 vec;} Cache;
extern void NNS_G3dGeFlushBuffer(void);
extern void G3_MultMtx43(const MtxFx43*),G3_MultMtx33(const MtxFx33*),G3_LoadMtx43(const MtxFx43*);
extern int G3X_GetClipMtx(MtxFx44*),G3X_GetVectorMtx(MtxFx33*);
extern Cache _021DBB04[];extern MtxFx33 _021DBB44[];
static inline u32 BVCheck(const u32*vec,u32 idx){return vec[idx>>5]&(1<<(idx&31));}
static inline void BVSet(u32*vec,u32 idx){vec[idx>>5]|=1<<(idx&31);}
static inline BOOL BVCheckB(const u32*vec,u32 idx){return (vec[idx>>5]&(1<<(idx&31)))!=0;}
static inline BOOL BVCheckC(const u32*vec,u32 idx){return vec[idx>>5]&(1<<(idx&31))?TRUE:FALSE;}
void NNSi_G3dFuncSbc_NODEMIX(State*);
#define REG(o) (*(volatile u32*)(0x04000440+(o)))
// @fn NNSi_G3dFuncSbc_NODEMIX
void NNSi_G3dFuncSbc_NODEMIX(State*s){Mix*table;s64 previousWeight;u32 count;u8*tuple;Cache*cache;Mix sum;MtxFx33*previous;u32 i;{u8*model=*(u8**)((u8*)s->render+4);tuple=s->script;count=tuple[2];table=(Mix*)(model+*(u32*)(model+16));}previousWeight=0;tuple+=3;MIi_CpuClearFast(0,(u32*)&sum,84);NNS_G3dGeFlushBuffer();REG(0)=0;REG(12)=1;REG(20)=0;REG(0)=2;for(i=0;i<count;i++){u32 index;u32 present;u32 offset;index=tuple[1];present=BVCheck(s->used,index);cache=(Cache*)((u8*)_021DBB04+index*100);offset=index*100;if(!present){BVSet(s->used,index);REG(16)=tuple[0];REG(0)=1;G3_MultMtx43(&table[index].pos);}if(i){
sum.vec._00+=(previousWeight*previous->_00)>>12;
sum.vec._01+=(previousWeight*previous->_01)>>12;
sum.vec._02+=(previousWeight*previous->_02)>>12;
sum.vec._10+=(previousWeight*previous->_10)>>12;
sum.vec._11+=(previousWeight*previous->_11)>>12;
sum.vec._12+=(previousWeight*previous->_12)>>12;
sum.vec._20+=(previousWeight*previous->_20)>>12;
sum.vec._21+=(previousWeight*previous->_21)>>12;
sum.vec._22+=(previousWeight*previous->_22)>>12;
}if(!present){while(G3X_GetClipMtx(&cache->clip)){}REG(0)=2;G3_MultMtx33(&table[index].vec);} {fx32 weight=tuple[2]<<4;previousWeight=weight;
sum.pos._00+=((s64)weight*cache->clip._00)>>12;
sum.pos._01+=((s64)weight*cache->clip._01)>>12;
sum.pos._02+=((s64)weight*cache->clip._02)>>12;
sum.pos._10+=((s64)weight*cache->clip._10)>>12;
sum.pos._11+=((s64)weight*cache->clip._11)>>12;
sum.pos._12+=((s64)weight*cache->clip._12)>>12;
sum.pos._20+=((s64)weight*cache->clip._20)>>12;
sum.pos._21+=((s64)weight*cache->clip._21)>>12;
sum.pos._22+=((s64)weight*cache->clip._22)>>12;
sum.pos._30+=((s64)weight*cache->clip._30)>>12;
sum.pos._31+=((s64)weight*cache->clip._31)>>12;
sum.pos._32+=((s64)weight*cache->clip._32)>>12;
}tuple+=3;previous=(MtxFx33*)((u8*)_021DBB44+offset);if(!present){while(G3X_GetVectorMtx(previous)){}}}
sum.vec._00+=(previousWeight*previous->_00)>>12;
sum.vec._01+=(previousWeight*previous->_01)>>12;
sum.vec._02+=(previousWeight*previous->_02)>>12;
sum.vec._10+=(previousWeight*previous->_10)>>12;
sum.vec._11+=(previousWeight*previous->_11)>>12;
sum.vec._12+=(previousWeight*previous->_12)>>12;
sum.vec._20+=(previousWeight*previous->_20)>>12;
sum.vec._21+=(previousWeight*previous->_21)>>12;
sum.vec._22+=(previousWeight*previous->_22)>>12;
G3_LoadMtx43((MtxFx43*)&sum.vec);REG(0)=1;G3_LoadMtx43(&sum.pos);REG(0)=0;REG(16)=1;REG(0)=2;REG(12)=s->script[1];s->script+=3*(s->script[2]+1);}
