#pragma force_active on
#include "global.h"
#pragma force_active on
extern int EasyChatApp_GetListMode(void*);extern void ov102_021EC298(void*,int,int,int,int,int);
void ov102_021EBBF4(u8*s);
void ov102_021EBBF4(u8*s){int v;if(EasyChatApp_GetListMode(*(void**)(s+4))==0)v=-256;else v=256;ov102_021EC298(s+60,*(int*)(s+12),1,0,v,16);}
