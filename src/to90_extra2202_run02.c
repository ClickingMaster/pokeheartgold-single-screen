#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u8 ov18_021FB744[],ov18_021FB8D4[],ov18_021FB6B8[];
void*GridInputHandler_Create(const void*,const void*,const void*,void*,int,u8,int);
void ov18_021F11C0(void*,int,int),ov18_021F76E0(void*,int);void ov18_021F7668(void*);
void ov18_021F7668(void*p){*(void**)((u8*)p+0x1864)=GridInputHandler_Create(ov18_021FB744,ov18_021FB8D4,ov18_021FB6B8,p,1,0,37);ov18_021F11C0(p,0,1);ov18_021F76E0(p,0);}
