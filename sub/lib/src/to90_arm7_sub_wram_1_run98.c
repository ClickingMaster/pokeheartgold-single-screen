#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern int sMasterPan,sSurroundDecay;
extern u8 sOrgPan[],sOrgVolume[];
extern int CalcSurroundDecay(int,int);
#define R8(c,o) (*(volatile u8 *)(0x04000000u+((u32)(c)<<4)+(o)))
#define R16(c,o) (*(volatile u16 *)(0x04000000u+((u32)(c)<<4)+(o)))
#define R32(c,o) (*(volatile u32 *)(0x04000000u+((u32)(c)<<4)+(o)))

void SND_SetMasterPan(int pan);
// @fn SND_SetMasterPan
void SND_SetMasterPan(int pan){int i;sMasterPan=pan;if(pan>=0){for(i=0;i<16;i++)R8(i,0x402)=pan;}else{for(i=0;i<16;i++)R8(i,0x402)=sOrgPan[i];}}
