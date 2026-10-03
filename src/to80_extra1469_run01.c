#pragma force_active on
// clang-format off
#define NITRO_CARD_BACKUP_BACKUP_H_
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma opt_strength_reduction off
#define W(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define H(p,o) (*(u16 *)((u8 *)(p)+(o)))
typedef struct Bits{u32 a:6,b:6,c:6,d:6,e:1,f:7;}Bits;
void spl_push_front(void*,void*);
void *SPL_Init(void*(*)(u32),int,int,u16,u16,u16);
void *SPL_Init(void*(*alloc)(u32),int count,int particles,u16 d,u16 a,u16 b){u8*manager=alloc(76);int size,i;u8*pool;MI_CpuFill8(manager,0,76);H(manager,0x34)=count;H(manager,0x36)=particles;((Bits*)(manager+0x38))->a=a;((Bits*)(manager+0x38))->b=b;((Bits*)(manager+0x38))->c=((Bits*)(manager+0x38))->a;((Bits*)(manager+0x38))->d=d;((Bits*)(manager+0x38))->e=0;((Bits*)(manager+0x38))->f=0;W(manager,0)=(u32)alloc;W(manager,8)=0;W(manager,0x14)=0;W(manager,0x20)=0;W(manager,4)=0;W(manager,0x10)=0;W(manager,0x1c)=0;W(manager,0x3c)=0;H(manager,0x48)=0;size=count*156;pool=alloc(size);MI_CpuFill8(pool,0,size);for(i=0;i<count;i++){spl_push_front(manager+0x10,pool);pool+=156;}size=particles*68;pool=alloc(size);MI_CpuFill8(pool,0,size);for(i=0;i<particles;i++){spl_push_front(manager+0x1c,pool);pool+=68;}W(manager,0x28)=0;W(manager,0x2c)=0;H(manager,0x32)=0;H(manager,0x30)=H(manager,0x32);return manager;}
