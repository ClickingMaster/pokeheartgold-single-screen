#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u16 kind;u8 pad[0x42];} AnimCell;
typedef struct {u8 p[0x100];union {AnimCell cells[16];struct {u8 pad[0x100];u16 timer,goal;u32 unused;u32 enabled[16];u8 rest[0xa93-0x248];u8 active;u32 frame;} f;} u;} Work;
extern Work *ov13_0224F5C8;
static inline u16 *cellkind(Work*w,int i){return (u16*)((u8*)w+0x100+i*0x44);}
void ov13_0223F568(void);
void ov13_0223F568(void){int i,stride=0x44;for(i=0;i<16;i++){Work*w=ov13_0224F5C8;if(w->u.f.enabled[i]){u32 offset=(u32)i*(u32)stride;u16*kind=(u16*)((u32)w+0x100+offset);if(i==1){if(w->u.f.active==1){if(*kind!=16)return;w->u.f.frame++;if(!(ov13_0224F5C8->u.f.frame&1)){ov13_0224F5C8->u.f.timer++;if(ov13_0224F5C8->u.f.timer>=36)ov13_0224F5C8->u.f.timer=0;}}else{w->u.f.goal=188;if(*kind==189){ov13_0224F5C8->u.f.active=1;ov13_0224F5C8->u.f.timer=0;ov13_0224F5C8->u.f.frame=0;}}}}}}
