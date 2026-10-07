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

void WlIntrTxErr(void);
typedef struct {u16 low:4;u16 enabled:1;u16 high:11;} W35510Flags;
// @fn WlIntrTxErr
void WlIntrTxErr(void){u8 *work;H(0x04808010)=8;if(((W35510Flags *)(WL+0x33a))->enabled){if(!(H(0x0480819c)&1))H(0x04808290)^=1;}work=WL;if(R16(work,0x690)&8){u32 i=0;u8 *stats=work+0x344;u8 *slots=work+0x42c;for(;i<3;i++){u8 *entry=slots+i*20;if(R16(entry,0)){u8 *frame=P(entry,8);if((R16(frame,0xc)&0x4000)&&(R16(frame,4)&255)){u16 *tail=(u16 *)(((u32)((u32)(frame+0xc)+R16(frame,0xa)-7))&~1u);if(tail[0]==0&&tail[1]==0){R16(frame,4)=0;H(0x04808032)=0;H(0x04808032)=0x8000;R16(stats,0xba)++;}}}}}}
