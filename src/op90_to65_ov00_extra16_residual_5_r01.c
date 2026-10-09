// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct ResetPrefix {
 u8 gap000[0xc];
 u8 f00c;
 u8 f00d;
 u8 f00e;
 u8 gap00f[0x5];
 u8 f014;
 u8 f015;
 u8 f016;
 u8 f017;
 u8 f018;
 u8 gap019[0x7];
 u32 f020;
 u32 f024[32];
 u16 f0a4[32];
 u8 gap0e4[0x4];
 u32 f0e8;
 u8 gap0ec[0x8];
 u32 f0f4[32];
 u8 gap174[0x8];
 u8 f17c;
 u8 gap17d[0x1];
 u16 f17e;
 u32 f180;
 u32 f184;
 u32 f188;
 u32 f18c;
 u32 f190;
 u32 f194[3];
 u8 gap1a0[0x4];
 u8 f1a4;
 u8 f1a5;
 u8 f1a6;
 u8 f1a7;
 u8 f1a8;
 u8 f1a9;
 u8 f1aa;
 u8 f1ab;
 u8 f1ac;
 u8 f1ad;
 u8 f1ae;
 u8 gap1af[0x1];
 u16 f1b0;
 u16 f1b2;
 u16 f1b4;
 u8 gap1b6[0x2];
 u32 f1b8;
 u32 f1bc;
 u32 f1c0;
 u32 f1c4;
 u32 f1c8;
 u32 f1cc;
 u32 f1d0;
 u32 f1d4;
 u8 gap1d8[0x8];
 u32 f1e0;
 u32 f1e4;
 u8 gap1e8[0x8];
 u32 f1f0;
 u32 f1f4;
 u32 f1f8;
 u32 f1fc;
 u8 gap200[0x4];
 u32 f204;
 u32 f208;
 u32 f20c;
 u32 f210[32];
 u16 f290[32];
 u8 f2d0[32];
 u32 f2f0;
 u8 gap2f4[0x54];
 u32 f348[33];
 u32 f3cc[38];
 u8 gap464[0x8];
 u32 f46c;
 u32 f470;
} ResetPrefix;
extern u8 *ov00_021F955C(void);
static inline ResetPrefix *GetReset(void){return (ResetPrefix *)ov00_021F955C();}
extern u32 ov00_021ED5E4(u32);
extern void ov00_021F956C(int);
void ov00_021F381C(int);
#define BYTE(w,o) (*(u8*)((u8*)(w)+(o)))
#define HALF(w,o) (*(u16*)((u8*)(w)+(o)))
#define WORD(w,o) (*(u32*)((u8*)(w)+(o)))
static inline void AssignHalfword(u16 *field,u16 value){*field=value;}
// @fn ov00_021F381C
void ov00_021F381C(int mode){
 u8 *p;
 BYTE(ov00_021F955C(),0xc)=0;BYTE(ov00_021F955C(),0x17c)=0;
 p=ov00_021F955C();HALF(p,0x17e)=ov00_021ED5E4(0x10000);
 WORD(ov00_021F955C(),0x180)=0;
 p=ov00_021F955C();WORD(p,0x184)=0;WORD(p,0x188)=0;
 p=ov00_021F955C();WORD(p,0x18c)=0;WORD(p,0x190)=0;
 BYTE(ov00_021F955C(),0x1a4)=0;
 BYTE(ov00_021F955C(),0x1a9)=0;
 BYTE(ov00_021F955C(),0x1aa)=0;
 BYTE(ov00_021F955C(),0x1ab)=0;
 BYTE(ov00_021F955C(),0x1ac)=0;
 BYTE(ov00_021F955C(),0x1a7)=0;
 BYTE(ov00_021F955C(),0x1a8)=0;
 HALF(ov00_021F955C(),0x1b2)=0;
 HALF(ov00_021F955C(),0x1b4)=0;
 WORD(ov00_021F955C(),0x1b8)=0;
 p=ov00_021F955C();WORD(p,0x1e0)=0;WORD(p,0x1e4)=0;
 p=ov00_021F955C();WORD(p,0x1f0)=0;WORD(p,0x1f4)=0;
 MIi_CpuClear32(0,(u32*)(ov00_021F955C()+0x3cc),0x98);
 if(mode==2){p=ov00_021F955C();BYTE(p,0x14)=BYTE(ov00_021F955C(),0xd);
  if(BYTE(ov00_021F955C(),0x15)==3)ov00_021F956C(1);else if(BYTE(ov00_021F955C(),0x15)==2)ov00_021F956C(10);
  return;
 }
 BYTE(ov00_021F955C(),0xd)=0;
 BYTE(ov00_021F955C(),0xe)=0;
 BYTE(ov00_021F955C(),0x14)=0;
 BYTE(ov00_021F955C(),0x17)=0;
 WORD(ov00_021F955C(),0x20)=0;
 WORD(ov00_021F955C(),0xe8)=0;
 BYTE(ov00_021F955C(),0x1a5)=0;
 HALF(ov00_021F955C(),0x1b0)=0;
 WORD(ov00_021F955C(),0x1bc)=0;
 p=ov00_021F955C();WORD(p,0x1c0)=0;WORD(p,0x1c4)=0;
 WORD(ov00_021F955C(),0x1c8)=0;
 p=ov00_021F955C();WORD(p,0x1cc)=0;WORD(p,0x1d0)=0;
 WORD(ov00_021F955C(),0x1d4)=0;
 WORD(ov00_021F955C(),0x204)=0;
 WORD(ov00_021F955C(),0x208)=0;
 WORD(ov00_021F955C(),0x2f0)=0;
 MIi_CpuClear32(0,(u32*)(ov00_021F955C()+0x24),128);
 MIi_CpuClear16(0,(u16*)(ov00_021F955C()+0xa4),64);
 MIi_CpuClear32(0,(u32*)(ov00_021F955C()+0xf4),128);
 MIi_CpuClear32(0,(u32*)(ov00_021F955C()+0x194),12);
 MIi_CpuClear32(0,(u32*)(ov00_021F955C()+0x210),128);
 MIi_CpuClear16(0,(u16*)(ov00_021F955C()+0x290),64);
 MI_CpuFill8(ov00_021F955C()+0x2d0,0,32);MIi_CpuClear32(0,(u32*)(ov00_021F955C()+0x348),0x84);
 if(mode==1){if(BYTE(ov00_021F955C(),0x15)==0)ov00_021F956C(3);else if(BYTE(ov00_021F955C(),0x15)==1)ov00_021F956C(4);return;}
 BYTE(ov00_021F955C(),0x15)=0;
 BYTE(ov00_021F955C(),0x16)=0;
 BYTE(ov00_021F955C(),0x18)=0;
 WORD(ov00_021F955C(),0x20c)=0;
 BYTE(ov00_021F955C(),0x1a6)=0;
 BYTE(ov00_021F955C(),0x1ae)=0;
 BYTE(ov00_021F955C(),0x1ad)=0;
 p=ov00_021F955C();WORD(p,0x1f8)=0;WORD(p,0x1fc)=0;
 WORD(ov00_021F955C(),0x46c)=0;
 WORD(ov00_021F955C(),0x470)=0;
}
