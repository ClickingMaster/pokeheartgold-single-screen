#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
struct Range {u32 head,tail; int active,compressed; u16 id,pad; u32 base;};
extern u16 _021D84C0[];
extern struct Range _02110950[],_02110968,_02110980;
extern struct Range *_02110934[],*_0211093C[];
void NNS_GfdResetFrmTexVramState(void);
u32 NNS_GfdAllocFrmTexVram(u32,BOOL,u32);
void NNS_GfdResetFrmTexVramState(void) { int i=0; struct Range *r=_02110950; u32 limit=_021D84C0[0]; if(limit>1) limit++; for(i=0;i<5;i++,r++) { if(i<(int)limit) r->active=1;else r->active=0; if(r->compressed) {r->head=0;r->tail=0x10000;}else {r->head=0;r->tail=0x20000;} } }
