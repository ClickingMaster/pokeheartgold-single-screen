#pragma force_active on
// clang-format off
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define B(p,o) (*(u8 *)((u8 *)(p)+(o)))
typedef struct Flags{u32 a:4,b:2,c:2,d:3,e:1,f:20;}Flags;
void spl_set_tex(void*);void spl_set_tex_dummy(void*);
void spl_draw_bb(void*,void*);void spl_draw_dbb(void*,void*);void spl_draw_pol(void*,void*);void spl_draw_dpl(void*,void*);
typedef struct FullFlags{u32 low:16,child:1,high:15;}FullFlags;
typedef struct DrawFlags{u32 pad:4,type:2,pad2:5,tex:1,rest:20;}DrawFlags;
void sDrawParent(u8*);

typedef struct SizeFlags{u32 pad:24,x:2,y:2,z:4;}SizeFlags;
typedef struct NegFlags{u32 x:1,y:1,pad:30;}NegFlags;
typedef struct ChildFlags{u32 x:2,y:2,nx:1,ny:1,pad:26;}ChildFlags;
typedef struct StateFlags{u32 a:16,b:3,c:13;}StateFlags;
typedef struct Vec16{u16 x,y,z;}Vec16;
void spl_init(u8*,u8*,s32*);
void sDrawParent(u8*m){void(*tex)(void*);u8*particle;u8*emitter=(u8*)W(m,0x40);u8*resource=(u8*)W(emitter,0x20);u8*base=(u8*)W(resource,0);void(*draw)(void*,void*)=0;spl_set_tex((void*)(W(m,0x2c)+B(base,0x47)*20));switch(((DrawFlags*)base)->type){case 0:draw=spl_draw_bb;break;case 1:draw=spl_draw_dbb;break;case 2:draw=spl_draw_pol;break;case 3:draw=spl_draw_dpl;break;case 4:draw=spl_draw_dpl;break;}if(((volatile DrawFlags*)base)->tex)tex=spl_set_tex;else tex=spl_set_tex_dummy;particle=(u8*)W(emitter,8);while(particle){tex((void*)(W(m,0x2c)+B(particle,0x2c)*20));draw(m,particle);particle=(u8*)W(particle,0);}}
