#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
struct Params{u32 p[16];void *(*alloc)(char*,int);u32 tail;};
struct Work{u8 p[0x1000];u32 zero,handle;u8 buffer[0x1C4];struct Params params;u8 q[0x100];int arg;};
struct Global{u32 a,b;struct Work *w;};
extern struct Global ov00_0221A694;
extern char ov00_02216464[];
extern int ov00_021E7F38(int);
extern void ov00_021E7C94(void);
int ov00_021E7B70(struct Params*,int);
int ov00_021E7B70(struct Params*p,int arg){struct Work*w;void *(*alloc)(char*,int)=p->alloc;if(ov00_0221A694.w)return 2;ov00_0221A694.w=alloc(ov00_02216464,0x13F4);if(!ov00_0221A694.w)return 2;MI_CpuFill8(ov00_0221A694.w,0,0x13F4);ov00_0221A694.w->arg=arg;ov00_0221A694.a=0;MI_CpuFill8(ov00_0221A694.w->buffer,0,0x1C4);*(u32*)ov00_0221A694.w->buffer=0x4E84;w=ov00_0221A694.w;w->params=*p;*(u16*)((u8*)w+0x11FE)=0;*(u8*)((u8*)ov00_0221A694.w+0x120B)=0;ov00_0221A694.w->handle=ov00_021E7F38(1);if(ov00_0221A694.w->handle)return ov00_0221A694.w->handle;ov00_0221A694.b=0;ov00_021E7C94();return 0;}
