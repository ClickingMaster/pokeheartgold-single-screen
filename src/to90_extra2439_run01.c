#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))

void*ov85_021E85F0(void*,int),*sub_02034818(int);int ov85_021E86B0(void*,int);extern const u16 ov85_021EA788[][5];void PlayerName_FlatToString(void*,void*),Sound_SetSceneAndPlayBGM(int,int,int),ov85_021E72E8(void*);int ov85_021E5DAC(void*);
int ov85_021E5DAC(void*s){void*work=ov85_021E85F0(s,24);W(work,0x10)=W(s,0x2c);u32 row=W(s,0x30);W(work,0x14)=row;W(work,4)=ov85_021EA788[row][W(s,0x2c)];int i=0;u8*view=s;for(;i<8;i++,view+=4){if(ov85_021E86B0(s,i)){P(view,0x84)=sub_02034818(i);PlayerName_FlatToString(P(view,0x84),P(view,0x98));}}W(s,0x114)=W(work,4)<<12;W(s,0)=17;Sound_SetSceneAndPlayBGM(15,1155,1);ov85_021E72E8(s);return 1;}
