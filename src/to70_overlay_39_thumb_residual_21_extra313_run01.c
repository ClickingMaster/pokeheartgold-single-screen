#pragma force_active on
#include "global.h"
#pragma force_active on
struct Inner{int x;void*save;};struct Outer{struct Inner*inner;u8 gap[0x7c-4];int flag;};struct S{struct Outer*outer;int x,state;};void*sub_0202C6F4(void*);void*Save_SysInfo_Get(void*);void sub_0202C08C(void*);u32 Save_SysInfo_GetDwcProfileId(void*);u32 sub_0203A040(void*);void Save_SysInfo_SetDwcProfileId(void*,u32);int ov39_02228E54(struct S*);
int ov39_02228E54(struct S*s){void*a=sub_0202C6F4(s->outer->inner->save);void*b=Save_SysInfo_Get(s->outer->inner->save);sub_0202C08C(a);if(Save_SysInfo_GetDwcProfileId(b)==0)Save_SysInfo_SetDwcProfileId(b,sub_0203A040(a));Save_SysInfo_GetDwcProfileId(b);s->state=9;s->outer->flag=1;return 0;}
