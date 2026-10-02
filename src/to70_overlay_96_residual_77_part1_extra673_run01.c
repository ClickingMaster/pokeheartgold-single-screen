#pragma force_active on
#include "global.h"
extern void*PokeathlonCourse_GetGraphicsSystem(void*);extern void ov96_021E9510(void*),ov96_021E93B4(void*,int),ov96_0221A56C(void*,int),ov96_021E952C(void*),ov96_021E9570(void*,int);extern int PokeathlonCourse_GetField5F0_AtIndex(void*,u8);extern void ov96_021E966C(void*,void*,int,int,int,int,int,int,int,int,int);
int ov96_021FCD84(void*w);
int ov96_021FCD84(void*w){void*g=PokeathlonCourse_GetGraphicsSystem(w);int i;ov96_021E9510(g);for(i=0;i<4;i++){int v=(u16)PokeathlonCourse_GetField5F0_AtIndex(w,(u8)i);ov96_021E966C(w,g,i,v,(u32)v>>10,(v%1024)*10/1024,0,1,2,1,297);}ov96_021E93B4(g,1);ov96_0221A56C(g,6);ov96_021E952C(g);ov96_021E9570(g,6);return 1;}
