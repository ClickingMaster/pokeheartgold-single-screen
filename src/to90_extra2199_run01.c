#pragma force_active on
#include "global.h"
#pragma force_active on
extern const u8 ov18_021FB6C8[],ov18_021FB780[],ov18_021FB648[];
void*GridInputHandler_Create(const void*,const void*,const void*,void*,int,u8,int);
void ov18_021F11C0(void*,int,int),ov18_021F7444(void*,int);void ov18_021F7354(void*);
void ov18_021F7354(void*p){*(void**)((u8*)p+0x1864)=GridInputHandler_Create(ov18_021FB6C8,ov18_021FB780,ov18_021FB648,p,1,0,37);ov18_021F11C0(p,0,1);ov18_021F7444(p,0);}
