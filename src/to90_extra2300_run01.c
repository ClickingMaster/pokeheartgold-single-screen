#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov112_021E7DA4(void*),ov112_021E7594(void*,void*,void*,void*),ov112_021E7CC8(void*),ov112_021E9290(void*,void*,void*,int),ManagedSprite_SetOamMode(void*,int),ov112_021EA670(void*,int),ManagedSprite_SetAnim(void*,int),ManagedSprite_SetAnimationFrame(void*,int),ov112_021EA5A4(void*,int),ov112_021ED25C(void*);
int ov112_021EDA4C(void*);
int ov112_021EDA4C(void*s){ov112_021E7DA4(s);ov112_021E7594((u8*)s+0xc910,(u8*)s+0xf1d0,(u8*)s+0x9d70,(u8*)s+0xaabc);ov112_021E7CC8(s);if(*(void**)((u8*)s+0x1e42c))ov112_021E9290(*(void**)((u8*)s+0x1e42c),(u8*)s+0x1d7ac,(u8*)s+0x1d79c,0);else ov112_021E9290(*(void**)((u8*)s+0x1e430),(u8*)s+0x1d7ac,(u8*)s+0x1d79c,0);ManagedSprite_SetOamMode(*(void**)((u8*)s+0x1e550),1);ov112_021EA670(s,8);ManagedSprite_SetAnim(*(void**)((u8*)s+0x1e550),25);ManagedSprite_SetAnimationFrame(*(void**)((u8*)s+0x1e550),0);*(u16*)((u8*)s+0x1f2d4)=0;ov112_021EA5A4(s,2);ov112_021ED25C(s);return 31;}
