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
extern void TxEndKeyData(void *);
u32 CheckKeyTxEndMain(u8 *slot);
// @fn CheckKeyTxEndMain
u32 CheckKeyTxEndMain(u8 *slot){u32 result=0;if(R16(slot,0)==2){if(((R32(slot,8)&0x3fff)>>1|0x8000)!=H(0x04808094)){R16(slot,0)=1;result|=1;}}if(R16(slot,0)==1){if(R16(P(slot,8),0)&1){TxEndKeyData(slot);result|=2;}}return result;}
