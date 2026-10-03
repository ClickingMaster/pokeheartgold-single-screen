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
void spl_init(u8*p,u8*res,s32*pos){u8*base;W(p,0x20)=(u32)res;W(p,0x24)=0;W(p,0x28)=pos[0]+W((void*)W((void*)W(p,0x20),0),4);W(p,0x2c)=pos[1]+W((void*)W((void*)W(p,0x20),0),8);W(p,0x30)=pos[2]+W((void*)W((void*)W(p,0x20),0),12);W(p,0x40)=0;W(p,0x44)=0;W(p,0x48)=0;W(p,0x3c)=0;W(p,0x38)=W(p,0x3c);W(p,0x34)=W(p,0x38);H(p,0x4c)=0;H(p,0x4e)=0;base=(u8*)W((void*)W(p,0x20),0);*(Vec16*)(p+0x50)=*(Vec16*)(base+0x1c);
H(p,86)=H((void*)W((void*)W(p,0x20),0),56);W(p,88)=W((void*)W((void*)W(p,0x20),0),16);W(p,92)=W((void*)W((void*)W(p,0x20),0),20);W(p,96)=W((void*)W((void*)W(p,0x20),0),24);W(p,100)=W((void*)W((void*)W(p,0x20),0),36);W(p,104)=W((void*)W((void*)W(p,0x20),0),40);W(p,108)=W((void*)W((void*)W(p,0x20),0),44);H(p,112)=H((void*)W((void*)W(p,0x20),0),62);H(p,0x72)=32767;B(p,0x80)=B((void*)W((void*)W(p,0x20),0),0x44);B(p,0x81)=B((void*)W((void*)W(p,0x20),0),0x45);((StateFlags*)(p+0x80))->b=0;((StateFlags*)(p+0x80))->c=0;W(p,0x74)=0x80000000;H(p,0x78)=4096<<((SizeFlags*)(W((void*)W(p,0x20),0)+0x48))->x;H(p,0x7a)=4096<<((SizeFlags*)(W((void*)W(p,0x20),0)+0x48))->y;if(((NegFlags*)(W((void*)W(p,0x20),0)+0x4c))->x)*(s16*)(p+0x78)*=(s16)-1;if(((NegFlags*)(W((void*)W(p,0x20),0)+0x4c))->y)*(s16*)(p+0x7a)*=(s16)-1;if(((FullFlags*)W((void*)W(p,0x20),0))->child){H(p,0x7c)=4096<<((ChildFlags*)(W((void*)W(p,0x20),0x14)+0x10))->x;H(p,0x7e)=4096<<((ChildFlags*)(W((void*)W(p,0x20),0x14)+0x10))->y;if(((ChildFlags*)(W((void*)W(p,0x20),0x14)+0x10))->nx)*(s16*)(p+0x7c)*=(s16)-1;if(((ChildFlags*)(W((void*)W(p,0x20),0x14)+0x10))->ny)*(s16*)(p+0x7e)*=(s16)-1;}W(p,4)=0;W(p,0)=W(p,4);W(p,0x14)=0;W(p,8)=W(p,0x14);W(p,0x18)=0;W(p,0xc)=W(p,0x18);W(p,0x90)=0;W(p,0x94)=0;W(p,0x98)=0;}
