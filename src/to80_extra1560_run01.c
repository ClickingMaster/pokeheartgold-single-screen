#pragma force_active on
#include "global.h"
#pragma force_active on
extern void*Save_Frontier_GetStatic(void*);extern int sub_0205C174(int,int),sub_0205C268(int),FrontierSave_GetStat(void*,int,int);
u8 ov83_0224777C(void*s,int a,int b);
u8 ov83_0224777C(void*s,int a,int b){void*p=Save_Frontier_GetStatic(s);int v=sub_0205C174(a,b);return FrontierSave_GetStat(p,v,sub_0205C268(sub_0205C174(a,b)));}
