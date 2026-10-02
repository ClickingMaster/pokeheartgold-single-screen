#pragma force_active on
#include "global.h"
#include "system.h"
typedef struct {u32 state;u16*result;u8 pad[0x214-8];u32 arg;u8 p[0x388-0x218];void*sprite;u8 q[8];u32 index;u8 r[4];void(*callback)(void*,u32);void*userdata;}Work;
extern void ov27_0225C618(Work*),Sprite_SetDrawFlag(void*,int),ov27_0225CD18(Work*,u32,u32);
int ov27_0225C9F8(void),ov27_0225CA14(Work*);
int ov27_0225C9F8(void){if(gSystem.touchHeld)gSystem.simulatedInputs=1;return 0;}
int ov27_0225CA14(Work*w){w->state=8;w->index=0;ov27_0225C618(w);Sprite_SetDrawFlag(w->sprite,1);ov27_0225CD18(w,w->arg,w->index);if(w->callback)w->callback(w->userdata,w->index);*w->result=0xeeee;return 0;}
