#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct C{u8 pad[8];void*save;u8 padc[8];void*profile;u8 pad18[4];u32 id;}C;typedef struct W{C*ctx;u8 pad[24];int state;u8 tail[3888];int status,value;}W;extern u32 Save_SysInfo_GetDwcProfileId(void*);extern void Save_SysInfo_SetDwcProfileId(void*,u32);extern u64 DWC_CreateFriendKey(void*);extern void ov72_022378C0(u32,u64);
extern u32 ov00_021ECDC8(void);extern void ov72_0223A444(W*),ov00_021EC210(void),ov00_021EC8D8(void),ov00_021FA0D8(void),ov00_021ED9B4(void),sub_020399EC(void);extern int ov00_021EC11C(int*,u32*);int ov72_022392BC(W*),ov72_02239370(W*);
int ov72_022392BC(W*w){int value;u32 category;switch(ov00_021ECDC8()){case 3:w->state=6;break;case 0:case 4:case 5:ov72_0223A444(w);w->status=ov00_021EC11C(&value,&category);w->value=value;ov00_021EC210();ov00_021EC8D8();w->state=55;switch(category){case 1:case 2:w->state=55;break;case 4:ov00_021FA0D8();w->state=55;break;case 6:w->state=55;break;case 3:ov00_021ED9B4();w->state=55;break;case 5:case 7:sub_020399EC();break;}if(value<-20000&&value>=-29999)w->state=55;break;}return 3;}
int ov72_02239370(W*w){u32 id;u64 key;if(Save_SysInfo_GetDwcProfileId(w->ctx->save)==0)Save_SysInfo_SetDwcProfileId(w->ctx->save,w->ctx->id);id=Save_SysInfo_GetDwcProfileId(w->ctx->save);key=DWC_CreateFriendKey(w->ctx->profile);ov72_022378C0(id,key);w->state=7;return 3;}
