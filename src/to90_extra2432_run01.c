#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))

typedef struct {u32 enabled:1;u32 rest:31;} Flags;void ov27_0225A4D0(void*),ov27_0225A9C0(void*,int),Sprite_SetDrawFlag(void*,int),FillWindowPixelBuffer(void*,u8),CopyWindowToVram(void*);void ov27_0225A690(void*,int);
void ov27_0225A690(void*s,int on){if(on){((Flags*)((u8*)s+0x51c))->enabled=1;ov27_0225A4D0(s);ov27_0225A9C0(s,1);Sprite_SetDrawFlag(P(s,0x3c0),1);}else{((Flags*)((u8*)s+0x51c))->enabled=0;FillWindowPixelBuffer((u8*)s+0x3d0,0);CopyWindowToVram((u8*)s+0x3d0);Sprite_SetDrawFlag(P(s,0x3c0),0);Sprite_SetDrawFlag(P(s,0x3bc),0);Sprite_SetDrawFlag(P(s,0x3c4),0);ov27_0225A9C0(s,0);}}
