#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct{ s16 x,y;u8 id,anim;u16 frame:13,special:1,draw:1,ready:1;} Record;
void*sub_02096878(void*);int ManagedSprite_GetActiveAnim(void*);int ManagedSprite_GetAnimationFrame(void*);int ov80_02239734(void*,u16);int ManagedSprite_GetDrawFlag(void*);void ManagedSprite_GetPositionXY(void*,s16*,s16*);
void ov80_02239740(u8*w);
void ov80_02239740(u8*w){u8*dest=sub_02096878(P(w,8));u8*src=w+0x3c;int i;
 for(i=0;i<8;i++){u16 n=H(src+2*i,0x34);if(n!=0xffff){H(dest,2*i)=n;i++;}}
 u8*ids;s16*y=(s16*)dest;s16*x;u8*flags;y+=9;x=(s16*)dest;flags=dest;i=0;flags+=22;ids=src;x+=8;
 for(;i<8;i++,ids+=2,y+=4,src+=4,x+=4,dest+=8,flags+=8){
 if(P(src,0)){B(dest,0x15)=ManagedSprite_GetActiveAnim(P(src,0));H(flags,0)=(H(flags,0)&~0x1fff)|(ManagedSprite_GetAnimationFrame(P(src,0))&0x1fff);H(flags,0)=(H(flags,0)&~0x2000)|(((u16)ov80_02239734(w,(u16)i)&1)<<13);H(flags,0)=(H(flags,0)&~0x4000)|(((u16)ManagedSprite_GetDrawFlag(P(src,0))&1)<<14);B(dest,0x14)=H(ids,0x20);ManagedSprite_GetPositionXY(P(src,0),x,y);H(flags,0)|=0x8000;}
 }
}
