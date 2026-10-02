#pragma force_active on
#include "global.h"
extern void*Save_Frontier_GetStatic(void*);extern int sub_0205C0A0(int,int),sub_0205C268(int);extern int FrontierSave_GetStat(void*,int,int);
u16 ov80_022318D0(void*save,int a,int b,u16*low,u16*high);
static inline u16 toWord(u32 v){return v;}
u16 ov80_022318D0(void*save,int a,int b,u16*low,u16*high){void*p=Save_Frontier_GetStatic(save);int index=sub_0205C0A0(a,b);u16 v=toWord((u8)FrontierSave_GetStat(p,index,sub_0205C268(sub_0205C0A0(a,b))));*low=v&15;*high=v>>4;if(b%2==0)return *low;return *high;}
