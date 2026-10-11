#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u8 pad[0x1e460];void*a,*b;u8 p1E468[0x1ec50-0x1e468];u32 index;};extern void*ov112_021FF090[];void PokewalkerApp_LoadTilemap(struct S*,int,int);void ov112_021E9FA4(struct S*,int,void*,int,u32);void ov112_021E9F40(struct S*,int);void ov112_021EA51C(struct S*,void*);int ov112_021EB3D8(struct S*);
int ov112_021EB3D8(struct S*s){PokewalkerApp_LoadTilemap(s,2,11);PokewalkerApp_LoadTilemap(s,5,16);ov112_021E9FA4(s,3,s->b,4,0xf0100);ov112_021E9FA4(s,4,s->a,4,0xf0100);ov112_021E9F40(s,1);s->index=0;ov112_021EA51C(s,ov112_021FF090[s->index]);return 13;}
