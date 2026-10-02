#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
extern u8 _021D84E4[], _021D84E8[], _021D84EC[];
extern BOOL NNSi_GfdFreeLnkVram(void *,void *,u32,u32);
extern int NNS_GfdFreeLnkTexVram(u32);
struct VramRange { u32 bytes,normal,compressed; };
struct VramRanges { struct VramRange ranges[4]; };
extern struct VramRanges _021092A0;
extern void NNSi_GfdInitLnkVramMan(void *);
extern void *NNSi_GfdInitLnkVramBlockPool(void *,u32);
extern void NNSi_GfdAddNewFreeBlock(void *,void *,u32,u32);
extern void NNSi_GfdMergeAllFreeBlocks(void *,void *);
extern void NNS_GfdResetLnkTexVramState(void);
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
int NNS_GfdFreeLnkTexVram(u32 key) { u32 addr=(key&0xffff)<<3; u32 size=((key&0x7fff0000)>>16)<<4; u32 compressed=(key&0x80000000)>>31; BOOL ok; if(size!=0) { if(compressed) ok=NNSi_GfdFreeLnkVram(_021D84E8,_021D84EC,addr,size); else ok=NNSi_GfdFreeLnkVram(_021D84E4,_021D84EC,addr,size); if(ok) return 0; else return 1; } return 2; }
