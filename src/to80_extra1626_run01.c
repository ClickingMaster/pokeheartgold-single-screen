#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
int ArrayLength(void*);
u32*ov00_021FE6EC(void*,int);
void ov00_02213D6C(void*,u32);
void ov00_021FEC3C(void*);
void ov00_02213E10(void*);
void ov00_02213E68(u32*);
void ov00_02213E68(u32*p){int i;int n=ArrayLength((void*)p[1]);for(i=0;i<n;i++){u32*item=ov00_021FE6EC((void*)p[1],i);ov00_02213D6C(p,*item);}ov00_021FEC3C((void*)p[1]);ov00_02213E10(p);}
