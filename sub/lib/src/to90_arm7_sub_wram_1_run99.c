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
extern s8 SinTable[];
s8 SND_SinIdx(int index);
// @fn SND_SinIdx
s8 SND_SinIdx(int index){if(index<32)return SinTable[index];if(index<64)return SinTable[64-index];if(index<96)return -SinTable[index-64];index-=96;index=32-index;return -SinTable[index];}
