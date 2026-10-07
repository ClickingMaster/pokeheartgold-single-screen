#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define WL (*(u8 **)0x0380fff4)
#define R16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define R32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define P(p,o) (*(u8 **)((u8 *)(p)+(o)))
extern int MatchMacAdrs(const void*,const void*);
u32 CAM_Search(u16 *address);
// @fn CAM_Search
u32 CAM_Search(u16 *address){u8 *work;u8 *entry;u32 index,seen;if(address[0]&1)return 0;work=WL;if(R16(work,0x52c)>1){entry=P(work,0x31c)+28;seen=0;for(index=1;index<R16(work,0x322);index++,entry+=28){if(R16(entry,0)){if(MatchMacAdrs(entry+4,address))return index;work=WL;seen++;if(seen>=R16(work,0x52c))break;}}}return 255;}
