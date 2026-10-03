#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern u8*_022577C4;extern u8 ov45_022550F4[];extern void ov45_0223694C(void),ov45_022320B4(void*,void*),ov45_02237BC8(void*),ov45_022417B4(void*,int,void*,void(*)(void),int);
typedef union{struct{u32 flag:1,other:31;u32 a;void*pointer;}longform;struct{u8 prefix;u8 data[11];}shortform;}String;
static inline BOOL limit(int value){return value>=4;}
int ov45_02234944(void);
#pragma force_active on
int ov45_02234944(void){String s;switch(limit(*(int*)(_022577C4+0x144))){case 0:break;default:return 0;}ov45_022320B4(&s,_022577C4+0xb8);ov45_022417B4(s.longform.flag==0?s.shortform.data:s.longform.pointer,-1,ov45_022550F4,ov45_0223694C,0);ov45_02237BC8(&s);return 1;}
