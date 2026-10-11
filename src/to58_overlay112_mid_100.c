#include "to58_overlay112_mid_private.h"
typedef struct { u8 pad[0x1f2d6]; u16 blend; } Ov112Blend;
extern void ManagedSprite_SetOamMode(void *, int);
extern void G2x_SetBlendAlpha_(u32, int, int, int, int);
extern void *SysTask_CreateOnMainQueue(void *, void *, int);
extern void SysTask_Destroy(void *);
void ov112_021ED0D0(Ov112State *);
void ov112_021ED13C(void *task, Ov112Blend *p);
void ov112_021ED0C8(Ov112State *p) { ov112_021ED0D0(p); }

void ov112_021ED0D0(Ov112State *p) {
    PokewalkerApp_LoadMonPicture((void *)W(p,0x1e430),(u8 *)p+0x1d7ac,(u8 *)p+0x1d79c,0);
    ManagedSprite_SetOamMode((void *)W(p,0x1e550),1);
    ov112_021EA670(p,8);
    H(p,0x1f2d6)=0;
    G2x_SetBlendAlpha_(0x04000050,0,8,0,0x1f);
    W(p,0x1f2d8)=(u32)SysTask_CreateOnMainQueue(ov112_021ED13C,p,2);
}

void ov112_021ED13C(void *task, Ov112Blend *p) {
    p->blend += 2;
    if (p->blend >= 0x1f) p->blend = 0x1f;
    G2x_SetBlendAlpha_(0x04000050,0,8,p->blend,0x1f-p->blend);
    if (p->blend == 0x1f) SysTask_Destroy(task);
}
