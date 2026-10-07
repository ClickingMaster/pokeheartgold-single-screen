#pragma force_active on
// clang-format off
#include "global.h"
#pragma force_active on
#include <nitro/code32.h>
// clang-format on
typedef struct {int engine;u16 start,end,current,affStart,affEnd,affCurrent,owner,pad;u32 fast;} Manager;
typedef struct {u16 owners[128];u8 oam[0x400];u16 affine[32];} Bank;
extern Bank _021D8518[];extern u16 _021D8514;
static inline int Available(const u16 *base,u16 start,u16 end){const u16 *p=base+start,*q=base+end;for(;p<=q;p++){if(*p!=0xffff)return 0;}return 1;}
int NNS_G2dGetNewOamManagerInstance(Manager*,u16,u16,u16,u16,int);
static inline int AffAvailable(const u16 *base,u32 start,u32 end){const u16 *p=base+start,*q=base+end;for(;p<=q;p++){if(*p!=0xffff)return 0;}return 1;}
int NNS_G2dGetNewOamManagerInstance(Manager *w,u16 start,u16 count,u16 affine,u16 acount,int engine){Bank *b=&_021D8518[engine];u16 end=start+(count-1);
if(Available(b->owners,start,end)){w->owner=_021D8514++;w->start=start;w->end=end;w->current=start;MIi_CpuClear16(w->owner,&b->owners[start],(end-start+1)*2);}else return 0;
u16 *ao=b->affine;
if(acount==0){w->affStart=32;w->affEnd=0;w->affCurrent=w->affStart;}else {u16 affEnd=affine+(acount-1);if(AffAvailable(ao,affine,affEnd)){w->affStart=affine;w->affEnd=affEnd;w->affCurrent=affine;MIi_CpuClear16(w->owner,&ao[affine],(affEnd-affine+1)*2);}else return 0;}
w->fast=0;w->engine=engine;return 1;}
