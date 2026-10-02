#pragma force_active on
#include "global.h"
typedef struct {u8 pad[8];void*bg,*save;u8 rest[0xc2];u8 flags;}Field;
typedef struct {u8 pad[16];void*str;u8 rest[0x85a];u16 printer;}Work;
extern void*String_New(int,int),*Save_PlayerData_GetOptionsAddr(void*);extern void DialogBox_AddWindowToLayer3(void*,Work*,int),FollowMon_ExpandInteractionMessage(Field*,void*,int,int),DialogBox_LoadFrame(Work*,void*);extern u16 DialogBox_PrintMessage(Work*,void*,void*,int);
int FollowMon_TryPrintInteractionMessage(Field*,Work*,u16*);
int FollowMon_TryPrintInteractionMessage(Field*f,Work*w,u16*p){void*opt;if(p[1]!=0){w->str=String_New(1024,11);DialogBox_AddWindowToLayer3(f->bg,w,3);FollowMon_ExpandInteractionMessage(f,w->str,11,p[1]-1);opt=Save_PlayerData_GetOptionsAddr(f->save);DialogBox_LoadFrame(w,opt);w->printer=DialogBox_PrintMessage(w,w->str,opt,1);f->flags|=64;return 1;}return 0;}
