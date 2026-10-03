#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 p[0x18];void *animation;int(*blend)(int*,void*,u32);u8 q[0x2c];u32 hints[4];} Render;
typedef struct RS RS;struct RS {u8 *pc;Render *render;u32 flags;u8 p[8];void(*callback)(RS*);u8 q[0x76];u8 timing;u8 r[0x1d];u8 node;u8 s[11];int *vis;u8 t[0xc8];int temporary;};
static inline u32 getTiming(RS*w) {return w->callback?w->timing:0;}
static inline void invoke(RS*w) {w->flags&=~0x40;w->callback(w);}
void NNSi_G3dFuncSbc_NODE(RS*);
void NNSi_G3dFuncSbc_NODE(RS*w) {u32 node,timing,skip;if(!(w->flags&0x200)){node=w->pc[1];w->node=node;w->flags|=4;w->vis=&w->temporary;timing=getTiming(w);if(timing==1){invoke(w);timing=getTiming(w);skip=w->flags&0x40;}else skip=0;if(!skip){Render*r=w->render;if(!r->animation || !(r->hints[node>>5]&(1<<(node&31))) || !r->blend(w->vis,r->animation,node))*w->vis=w->pc[2]&1;}if(timing==2){invoke(w);timing=getTiming(w);skip=w->flags&0x40;}else skip=0;if(!skip){if(*w->vis)w->flags|=1;else w->flags&=~1;}if(timing==3)invoke(w);}w->pc+=3;}
