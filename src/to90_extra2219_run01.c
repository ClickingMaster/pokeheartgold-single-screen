#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *first,*save;u8 pad[0x110];int flag;} Parent;typedef struct {Parent *parent;u32 pad;int state;} Work;
extern void *sub_0202C6F4(void *),*Save_SysInfo_Get(void *),*sub_0202C08C(void *);extern u32 Save_SysInfo_GetDwcProfileId(void *),sub_0203A040(void *);extern void Save_SysInfo_SetDwcProfileId(void *,u32),ov70_022378C0(u32,u32,u32);extern u64 DWC_CreateFriendKey(void *);
int ov75_02248994(Work *);
int ov75_02248994(Work *w){void *data=sub_0202C6F4(w->parent->save);void *sys=Save_SysInfo_Get(w->parent->save);void *profile=sub_0202C08C(data);u32 id;u64 key;if(Save_SysInfo_GetDwcProfileId(sys)==0)Save_SysInfo_SetDwcProfileId(sys,sub_0203A040(data));id=Save_SysInfo_GetDwcProfileId(sys);key=DWC_CreateFriendKey(profile);ov70_022378C0(id,(u32)key,(u32)(key>>32));w->parent->flag=1;w->state=17;return 0;}
