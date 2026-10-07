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
void SND_StopChannel(int c,int flags);
typedef struct {volatile u32 control,source;volatile u16 timer,loop;volatile u32 length;} W40870Channel;
typedef struct {u8 prefix[0x400];W40870Channel channels[16];} W40870Sound;
extern u8 sOrgPan[16],sOrgVolume[16];
void SND_SetChannelVolume(int c,int volume,int shift);
// @fn SND_StopChannel
void SND_StopChannel(int c,int flags){u32 offset=(u32)c<<4;W40870Sound *input=(W40870Sound*)(0x04000000u+offset);u32 value=input->channels[0].control&~0x80000000u;volatile u32 *output=(volatile u32*)(offset+0x400u+0x04000000u);if(flags&1)value|=0x8000;*output=value;}
// @fn SND_SetChannelVolume
void SND_SetChannelVolume(int c,int volume,int shift){int decay=sSurroundDecay;sOrgVolume[c]=volume;if(decay>0&&(0xfff5u&(1u<<c))){int pan=R8(c,0x402);volume=CalcSurroundDecay(volume,pan);}R16(c,0x400)=(u32)volume|((u32)shift<<8);}
