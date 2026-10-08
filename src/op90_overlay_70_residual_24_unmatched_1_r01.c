#include "global.h"
#include "ov70_wave34_menu_private.h"
#pragma force_active on
typedef struct W{u32 words[1138];}W;typedef struct P{u32 base,f58,f1168,dd0,f04,f08,flag,ba0,ba4,bb0,nested10,nested14;}P;extern void ov70_0223ECCC(W*),ov70_0223E954(void*),ov70_0223EA6C(W*),ov70_0223EB34(W*),ov70_0223EC0C(W*),BeginNormalPaletteFade(int,int,int,int,int,int,int),ov70_02245124(W*),ov70_0223F3D8(void*,void*,void*,void*,int,int,int),ov70_0223F508(void*,void*,void*,void*,void*);int ov70_0223E78C(W*);
#define F(o) ((void*)w->words[(o)/4])
#define A(o) (&w->words[(o)/4])
// @fn ov70_0223E78C

int ov70_0223E78C(W*w){P p;ov70_0223ECCC(w);ov70_0223E954(F(4));ov70_0223EA6C(w);ov70_0223EB34(w);ov70_0223EC0C(w);BeginNormalPaletteFade(3,1,1,0,6,1,61);ov70_02245124(w);ov70_0223F3D8(F(0xba0),F(0xba4),F(0xb9c),A(0x1058),0,3,-1);ov70_0223F508(F(0xba0),F(0xb9c),A(0x1088),F(0x124),A(0xb80));p.base=(u32)F(4);p.f58=(u32)A(0xf58);p.f1168=(u32)A(0x1168);p.dd0=(u32)F(0xdd0);p.f04=(u32)F(0xf04);p.f08=(u32)F(0xf08);p.flag=0;p.ba0=(u32)F(0xba0);p.ba4=(u32)F(0xba4);p.bb0=(u32)F(0xbb0);p.nested10=(u32)((void**)F(0))[4];p.nested14=(u32)((void**)F(0x11c4))[5];w->words[0x11a8/4]=(u32)ov70_02242014(&p,2,NULL);w->words[11]=0;return 2;}
