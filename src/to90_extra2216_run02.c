#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct {u32 values[12];} Descriptor;extern const u16 ov70_022464F0[];
void ov70_02238B54(void*,void*,void*,int);void*Sprite_CreateAffine(void*);void Sprite_SetAnimActiveFlag(void*,int);void Sprite_SetAnimCtrlSeq(void*,int);void Sprite_SetPriority(void*,int);void Sprite_SetOamMode(void*,int);void Sprite_SetDrawFlag(void*,int);void sub_0203A880(void);
void ov70_0223B9C8(u8*w);
void ov70_0223B9C8(u8*w){Descriptor descriptor;ov70_02238B54(&descriptor,w,w+0xd60,1);descriptor.values[2]=ov70_022464F0[7]<<12;descriptor.values[3]=ov70_022464F0[8]<<12;P(w,0xdcc)=Sprite_CreateAffine(&descriptor);Sprite_SetAnimActiveFlag(P(w,0xdcc),1);Sprite_SetAnimCtrlSeq(P(w,0xdcc),45);Sprite_SetPriority(P(w,0xdcc),1);Sprite_SetOamMode(P(w,0xdcc),1);
 descriptor.values[2]=10<<16;descriptor.values[3]=2<<16;P(w,0xdd0)=Sprite_CreateAffine(&descriptor);Sprite_SetAnimCtrlSeq(P(w,0xdd0),47);Sprite_SetDrawFlag(P(w,0xdd0),0);
 descriptor.values[2]=57<<14;descriptor.values[3]=117<<12;P(w,0xf04)=Sprite_CreateAffine(&descriptor);Sprite_SetAnimCtrlSeq(P(w,0xf04),38);Sprite_SetDrawFlag(P(w,0xf04),0);
 descriptor.values[2]=35<<14;P(w,0xf08)=Sprite_CreateAffine(&descriptor);Sprite_SetAnimCtrlSeq(P(w,0xf08),39);Sprite_SetDrawFlag(P(w,0xf08),0);sub_0203A880();
}
