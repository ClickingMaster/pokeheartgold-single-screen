#pragma force_active on
#include "global.h"
#pragma force_active on
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(u8**)((u8*)(p)+(o)))
void ov07_0221C7B8(u8*,u8*,u32);
void ov07_0221C7B8(u8*w,u8*out,u32 mode){s32 i=0,word=i;u8*p=out;s32 half=i;u8*q=out;for(;i<4;i++){U(p,8)=U(V(w,0xc0),0xb0+word);U(p,0x18)=U(V(w,0xc0),0xc4+word);H(q,0x28)=H(V(w,0xc0),0xd8+half);q+=2;B(out+i,0x30)=B(V(w,0xc0)+i,0xe0);B(out+i,0x34)=B(V(w,0xc0)+i,0xe4);B(out+i,0x38)=B(V(w,0xc0)+i,0xe8);U(p,0x3c)=U(V(w,0xc0),0xec+word);word+=4;p+=4;B(out+i,0x4c)=B(V(w,0xc0)+i,0xc0);half+=2;}switch(mode){case 0:U(out,0)=H(V(w,0xc0),0x14);U(out,4)=H(V(w,0xc0),0x16);break;case 1:U(out,0)=H(V(w,0xc0),0x14);U(out,4)=H(V(w,0xc0),0x14);break;case 2:U(out,0)=H(V(w,0xc0),0x14);U(out,4)=H(V(w,0xc0),0x14);break;case 3:U(out,0)=H(V(w,0xc0),0x14);U(out,4)=H(V(w,0xc0),0x14);break;default:GF_AssertFail();break;}}
