#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct Range {u32 head,tail;int active,compressed;u16 id,pad;u32 base;} Range;
extern Range *_02110934[2],*_0211093C[5];
extern Range _02110968,_02110980;
static inline BOOL IsAllocatable(Range *a,u32 n){return a->active && a->tail-a->head>=n;}
static inline BOOL IsAllocatableC(const Range *a,u32 n){return a->active && a->tail-a->head>=n;}
static inline Range *GetPltt(u16 id){switch(id){case 0:return &_02110968;case 3:return &_02110980;default:return NULL;}}
static inline BOOL Alloc4x4(u32 n,u32 *addr){int i;for(i=0;i<2;i++){Range *a=_02110934[i];if(IsAllocatable(a,n)){Range *b=GetPltt(a->id);if(IsAllocatable(b,n>>1)){u32 head=a->head;a->head=head+n;b->head+=n>>1;*addr=head+a->base;return TRUE;}}}return FALSE;}
static inline BOOL AllocNormal(u32 n,u32 *addr){int i;for(i=0;i<5;i++){Range *a=_0211093C[i];if(IsAllocatableC(a,n)){a->tail-=n;*addr=a->tail+a->base;return TRUE;}}return FALSE;}
u32 NNS_GfdAllocFrmTexVram(u32,BOOL,u32);
// @fn NNS_GfdAllocFrmTexVram
u32 NNS_GfdAllocFrmTexVram(u32 size,BOOL compressed,u32 options){
 u32 addr; BOOL ok; u32 bytes;
 if(size==0)bytes=16;else bytes=(size+15)&~15;
 if(bytes>=0x7fff0)return 0;
 if(compressed)ok=Alloc4x4(bytes,&addr);else ok=AllocNormal(bytes,&addr);
 if(!ok)return 0;
 return ((bytes>>4)<<16)|((addr<<13)>>16)|(compressed<<31);
}
