#pragma force_active on
#include "global.h"
typedef u64 Pair;
typedef struct {u32 x;void *arg;u8 p[0xd4-8];u32 d;Pair pair;u8 q[0x52c-0xe0];u32 flag;} Work;
extern void ov45_0222D8A4(void*),ov45_0222ECB8(Pair*),ov45_0222D500(void*,Pair*),ov45_0222B840(void*),ov45_0222BE5C(void*),ov45_0222BD30(void*),ov45_0222BB58(void*),ov45_0222BB60(void*,void*,void*,void*),ov45_0222BCB8(void*),ov45_0222C3B0(void*),ov45_0222CAA0(void*);extern int ov45_0222CCDC(void*);void ov45_0222A15C(Work*);
void ov45_0222A15C(Work*w){Pair pair;ov45_0222D8A4(w->arg);ov45_0222ECB8(&pair);w->pair=pair;ov45_0222D500(&w->d,&pair);ov45_0222B840(w);ov45_0222BE5C((u8*)w+0x1c0);ov45_0222BD30((u8*)w+0x1e8);ov45_0222BB58((u8*)w+0xf8);ov45_0222BB60((u8*)w+0x1fc,(u8*)w+0xf8,(u8*)w+0x20c,(u8*)w+0x49c);ov45_0222BCB8((u8*)w+0x3a0);ov45_0222C3B0((u8*)w+0x20c);ov45_0222CAA0((u8*)w+0x49c);if(ov45_0222CCDC((u8*)w+0x4bc)==1)w->flag=1;}
