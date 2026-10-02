#pragma force_active on
#include "global.h"
typedef struct {u32 enabled:1;u32 state:7;u32 ticks:24;} Flags;
typedef struct {u8 pad[0x84];void *sprite;u8 pad2[0xc4];Flags flags;} W;
extern void ManagedSprite_SetDrawFlag(void *,int),ManagedSprite_SetAnim(void *,int),ov112_021F1EFC(void *,int);
void ov112_021F24F8(W *);
void ov112_021F24F8(W *w){if(w->flags.enabled){switch(w->flags.state){case 0:ManagedSprite_SetDrawFlag(w->sprite,1);ManagedSprite_SetAnim(w->sprite,24);w->flags.state++;break;case 1:if(w->flags.ticks>=25){ov112_021F1EFC((u8*)w+0xac,0);w->flags.ticks=0;}else w->flags.ticks++;break;case 2:break;default:GF_AssertFail();break;}}}
